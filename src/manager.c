#include "init.h"
#include "gui.h"
#include "manager.h"
/*
inline void __attribute__ ((always_inline))
dim_cursor(void)
{
	mvwchgat(win[ACTIVE_W], content[ACTIVE_W].y_pos,
		content[ACTIVE_W].x_pos, COLS / 2 - 2,
		A_DIM | A_REVERSE, CURSOR_C, NULL);
}
*/
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

char **
split_s(char *str, const char delim)
{
	char **result = NULL;
	char *tmp = str;
	char *last_delim = NULL;
	size_t count = 0;

	char d[2] = { delim, '\0' };

	while (*tmp) {
		if (delim == *tmp) {
			count++;
			last_delim = tmp;
		}

		tmp++;
	}

	count += last_delim < (str + strlen(str) - 1);
	count++;

	result = malloc(sizeof(char *) * count);

	if (result) {
		size_t idx = 0;
		char *tok = strtok(str, d);

		while (tok) {
			assert(idx < count);
			*(result + idx++) = strdup(tok);
			tok = strtok(0, d);
		}

		assert (idx == count - 1);
		*(result + idx) = 0;
	}

	return result;
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
perform_action(char *action)
{
	int rc = OK, status;
	char *name = list_find_data(&content[ACTIVE_W].files,
		content[ACTIVE_W].y_pos);

	if (name == NULL) // output err
		return ERR;

	curs_set(1);

	switch (name[0]) {
		case '/':
			set_default_attr();
			rc = change_dir_to(name);
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
			if (strcmp(action, "edit") == 0) {
				if (fork() == 0) {
					rc = edit_file(name);
				} else {
					wait(&status);
					finalize();
					restore_windows();
				}
			} else if (strcmp(action, "copy") == 0) {
				copy_file(name);
			} else if (strcmp(action, "paste") == 0) {
				rc = paste_file();
			} else if (strcmp(action, "remove") == 0) {
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
/*
inline void __attribute__ ((always_inline))
copy_file(char *name)
{
	sprintf(copy_buffer, "%s/%s", content[ACTIVE_W].path, ++name);
}
*/
int
create_file(char *name)
{
	int rc = OK;

/*
	if (buf == NULL) {
		fclose(fp);
		return rc;
	}
*/

//	char file_path[PATH_MAX];
	char file_name[FILENAME_MAX];

	size_t len = strlen(name);
	size_t i;

	for (i = len; i > 0; --i)
		if (name[i] == '/') break;

//	strncpy(file_path, name, len - i);
//	file_path[len - i] = '\0';

	strncpy(file_name, name + len, i);
	file_name[i] = '\0';

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

	return rc;
}

int
edit_file(char *name)
{
	char editor_path[PATH_MAX + FILENAME_MAX];
	char file_path[PATH_MAX + FILENAME_MAX];
	sprintf(editor_path, EDITOR_PATH);
	sprintf(file_path, "%s/%s", content[ACTIVE_W].path, ++name);
	return execl(editor_path, editor_path, file_path, (char *) NULL);
}

int
exec_prog(char *name)
{
	char prog[PATH_MAX + FILENAME_MAX];
	sprintf(prog, "%s/%s", content[ACTIVE_W].path, ++name);
	return execl(prog, prog, (char *) NULL);
}

int
paste_file(void)
{
	return create_file(copy_buffer);
}

int
remove_file(char *name)
{
	int rc = OK;
	char file_path[PATH_MAX + FILENAME_MAX];

	sprintf(file_path, "%s/%s", content[ACTIVE_W].path, ++name);

	rc = unlink(file_path);
	if (rc == ERR) return rc;

	for (int i = 0; i < NWINDOWS; ++i)
		repaint_window(i);

	if (display_content(LEFT_W) | display_content(RITE_W))
		return ERR;

	return rc;
}
