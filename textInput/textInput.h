#include <gui/modules/text_input.h>
#include <gui/view_dispatcher.h>
#include <furi.h>

/**
 * @brief Creates and configures a TextInput view for entering a credential name (e.g., service or website).
 *
 * @param context Pointer to the application context (AppContext* expected).
 * @return Initialized TextInput view for entering the credential name.
 */
TextInput* credential_name_TextInput_alloc(void* context);

/**
 * @brief Creates and configures a TextInput view for entering a username.
 *
 * @param context Pointer to the application context (AppContext* expected).
 * @return Initialized TextInput view for entering the username.
 */
TextInput* credential_username_TextInput_alloc(void* context);

/**
 * @brief Creates and configures a TextInput view for entering a password.
 *
 * @param context Pointer to the application context (AppContext* expected).
 * @return Initialized TextInput view for entering the password.
 */
TextInput* credential_password_TextInput_alloc(void* context);
