#ifndef FILE_H
#define FILE_H

int get_pwd(char *path);

int move_file(const char *src_path, const char *dest_path);

int link_file(const char *dest_path, const char *src_path);

#endif
