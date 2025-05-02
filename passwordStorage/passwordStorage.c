#include "passwordStorage.h"
#include "../main.h"
#include <string.h>

/**
 * @brief Add a new credential entry to the in-memory credentials list.
 * 
 * @param credentials Array of Credential structs.
 * @param i Index at which to insert the new credential.
 * @param service Service name (e.g., "gmail").
 * @param username Username for the service.
 * @param password Password for the service.
 * @return true on success.
 */
bool password_list_add(Credential* credentials, size_t i, const char* service, const char* username, const char* password) {
    
    strcpy(credentials[i].name, service);
    strcpy(credentials[i].username, username);
    strcpy(credentials[i].password, password);
    
    return true;
}

/**
 * @brief Read a single line from a file, stripping `\r` and stopping at `\n`.
 * 
 * @param file Pointer to the open File.
 * @param buffer Buffer to store the line.
 * @param max_len Maximum number of characters to read (including null terminator).
 * @return Number of characters read, 0 on EOF, or -1 on error.
 */
ssize_t storage_file_read_line(File* file, char* buffer, size_t max_len) {
    if(!file || !buffer || max_len == 0) return -1;

    size_t count = 0;
    uint8_t ch;

    while(count < max_len - 1) {
        ssize_t read = storage_file_read(file, &ch, 1);
        if(read != 1) break; // EOF or read error

        if(ch == '\n') break; // End of line

        if(ch != '\r') { // Skip carriage returns
            buffer[count++] = (char)ch;
        }
    }

    if(count == 0) return 0; // No data read (EOF or empty line)

    buffer[count] = '\0'; // Null-terminate the string
    return count;
}

/**
 * @brief Reads credentials from a CSV file into memory.
 * 
 * Each line in the file should follow the format: `service,username,password`.
 * 
 * @param filename Path to the credentials file.
 * @param credentials Array to store parsed credentials.
 * @return Number of credentials read.
 */
size_t read_passwords_from_file(const char* filename, Credential* credentials) {
    Storage* storage = furi_record_open(RECORD_STORAGE);
    if(!storage) return 0;
    
    File* file = storage_file_alloc(storage);
    if(!storage_file_open(file, filename, FSAM_READ, FSOM_OPEN_EXISTING)) {
        FURI_LOG_E("PasswordManager", "Failed to open file: %s", filename);
        storage_file_free(file);
        furi_record_close(RECORD_STORAGE);
        return 0;
    }
    
    char line[300];
    char service[100], username[100], password[100];
    
    size_t i = 0;
    while(true) {
        // Read a line
        memset(line, 0, sizeof(line));
        if(storage_file_read_line(file, line, sizeof(line)) <= 0) {
            break; // End of file or error
        }
        
        // Parse the line (expected format: "service,username,password")
        if(sscanf(line, "%99[^,],%99[^,],%99[^\n]", service, username, password) == 3) {
            // Add to our list
            password_list_add(credentials, i, service, username, password);   
            i++;
        }
    }
    
    storage_file_close(file);
    storage_file_free(file);
    furi_record_close(RECORD_STORAGE);
    
    return i;
}

/**
 * @brief Appends a new credential entry to the password file.
 * 
 * @param filename Path to the password file.
 * @param service Service name.
 * @param username Username.
 * @param password Password.
 * @return true on success, false on failure.
 */
bool write_password_to_file(const char* filename, const char* service, const char* username, const char* password) {
    Storage* storage = furi_record_open(RECORD_STORAGE);
    if(!storage) return false;
    
    File* file = storage_file_alloc(storage);
    if(!storage_file_open(file, filename, FSAM_WRITE, FSOM_OPEN_APPEND)) {
        // If file doesn't exist, create it
        if(!storage_file_open(file, filename, FSAM_WRITE, FSOM_CREATE_ALWAYS)) {
            FURI_LOG_E("PasswordManager", "Failed to create file: %s", filename);
            storage_file_free(file);
            furi_record_close(RECORD_STORAGE);
            return false;
        }
    }
    
    // Format CSV line
    char line[100];
    snprintf(line, sizeof(line), "%s,%s,%s\n", service, username, password);
    
    // Write to file
    bool success = storage_file_write(file, line, strlen(line)) == strlen(line);
    
    storage_file_close(file);
    storage_file_free(file);
    furi_record_close(RECORD_STORAGE);
    
    return success;
}

/**
 * @brief Deletes a specific line from the password file.
 * 
 * This is done by copying all lines except the target line to a temporary file,
 * then replacing the original file with the temp.
 * 
 * @param path Path to the original file.
 * @param line_to_delete Index (0-based) of the line to remove.
 * @return true on success, false on failure.
 */
bool delete_line_from_file(const char* path, size_t line_to_delete) {
    // Open original file for reading
    Storage* storage = furi_record_open(RECORD_STORAGE);
    File* source = storage_file_alloc(storage);

    if(!storage_file_open(source, path, FSAM_READ, FSOM_OPEN_EXISTING)) {
        FURI_LOG_E("FileEdit", "Failed to open source file");
        return false;
    }

    // Open a temporary file for writing
    File* tmp = storage_file_alloc(storage); 
    if(!storage_file_open(tmp, "/ext/passwordManager_tmp.txt", FSAM_WRITE, FSOM_CREATE_ALWAYS)) {
        FURI_LOG_E("FileEdit", "Failed to open temporary file");
        storage_file_close(source);
        return false;
    }

    // Line read buffer
    char line[300];
    size_t current_line = 0;

    while(true) {
        ssize_t len = storage_file_read_line(source, line, sizeof(line));
        if(len <= 0) break;

        // Write all lines except the one to delete
        if(current_line != line_to_delete) {
            storage_file_write(tmp, line, strlen(line));
            storage_file_write(tmp, "\n", 1); // Add newline back
        }
        current_line++;
    }
   // Cleanup
    storage_file_close(source);
    storage_file_close(tmp);

    // Delete original and rename temp
    storage_simply_remove(storage, path);
    storage_common_rename(storage, "/ext/passwordManager_tmp.txt", path);

    furi_record_close(RECORD_STORAGE);

    return true;
}
