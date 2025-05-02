#include <furi.h>
#include <storage/storage.h>
#include <toolbox/stream/file_stream.h>
#include <toolbox/stream/stream.h>
#include <string.h>
#include <stdlib.h>
#include "../main.h"

/**
 * @brief Reads credentials from a CSV-formatted file.
 *
 * Each line must be in the format: service,username,password
 * 
 * @param filename Path to the password file.
 * @param credentials Array to store the loaded credentials.
 * @return Number of credentials successfully read.
 */
size_t read_passwords_from_file(const char* filename, Credential* credentials);

/**
 * @brief Appends a new credential entry to the password file.
 * 
 * Writes the entry as a CSV line: service,username,password
 *
 * @param filename Path to the password file.
 * @param service Name of the service (e.g., "Gmail").
 * @param username Associated username.
 * @param password Associated password.
 * @return true if successfully written, false otherwise.
 */
bool write_password_to_file(const char* filename, const char* service, const char* username, const char* password);

/**
 * @brief Deletes a specific line from a password file.
 * 
 * Line index is zero-based (i.e., line 0 is the first line).
 *
 * @param path Path to the password file.
 * @param line_to_delete Index of the line to remove.
 * @return true on successful deletion and file update, false otherwise.
 */
bool delete_line_from_file(const char* path, size_t line_to_delete);
