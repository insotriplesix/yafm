#include "init.h"
#include "gui.h"
#include "manager.h"

void
enable_raw_mode(void)
{
	tcgetattr(STDIN_FILENO, &term_attr);

	struct termios raw = term_attr;

	raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
	raw.c_oflag &= ~(OPOST);
	raw.c_cflag |= (CS8);
	raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);

	tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

void
disable_raw_mode(void)
{
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &term_attr);
}

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
		content[ACTIVE_W].y_pos + content[ACTIVE_W].y_off);

	if (data == NULL)
		return OK; // it's not OK but just skip that

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
get_editor_path(void)
{
	char cwd[PATH_MAX];

	if (getcwd(cwd, sizeof(cwd)) == NULL) {
		perror("getcwd");
		return ERR;
	}

	snprintf(EDITOR_PATH, sizeof(EDITOR_PATH), "%s/%s", cwd,
		"editor/yate");

	return OK;
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

	for (int i = 0; i < scroll_offset && head; ++i)
		head = head->next;

	int line = DEFPOS_Y;
	long len, halfw = COLS / 2;

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

		char name[FILENAME_MAX];
		sprintf(name, "%s", data);

		len = strlen(name);

		// Don`t show full name if it is out of borders
		if ((len / (float)halfw * 100.f) > 85) {
			name[halfw - 8] = '.';
			name[halfw - 7] = '.';
			name[halfw - 6] = '.';
			name[halfw - 5] = '\0';
		}

		mvwprintw(win[active], line++, 1, "%s", name);
		wattroff(win[active], COLOR_PAIR(color));
		head = head->next;
	}

	// Display path
	wattron(win[active], BORDER_CLR);
	mvwhline(win[active], 0, 3, ACS_HLINE, COLS / 2 - 6);

	len = strlen(content[active].path);

	char path[PATH_MAX];

	// Don`t show full path if it is out of borders
	if ((len / (float)halfw * 100.f) > 85) {
		strncpy(path, content[active].path,
			sizeof(char) * (halfw - 11));
		path[halfw - 11] = '.';
		path[halfw - 10] = '.';
		path[halfw - 9] = '.';
		path[halfw - 8] = '\0';
	} else
		sprintf(path, "%s", content[active].path);

	mvwprintw(win[active], 0, 3, " %s ", path);
	wattroff(win[active], BORDER_CLR);

	wrefresh(win[active]);

	return OK;
}

int
perform_action(char *action)
{
	if (strcmp(action, "paste_file") == 0)
		return paste_file();
	else if (strcmp(action, "create_file") == 0)
		return create_file_popup();
	else if (strcmp(action, "make_dir") == 0)
		return make_dir_popup();

	int rc = OK, status;
	char *name = list_find_data(&content[ACTIVE_W].files,
		content[ACTIVE_W].y_pos);

	if (name == NULL) // output err
		return ERR;

	curs_set(1);

	switch (name[0]) {
		case '/':
			if (strcmp(action, "remove_dir") == 0) {
				rc = remove_dir(name);
			} else {
				set_default_attr();
				rc = change_dir_to(name);
			}
			break;
		case '*':
			if (fork() == 0) {
				rc = exec_prog(name);
			} else {
				wait(&status);
				finalize();
				restore_windows();
			}
			break;
		case '~':
			break;
		default:
			if (strcmp(action, "edit_file") == 0) {
				if (fork() == 0) {
					rc = edit_file(name);
				} else {
					wait(&status);
					finalize();
					restore_windows();
				}
			} else if (strcmp(action, "copy_file") == 0) {
				copy_file(name);
			} else if (strcmp(action, "remove_file") == 0) {
				rc = remove_file(name);
			}
			break;
	}

	curs_set(0);
	return rc;
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
make_dir(char *name)
{
	int rc = OK;

	rc = mkdir(name, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
	if (rc == ERR) return rc;

	for (int i = 0; i < NWINDOWS; ++i)
		repaint_window(i);

	if (display_content(LEFT_W) | display_content(RITE_W))
		return ERR;

	return rc;
}

int
remove_dir(char *name)
{
	// TMP STUFF
	char cmd[FILENAME_MAX];

	sprintf(cmd, "rm -rf %s", ++name);
	system(cmd);

	for (int i = 0; i < NWINDOWS; ++i)
		repaint_window(i);

	if (display_content(LEFT_W) | display_content(RITE_W))
		return ERR;

	return OK;
}

void
copy_file(char *name)
{
	sprintf(copy_buffer, "%s/%s", content[ACTIVE_W].path, ++name);
}

int
create_file(char *name)
{
	FILE *fp = fopen(name, "w");
	if (fp == NULL) return OK;	// not OK though

	for (int i = 0; i < NWINDOWS; ++i)
		repaint_window(i);

	if (display_content(LEFT_W) | display_content(RITE_W))
		return ERR;

	fclose(fp);
	return OK;
}

int
create_file_from_buf(char *name)
{
	int rc = OK;

	char file_name[FILENAME_MAX];

	size_t len = strlen(name);
	size_t last_slash_idx = 0;

	for (size_t i = 0; i < len; ++i) {
		if (name[i] == '/')
			last_slash_idx = i;
	}

	size_t file_name_len = len - last_slash_idx;

	strncpy(file_name, name + last_slash_idx + 1, file_name_len);
	file_name[file_name_len] = '\0';

	FILE *fin = fopen(name, "r");
	if (fin == NULL) return rc;

	FILE *fout = fopen(file_name, "w");
	if (fout == NULL) { fclose(fin); return rc; }

	char buf[BUFSIZ];
	size_t nbytes;

	while ((nbytes = fread(buf, sizeof(char), BUFSIZ, fin)))
		fwrite(buf, sizeof(char), nbytes, fout);

	fclose(fin);
	fclose(fout);

	for (int i = 0; i < NWINDOWS; ++i)
		repaint_window(i);

	if (display_content(LEFT_W) | display_content(RITE_W))
		return ERR;

	return rc;
}

int
edit_file(char *name)
{
	char file_path[PATH_MAX + FILENAME_MAX + 1];
	sprintf(file_path, "%s/%s", content[ACTIVE_W].path, ++name);
	return execl(EDITOR_PATH, EDITOR_PATH, file_path, (char *) NULL);
}

int
exec_prog(char *name)
{
	char prog[PATH_MAX + FILENAME_MAX + 1];
	sprintf(prog, "%s/%s", content[ACTIVE_W].path, ++name);
	return execl(prog, prog, (char *) NULL);
}

int
paste_file(void)
{
	return create_file_from_buf(copy_buffer);
}

int
remove_file(char *name)
{
	int rc = OK;
	char file_path[PATH_MAX + FILENAME_MAX + 1];

	sprintf(file_path, "%s/%s", content[ACTIVE_W].path, ++name);

	rc = unlink(file_path);
	if (rc == ERR) return rc;

	for (int i = 0; i < NWINDOWS; ++i)
		repaint_window(i);

	if (display_content(LEFT_W) | display_content(RITE_W))
		return ERR;

	return rc;
}
