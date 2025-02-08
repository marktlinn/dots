#ifndef FILE_H
#define FILE_H
#include <stdbool.h>

char *create_path(const char *path_a, const char *path_b);

char *get_default_dots_dir();

bool get_pwd(char *path);

bool move_file(const char *src_path, const char *dest_path);

bool link_file(const char *dest_path, const char *src_path);

#endif
