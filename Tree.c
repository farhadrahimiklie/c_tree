#include "Tree.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>


int is_directory(const char *path){
    struct stat st;
    if (stat(path, &st) != 0) {
        return 0;
    }
    return S_ISDIR(st.st_mode);
}

int count_entries(const char *path){
    DIR *dir = opendir(path);
    struct dirent *entry;
    int count = 0;

    if (!dir) {
        return 0;
    }

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }
        count++;
    }
    closedir(dir);
    return count;
}

void print_indent(int level, int *is_last){
    for (int i = 0; i < level; i++) {
        if (is_last[i]) {
            printf("    ");
        }else {
            printf("|  ");
        }
    }
}

void Tree(const char *path, int level, int *is_last){
    DIR *dir = opendir(path);
    struct dirent *entry;
    if (!dir) {
        return;
    }

    int total = count_entries(path);
    int index = 0;

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }
        index++;
        char new_path[MAX_PATH];
        snprintf(new_path, sizeof(new_path), "%s/%s", path, entry->d_name);

        print_indent(level, is_last);

        if (index == total) {
            printf("L__ %s\n", entry->d_name);
            is_last[level] = 1;
        }else {
            printf("|-- %s\n", entry->d_name);
            is_last[level] = 0;
        }
        if (is_directory(new_path)) {
            Tree(new_path, level +1, is_last);
        }
    }
    closedir(dir);
}

int main(int argc, char *argv[]){
    const char *path = ".";

    if (argc > 1) {
        path = argv[1];
    }
    int is_last[100] = {0};

    printf("%s\n", path);
    Tree(path, 0, is_last);
    return 0;
}
