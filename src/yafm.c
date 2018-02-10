#include "gui.h"
#include "init.h"
#include "manager.h"
#include "readkey.h"

// TODO 1: config parser
// TODO 2: better gui
// TODO 3: attach yate
// TODO 4: extra options
// TODO 5: file options
// TODO 6: dir options
// TODO 7: valgrind memcheck
// TODO 8: fix scroll painting

int
main(void)
{
	int rc = OK;

	initialize();

	while (0x1) {
		rc = readkey();
		if (rc == ERR) break;
		update_gui();
	}

	finalize();

	return rc;
}
