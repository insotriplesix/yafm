/********************************************************************
 * PROGRAM: yafm
 * FILE: manager.c
 * PURPOSE: main file manager functionality
 * AUTHOR: 5aboteur <5aboteur@protonmail.com>
 *******************************************************************/

#include "init.h"
#include "gui.h"
#include "manager.h"
#include "movement.h"

/*
 * Function: enable_raw_mode
 * -------------------------
 * Description:
 *  Enables the "raw" mode for the terminal. By default most of the
 *  terminals start in canonical or "cooked" mode and your program can
 *  get the keyboard input only if user presses the 'Enter' button.
 *  "Raw" mode allows you to process each keypress as it comes in.
 */

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

/*
 * Function: disable_raw_mode
 * --------------------------
 * Description:
 *  Disables the "raw" mode for the terminal.
 */

void
disable_raw_mode(void)
{
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &term_attr);
}

/*
 * Function: display_content
 * -------------------------
 * Description:
 *  Prints the content to the screen.
 *
 * Arguments:
 *  'active' - a window where the content needs to be displayed.
 *
 * Returns:
 *  OK (0) if 'grab_files' & 'show_files' are not failed,
 *  ERR (-1) otherwise.
 */

int
display_content(enum win_t active)
{
	return grab_files(active) & show_files(active);
}

/*
 * Function: grab_files
 * --------------------
 * Description:
 *  Scans current directory and adds all files into the linked list.
 *
 * Arguments:
 *  'active' - current window directory.
 *
 * Asserts:
 *  'malloc' & 'calloc' won`t return NULL.
 *
 * Returns:
 *  OK (0) if no errors occurred, ERR (-1) if 'scandir' failed.
 */

int
grab_files(enum win_t active)
{
	struct dirent **namelist;

	/* We want to know the type of the file, so we`ll
	 * keep it in 'prefix' array. */
	char prefix[2];

	struct stat finfo;
	int fcount = 0;

	fcount = scandir(content[active].path, &namelist,
		NULL, alphasort);

	if (fcount < 0) {
		perror("scandir");	// 2STATUSBAR
		return ERR;
	}

	char **files = malloc(sizeof(char *) * fcount);

	assert(files != NULL);

	for (int i = 0; i < fcount; ++i) {
		files[i] = calloc(FILENAME_MAX, sizeof(char));
		assert(files[i] != NULL);
	}

	for (int i = fcount - 1; i >= 0; --i) {
		char *fpath = malloc(sizeof(char) * (PATH_MAX + 1));
		assert(fpath != NULL);

		sprintf(fpath, "%s/%s", content[active].path,
			namelist[i]->d_name);

		lstat(fpath, &finfo);

		// Get the type of the file (exec, link...)
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

		free(fpath);
		free(namelist[i]);
	}

	qsort(files, fcount, sizeof(char*), fcmpr);

	struct list_t *list = &content[active].files;

	list_mem_free(list);

	// 'fcount - 1' excludes the last file in the list -> '.'
	for (int i = 0; i < fcount - 1; ++i)
		list_add_data(list, i + 1, files[i], strlen(files[i]));

	content[active].count = fcount - 1;

	for (int i = 0; i < fcount; ++i)
		free(files[i]);

	free(files);
	free(namelist);

	return OK;
}

/*
 * Function: show_files
 * --------------------
 * Description:
 *  Gets files from the list and prints them out.
 *
 * Arguments:
 *  'active' - a window where the content needs to be displayed.
 *
 * Asserts:
 *  'malloc' won`t return NULL.
 *
 * Returns:
 *  Always OK.
 */

int
show_files(enum win_t active)
{
	struct node_t *head = content[active].files.head;
	int scroll_offset = content[active].y_off;

	for (int i = 0; i < scroll_offset && head; ++i)
		head = head->next;

	int line = DEFPOS_Y;
	long len, halfw = COLS / 2;

	for (int i = 0; i < LIMIT_Y && head; ++i) {
		int color;
		char *data = head->data;

		switch (data[0]) {
			case '/': color = 5; break;
			case '*': color = 7; break;
			case '~': color = 8; break;
			default:  color = 6; break;
		}

		wattron(win[active], COLOR_PAIR(color));

		char *name = malloc(sizeof(char) * (FILENAME_MAX + 1));
		assert(name != NULL);

		sprintf(name, "%s", data);

		len = strlen(name);

		// Don`t show full name if it`s go out of borders
		if ((len / (float)halfw * 100.f) > 85) {
			name[halfw - 8] = '.';
			name[halfw - 7] = '.';
			name[halfw - 6] = '.';
			name[halfw - 5] = '\0';
		}

		mvwprintw(win[active], line++, 1, "%s", name);
		wattroff(win[active], COLOR_PAIR(color));
		head = head->next;

		free(name);
	}

	// Display current path on the frame
	wattron(win[active], BORDER_CLR);
	mvwhline(win[active], 0, 3, ACS_HLINE, COLS / 2 - 6);

	len = strlen(content[active].path);

	char *path = malloc(sizeof(char) * (len + 1));

	assert(path != NULL);

	// Don`t show full path if it`s go out of borders
	if ((len / (float)halfw * 100.f) > 85) {
		strncpy(path, content[active].path,
			sizeof(char) * (halfw - 11));
		path[halfw - 11] = '.';
		path[halfw - 10] = '.';
		path[halfw - 9] = '.';
		path[halfw - 8] = '\0';
	} else {
		sprintf(path, "%s", content[active].path);
	}

	mvwprintw(win[active], 0, 3, " %s ", path);
	wattroff(win[active], BORDER_CLR);
	wrefresh(win[active]);

	free(path);

	return OK;
}

/*
 * Function: dim_cursor
 * --------------------
 * Description:
 *  Dims the cursor.
 */

void
dim_cursor(void)
{
	mvwchgat(win[ACTIVE_W], content[ACTIVE_W].y_pos,
		content[ACTIVE_W].x_pos, LIMIT_X,
		A_DIM | A_REVERSE, CURSOR_C, NULL);
}

/*
 * Function: set_default_attr
 * --------------------------
 * Description:
 *  Sets default attributes to the end of a line.
 */

void
set_default_attr(void)
{
	int color;
	char *data = list_find_data(&content[ACTIVE_W].files,
		content[ACTIVE_W].y_pos + content[ACTIVE_W].y_off);

	if (data == NULL) return;

	switch (data[0]) {
		case '/': color = 5; break;
		case '*': color = 7; break;
		case '~': color = 8; break;
		default:  color = 6; break;
	}

	mvwchgat(win[ACTIVE_W], content[ACTIVE_W].y_pos,
		content[ACTIVE_W].x_pos, LIMIT_X, A_NORMAL,
		color, NULL);
}

/*
 * Function: perform_action
 * ------------------------
 * Description:
 *  Calls a function of the corresponding action.
 *
 * Arguments:
 *  'action' - current action.
 */

void
perform_action(char *action)
{
	if (strcmp(action, "paste_file") == 0) {
		paste_file();
		return;
	} else if (strcmp(action, "create_file") == 0) {
		create_file();
		return;
	} else if (strcmp(action, "make_dir") == 0) {
		make_dir();
		return;
	}

	int status;
	char *name = list_find_data(&content[ACTIVE_W].files,
		content[ACTIVE_W].y_pos);

	if (name == NULL) return; // no match

	curs_set(1);

	switch (name[0]) {
		case '/':
			if (strcmp(action, "remove_dir") == 0) {
				set_default_attr();
				remove_dir(name);
				move_up();
			} else {
				set_default_attr();
				change_dir_to(name);
			}
			break;
		case '*':
			if (fork() == 0) {
				exec_prog(name);
			} else {
				wait(&status);
				restore_windows();
			}
			break;
		case '~':
			// Do nothing club
			break;
		default:
			if (strcmp(action, "edit_file") == 0) {
				if (fork() == 0) {
					edit_file(name);
				} else {
					wait(&status);
					restore_windows();
				}
			} else if (strcmp(action, "copy_file") == 0) {
				copy_file(name);
			} else if (strcmp(action, "remove_file") == 0) {
				set_default_attr();
				remove_file(name);
				move_up();
			}
	}

	curs_set(0);
}

/*
 * Function: copy_file
 * -------------------
 * Description:
 *  Copies a full path to the file into the copy buffer.
 *
 * Arguments:
 *  'name' - name of the file.
 */

void
copy_file(char *name)
{
	sprintf(copy_buffer, "%s/%s", content[ACTIVE_W].path, ++name);
}

/*
 * Function: create_file
 * ---------------------
 * Description:
 *  Creates an empty file and repaints file manager windows.
 *
 * Asserts:
 *  'malloc' won`t return NULL.
 */

void
create_file(void)
{
	char *fname = malloc(sizeof(char) * (FILENAME_MAX + 1));

	assert(fname != NULL);

	create_file_popup(fname);

	FILE *fp = fopen(fname, "w");
	if (fp == NULL) {
		// status
		free(fname);
		return;
	}

	for (int i = 0; i < NWINDOWS; ++i)
		repaint_window(i);

	if (display_content(LEFT_W) | display_content(RITE_W)) {
		//status
	}

	fclose(fp);
	free(fname);
}

/*
 * Function: create_file_from_buf
 * ------------------------------
 * Description:
 *  Creates the copy of a file from 'copy_buffer', writes its content
 *  to that copy and repaints file manager windows.
 *
 * Arguments:
 *  'name' - name of the file ('copy_buffer' string).
 *
 * Asserts:
 *  'malloc' won`t return NULL.
 */

void
create_file_from_buf(char *name)
{
	char *file_name = malloc(sizeof(char) * (FILENAME_MAX + 1));

	assert(file_name != NULL);

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
	if (fin == NULL) {
		// statusbar
		free(file_name);
		return;
	}

	FILE *fout = fopen(file_name, "w");
	if (fout == NULL) {
		// statusbar
		fclose(fin);
		free(file_name);
		return;
	}

	char *buf = malloc(sizeof(char) * BUFSIZ);

	assert(buf != NULL);

	size_t nbytes;

	while ((nbytes = fread(buf, sizeof(char), BUFSIZ, fin)))
		fwrite(buf, sizeof(char), nbytes, fout);

	fclose(fin);
	fclose(fout);

	free(file_name);
	free(buf);

	for (int i = 0; i < NWINDOWS; ++i)
		repaint_window(i);

	if (display_content(LEFT_W) | display_content(RITE_W)) {
		// status
	}
}

/*
 * Function: edit_file
 * -------------------
 * Description:
 *  Runs the YATE editor and passes the filename as an argument.
 *
 * Arguments:
 *  'name' - name of the file.
 */

void
edit_file(char *name)
{
	char file_path[PATH_MAX + FILENAME_MAX + 1];
	sprintf(file_path, "%s/%s", content[ACTIVE_W].path, ++name);
	if (execl(EDITOR_PATH, EDITOR_PATH, file_path, (char *) NULL) < 0) {
		// status
	}
}

/*
 * Function: exec_prog
 * -------------------
 *  Runs the binary file and passes the filename as an argument.
 *
 * Arguments:
 *  'name' - name of the file.
 */

void
exec_prog(char *name)
{
	char prog[PATH_MAX + FILENAME_MAX + 1];
	sprintf(prog, "%s/%s", content[ACTIVE_W].path, ++name);
	if (execl(prog, prog, (char *) NULL) < 0) {
		// status
	}
}

/*
 * Function: paste_file
 * --------------------
 * Description:
 *  Creates the copy of a file via 'create_file_from_buf' func.
 */

void
paste_file(void)
{
	create_file_from_buf(copy_buffer);
}

/*
 * Function: remove_file
 * ---------------------
 * Description:
 *  Removes a file.
 *
 * Arguments:
 *  'name' - name of the file.
 */

void
remove_file(char *name)
{
	char *file_path = malloc(sizeof(char) *
		(PATH_MAX + FILENAME_MAX + 1));

	assert(file_path != NULL);

	sprintf(file_path, "%s/%s", content[ACTIVE_W].path, ++name);

	if (unlink(file_path) == ERR) {
		// statusbar
		free(file_path);
		return;
	}

	free(file_path);

	for (int i = 0; i < NWINDOWS; ++i)
		repaint_window(i);

	if (display_content(LEFT_W) | display_content(RITE_W)) {
		// statusbar
	}
}

/*
 * Function: change_dir_to
 * -----------------------
 * Description:
 *  Changes cwd to a 'name' dir.
 *
 * Arguments:
 *  'name' - name of the dir.
 */

void
change_dir_to(char *name)
{
	char *new_path = malloc(sizeof(char) * (PATH_MAX + 1));

	assert(new_path != NULL);

	sprintf(new_path, "%s/%s", content[ACTIVE_W].path, name);

	if (chdir(new_path) == ERR) {
		// statusbar
		free(new_path);
		return;
	}

	free(new_path);

	wclear(win[ACTIVE_W]);
	draw_window(ACTIVE_W);

	getcwd(content[ACTIVE_W].path, PATH_MAX);
	display_content(ACTIVE_W);

	content[ACTIVE_W].y_pos = DEFPOS_Y;
	content[ACTIVE_W].x_pos = DEFPOS_X;
}

/*
 * Function: make_dir
 * ------------------
 * Description:
 *  Creates a dir.
 */

void
make_dir(void)
{
	char *dname = malloc(sizeof(char) * (FILENAME_MAX + 1));

	assert(dname != NULL);

	make_dir_popup(dname);

	if (mkdir(dname, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) == ERR) {
		// status
		free(dname);
		return;
	}

	free(dname);

	for (int i = 0; i < NWINDOWS; ++i)
		repaint_window(i);

	if (display_content(LEFT_W) | display_content(RITE_W)) {
		// status
	}
}

/*
 * Function: remove_dir
 * --------------------
 * Description:
 *  Removes a dir.
 *
 * Arguments:
 *  'name' - name of the dir.
 */

void
remove_dir(char *name)
{
	char *cmd = malloc(sizeof(char) * (FILENAME_MAX + 8));

	assert(cmd != NULL);

	sprintf(cmd, "rm -rf %s", ++name);
	system(cmd);

	free(cmd);

	for (int i = 0; i < NWINDOWS; ++i)
		repaint_window(i);

	if (display_content(LEFT_W) | display_content(RITE_W)) {
		// status
	}
}

/*
 * Function: get_config
 * --------------------
 * Description:
 *  Gets CWD and appends to it the '.config' string.
 *
 * Asserts
 *  'malloc' won`t return NULL.
 */

void
get_config(void)
{
	size_t sz = sizeof(char) * (PATH_MAX + 1);
	char *cwd = malloc(sz);

	assert(cwd != NULL);

	if (getcwd(cwd, sz) == NULL) {
//		set_status(1, "config path not defined");
		free(cwd);
		return;
	}

	sprintf(CONFIG_PATH, "%s/%s", cwd, ".config");

	free(cwd);
}

/*
 * Function: load_config
 * ---------------------
 * Description:
 *  Loads the config file and parses its attributes.
 */

void
load_config(void)
{
	FILE *fp = fopen(CONFIG_PATH, "r");
	if (fp == NULL)	{
//		set_status(1, "open config file");
		return;
	}

	char *line = NULL;
	size_t sz = 0;
	ssize_t len;
	char theme = '0';

	while ((len = getline(&line, &sz, fp)) != ERR) {
		if ((strlen(line) >= 7) && (strncmp(line, "theme", 5) == 0)) {
			theme = line[6];
			current_theme = theme;
			change_theme(0);
		}
	}

	free(line);
	fclose(fp);
}

/*
 * Function: save_config
 * ---------------------
 * Description:
 *  Saves current editor configuration to the config file.
 */

void
save_config(void)
{
	FILE *fp = fopen(CONFIG_PATH, "w");
	if (fp == NULL)	{
//		set_status(1, "open config file");
		return;
	}

	fprintf(fp, "theme %c\n", current_theme);
	fclose(fp);
}

/*
 * Function: init_content
 * ----------------------
 * Description:
 *  Initializes editor content with default values.
 */

void
init_content(void)
{
	for (int i = 0; i < NWINDOWS; ++i) {
		content[i].x_pos = 1;
		content[i].y_pos = 1;
		content[i].count = 0;
		content[i].y_off = 0;
		getcwd(content[i].path, PATH_MAX);
		list_init(&content[i].files);
	}
}


/*
 * Function: fcmpr
 * ---------------
 * Description:
 *  Filenames comparer func (used by qsort func).
 *
 * Arguments:
 *  'a' - left operand;
 *  'b' - right operand.
 *
 * Returns:
 *  -1 if a > b,
 *   1 if b > a,
 *   0 otherwise.
 */

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

/*
 * Function: get_editor_path
 * -------------------------
 * Description:
 *  Gets CWD and appends to it editor path.
 */

void
get_editor_path(void)
{
	size_t sz = sizeof(char) * (PATH_MAX + 1);
	char *cwd = malloc(sz);

	assert(cwd != NULL);

	if (getcwd(cwd, sz) == NULL) {
		// status
		free(cwd);
		return;
	}

	sprintf(EDITOR_PATH, "%s/%s", cwd, "editor/yate");

	free(cwd);
}

