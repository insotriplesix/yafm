#include "gui.h"
#include "init.h"
#include "readkey.h"

int
main(void)
{
	initialize(1);

	while (0x1) {
		update_gui();
		readkey();
	}

	finalize(1);

	return 0;
}
