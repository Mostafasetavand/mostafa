#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>

#define MAX_LEN 4096

void create_dir(const char *dir_name);
void create_file(const char *path, const char *file_name, FILE *input_file);
void copy_path(const char *src_path, const char *dest_path);
void move_path(const char *src_path, const char *dest_path);
void tree(const char *base_path, int depth, const char *prefix);
void whereami(const char *base_dir, const char *current_dir);
void source(const char *file_name, const char *base_dir);
void process_commands(const char *file_name, const char *base_dir);

void create_dir(const char *dir_name) {
    if (mkdir(dir_name, 0755) == -1) {
        if (errno == EEXIST) {
            printf("A file or directory exists with name %s\n", dir_name);
        } else {
            perror("mkdir");
        }
    } else {
        printf("Directory '%s' created successfully.\n", dir_name);
    }
}

void create_file(const char *path, const char *file_name, FILE *input_file) {
    char full_path[MAX_LEN];
    snprintf(full_path, sizeof(full_path), "%s/%s", path, file_name);

    if (access(full_path, F_OK) == 0) {
        printf("A file or directory exists with name %s\n", file_name);
        return;
    }

    FILE *file = fopen(full_path, "w");
    if (!file) {
        perror("Error creating file");
        return;
    }

    char line[MAX_LEN];
    while (fgets(line, sizeof(line), input_file)) {
        if (strcmp(line, "!end!\n") == 0) break;
        fprintf(file, "%s", line);
    }

    fclose(file);
    printf("File '%s' created successfully at '%s'.\n", file_name, path);
}

void copy_path(const char *src_path, const char *dest_path) {
    struct stat src_stat;
    if (stat(src_path, &src_stat) == -1) {
        printf("Invalid source path: %s\n", src_path);
        return;
    }

    char dest_full[MAX_LEN];    
    snprintf(dest_full, sizeof(dest_full), "%s/%s", dest_path, strrchr(src_path, '/') + 1); 

    if (access(dest_full, F_OK) == 0) {
        printf("A file with this name already exists in the destination: %s\n", dest_full);
        return;
    }

    if (S_ISDIR(src_stat.st_mode)) {
        mkdir(dest_full, 0755); 
        DIR *dir = opendir(src_path);
        if (dir == NULL) {
            perror("Failed to open directory");
            return;
        }

        struct dirent *entry;
        while ((entry = readdir(dir))) {
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) continue;

            char src_entry[MAX_LEN], dest_entry[MAX_LEN];
            snprintf(src_entry, sizeof(src_entry), "%s/%s", src_path, entry->d_name);
            snprintf(dest_entry, sizeof(dest_entry), "%s", dest_full);
            copy_path(src_entry, dest_entry);
        }
        closedir(dir);
    } else {
        FILE *src_file = fopen(src_path, "r");
        if (!src_file) {
            perror("Error opening source file");
            return;
        }
        
        FILE *dest_file = fopen(dest_full, "w");
        if (!dest_file) {
            perror("Error creating destination file");
            fclose(src_file);
            return;
        }

        char buffer[4096];
        size_t bytes;
        while ((bytes = fread(buffer, 1, sizeof(buffer), src_file)) > 0) {
            fwrite(buffer, 1, bytes, dest_file);
        }

        fclose(src_file);
        fclose(dest_file);
    }
    printf("Copied '%s' to '%s'.\n", src_path, dest_path);
}

void move_path(const char *src_path, const char *dest_path) {
    if (rename(src_path, dest_path) == -1) {
        if (access(src_path, F_OK) == -1) {
            printf("Invalid source path: %s\n", src_path);
        } else if (access(dest_path, F_OK) == 0) {
            printf("A file with this name already exists in the destination: %s\n", dest_path);
        } else {
            perror("Failed to move");
        }
        return;
    }
    printf("Moved '%s' to '%s'.\n", src_path, dest_path);
}

void tree(const char *base_path, int depth, const char *prefix) {
    DIR *dir = opendir(base_path);
    if (!dir) {
        perror("Failed to open directory");
        return;
    }

    struct dirent *entry;
    int count = 0;
    while ((entry = readdir(dir))) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) continue;
        count++;
    }

    rewinddir(dir);
    int i = 0;

    while ((entry = readdir(dir))) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) continue;

        printf("%s%s── %s\n", prefix, i == count - 1 ? "└" : "├", entry->d_name);

        char new_base[MAX_LEN];
        snprintf(new_base, sizeof(new_base), "%s/%s", base_path, entry->d_name);

        if (entry->d_type == DT_DIR) {
            char new_prefix[MAX_LEN];
            snprintf(new_prefix, sizeof(new_prefix), "%s%s\t", prefix, i == count - 1 ? " " : "│");
            tree(new_base, depth + 1, new_prefix);
        }
        i++;
    }

    closedir(dir);
}

void whereami(const char *base_dir, const char *current_dir) {
    char relative_path[MAX_LEN];
    realpath(base_dir, relative_path);
    printf("Relative Path: %s\n", current_dir + strlen(relative_path) + 1);
}

void source(const char *file_name, const char *base_dir) {
    char new_base[MAX_LEN];
    realpath(base_dir, new_base);

    FILE *file = fopen(file_name, "r");
    if (!file) {
        perror("Error opening source file");
        return;
    }

    char line[MAX_LEN];
    while (fgets(line, sizeof(line), file)) {
        process_commands(line, new_base);
    }

    fclose(file);
}

void process_commands(const char *file_name, const char *base_dir) {
    FILE *file = fopen(file_name, "r");
    if (!file) {
        perror("Error opening commands file");
        return;
    }

    char line[MAX_LEN];
    while (fgets(line, sizeof(line), file)) {
        char command[MAX_LEN], arg1[MAX_LEN], arg2[MAX_LEN];
        if (sscanf(line, "%s %s %s", command, arg1, arg2) < 1) continue;

        if (strcmp(command, "create-dir") == 0) {
            create_dir(arg1);
        } else if (strcmp(command, "create") == 0) {
            FILE *input_file = fopen(arg2, "r");
            if (input_file) {
                create_file(base_dir, arg1, input_file);
                fclose(input_file);
            } else {
                perror("Error opening input file for create");
            }
        } else if (strcmp(command, "copy") == 0) {
            copy_path(arg1, arg2);
        } else if (strcmp(command, "move") == 0) {
            move_path(arg1, arg2);
        } else if (strcmp(command, "tree") == 0) {
            tree(base_dir, 0, "");
        } else if (strcmp(command, "whereami") == 0) {
            whereami(base_dir, ".");
        } else if (strcmp(command, "source") == 0) {
            source(arg1, base_dir);
        }
    }

    fclose(file);
}

int main(int argc, char **argv) {
    if (argc != 2) {
        printf("Usage: %s <commands-file>\n", argv[0]);
        return 1;
    }

    char base_dir[MAX_LEN];
    if (getcwd(base_dir, sizeof(base_dir))) {
        process_commands(argv[1], base_dir);
    } else {
        perror("getcwd() error");
        return 1;
    }

    return 0;
}
