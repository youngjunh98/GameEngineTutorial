#include <process.h>
#include <mmdeviceapi.h>

#include "Audio.h"
#include "AudioClip.h"

#define REFTIMES_PER_SEC  10000000
#define REFTIMES_PER_MILLISEC  10000

static HANDLE g_audioThread;
static CRITICAL_SECTION g_audioThreadCS;
static bool g_playAudio = false;

unsigned WINAPI AudioThreadFunction (void* data)
{
	if (data == nullptr)
	{
		return 0;
	}

	Audio& audio = *reinterpret_cast<Audio*>(data);
	BYTE* audioBuffer;
	DWORD releaseBufferFlags = 0;

	Microsoft::WRL::ComPtr<IAudioClient> client = audio.m_audioClient;
	Microsoft::WRL::ComPtr<IAudioRenderClient> renderClient = audio.m_audioRenderClient;

	UINT channelCount = static_cast<UINT> (audio.m_channelCount);
	UINT32 bufferFrameCount = audio.m_bufferFrameCount;
	UINT sampleBytes = audio.m_sampleFrameBytes / channelCount;

	if (FAILED (renderClient->GetBuffer (bufferFrameCount, &audioBuffer)))
	{
		return 0;
	}

	if (FAILED (renderClient->ReleaseBuffer (bufferFrameCount, AUDCLNT_BUFFERFLAGS_SILENT)))
	{
		return 0;
	}

	if (FAILED (client->Start ()))
	{
		return 0;
	}

	float sampleRate = audio.m_sampleRate;
	float bufferSeconds = static_cast<float> (bufferFrameCount) / sampleRate;
	float bufferRefTimes = bufferSeconds * static_cast<float> (REFTIMES_PER_SEC);
	REFERENCE_TIME hnsActualDuration = static_cast<REFERENCE_TIME> (bufferRefTimes);

	bool bShutdownAudio = false;

	while (bShutdownAudio == false)
	{
		Sleep (static_cast<DWORD> (hnsActualDuration / REFTIMES_PER_MILLISEC / 2));

		UINT numFramesPadding;

		if (FAILED (client->GetCurrentPadding (&numFramesPadding)))
		{
			return 0;
		}

		UINT numFramesAvailable = bufferFrameCount - numFramesPadding;

		if (FAILED (renderClient->GetBuffer (numFramesAvailable, &audioBuffer)))
		{
			return 0;
		}

		UINT initBytes = audio.m_sampleFrameBytes * numFramesAvailable;
		std::memset (audioBuffer, 0, initBytes);

		Audio::LockAudioThread ();

		if (g_playAudio)
		{
			auto trackIter = audio.m_tracks.begin ();

			while (trackIter != audio.m_tracks.end ())
			{
				bool bTrackFinished = trackIter->ReadNextSamples (audioBuffer, channelCount, numFramesAvailable, sampleRate, sampleBytes * 8);

				if (bTrackFinished)
				{
					trackIter = audio.m_tracks.erase (trackIter);
				}
				else
				{
					++trackIter;
				}
			}
		}
		else
		{
			bShutdownAudio = true;
		}

		Audio::UnlockAudioThread ();

		if (FAILED (renderClient->ReleaseBuffer (numFramesAvailable, releaseBufferFlags)))
		{
			return 0;
		}
	}

	Sleep (static_cast<DWORD> (hnsActualDuration / REFTIMES_PER_MILLISEC / 2));
	client->Stop ();

	return 0;
}

AudioTrack::AudioTrack (const AudioClip& clip) : m_clip (&clip), m_framesPlayed (0.0f)
{
}

AudioTrack::~AudioTrack ()
{
	m_clip = nullptr;
	m_framesPlayed = 0.0;
}

bool AudioTrack::ReadNextSamples (byte* buffer, int channelCount, int sampleFrameCount, float sampleRate, int sampleBitSize)
{
	float clipSampleRate = m_clip->SampleRate ();
	float pitch = clipSampleRate / sampleRate;

	int clipChannelCount = m_clip->ChannelCount ();
	int clipFrameCount = m_clip->SampleFrameCount ();
	const float* clipSampleData = m_clip->GetSampleData ();

	for (int copyFrame = 0; copyFrame < sampleFrameCount; copyFrame++)
	{
		int currentFrame = static_cast<int> (m_framesPlayed);

		if (currentFrame >= clipFrameCount)
		{
			return true;
		}

		for (int copyChannel = 0; copyChannel < channelCount; copyChannel++)
		{
			int copyDestinationFrame = copyFrame * channelCount + copyChannel;
			int copySourceFrame = currentFrame * clipChannelCount + copyChannel;
			float sample = clipSampleData[copySourceFrame];

			if (sampleBitSize == 8)
			{
				auto* copyDestination = reinterpret_cast<char*> (buffer);
				copyDestination[copyDestinationFrame] += static_cast<char> (sample * 128.0f);
			}
			else if (sampleBitSize == 16)
			{
				auto* copyDestination = reinterpret_cast<short*> (buffer);
				copyDestination[copyDestinationFrame] += static_cast<short> (sample * 32768.0f);
			}
			else if (sampleBitSize == 32)
			{
				auto* copyDestination = reinterpret_cast<float*> (buffer);
				copyDestination[copyDestinationFrame] += sample;
			}
		}

		m_framesPlayed += pitch;
	}

	return false;
}

Audio::Audio () : m_bufferFrameCount (0), m_channelCount (0), m_sampleRate (0.0f), m_sampleFrameBytes (0)
{
}

Audio::~Audio ()
{
}

Audio& Audio::GetInstance ()
{
	static Audio instance;
	return instance;
}

bool Audio::Initialize (int desiredBufferFrames)
{
	if (BeginAudioRenderClient (desiredBufferFrames) == false)
	{
		return false;
	}

	if (BeginAudioThread () == false)
	{
		return false;
	}

	return true;
}

void Audio::Shutdown ()
{
	ShutdownAudioThread ();

	m_audioClient = nullptr;
	m_audioRenderClient = nullptr;

	m_bufferFrameCount = 0;
	m_channelCount = 0;
	m_sampleRate = 0.0f;
	m_sampleFrameBytes = 0;

	m_tracks.clear ();
}

void Audio::PlayClip (const AudioClip& clip)
{
	LockAudioThread ();
	m_tracks.emplace_back (AudioTrack (clip));
	UnlockAudioThread ();
}

bool Audio::BeginAudioRenderClient (int desiredBufferFrames)
{
	const CLSID CLSID_MMDeviceEnumerator = __uuidof(MMDeviceEnumerator);
	const IID IID_IMMDeviceEnumerator = __uuidof(IMMDeviceEnumerator);
	Microsoft::WRL::ComPtr<IMMDeviceEnumerator> deviceEnumerator;

	if (FAILED (CoCreateInstance (CLSID_MMDeviceEnumerator, nullptr, CLSCTX_ALL, IID_IMMDeviceEnumerator, reinterpret_cast<void**> (deviceEnumerator.ReleaseAndGetAddressOf ()))))
	{
		return false;
	}

	Microsoft::WRL::ComPtr<IMMDevice> device;

	if (FAILED (deviceEnumerator->GetDefaultAudioEndpoint (EDataFlow::eRender, ERole::eConsole, device.ReleaseAndGetAddressOf ())))
	{
		return false;
	}

	const IID IID_IAudioClient = __uuidof(IAudioClient);

	if (FAILED (device->Activate (IID_IAudioClient, CLSCTX_ALL, nullptr, reinterpret_cast<void**> (m_audioClient.ReleaseAndGetAddressOf ()))))
	{
		return false;
	}

	WAVEFORMATEX* waveFormat;

	if (FAILED (m_audioClient->GetMixFormat (&waveFormat)))
	{
		return false;
	}

	bool bIsPCM = waveFormat->wFormatTag == WAVE_FORMAT_PCM;
	bool bisFloat = waveFormat->wFormatTag == WAVE_FORMAT_IEEE_FLOAT;
	const int minExtensibleSize = 22;

	if (waveFormat->wFormatTag == WAVE_FORMAT_EXTENSIBLE && waveFormat->cbSize >= minExtensibleSize)
	{
		WAVEFORMATEXTENSIBLE* waveFormatExtensible = reinterpret_cast<WAVEFORMATEXTENSIBLE*> (waveFormat);
		bIsPCM = waveFormatExtensible->SubFormat == KSDATAFORMAT_SUBTYPE_PCM;
		bisFloat = waveFormatExtensible->SubFormat == KSDATAFORMAT_SUBTYPE_IEEE_FLOAT;
	}

	if (bIsPCM == false && bisFloat == false)
	{
		return false;
	}

	float desiredBufferSeconds = static_cast<float> (desiredBufferFrames) / static_cast<float> (waveFormat->nSamplesPerSec);
	float desiredBufferRefTimes = desiredBufferSeconds * static_cast<float> (REFTIMES_PER_SEC);
	REFERENCE_TIME hnsRequestDuration = static_cast<REFERENCE_TIME> (desiredBufferRefTimes);
	DWORD streamFlags = 0;

	if (FAILED (m_audioClient->Initialize (AUDCLNT_SHAREMODE_SHARED, streamFlags, hnsRequestDuration, 0, waveFormat, nullptr)))
	{
		return false;
	}

	UINT bufferSize;

	if (FAILED (m_audioClient->GetBufferSize (&bufferSize)))
	{
		return false;
	}

	const IID IID_IAudioRenderClient = __uuidof(IAudioRenderClient);

	if (FAILED (m_audioClient->GetService (IID_IAudioRenderClient, reinterpret_cast<void**> (m_audioRenderClient.ReleaseAndGetAddressOf ()))))
	{
		return false;
	}

	if (InitializeCriticalSectionEx (&g_audioThreadCS, 0, 0) == false)
	{
		return false;
	}

	m_bufferFrameCount = bufferSize;
	m_channelCount = waveFormat->nChannels;
	m_sampleRate = static_cast<float> (waveFormat->nSamplesPerSec);
	m_sampleFrameBytes = waveFormat->nBlockAlign;

	CoTaskMemFree (waveFormat);

	return true;
}

bool Audio::BeginAudioThread ()
{
	bool bSuccess = false;

	g_playAudio = true;
	auto threadHandle = _beginthreadex (nullptr, 0, &AudioThreadFunction, &GetInstance(), 0, nullptr);

	if (threadHandle != 0)
	{
		g_audioThread = reinterpret_cast<HANDLE> (threadHandle);

		bSuccess = true;
	}

	return bSuccess;
}

void Audio::ShutdownAudioThread ()
{
	if (g_audioThread != nullptr)
	{
		LockAudioThread ();

		g_playAudio = false;

		UnlockAudioThread ();

		WaitForSingleObject (g_audioThread, INFINITE);
		CloseHandle (g_audioThread);

		g_audioThread = nullptr;
	}

	DeleteCriticalSection (&g_audioThreadCS);
}

void Audio::LockAudioThread ()
{
	EnterCriticalSection (&g_audioThreadCS);
}

void Audio::UnlockAudioThread ()
{
	LeaveCriticalSection (&g_audioThreadCS);
}
