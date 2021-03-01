#pragma once

#include <Windows.h>
#include <wrl.h>
#include <Audioclient.h>
#include <vector>

class AudioClip;

class AudioTrack
{
public:
    AudioTrack (const AudioClip& clip);
    virtual ~AudioTrack ();

    bool ReadNextSamples (byte* buffer, int channelCount, int sampleFrameCount, float sampleRate, int sampleBitSize);

private:
    const AudioClip* m_clip;
    float m_framesPlayed;
};

class Audio
{
private:
    Audio ();
    ~Audio ();

public:
    static Audio& GetInstance ();

    Audio (const Audio&) = delete;
    Audio& operator= (const Audio&) = delete;

    bool Initialize (int desiredBufferFrames);
    void Shutdown ();

    void PlayClip (const AudioClip& clip);

private:
    bool BeginAudioRenderClient (int desiredBufferFrames);

    static bool BeginAudioThread ();
    static void ShutdownAudioThread ();
    static void LockAudioThread ();
    static void UnlockAudioThread ();

    friend unsigned WINAPI AudioThreadFunction (void* data);

private:
    int m_bufferFrameCount;
    int m_channelCount;
    float m_sampleRate;
    int m_sampleFrameBytes;

    std::vector<AudioTrack> m_tracks;

    Microsoft::WRL::ComPtr<IAudioClient> m_audioClient;
    Microsoft::WRL::ComPtr<IAudioRenderClient> m_audioRenderClient;
};
