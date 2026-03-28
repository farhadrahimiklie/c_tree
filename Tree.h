#ifndef TREE_H
#define TREE_H

#define MAX_PATH 1024

typedef struct {
    int show_dirs_only; // -d
    int max_depth;      // -L
} TreeOptions;

int is_directory(const char *path);
long get_file_size(const char *path);
void print_indent(int level, int *is_last);

void Tree(const char *path, int level, int *is_last,
        int *dir_count, int *file_count,
        TreeOptions *opts);

#endif
