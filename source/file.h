#ifndef FILE_h
#define FILE_h

#include "bloc.h"

#include <fstream>

#include <string>

namespace game {
	class io {
	public:
		/// <summary>
		/// Closes the internal file stream if it is currently open.
		/// </summary>
		static void close ();
		
		/// <summary>
		/// Opens the file stream. </summary>
		/// <param name="name">Directory of the file.</param>
		/// <param name="mode">Open mode for the file stream.</param>
		static void open (std::string name, std::ios_base::openmode mode);

		/// <summary>
		/// Reads the currently open file as a bloc file into the bloc reference. </summary>
		/// <remarks>
		/// The bloc object passed as the argument will be cleared prior to loading. </remarks>
		/// <param name="b">Reference to a bloc object.</param>
		static void read_bloc (bloc & b);
		
		/// <summary>
		/// Returns a string representing a line in the currently opened file. </summary>
		/// <param name="line">Line number of the file to be read.
		/// <returns>The requested line of the file as a string.</returns>
		static std::string read_line (int line); 

		/// <summary>
		/// Writes bloc object data to the currently opened file. </summary>
		static void write (bloc & b); // Writes bloc data into the file
		static void write_line (int line, std::string data); // Writes the line into the file, creating empty lines if needed

	private:
		static std::fstream file; // The singular file stream owned by the class - everything in the game is expected to use this
		static std::string loaded_file; // The name of the loaded file; used for debugging purposes
	};
}

#endif