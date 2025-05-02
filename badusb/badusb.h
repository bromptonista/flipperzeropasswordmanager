#include <furi.h>
#include <furi_hal.h>
#include <furi_hal_usb.h>
#include <furi_hal_usb_hid.h>
#include <usb_hid.h>

/** 
 * @brief Initializes the USB HID interface in keyboard mode.
 *        Must be called before sending any HID input.
 */
void initialize_hid(void);

/** 
 * @brief Releases all currently pressed keys and modifiers.
 *        Useful after sending combinations to avoid stuck keys.
 */
void release_all_keys(void);

/**
 * @brief Sends a string as HID keyboard input.
 * 
 * @param str Null-terminated ASCII string to type.
 *            Characters are typed one at a time with delays.
 */
void type_string(const char* str);

/**
 * @brief Sends a single keypress and releases it.
 * 
 * @param key HID keycode to press (e.g., HID_KEYBOARD_A).
 */
void press_key(uint8_t key);
