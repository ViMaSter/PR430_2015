// audio.cpp : Defines the entry point for the console application.
//

#include "../Public/stdafx.h"
#include <stdio.h>

#include <vector>

#include "FileHandle.h"
#include "WaveFormat.h"

#pragma comment(lib, "winmm.lib")

class AudioPlayer;

class AudioPlayer {
public:
	static void CALLBACK AudioCallback(HWAVEOUT hwo, UINT uMsg, DWORD_PTR dwInstance, DWORD_PTR dwParam1, DWORD_PTR dwParam2) {
		switch (uMsg) {
		case WOM_OPEN:
			printf("a");
			break;
		case WOM_DONE:
			((AudioPlayer*)dwInstance)->ExecuteSound((WAVEHDR*)dwParam1);
			break;
		case WOM_CLOSE:
			printf("c");
			break;
		}
	}
private:
	FILE* InternalFileHandle;
	int DataBegin = 0;
	WAVEFORMATEX WaveFormat;
	WaveFormat::Header Header;
	std::vector<WaveFormat::Chunk> ChunkList;
	std::string Filename;

	HWAVEOUT WaveOutHandle;
	WAVEHDR WaveHeader;
	WAVEHDR WaveHeader2;

	void PrepareFile() {
		// Open file
		fopen_s(&InternalFileHandle, Filename.c_str(), "rb");

		// Get length
		fseek(InternalFileHandle, 0, SEEK_END);
		int FileLength = ftell(InternalFileHandle);
		fseek(InternalFileHandle, 0, 0);

		fread((char*)&Header, sizeof(WaveFormat::Header), 1, InternalFileHandle);

		WaveFormat::Chunk temporaryChunk;
		while (!feof(InternalFileHandle)) {
			int readSize = fread((char*)&temporaryChunk, sizeof(WaveFormat::Chunk), 1, InternalFileHandle);
			if (readSize == 0) {
				break;
			}

			switch (temporaryChunk.Name[0]) {
			case 'f':
				// RIFF Header
				fread((char*)&WaveFormat, 16, 1, InternalFileHandle);
				if (WaveFormat.wFormatTag != WAVE_FORMAT_PCM) {
					// This is no PCM-file - abort
					printf("File does not contain PCM-data. : (");
					break;
				}
				break;
			case 'd':
				// DATA Area
				DataBegin = ftell(InternalFileHandle);
				break;
			default:
				// Other Chunk
				ChunkList.push_back(temporaryChunk);
				fseek(InternalFileHandle, temporaryChunk.Length, SEEK_CUR);
				break;
			}
		}
	}

	bool OpenDevice() {
		MMRESULT result = waveOutOpen(&WaveOutHandle, 0, &WaveFormat, (DWORD_PTR)AudioPlayer::AudioCallback, (DWORD_PTR)this, CALLBACK_FUNCTION);
		printf("%s opened device!\r\n", result == MMSYSERR_NOERROR ? "Successfully" : "Unsuccessfully");
		return result == MMSYSERR_NOERROR;
	}

	bool PrepareHeader(WAVEHDR* waveHeader) {
		waveHeader->dwBufferLength = 32 * 10024;
		char* data = new char[waveHeader->dwBufferLength];
		waveHeader->lpData = data;

		MMRESULT result = waveOutPrepareHeader(WaveOutHandle, waveHeader, sizeof(*waveHeader));
		printf("%s prepared header!\r\n", result == MMSYSERR_NOERROR ? "Successfully" : "Unsuccessfully");
		return result == MMSYSERR_NOERROR;
	}

	bool ExecuteSound(WAVEHDR* waveHeader) {
		fread(waveHeader->lpData, 1, waveHeader->dwBufferLength, InternalFileHandle);

		MMRESULT result = waveOutWrite(WaveOutHandle, waveHeader, sizeof(*waveHeader));
		printf("%s wrote out!\r\n", result == MMSYSERR_NOERROR ? "Successfully" : "Unsuccessfully");

		return result == MMSYSERR_NOERROR;
	}
public:
	bool Play() {
		PrepareFile();

		OpenDevice();

		// Prepare wave out header
		PrepareHeader(&WaveHeader);
		PrepareHeader(&WaveHeader2);

		fseek(InternalFileHandle, DataBegin, SEEK_SET);

		ExecuteSound(&WaveHeader);
		ExecuteSound(&WaveHeader2);

		return true;
	}

	AudioPlayer(const char* filename) {
		Filename = filename;
	}

	~AudioPlayer() {
		MMRESULT waveOutUnprepareHeaderResult = waveOutUnprepareHeader(WaveOutHandle, &WaveHeader, sizeof(WaveHeader));
		printf("%s unprepared header!\r\n", waveOutUnprepareHeaderResult == MMSYSERR_NOERROR ? "Successfully" : "Unsuccessfully");

		// Close wave out
		MMRESULT waveOutCloseResult = waveOutClose(WaveOutHandle);
		printf("%s closed handle!\r\n", waveOutCloseResult == MMSYSERR_NOERROR ? "Successfully" : "Unsuccessfully");

		fclose(InternalFileHandle);
	}
};

int _tmain(int argc, _TCHAR* argv[])
{
	AudioPlayer ap = AudioPlayer("D:\\prj\\6_PR430\\elevator\\audio\\Files\\Forest.wav");
	AudioPlayer ap2 = AudioPlayer("D:\\prj\\6_PR430\\elevator\\audio\\Files\\David Fenn - Titan Souls - 25 Epilogue.wav");

	ap.Play();

	Sleep(5000);

	ap2.Play();

	Sleep(50000);

	// Close file

	return 0;
}

