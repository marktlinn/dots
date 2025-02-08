#include "../include/file.h"
#include <errno.h>
#include <getopt.h>
#include <linux/limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

const char *DEFAULT_DOT_DIR = "/.dots/";

static bool path_ends_with_slash(const char *path) {
  size_t len = strlen(path);
  return len > 0 && path[len - 1] == '/';
}

char *create_path(const char *path_a, const char *path_b) {
  size_t len_a = strlen(path_a);
  size_t len_b = strlen(path_b);
  size_t sep_len = 0;

  bool needs_separator = !path_ends_with_slash(path_a) && path_b[0] != '/';

  if (needs_separator) {
    sep_len = 1;
  }

  size_t ttl_len = len_a + len_b + sep_len + 1;

  if (ttl_len > PATH_MAX) {
    fprintf(stderr, "Combined path too long: %s + %s\n", path_a, path_b);
    errno = ENAMETOOLONG;
    return NULL;
  }

  char *final_path = malloc(ttl_len);
  if (final_path == NULL) {
    perror("Failed to allocate memory for path");
    return NULL;
  }

  int written = snprintf(final_path, ttl_len, "%s%s%s", path_a,
                         needs_separator ? "/" : "", path_b);

  if (written < 0 || (size_t)written >= ttl_len) {
    fprintf(stderr, "snprintf failed or truncated.\n");
    free(final_path);
    return NULL;
  }

  return final_path;
}

bool get_pwd(char *path) {
  if (getcwd(path, PATH_MAX) != NULL) {
    printf("Current working dir: %s\n", path);

  } else {
    perror("getcwd() error");
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

char *get_home_dir() {
  char *home_dir = getenv("HOME");
  if (home_dir != NULL) {
    printf("HOME dir = %s\n", home_dir);
  } else {
    fprintf(stderr, "HOME environment variable not set.\n");
    return NULL;
  }

  return home_dir;
}

/*
 * Gets the default path for the `dots dir`.
 * Defaults to `$HOME/.dots/`
 * Returns path. Must be manually freed when finished.
 * */
char *get_default_dots_dir() {
  char *home_dir = get_home_dir();
  char *dots_dir = create_path(home_dir, DEFAULT_DOT_DIR);
  return dots_dir;
}

bool move_file(const char *src_path, const char *dest_path) {
  int res = rename(src_path, dest_path);
  if (res == 0) {
    printf("File moved from '%s' to '%s'\n", src_path, dest_path);
  } else {
    perror("Error moving file");
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

bool link_file(const char *dest_path, const char *src_path) {
  if (symlink(src_path, dest_path) == 0) {
    printf("Symbolic link created from '%s' to '%s'\n", src_path, dest_path);
  } else {
    perror("Error creating symbolic link");
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
