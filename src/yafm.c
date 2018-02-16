#include "gui.h"
#include "init.h"
#include "readkey.h"

// TODO 0: disable control keys
// TODO 1: file options
// TODO 2: dir options
// TODO 3: valgrind -v --leak-check=full --show-leak-kinds=all ./yafm

int
main(void)
{
	int rc = OK;

	initialize();

	while (0x1) {
		update_gui();
		rc = readkey();
		if (rc == ERR) break;
	}

	finalize();

	return rc;
}
