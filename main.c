#include "include/file.h"
#include <getopt.h>
#include <linux/limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char **argv) {
  char path[PATH_MAX];
  int opt;
  bool dry_run = false; // zero false, else true
  bool manual = false;  // zero false, else true
  char *target = NULL;

  while ((opt = getopt(argc, argv, "hdmt:")) != -1) {
    switch (opt) {
    case 'h':
      printf("Options are: '-d' dry-run, '-t' target, '-m' manual.\n");
      break;

    case 'd':
      dry_run = true;
      printf("Dry run enabled.\n");
      break;

    case 't':
      target = optarg;
      printf("Target = %s\n", target);
      break;

    case 'm':
      manual = true;
      printf("Manual enabled.\n");
      break;

    default:
      fprintf(stderr, "Usage: %s [-d] [-m] [-t target]\n", argv[0]);
      exit(EXIT_FAILURE);
    }
  }

  if (dry_run) {
    printf("Dry run is set.\n");
    // Show expected output.
  }

  if (manual) {
    printf("Manual is set.\n");
    // If set, override pwd
  } else {
    if (get_pwd(path) != EXIT_SUCCESS) {
      return EXIT_FAILURE; // Exit if get_pwd fails
    }
  }

  if (target) {
    // Extract the filename from the target path
    const char *filename = strrchr(target, '/');
    if (filename) {
      filename++; // Move past the '/'
    } else {
      filename = target; // No '/' found, use the whole target
    }

    // Construct the destination path
    char *destination = create_path(path, filename);
    if (destination == NULL) {
      return EXIT_FAILURE;
    }

    // Move the file to the destination directory
    int moved = move_file(target, destination);
    if (moved == EXIT_SUCCESS) {
      printf("Moved file from %s to %s\n", target, destination);

      // Create a symbolic link from the original path to the current working
      // directory
      int linked = link_file(target, destination);
      if (linked != EXIT_SUCCESS) {
        free(destination);
        return EXIT_FAILURE;
      }
    }
    free(destination);
  }

  printf("Final path: %s\n", path);
  return EXIT_SUCCESS;
}
