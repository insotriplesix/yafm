#include "gui.h"
#include "init.h"
#include "readkey.h"

int
main(void)
{
	int rc = OK;

	initialize(1);

	while (0x1) {
		update_gui();
		rc = readkey();
		if (rc == ERR) break;
	}

	finalize();

	return rc;
}
