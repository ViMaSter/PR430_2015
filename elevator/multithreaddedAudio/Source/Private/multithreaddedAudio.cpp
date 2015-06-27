// multithreaddedAudio.cpp : Defines the entry point for the console application.
//

#include "../Public/stdafx.h"
#include <stdio.h>

#include <vector>

#include "FileHandle.h"
#include "WaveFormat.h"

#pragma comment(lib, "winmm.lib")

void CALLBACK audioCallback(HWAVEOUT hwo, UINT uMsg, DWORD_PTR dwInstance, DWORD_PTR dwParam1, DWORD_PTR dwParam2) {
	MMRESULT waveOutWriteResult;
	int bytesRead;
	switch (uMsg) {
		case WOM_OPEN:
			printf("a");
			break;
		case WOM_DONE:
		{
			bytesRead = fread(((LPWAVEHDR)dwParam1)->lpData, 1, ((LPWAVEHDR)dwParam1)->dwBufferLength, (FILE*)dwInstance);

			waveOutWriteResult = waveOutWrite(hwo, (LPWAVEHDR)dwParam1, sizeof(WAVEHDR));
			printf("%s wrote out!\r\n", waveOutWriteResult == MMSYSERR_NOERROR ? "Successfully" : "Unsuccessfully");
		}
			break;
		case WOM_CLOSE:
			printf("c");
			break;
	}
};

int _tmain(int argc, _TCHAR* argv[])
{
	// Open file
	FILE* InternalFileHandle;
	fopen_s(&InternalFileHandle, "D:\\prj\\6_PR430\\elevator\\multithreaddedAudio\\Files\\Forest.wav", "rb");
	
	// Get length
	fseek(InternalFileHandle, 0, SEEK_END);
	int FileLength = ftell(InternalFileHandle);
	char* characterBuffer = new char[FileLength];
	fseek(InternalFileHandle, 0, 0);

	// Load file to char buffer
	fgets(characterBuffer, FileLength, InternalFileHandle);
	fseek(InternalFileHandle, 0, 0);

	const int ChunkArraySize = 20;
	int DataBegin = 0;
	WaveFormat::Header header;
	WaveFormat::Chunk* chunks = new WaveFormat::Chunk[ChunkArraySize];

	std::vector<WaveFormat::Chunk> chunkList;

	fread((char*)&header, sizeof(WaveFormat::Header), 1, InternalFileHandle);

	WaveFormat::Chunk temporaryChunk;
	WAVEFORMATEX WaveFormat;

	while (!feof(InternalFileHandle)) {
		int readSize = fread((char*)&temporaryChunk, sizeof(WaveFormat::Chunk), 1, InternalFileHandle);
		if (readSize == 0) {
			break;
		}
		if (temporaryChunk.Name[0] == 'f') {
			fread((char*)&WaveFormat, 16, 1, InternalFileHandle);
			if (WaveFormat.wFormatTag != WAVE_FORMAT_PCM) {
				// This is no PCM-file - abort
				printf("File does not contain PCM-data. : (");
				break;
			}
		}
		else {
			if (temporaryChunk.Name[0] == 'd') {
				DataBegin = ftell(InternalFileHandle);
			}
			WaveFormat::ChunkWithPtr chunkWithLocation;
			memcpy((void*)&chunkWithLocation, (void*)&temporaryChunk, sizeof(WaveFormat::Chunk));
			chunkWithLocation.Beginning = ftell(InternalFileHandle);
			chunkList.push_back(temporaryChunk);
			fseek(InternalFileHandle, temporaryChunk.Length, SEEK_CUR);
		}
	}

	HWAVEOUT waveOutHandle;
	// Open wave out
	MMRESULT waveOutOpenResult = waveOutOpen(&waveOutHandle, 0, &WaveFormat, (DWORD_PTR)audioCallback, (DWORD_PTR)InternalFileHandle, CALLBACK_FUNCTION);

	printf("%s created handle!\r\n", waveOutOpenResult == MMSYSERR_NOERROR ? "Successfully" : "Unsuccessfully");
	if (waveOutOpenResult != MMSYSERR_NOERROR) {
		return 1;
	}

	int CurrentPosition = 0;

	// Prepare wave out header
	WAVEHDR waveHeader = { 0 };
	waveHeader.dwBufferLength = 32 * 1024;
	char* data = new char[waveHeader.dwBufferLength];
	waveHeader.lpData = data;

	WAVEHDR waveHeader2 = { 0 };
	waveHeader2.dwBufferLength = 32 * 1024;
	char* data2 = new char[waveHeader2.dwBufferLength];
	waveHeader2.lpData = data2;

	MMRESULT waveOutPrepareHeaderResult = waveOutPrepareHeader(waveOutHandle, &waveHeader, sizeof(waveHeader));
	printf("%s prepared header!\r\n", waveOutPrepareHeaderResult == MMSYSERR_NOERROR ? "Successfully" : "Unsuccessfully");
	if (waveOutPrepareHeaderResult != MMSYSERR_NOERROR) {
		return 1;
	}

	MMRESULT waveOutPrepareHeaderResult2 = waveOutPrepareHeader(waveOutHandle, &waveHeader2, sizeof(waveHeader2));
	printf("%s prepared header!\r\n", waveOutPrepareHeaderResult2 == MMSYSERR_NOERROR ? "Successfully" : "Unsuccessfully");
	if (waveOutPrepareHeaderResult2 != MMSYSERR_NOERROR) {
		return 1;
	}

	fseek(InternalFileHandle, DataBegin, SEEK_SET);

	int bytesRead = 0;
	bytesRead = fread(waveHeader.lpData, 1, waveHeader.dwBufferLength, InternalFileHandle);
	bytesRead = fread(waveHeader2.lpData, 1, waveHeader2.dwBufferLength, InternalFileHandle);

	MMRESULT waveOutWriteResult = waveOutWrite(waveOutHandle, &waveHeader, sizeof(waveHeader));
	printf("%s wrote out!\r\n", waveOutWriteResult == MMSYSERR_NOERROR ? "Successfully" : "Unsuccessfully");
	if (waveOutWriteResult != MMSYSERR_NOERROR) {
		return 1;
	}

	MMRESULT waveOutWriteResult2 = waveOutWrite(waveOutHandle, &waveHeader2, sizeof(waveHeader2));
	printf("%s wrote out!\r\n", waveOutWriteResult2 == MMSYSERR_NOERROR ? "Successfully" : "Unsuccessfully");
	if (waveOutWriteResult2 != MMSYSERR_NOERROR) {
		return 1;
	}

	Sleep(50000);

	MMRESULT waveOutUnprepareHeaderResult = waveOutUnprepareHeader(waveOutHandle, &waveHeader, sizeof(waveHeader));
	printf("%s unprepared header!\r\n", waveOutUnprepareHeaderResult == MMSYSERR_NOERROR ? "Successfully" : "Unsuccessfully");
	if (waveOutUnprepareHeaderResult != MMSYSERR_NOERROR) {
		return 1;
	}


	// Close wave out
	MMRESULT waveOutCloseResult = waveOutClose(waveOutHandle);
	printf("%s closed handle!\r\n", waveOutCloseResult == MMSYSERR_NOERROR ? "Successfully" : "Unsuccessfully");
	if (waveOutCloseResult != MMSYSERR_NOERROR) {
		return 1;
	}

	printf("\r\n", FileLength);

	printf("Length: %i\r\n", FileLength);
	for (std::vector<WaveFormat::Chunk>::iterator it = chunkList.begin(); it != chunkList.end(); ++it) {
		printf("Chunk: %s (%i)\r\n", it->Name, it->Length);
	}

	// Close file
	fclose(InternalFileHandle);

	return 0;
}

