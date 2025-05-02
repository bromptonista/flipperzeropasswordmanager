#include "textInput.h"
#include "../main.h" 
#include "../passwordStorage/passwordStorage.h"
#include <string.h>

/**
 * @brief Callback after the user submits the credential name (e.g., website).
 * Stores the result and moves to the username input view.
 */
static void credential_name_callback(void* context) {
    AppContext* app = context;

    strcpy(app->credentials[app->credentials_number].name, app->tmp_credential_name);

    view_dispatcher_switch_to_view(app->view_dispatcher, ViewTextInputUsername);
}

/**
 * @brief Callback after the user submits the username.
 * Stores the result and moves to the password input view.
 */
static void username_callback(void* context) {
    AppContext* app = context;

    strcpy(app->credentials[app->credentials_number].username, app->tmp_username);

    view_dispatcher_switch_to_view(app->view_dispatcher, ViewTextInputPassword);
}

/**
 * @brief Callback after the user submits the password.
 * Saves the complete credential to file and returns to the main menu.
 */
static void password_callback(void* context) {
    AppContext* app = context;

    strcpy(app->credentials[app->credentials_number].password, app->tmp_password);

    // Add to file!!!
    write_password_to_file("/ext/passwordManager.txt", app->tmp_credential_name, app->tmp_username, app->tmp_password);
    // Clear temporary buffers
    strcpy(app->tmp_credential_name, "");
    strcpy(app->tmp_username, "");
    strcpy(app->tmp_password, "");
    
    // Return to main menu
    view_dispatcher_switch_to_view(app->view_dispatcher, ViewMainMenu);
}

/**
 * @brief Allocates and configures a TextInput view for entering the credential name.
 * 
 * @param context Pointer to the AppContext.
 * @return Initialized TextInput view.
 */
TextInput* credential_name_TextInput_alloc(void* context) {

    AppContext* app = context;

    TextInput* textInput = text_input_alloc();
    text_input_set_header_text(textInput, "Insert Website:");
    text_input_set_result_callback(
        textInput,
        credential_name_callback, // Callback when user submits
        app,                 // Context passed to callback
        app->tmp_credential_name,   // Buffer where text will be stored
        sizeof(char)*99,
        true
    );

    return textInput;
}

/**
 * @brief Allocates and configures a TextInput view for entering the username.
 * 
 * @param context Pointer to the AppContext.
 * @return Initialized TextInput view.
 */
TextInput* credential_username_TextInput_alloc(void* context) {
    
    AppContext* app = context;

    TextInput* textInput = text_input_alloc();
    text_input_set_header_text(textInput, "Insert Username:");
    text_input_set_result_callback(
        textInput,
        username_callback, // Callback when user submits
        app,                 // Context passed to callback
        app->tmp_username,   // Buffer where text will be stored
        sizeof(char)*99,
        true
    );

    return textInput;
}

/**
 * @brief Allocates and configures a TextInput view for entering the password.
 * 
 * @param context Pointer to the AppContext.
 * @return Initialized TextInput view.
 */
TextInput* credential_password_TextInput_alloc(void* context) {

    AppContext* app = context;

    TextInput* textInput = text_input_alloc();
    text_input_set_header_text(textInput, "Insert Password:");
    text_input_set_result_callback(
        textInput,
        password_callback, // Callback when user submits
        app,                 // Context passed to callback
        app->tmp_password,   // Buffer where text will be stored
        sizeof(char)*99,
        true
    );

    return textInput;

}
