#include "Tree.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>

int is_directory(const char *path){
    struct stat st;
    if (stat(path, &st) != 0) return 0;
    return S_ISDIR(st.st_mode);
}

long get_file_size(const char *path){
    struct stat st;
    if (stat(path, &st) != 0) return 0;
    return st.st_size;
}

void print_indent(int level, int *is_last){
    for (int i = 0; i < level; i++) {
        if (is_last[i]) printf("    ");
        else printf("|   ");
    }
}

int compare_names(const void *a, const void *b){
    char **sa = (char **)a;
    char **sb = (char **)b;
    return strcmp(*sa, *sb);
}

void Tree(const char *path, int level, int *is_last,
        int *dir_count, int *file_count,
        TreeOptions *opts)
{
    // depth limit
    if (opts->max_depth != -1 && level >= opts->max_depth) {
        return;
    }

    DIR *dir = opendir(path);
    if (!dir) return;

    struct dirent *entry;

    // STEP 1: collect all entries
    char *names[1024];
    int count = 0;

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 ||
                strcmp(entry->d_name, "..") == 0)
            continue;

        names[count++] = strdup(entry->d_name);
    }
    closedir(dir);

    // STEP 2: sort
    qsort(names, count, sizeof(char *), compare_names);

    // STEP 3: filter (IMPORTANT FIX)
    char *filtered[1024];
    int fcount = 0;

    for (int i = 0; i < count; i++) {
        char full[MAX_PATH];
        snprintf(full, sizeof(full), "%s/%s", path, names[i]);

        int is_dir = is_directory(full);

        if (opts->show_dirs_only && !is_dir) {
            free(names[i]);
            continue;
        }

        filtered[fcount++] = names[i];
    }

    // STEP 4: print (correct last detection)
    for (int i = 0; i < fcount; i++) {
        char full[MAX_PATH];
        snprintf(full, sizeof(full), "%s/%s", path, filtered[i]);

        int is_dir = is_directory(full);

        print_indent(level, is_last);

        if (i == fcount - 1) {
            printf("L__ ");
            is_last[level] = 1;
        } else {
            printf("|-- ");
            is_last[level] = 0;
        }

        if (is_dir) {
            printf("%s\n", filtered[i]);
            (*dir_count)++;
        } else {
            long size = get_file_size(full);
            printf("%s (%ld bytes)\n", filtered[i], size);
            (*file_count)++;
        }

        if (is_dir) {
            Tree(full, level + 1, is_last,
                    dir_count, file_count, opts);
        }

        free(filtered[i]); // clean memory
    }
}
