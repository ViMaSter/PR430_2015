#pragma once

#include "Windows.h"

namespace WaveFormat {
	struct Header {
		unsigned char Name[4];
		unsigned long Length;
		unsigned char Type[4];
	};
	struct Chunk {
		char Name[4];
		unsigned long Length;
	};
	struct ChunkWithPtr : public Chunk {
		long Beginning;
	};
};