#pragma once
#ifdef EXPORTING_AUDIO
#define AUDIO_API __declspec(dllexport)
#else
#define AUDIO_API __declspec(dllimport)
#endif