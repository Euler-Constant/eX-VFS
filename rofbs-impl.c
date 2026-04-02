#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Helper functions
bool is_under_protected_dir(const char *file_path, const char *protected_dirs[]);
void record_backup(const char *file_path);
bool is_already_backed_up(const char *file_path);
bool is_malicious_process();                    // detector call
bool backup_file_exists(const char *original_path);
int create_backup_copy(const char *original_path);  // copy

int CreateBackupFile(const char *file_path, const char *protected_directories[]) {
    if (!is_under_protected_dir(file_path, protected_directories)) {
        return 0;  
    }

    if (is_already_backed_up(file_path)) {
        return 0; 
    }

    // pre-modification content as .tmp (core of ROFBS)
    if (create_backup_copy(file_path) == 0) {
        record_backup(file_path);   // mark as backed up
        return 1;
    }
    return -1;  // error
}

int RestoreBackup(const char *file_path) {
    if (!is_malicious_process()) {
        return 0;  // nothing to restore
    }

    if (backup_file_exists(file_path)) {
        // Rename .tmp back to original
        char backup_path[1024];
        snprintf(backup_path, sizeof(backup_path), "%s.tmp", file_path);
        if (rename(backup_path, file_path) == 0) {
            printf("[ROFBS] Restored: %s\n", file_path);
            return 1;
        }
    }
    return 0;
}
