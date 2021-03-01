#include <vector>

#include "WavImporter.h"
#include "AudioClip.h"

bool WavImporter::Import (AudioClip& clip, char* data, const int dataSize)
{
	bool bInvalidDataSize = dataSize < 44;

	if (bInvalidDataSize)
	{
		return false;
	}

	char chunkId[4];
	std::memcpy (chunkId, data, 4);

	int chunkSize;
	std::memcpy (&chunkSize, data + 4, 4);

	char format[4];
	std::memcpy (&format, data + 8, 4);

	char subchunk1Id[4];
	std::memcpy (&subchunk1Id, data + 12, 4);

	int subchunk1Size;
	std::memcpy (&subchunk1Size, data + 16, 4);

	short audioFormat;
	std::memcpy (&audioFormat, data + 20, 2);

	short channels;
	std::memcpy (&channels, data + 22, 2);;

	int sampleRate;
	std::memcpy (&sampleRate, data + 24, 4);

	int byteRate;
	std::memcpy (&byteRate, data + 28, 4);

	short blockAlign;
	std::memcpy (&blockAlign, data + 32, 2);

	short bitsPerSample;
	std::memcpy (&bitsPerSample, data + 34, 2);

	bool bUnsupportedFormat = subchunk1Size > 16 || audioFormat != 1;

	if (bUnsupportedFormat)
	{
		return false;
	}

	char subchunk2Id[4];
	std::memcpy (&subchunk2Id, data + 36, 4);

	int subchunk2Size;
	std::memcpy (&subchunk2Size, data + 40, 4);

	bool bNoData = subchunk2Size < 0;
	bool bUnsupportedChannels = channels > 2;
	bool bUnsupportedSampleSize = bitsPerSample != 8 && bitsPerSample != 16;

	if (bNoData || bUnsupportedChannels || bUnsupportedSampleSize)
	{
		return false;
	}

	const unsigned char* sampleData = reinterpret_cast<const unsigned char*> (data) + 44;
	int sampleCount = subchunk2Size / blockAlign;

	std::vector<float> samples;
	samples.resize (channels * sampleCount);

	for (int sampleIndex = 0; sampleIndex < channels * sampleCount; sampleIndex++)
	{
		float value = 0.0f;
		int byteOffset = sampleIndex * bitsPerSample / 8;

		if (bitsPerSample == 8)
		{
			char sample = sampleData[byteOffset];
			value = static_cast<float> (sample - 128) / 128.0f;
		}
		else if (bitsPerSample == 16)
		{
			short sample = (sampleData[byteOffset + 1] << 8) | (sampleData[byteOffset] & 0x00ff);
			value = static_cast<float> (sample) / 32768.0f;
		}

		samples.at (sampleIndex) = value;
	}

	clip.SetSampleData (samples, channels, static_cast<float> (sampleRate));

    return true;
}
