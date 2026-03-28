#include "Tree.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char *argv[]){
    const char *path = ".";

    TreeOptions opts;
    opts.show_dirs_only = 0;
    opts.max_depth = -1;

    // parse arguments
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-d") == 0) {
            opts.show_dirs_only = 1;
        }
        else if (strcmp(argv[i], "-L") == 0 && i + 1 < argc) {
            opts.max_depth = atoi(argv[i + 1]);
            i++;
        }
        else {
            path = argv[i];
        }
    }

    int is_last[100] = {0};
    int dir_count = 1; // include root
    int file_count = 0;

    printf("%s\n", path);

    Tree(path, 0, is_last, &dir_count, &file_count, &opts);

    printf("\n%d directories, %d files\n", dir_count, file_count);

    return 0;
}
