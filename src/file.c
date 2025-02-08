#include "../include/file.h"
#include <getopt.h>
#include <linux/limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int get_pwd(char *path) {
  if (getcwd(path, PATH_MAX) != NULL) {
    printf("Current working dir: %s\n", path);
  } else {
    perror("getcwd() error");
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int move_file(const char *src_path, const char *dest_path) {
  int res = rename(src_path, dest_path);
  if (res == 0) {
    printf("File moved from '%s' to '%s'\n", src_path, dest_path);
  } else {
    perror("Error moving file");
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int link_file(const char *dest_path, const char *src_path) {
  if (symlink(src_path, dest_path) == 0) {
    printf("Symbolic link created from '%s' to '%s'\n", src_path, dest_path);
  } else {
    perror("Error creating symbolic link");
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
