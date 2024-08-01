#include QMK_KEYBOARD_H

void keyboard_post_init_user(void) {  
	keymap_config.nkro = 1; // Enable NKRO

	// Set default RGB
    rgb_matrix_sethsv_noeeprom(191, 255, 255); // Violet
    rgb_matrix_mode_noeeprom(RGB_MATRIX_SOLID_COLOR); // Set to a solid color mode
}

enum custom_keycodes {
    MACRO_PARENTHESIS = SAFE_RANGE,  // "()"
    MACRO_CURLY_BRACES,              // "{}"
    MACRO_SQUARE_BRACKETS,           // "[]"
    MACRO_ANGLE_BRACKETS,            // "<>"
	MACRO_TOGGLE_SNAP_TAP
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
	[0] = LAYOUT(
		KC_ESC, KC_F1, KC_F2, KC_F3, KC_F4, KC_F5, KC_F6, KC_F7, KC_F8, KC_F9, KC_F10, KC_F11, KC_F12, KC_DEL, KC_MUTE,
		KC_GRV, KC_1, KC_2, KC_3, KC_4, KC_5, KC_6, KC_7, KC_8, KC_9, KC_0, KC_MINS, KC_EQL, KC_BSPC, KC_INS,
		KC_TAB, KC_Q, KC_W, KC_E, KC_R, KC_T, KC_Y, KC_U, KC_I, KC_O, KC_P, KC_LBRC, KC_RBRC, KC_PGUP,
		MO(1), KC_A, KC_S, KC_D, KC_F, KC_G, KC_H, KC_J, KC_K, KC_L, KC_SCLN, KC_QUOT, KC_NUHS, KC_ENT, KC_PGDN,
		KC_LSFT, KC_NUBS, KC_Z, KC_X, KC_C, KC_V, KC_B, KC_N, KC_M, KC_COMM, KC_DOT, KC_SLSH, KC_RSFT, KC_UP, KC_CAPS,
		KC_LCTL, KC_LGUI, KC_LALT, KC_SPC, KC_RALT, KC_LGUI, MACRO_TOGGLE_SNAP_TAP, KC_LEFT, KC_DOWN, KC_RGHT
	),
	[1] = LAYOUT(
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_PSCR,
		KC_TRNS, KC_ENT, KC_UP, KC_BSPC, KC_DEL, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		KC_NO, KC_LEFT, KC_DOWN, KC_RGHT, KC_F, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS, MACRO_PARENTHESIS, MACRO_CURLY_BRACES, MACRO_SQUARE_BRACKETS, MACRO_ANGLE_BRACKETS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, QK_BOOT,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_LCTL, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS
	)
};

#if defined(ENCODER_MAP_ENABLE)
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][NUM_DIRECTIONS] = {
    [0] = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU) },
    [1] = { ENCODER_CCW_CW(KC_TRNS, KC_TRNS) }
};
#endif

static bool is_snap_tap_enabled = false;
static bool is_a_active = false;
static bool is_d_active = false;
static bool a_held = false;
static bool d_held = false;

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
		case KC_A:
			if (!is_snap_tap_enabled) {
				return true;
			}
		
			if (record->event.pressed) {
				if (is_d_active) {
					unregister_code(KC_D);
					is_d_active = false;
				}
				
				is_a_active = true;
			} else {
				is_a_active = false;
			}
			
			return true;
			
		case KC_D:
			if (!is_snap_tap_enabled) {
				return true;
			}
		
			if (record->event.pressed) {
				if (is_a_active) {
					unregister_code(KC_A);
					is_a_active = false;
				}
				
				is_d_active = true;
			} else {
				is_d_active = false;
			}
			
			return true;
			
		case MACRO_TOGGLE_SNAP_TAP:
			if (record->event.pressed) {
				if (is_snap_tap_enabled) {					
					is_snap_tap_enabled = false;
					send_string("Snap Tap deactivated");
				} else {
					is_snap_tap_enabled = true;
					send_string("Snap Tap activated");
				}
			}
			
			return false;
		
        case MACRO_PARENTHESIS:
            if (record->event.pressed) {
                tap_code16(S(KC_8));       // Shift + 8
                tap_code16(S(KC_9));       // Shift + 9
            }
			
            return false;

        case MACRO_CURLY_BRACES:
            if (record->event.pressed) {
                tap_code16(RALT(KC_7));  // AltGr + 7
                tap_code16(RALT(KC_0));  // AltGr + 0
            }
			
            return false;

        case MACRO_SQUARE_BRACKETS:
            if (record->event.pressed) {
                tap_code16(RALT(KC_8));  // AltGr + 8
                tap_code16(RALT(KC_9));  // AltGr + 9
            }
			
            return false;

        case MACRO_ANGLE_BRACKETS:
            if (record->event.pressed) {
                tap_code16(KC_NUBS);        // Press "<"
                tap_code16(S(KC_NUBS));   // Shift + "<"
            }
			
            return false;
    }
	
    return true; // Process all other keycodes normally
};
