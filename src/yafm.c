#include "gui.h"
#include "init.h"
#include "readkey.h"

// TODO 1: fix scroll painting
// TODO 2: better gui
// TODO 3: attach yate
// TODO 4: extra options
// TODO 5: file options
// TODO 6: dir options
// TODO 7: valgrind -v --leak-check=full --show-leak-kinds=all ./yafm

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
