#pragma once
#include "memory.h"
#include "stdlib.h"
#include <iostream>
#include <fstream>

/// Binary file handler
/// 
/// File handler, that allows to read and write binary
/// contents to a file on the hard drive, using
/// std::fstream.
class FileHandle
{
public:
	static char AbsolutePathToFolder[FILENAME_MAX];
	/// Target filestream
	/// 
	/// Filestream values will be written to or read from.
	std::fstream FileStream;

	/// Check validity
	/// 
	/// Returns whether or not read/writing to the #FileStream
	/// is possible.
	bool IsValid()
	{
		return FileStream.good();
	}

	/// Create FileHandle based on existing file
	/// 
	/// Tries to open a read/write binary std::fstream with the
	/// file located at \p pathToFile or creates it, if no file
	/// was found at \p pathToFile.
	FileHandle(char* pathToFile)
	{
		FileStream.open(pathToFile, std::fstream::in | std::fstream::out | std::fstream::binary);

		// File doesn exist?
		if (!FileStream)
		{
			// Create an empty file
			FileStream.open(pathToFile, std::fstream::binary | std::fstream::trunc | std::fstream::out);
			FileStream.close();
			// Re-open it with initial flags
			FileStream.open(pathToFile, std::fstream::binary | std::fstream::in | std::fstream::out);
		}
	}

	~FileHandle()
	{
		FileStream.close();
	}

	/// Read raw data
	/// 
	/// Reads data of type T from the #FileStream to location \p writeTo.
	/// 
	/// \param writeTo Variable to write the read content to
	/// \param resetPointer Whether or not to read from the start
	///						of the file or continue reading, where
	///						the #FileStream last left off.
	template <typename T > bool Read(T& writeTo, bool resetPointer)
	{
		try
		{
			if (resetPointer)
			{
				FileStream.seekg(0, FileStream.beg);
			}

			FileStream.read((char*)&writeTo, sizeof(T));
		}
		catch (std::ifstream::failure e)
		{
			return false;
		}
		return true;
	}

	/// Write raw data
	/// 
	/// Writes data of type T into the #FileStream.
	/// 
	/// \param content Content to write
	/// \param append Whether or not to write at the start
	///               of the file or continue writing, where
	///               the #FileStream last left off.
	template <typename T > bool Write(T& content, bool append)
	{
		try
		{
			if (!append)
			{
				FileStream.seekg(0, FileStream.beg);
			}

			char* buffer = new char[sizeof(T)];
			memcpy(buffer, &content, sizeof(T));

			FileStream.write(buffer, sizeof(T));
		}
		catch (std::ifstream::failure e)
		{
			return false;
		}
		return true;
	}
};