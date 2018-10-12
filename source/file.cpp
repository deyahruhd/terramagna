#include <fstream>
#include <iostream>
#include <sstream>

#include <array>
#include <vector>

#include "file.h"

//#include "logger.h"

namespace game {
	std::fstream io::file;
	std::string io::loaded_file;

	void io::close (void) {
		file.close ();
	}

	void io::open (std::string path, std::ios_base::openmode mode) {
		file.open (path, mode);
		loaded_file = path;
	}

	/// <summary>
	/// Writes the specified b.
	/// </summary>
	/// <param name="b">The b.</param>
	void io::write (bloc & b) {
		if (! file.is_open ()) {
			std::cerr << "Error in IO: An error occured while writing bloc data." << std::endl;
			return;
		}

		vec3 bounds = b.get_bounds ();

		file << "bloc" << (char) bounds.x << (char) bounds.y << (char) bounds.z << (char) 255;

		for (int it = 0; it < (bounds.x * bounds.y * bounds.z); ++ it) {
			if (it == (bounds.x * bounds.y * bounds.z) - 1)
				file << b.get (it).get_value ();
			else
				file << b.get (it).get_value () << "~";
		}
	}
	void io::read_bloc (bloc & b) {
		if (!file.is_open ()) {
			//log_message (logger_errorsevere, "The file stream is not open", "While loading the file " + loaded_file + " an error occured, thus resulting in a bad file stream.\nFile operations for this file stream will therefore be invalid.");
			return;
		}

		file.seekg (0, file.end);
		int l = file.tellg ();
		file.seekg (0, file.beg);

		std::string header (9, '\0');
		file.read (&header [0], 8);
		std::string type = header.substr (0, 4);

		if (type != "bloc") {
			std::string mmsg = "The file" + loaded_file + "is either an unrecognized format or is not a bloc file";
			std::string smsg = "The file " + loaded_file + " was loaded as a bloc file, but was not recognized as a readable format and/or is not a bloc file.";

			//log_message (logger_errorwarning, mmsg, smsg);
			return;
		}

		std::string size = header.substr (4, 6);
		b.set_bounds (vec3 ((int) size.at (0), (int) size.at (1), (int) size.at (2)));
		b.clear ();

		std::string content (l - 7, '\0');
		file.read (&content [0], l - 8);
		std::vector <std::string> contents;
		int start = 0, end = 0;
		while ((end = content.find ('~', start)) != std::string::npos) {
			contents.push_back (content.substr (start, end - start));
			start = end + 1;
		}

		contents.push_back (content.substr (start));

		for (unsigned int it = 0; it < contents.size (); ++ it) {
			b.set (it, color (atoi (contents.at (it).c_str ())));
		}
	}
	
	// done
}