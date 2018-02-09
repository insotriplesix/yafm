#include "gui.h"
#include "manager.h"

void
dim_cursor(void)
{
	mvwchgat(win[ACTIVE_W], content[ACTIVE_W].y_pos,
		content[ACTIVE_W].x_pos, COLS / 2 - 2,
		A_DIM | A_REVERSE, CURSOR_C, NULL);
}

/* Set default attributes to the eol */
int
set_default_attr(void)
{
	int color;
	char *data = list_find_data(&content[ACTIVE_W].files,
		content[ACTIVE_W].y_pos);

	if (data == NULL)
		return OK;		// it's no OK but just skip that

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

	mvwchgat(win[ACTIVE_W], content[ACTIVE_W].y_pos,
		content[ACTIVE_W].x_pos, COLS / 2 - 2, A_NORMAL,
		color, NULL);

	return OK;
}

/* Files comparer func */
int
fcmpr(const void *a, const void *b)
{
	const char *f1 = *(const char **) a;
	const char *f2 = *(const char **) b;

	// Move '.' to the end of a list to exclude it then
	if (f1[0] == '/' && f1[1] == '.' && f1[2] == '\0')
		return 1;
	if (f2[0] == '/' && f2[1] == '.' && f2[2] == '\0')
		return -1;

	if (f1[0] == '/')
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
display_content(enum win_t active)
{
	return grab_files(active) & show_files(active);
}

int
grab_files(enum win_t active)
{
	struct dirent **namelist;
	char prefix[2];

	struct stat finfo;
	char fpath[PATH_MAX];
	int fcount = 0;

	fcount = scandir(content[active].path, &namelist,
		NULL, alphasort);

	if (fcount < 0) {
		perror("scandir");
		return ERR;
	}

	char **files = malloc(sizeof(char *) * fcount);
	for (int i = 0; i < fcount; ++i)
		files[i] = calloc(FILENAME_MAX, sizeof(char));

	for (int i = fcount - 1; i >= 0; --i) {
		sprintf(fpath, "%s/%s", content[active].path,
			namelist[i]->d_name);

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

	// fcount - 1 excludes the last file in the list -> '.'
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
change_dir_to(char *name)
{
	char new_path[PATH_MAX];
	sprintf(new_path, "%s/%s", content[ACTIVE_W].path, name);

	if (chdir(new_path) == ERR)
		return ERR;

	wclear(win[ACTIVE_W]);
	draw_window(ACTIVE_W);

	getcwd(content[ACTIVE_W].path, PATH_MAX);
	display_content(ACTIVE_W);

	content[ACTIVE_W].y_pos = DEFPOS_Y;
	content[ACTIVE_W].x_pos = DEFPOS_X;

	return OK;
}

int
edit_file(char *name)
{
	char editor_path[PATH_MAX + FILENAME_MAX];
	char file_path[PATH_MAX + FILENAME_MAX];
	sprintf(editor_path, "/home/saboteur/Programming/github/YATE/yate");
	sprintf(file_path, "%s/%s", content[ACTIVE_W].path, ++name);
	mvwprintw(win[ACTIVE_W], 0, 10, "%s", file_path);
	return execl(editor_path, editor_path, file_path, (char *) NULL);
}

int
exec_prog(char *name)
{
	char prog[PATH_MAX + FILENAME_MAX];
	sprintf(prog, "%s/%s", content[ACTIVE_W].path, ++name);
	return execl(prog, prog, (char *) NULL);
}
