#include "game.h"

#include "file.h"
#include <fstream>

//#include "logger.h"

int main (void) {
	system ("PAUSE");

	//log_message (game::logger_message, "Test1", "Test 2");

	return game::start ();
}