#pragma once

class AudioClip;

class WavImporter
{
public:
	static bool Import (AudioClip& clip, char* data, const int dataSize);
};

