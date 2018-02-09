#include "fmanager.h"

char filename[FILENAME_MAX];

void
draw_window(enum win_t type)
{
	wattron(win[type], COLOR_PAIR(4));
	box(win[type], ACS_VLINE, ACS_HLINE);
	wattroff(win[type], COLOR_PAIR(4));

	switch (type) {
		case MENU_W:
			draw_menu(type);
			break;
		case LEFT_W:
			draw_left(type);
			break;
		case RITE_W:
			draw_rite(type);
			break;
		default:
			break;
	}

	wrefresh(win[type]);
}

void
draw_menu(enum win_t type)
{
	wbkgd(win[type], COLOR_PAIR(4));

	int offset = 2;

	wattron(win[type], COLOR_PAIR(1));
	mvwprintw(win[type], 1, offset, "  F4 - File  ");
	offset += 15;
	mvwprintw(win[type], 1, offset, "  F5 - ????  ");
	offset += 15;
	mvwprintw(win[type], 1, offset, "  F6 - Extra  ");
	offset += 16;
	mvwprintw(win[type], 1, offset, "  F7 - Help  ");
	offset += 15;
	mvwprintw(win[type], 1, offset, "  F8 - Exit  ");
	mvwprintw(win[type], 1, COLS - 20, " made by 5aboteur ");
	wattroff(win[type], COLOR_PAIR(1));
}

void
draw_left(enum win_t type)
{
	wbkgd(win[type], COLOR_PAIR(2));

	/* ... */
}

void
draw_rite(enum win_t type)
{
	wbkgd(win[type], COLOR_PAIR(2));

	/* ... */
}

int
change_theme(void)
{
	int fg_field, bg_field,
		fg_menu, bg_menu,
		fg_popup, bg_popup,
		fg_dir, bg_dir,
		fg_reg, bg_reg,
		fg_exec, bg_exec,
		fg_oth, bg_oth,
		fg_curs, bg_curs;

	int choice = change_theme_popup();
	if (choice == ERR) return ERR;

	switch ((char)choice) {
		case '0': // default
			fg_menu = COLOR_BLACK, bg_menu = COLOR_YELLOW;
			fg_field = COLOR_WHITE, bg_field = COLOR_BLUE;
			fg_popup = COLOR_YELLOW, bg_popup = COLOR_BLACK;
			fg_dir = COLOR_YELLOW, bg_dir = COLOR_BLUE;
			fg_reg = COLOR_WHITE, bg_reg = COLOR_BLUE;
			fg_exec = COLOR_GREEN, bg_exec = COLOR_BLUE;
			fg_oth = COLOR_MAGENTA, bg_oth = COLOR_BLUE;
			fg_curs = COLOR_WHITE, bg_curs = COLOR_RED;
			break;
		case '1': // leet
			fg_menu = COLOR_BLACK, bg_menu = COLOR_GREEN;
			fg_field = COLOR_GREEN, bg_field = COLOR_BLACK;
			fg_popup = COLOR_GREEN, bg_popup = COLOR_BLACK;
			fg_dir = COLOR_YELLOW, bg_dir = COLOR_BLACK;
			fg_reg = COLOR_WHITE, bg_reg = COLOR_BLACK;
			fg_exec = COLOR_GREEN, bg_exec = COLOR_BLACK;
			fg_oth = COLOR_MAGENTA, bg_oth = COLOR_BLACK;
			fg_curs = COLOR_BLACK, bg_curs = COLOR_GREEN;
			break;
		case '2': // icy
			fg_menu = COLOR_CYAN, bg_menu = COLOR_BLACK;
			fg_field = COLOR_MAGENTA, bg_field = COLOR_WHITE;
			fg_popup = COLOR_CYAN, bg_popup = COLOR_BLACK;
			fg_dir = COLOR_BLUE, bg_dir = COLOR_WHITE;
			fg_reg = COLOR_BLACK, bg_reg = COLOR_WHITE;
			fg_exec = COLOR_GREEN, bg_exec = COLOR_WHITE;
			fg_oth = COLOR_RED, bg_oth = COLOR_WHITE;
			fg_curs = COLOR_WHITE, bg_curs = COLOR_MAGENTA;
			break;
		case '3': // hell
			fg_menu = COLOR_BLACK, bg_menu = COLOR_RED;
			fg_field = COLOR_RED, bg_field = COLOR_BLACK;
			fg_popup = COLOR_RED, bg_popup = COLOR_BLACK;
			fg_dir = COLOR_YELLOW, bg_dir = COLOR_BLACK;
			fg_reg = COLOR_WHITE, bg_reg = COLOR_BLACK;
			fg_exec = COLOR_GREEN, bg_exec = COLOR_BLACK;
			fg_oth = COLOR_MAGENTA, bg_oth = COLOR_BLACK;
			fg_curs = COLOR_WHITE, bg_curs = COLOR_RED;
			break;
		default:
			return OK;
	}

	init_pair(1, fg_menu, bg_menu);
	wattrset(win[MENU_W], COLOR_PAIR(1));

	init_pair(2, fg_field, bg_field);
	wattrset(win[LEFT_W], COLOR_PAIR(2));

	init_pair(3, fg_popup, bg_popup);
	wattrset(win[RITE_W], COLOR_PAIR(3));


	init_pair(5, fg_dir, bg_dir);
	wattrset(win[LEFT_W], COLOR_PAIR(5));
	wattrset(win[RITE_W], COLOR_PAIR(5));

	init_pair(6, fg_reg, bg_reg);
	wattrset(win[LEFT_W], COLOR_PAIR(6));
	wattrset(win[RITE_W], COLOR_PAIR(6));

	init_pair(7, fg_exec, bg_exec);
	wattrset(win[LEFT_W], COLOR_PAIR(7));
	wattrset(win[RITE_W], COLOR_PAIR(7));

	init_pair(8, fg_oth, bg_oth);
	wattrset(win[LEFT_W], COLOR_PAIR(8));
	wattrset(win[RITE_W], COLOR_PAIR(8));

	init_pair(9, fg_curs, bg_curs);
	wattrset(win[LEFT_W], COLOR_PAIR(9));
	wattrset(win[RITE_W], COLOR_PAIR(9));

	return OK;
}

int
change_theme_popup(void)
{
	WINDOW *win;

	int win_height = 6;
	int win_width = 13;
	int line = 1;

	int offset_y = LINES / 2;
	int offset_x = COLS / 2;

	win = newpad(win_height, win_width);

	if (win == NULL) {
		perror("newpad");
		return ERR;
	}

	wattron(win, COLOR_PAIR(4));
	box(win, ACS_VLINE, ACS_HLINE);
	wattroff(win, COLOR_PAIR(4));
	wbkgd(win, COLOR_PAIR(3));

//	curs_set(0);

	wmove(win, line++, 1);
	waddstr(win, " [0] def  ");
	wmove(win, line++, 1);
	waddstr(win, " [1] leet ");
	wmove(win, line++, 1);
	waddstr(win, " [2] icy  ");
	wmove(win, line++, 1);
	waddstr(win, " [3] hell ");

	prefresh(win, 0, 0,
		offset_y - win_height / 2,
		offset_x - win_width / 2,
		offset_y + win_height / 2,
		offset_x + win_width / 2);

	int choice = wgetch(win);

	wclear(win);
	wrefresh(win);
	delwin(win);

//	curs_set(1);

	return choice;
}

int
get_extra(void)
{
	WINDOW *win;

	int win_height = 4;
	int win_width = 14;
	int line = 0;

	int offset_y = 2;
	int offset_x = 32;

	win = newpad(win_height, win_width);

	if (win == NULL) {
		perror("newpad");
		return ERR;
	}

	wbkgd(win, COLOR_PAIR(1));

//	curs_set(0);

	wmove(win, line++, 1);
	waddstr(win, "------------");
	wmove(win, line++, 1);
	waddstr(win, "^W - hexedit");
	wmove(win, line++, 1);
	waddstr(win, "^Y - livexor");
	wmove(win, line++, 1);
	waddstr(win, "^G - themez");

	prefresh(win, 0, 0,
		offset_y, offset_x,
		offset_y + win_height,
		offset_x + win_width);

	wgetch(win);

	wclear(win);
	wrefresh(win);
	delwin(win);

//	curs_set(1);

	return OK;
}

int
get_help(void)
{
	WINDOW *win;

	int win_height = 15;
	int win_width = 30;
	int line = 1;

	int offset_y = LINES / 2;
	int offset_x = COLS / 2;

	win = newpad(win_height, win_width);

	if (win == NULL) {
		perror("newpad");
		return ERR;
	}

	wattron(win, COLOR_PAIR(4));
	box(win, ACS_VLINE, ACS_HLINE);
	wattroff(win, COLOR_PAIR(4));
	wbkgd(win, COLOR_PAIR(3));

//	curs_set(0);

	wmove(win, line++, win_width / 3);
	waddstr(win, "YAFM v1.0");
	wmove(win, line++, 1);
	waddstr(win, "");
	wmove(win, line++, 1);
	waddstr(win, "--- cmd --------------------");
	wmove(win, line++, 1);
	waddstr(win, " F4, ^O - open file");
	wmove(win, line++, 1);
	waddstr(win, " F5, ^K - save to file");
	wmove(win, line++, 1);
	waddstr(win, " F6, ^E - extra options");
	wmove(win, line++, 1);
	waddstr(win, " F7, ^F - get help");
	wmove(win, line++, 1);
	waddstr(win, " F8, ^X - exit");
	wmove(win, line++, 1);
	waddstr(win, "");
	wmove(win, line++, 1);
	waddstr(win, "--- ext --------------------");
	wmove(win, line++, 1);
	waddstr(win, " ^G - change theme");
	wmove(win, line++, 1);
	waddstr(win, " ^Y - live encryption");

	prefresh(win, 0, 0,
		offset_y - win_height / 2,
		offset_x - win_width / 2,
		offset_y + win_height / 2,
		offset_x + win_width / 2);

	wgetch(win);

	wclear(win);
	wrefresh(win);
	delwin(win);

//	curs_set(1);

	return OK;
}

int
open_file(char **buf, int *size, bool from_arg)
{
	if (!from_arg && open_file_popup() == ERR) {
		perror("open_file_popup");
		return ERR;
	}

	FILE *fp = fopen(filename, "r");

	if (fp == NULL) {	// MOVE2 INFO BAR
		fprintf(stderr, "Error opening file '%s', skipped.", filename);
		return OK;
	}

	// Get the file length
	fseek(fp, 0L, SEEK_END);
	long len = ftell(fp);
	rewind(fp);

	if (len > BUFSIZ && len > (long)*size) {
		// Allocate more space for the buffer
		char *tmp = realloc(*buf, sizeof(char) * len + 1);

		if (tmp == NULL) {
			perror("realloc");
			fclose(fp);
			return ERR;
		}

		*buf = tmp;
	}

	char ch;
	int i = 0;

	while ((ch = fgetc(fp)) != EOF)
		(*buf)[i++] = ch;

	(*buf)[i] = '\0';

	*size = i;
	fclose(fp);

	return OK;
}

int
open_file_popup(void)
{
	WINDOW *win;

	int win_height = 3;
	int win_width = 42;

	int offset_y = LINES / 2;
	int offset_x = COLS / 2;

	win = newwin(win_height, win_width,
		offset_y - win_height / 2,
		offset_x - win_width / 2);

	if (win == NULL) {
		perror("newwin");
		return ERR;
	}

	wattron(win, COLOR_PAIR(4));
	box(win, ACS_VLINE, ACS_HLINE);
	wattroff(win, COLOR_PAIR(4));
	wbkgd(win, COLOR_PAIR(3));

	echo();

	mvwaddstr(win, 1, 1, " Enter file name: ");
	wrefresh(win);
	memset(filename, '\0', FILENAME_MAX);
	scrollok(win, TRUE);
	idlok(win, TRUE);
	nonl();
	mvwgetstr(win, 1, 19, filename);

	noecho();

	wclear(win);
	wrefresh(win);
	delwin(win);

	return OK;
}

int
save_file(char *buf, int size)
{
	if (save_file_popup() == ERR) {
		perror("save_file_popup");
		return ERR;
	}

	FILE *fp = fopen(filename, "w");

	if (fp == NULL) {	// MV2INFOBAR
		fprintf(stderr, "Error opening file '%s', skipped.", filename);
		return OK;
	}

	for (int i = 0; i < size; ++i)
		fputc(buf[i], fp);

	fclose(fp);

	return OK;
}

int
save_file_popup(void)
{
	WINDOW *win;

	int win_height = 3;
	int win_width = 42;

	int offset_y = LINES / 2;
	int offset_x = COLS / 2;

	win = newwin(win_height, win_width,
		offset_y - win_height / 2,
		offset_x - win_width / 2);

	if (win == NULL) {
		perror("newwin");
		return ERR;
	}

	wattron(win, COLOR_PAIR(4));
	box(win, ACS_VLINE, ACS_HLINE);
	wattroff(win, COLOR_PAIR(4));
	wbkgd(win, COLOR_PAIR(3));

	echo();

	mvwaddstr(win, 1, 1, " Enter file name: ");
	wrefresh(win);
	memset(filename, '\0', FILENAME_MAX);
	mvwgetstr(win, 1, 19, filename);

	noecho();

	wclear(win);
	wrefresh(win);
	delwin(win);

	return OK;
}

int
fcmpr(const void *a, const void *b)
{
	const char *f1 = *(const char **) a;
	const char *f2 = *(const char **) b;

	if (f1[0] == '/' && f1[1] == '.' && f1[2] == '\0')
		return 1;
	if (f2[0] == '/' && f2[1] == '.' && f2[2] == '\0')
		return -1;
	else if (f1[0] == '/')
		return -1;
	else if (f2[0] == '/')
		return 1;
	else if (f1[0] == '~')
		return -1;
	else if (f2[0] == '~')
		return 1;
	else if (f1[0] == '*')
		return strcmp(++f1, f2);
	else if (f2[0] == '*')
		return strcmp(f1, ++f2);
	else
		return strcmp(f1, f2);
}

int
grab_files(enum win_t active)
{
	struct dirent **namelist;
	char prefix[2];

	struct stat finfo;
	char fpath[PATH_MAX];
	int fcount = 0;

	fcount = scandir(content[active].path, &namelist, NULL, alphasort);

	if (fcount < 0) {
		perror("scandir");
		return ERR;
	}

	char **files = malloc(sizeof(char *) * fcount);
	for (int i = 0; i < fcount; ++i)
		files[i] = calloc(FILENAME_MAX, sizeof(char));

	for (int i = fcount - 1; i >= 0; --i) {
		sprintf(fpath, "%s/%s", content[active].path, namelist[i]->d_name);
		lstat(fpath, &finfo);

		if (S_ISDIR(finfo.st_mode))
			prefix[0] = '/';
		else if (S_ISLNK(finfo.st_mode))
			prefix[0] = '~';
		else if (S_ISREG(finfo.st_mode) && finfo.st_mode & S_IXUSR)
			prefix[0] = '*';
		else
			prefix[0] = ' ';

		prefix[1] = '\0';

		sprintf(files[i], "%s%s", prefix, namelist[i]->d_name);
		free(namelist[i]);
	}

	qsort(files, fcount, sizeof(char*), fcmpr);

	struct list_t *list = &content[active].files;
	list_mem_free(list);

	for (int i = 0; i < fcount - 1; ++i)
		list_add_data(list, i + 1, files[i], strlen(files[i]));

	content[active].count = fcount - 1;

	for (int i = 0; i < fcount; ++i)
		free(files[i]);

	free(files);
	free(namelist);

	return OK;
}

int
dim_cursor(void)
{
	mvwchgat(win[ACTIVE_W], content[ACTIVE_W].y_pos, content[ACTIVE_W].x_pos,
		COLS / 2 - 2, A_DIM | A_REVERSE, CURSOR_CLR, NULL);

	return OK;
}

int
set_default_attr(void)
{
	int color;
	char *data = list_find_data(&content[ACTIVE_W].files, content[ACTIVE_W].y_pos);

	if (data == NULL)
		return OK;

	switch (data[0]) {
	case '/':
		color = 5;
		break;
	case '*':
		color = 7;
		break;
	case '~':
		color = 8;
		break;
	default:
		color = 6;
		break;
	}

	mvwchgat(win[ACTIVE_W], content[ACTIVE_W].y_pos, content[ACTIVE_W].x_pos,
		COLS / 2 - 2, A_NORMAL, color, NULL);

	return OK;
}

int
show_files(enum win_t active)
{
	struct node_t *head = content[active].files.head;
	int scroll_offset = content[active].y_off;

	if (content[active].y_off) {
		for (int i = 0; i < scroll_offset && head; ++i)
			head = head->next;
	}

	int line = DEFPOS_Y;

	for (int i = 0; i < LINES - 5 && head; ++i) {
		int color;
		char *data = head->data;

		switch (data[0]) {
		case '/':
			color = 5;
			break;
		case '*':
			color = 7;
			break;
		case '~':
			color = 8;
			break;
		default:
			color = 6;
			break;
		}

		wattron(win[active], COLOR_PAIR(color));
		mvwprintw(win[active], line++, 1, "%s", data);
		wattroff(win[active], COLOR_PAIR(color));
		head = head->next;
	}

	wrefresh(win[active]);

	return OK;
}

int
display_content(enum win_t active)
{
	return grab_files(active) & show_files(active);
}

int
change_dir_to(char *dirname)
{
	char newpath[PATH_MAX];
	sprintf(newpath, "%s/%s", content[ACTIVE_W].path, dirname);

	if (chdir(newpath) == ERR)
		return ERR;

	wclear(win[ACTIVE_W]);
	draw_window(ACTIVE_W);

	getcwd(content[ACTIVE_W].path, PATH_MAX);
	display_content(ACTIVE_W);

	content[ACTIVE_W].y_pos = 1;
	content[ACTIVE_W].x_pos = 1;

	return OK;
}

void
finalize(void)
{
	for (int i = 0; i < NWINDOWS; ++i)
		delwin(win[i]);

	endwin();
}

int
exec_prog(char *progname)
{
	char prog[PATH_MAX + FILENAME_MAX];
	sprintf(prog, "%s/%s", content[ACTIVE_W].path, ++progname);
	return execl(prog, prog, (char *) NULL);
}
