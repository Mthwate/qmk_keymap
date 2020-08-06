#include QMK_KEYBOARD_H

enum ctrl_keycodes {
    MD_BOOT = SAFE_RANGE, // Restart into bootloader after hold timeout
    TM_SH,
    TM_SV,
    TM_EXIT,
    TM_MVU,
    TM_MVD,
    TM_MVL,
    TM_MVR,
    TM_ZOOM,
};

enum layers {
    LO_KB, // Default keyboard layer
    LO_FN, // Function layer
};

keymap_config_t keymap_config;

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [LO_KB] = LAYOUT(
        KC_ESC,    KC_F1,     KC_F2,     KC_F3,     KC_F4,     KC_F5,     KC_F6,     KC_F7,     KC_F8,     KC_F9,     KC_F10,    KC_F11,    KC_F12,               KC_PSCR,   KC_SLCK,   KC_PAUS,   \
        KC_GRV,    KC_1,      KC_2,      KC_3,      KC_4,      KC_5,      KC_6,      KC_7,      KC_8,      KC_9,      KC_0,      KC_MINS,   KC_EQL,    KC_BSPC,   KC_INS,    KC_HOME,   KC_PGUP,   \
        KC_TAB,    KC_Q,      KC_W,      KC_E,      KC_R,      KC_T,      KC_Y,      KC_U,      KC_I,      KC_O,      KC_P,      KC_LBRC,   KC_RBRC,   KC_BSLS,   KC_DEL,    KC_END,    KC_PGDN,   \
        KC_CAPS,   KC_A,      KC_S,      KC_D,      KC_F,      KC_G,      KC_H,      KC_J,      KC_K,      KC_L,      KC_SCLN,   KC_QUOT,   KC_ENT,                                                \
        KC_LSFT,   KC_Z,      KC_X,      KC_C,      KC_V,      KC_B,      KC_N,      KC_M,      KC_COMM,   KC_DOT,    KC_SLSH,   KC_RSFT,                                    KC_UP,                \
        KC_LCTL,   KC_LGUI,   KC_LALT,                         KC_SPC,                                     KC_RALT,   MO(LO_FN), KC_APP,    KC_RCTL,              KC_LEFT,   KC_DOWN,   KC_RGHT    \
    ),
    [LO_FN] = LAYOUT(
        TM_EXIT,   _______,   _______,   _______,   _______,   _______,   _______,   _______,   _______,   _______,   _______,   _______,   _______,              KC_MUTE,   _______,   _______,   \
        _______,   _______,   _______,   _______,   _______,   _______,   _______,   _______,   _______,   _______,   _______,   TM_SH,     _______,   _______,   KC_MPLY,   KC_MSTP,   KC_VOLU,   \
        _______,   RGB_SPD,   RGB_VAI,   RGB_SPI,   RGB_HUI,   RGB_SAI,   _______,   _______,   _______,   _______,   _______,   _______,   _______,   TM_SV,     KC_MPRV,   KC_MNXT,   KC_VOLD,   \
        _______,   RGB_RMOD,  RGB_VAD,   RGB_MOD,   RGB_HUD,   RGB_SAD,   _______,   _______,   _______,   _______,   _______,   _______,   _______,                                               \
        TM_ZOOM,   RGB_TOG,   _______,   _______,   _______,   MD_BOOT,   NK_TOGG,   _______,   _______,   _______,   _______,   _______,                                    TM_MVU,               \
        _______,   _______,   _______,                         _______,                                    _______,   _______,   _______,   _______,              TM_MVL,    TM_MVD,    TM_MVR     \
    ),
};

// Runs just one time when the keyboard initializes.
void matrix_init_user(void) {
};

// Runs constantly in the background, in a loop.
void matrix_scan_user(void) {
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    static uint32_t key_timer;

    switch (keycode) {
        case MD_BOOT:
            if (record->event.pressed) {
                key_timer = timer_read32();
            } else {
                if (timer_elapsed32(key_timer) >= 500) {
                    reset_keyboard();
                }
            }
            return false;
        case TM_SV:
            if (record->event.pressed) {
                SEND_STRING(SS_DOWN(X_LCTRL) "b" SS_UP(X_LCTRL) "%");
            }
            return false;
        case TM_SH:
            if (record->event.pressed) {
                SEND_STRING(SS_DOWN(X_LCTRL) "b" SS_UP(X_LCTRL) "\"");
            }
            return false;
        case TM_MVU:
            if (record->event.pressed) {
                SEND_STRING(SS_DOWN(X_LCTRL) "b" SS_UP(X_LCTRL) SS_TAP(X_UP));
            }
            return false;
        case TM_MVD:
            if (record->event.pressed) {
                SEND_STRING(SS_DOWN(X_LCTRL) "b" SS_UP(X_LCTRL) SS_TAP(X_DOWN));
            }
            return false;
        case TM_MVL:
            if (record->event.pressed) {
                SEND_STRING(SS_DOWN(X_LCTRL) "b" SS_UP(X_LCTRL) SS_TAP(X_LEFT));
            }
            return false;
        case TM_MVR:
            if (record->event.pressed) {
                SEND_STRING(SS_DOWN(X_LCTRL) "b" SS_UP(X_LCTRL) SS_TAP(X_RIGHT));
            }
            return false;
        case TM_EXIT:
            if (record->event.pressed) {
                SEND_STRING("exit" SS_TAP(X_ENTER));
            }
            return false;
        case TM_ZOOM:
            if (record->event.pressed) {
                SEND_STRING(SS_DOWN(X_LCTRL) "b" SS_UP(X_LCTRL) "z");
            }
            return false;
        case RGB_TOG:
            if (record->event.pressed) {
              switch (rgb_matrix_get_flags()) {
                case LED_FLAG_ALL: {
                    rgb_matrix_set_flags(LED_FLAG_KEYLIGHT | LED_FLAG_MODIFIER);
                    rgb_matrix_set_color_all(0, 0, 0);
                  }
                  break;
                case LED_FLAG_KEYLIGHT | LED_FLAG_MODIFIER: {
                    rgb_matrix_set_flags(LED_FLAG_UNDERGLOW);
                    rgb_matrix_set_color_all(0, 0, 0);
                  }
                  break;
                case LED_FLAG_UNDERGLOW: {
                    rgb_matrix_set_flags(LED_FLAG_NONE);
                    rgb_matrix_disable_noeeprom();
                  }
                  break;
                default: {
                    rgb_matrix_set_flags(LED_FLAG_ALL);
                    rgb_matrix_enable_noeeprom();
                  }
                  break;
              }
            }
            return false;
        default:
            return true; //Process all other keycodes normally
    }
}
