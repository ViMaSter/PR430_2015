// audio.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdio.h>

#include <vector>

#include "AudioPlayer.h"
#include "FileHandle.h"
#include "WaveFormat.h"

#pragma comment(lib, "winmm.lib")

int _tmain(int argc, _TCHAR* argv[]) {
	// Absolute paths, since relative paths are relative to the "working directory"
	AudioPlayer ap = AudioPlayer("D:\prj\6_PR430\audio\Files\\File1.wav");
	AudioPlayer ap2 = AudioPlayer("D:\prj\6_PR430\audio\Files\\File2.wav");

	ap.Play();

	Sleep(5000);

	ap2.Play();

	while (true) {
		Sleep(10);
	}

	return 0;
}

