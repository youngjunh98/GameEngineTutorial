#pragma once

#include <vector>

class AudioClip
{
public:
	AudioClip ();
	virtual ~AudioClip ();

	int ChannelCount () const;
	int SampleFrameCount () const;
	float SampleRate () const;
	float Length () const;

	const float* GetSampleData () const;
	void SetSampleData (const std::vector<float>& data, int channelCount, float sampleRate);

private:
	int m_channelCount;
	int m_sampleFrameCount;
	float m_sampleRate;
	std::vector<float> m_sampleData;
};

