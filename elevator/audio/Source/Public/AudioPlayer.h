#pragma once

#include "export.h"

#include "../Public/stdafx.h"

#include <vector>

#include "FileHandle.h"
#include "WaveFormat.h"

#pragma comment(lib, "winmm.lib")

class AUDIO_API AudioPlayer {
public:
	static void CALLBACK AudioCallback(HWAVEOUT hwo, UINT uMsg, DWORD_PTR dwInstance, DWORD_PTR dwParam1, DWORD_PTR dwParam2);
private:
	std::string* Filename;
	FILE* InternalFileHandle;
	// Denotes the fseek-usable begin of the data-chunk of the file
	int DataBegin = 0;

	WAVEFORMATEX WaveFormat;
	WaveFormat::Header Header;

	std::vector<WaveFormat::Chunk>* ChunkList;

	HWAVEOUT WaveOutHandle;
	WAVEHDR WaveHeader;
	WAVEHDR WaveHeader2;

	// Inner helper functions
	void PrepareFile();
	bool OpenDevice();
	bool PrepareHeader(WAVEHDR* waveHeader);
	bool ExecuteSound(WAVEHDR* waveHeader);
public:
	bool Play();

	AudioPlayer(const char* filename);

	~AudioPlayer();
};