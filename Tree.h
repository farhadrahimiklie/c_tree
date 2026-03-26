#ifndef TREE_H
#define TREE_H

#define MAX_PATH 1024

int is_directory(const char *path);
int count_entries(const char *path);
void print_indent(int level, int *is_last);
void Tree(const char *path, int level, int *ls_last);

#endif
