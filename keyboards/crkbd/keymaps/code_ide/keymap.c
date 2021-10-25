#include QMK_KEYBOARD_H
#include <stdio.h>
#include "sync_timer.h"

// Each layer gets a name for readability, which is then used in the keymap matrix below.
// The underscores don't mean anything - you can have a layer called STUFF or any other name.
// Layer names don't all need to be of the same length, obviously, and you can also skip them
// entirely and just use numbers.
enum layers {
  _QWERTY,
  _LOWER,
  _RAISE,
  _ADJUST
};

enum custom_keycodes {
  QWERTY = SAFE_RANGE,
  LOWER,
  RAISE,
  ADJUST,
  RGBRST
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [_QWERTY] = LAYOUT_split_3x6_3(
    KC_ESC,          KC_Q,          KC_W,          KC_E,          KC_R,          KC_T,              KC_Y,          KC_U,          KC_I,          KC_O,          KC_P,            KC_BSPC,
    KC_TAB,          LGUI_T(KC_A),  LALT_T(KC_S),  LCTL_T(KC_D),  LSFT_T(KC_F),  KC_G,              KC_H,          RSFT_T(KC_J),  RCTL_T(KC_K),  LALT_T(KC_L),  RGUI_T(KC_SCLN), KC_QUOT,
    KC_LCTRL,        ALGR_T(KC_Z),  KC_X,          KC_C,          KC_V,          KC_B,              KC_N,          KC_M,          KC_COMM,       KC_DOT,        ALGR_T(KC_SLSH), KC_BSLS,
                                                 LT(3, KC_ESC),    LT(1,KC_SPC),    KC_TAB,    KC_BSPC,  LT(2,KC_ENT),   LT(3,KC_DEL)
  ),

  [_LOWER] = LAYOUT_split_3x6_3(
    KC_NO,           KC_F9,         KC_F10,        KC_F11,        KC_F12,        KC_NO,             KC_NUMLOCK,     KC_7,          KC_8,          KC_9,          KC_PPLS,         KC_PMNS,
    KC_NO,           LGUI_T(KC_F5), LALT_T(KC_F6), LCTL_T(KC_F7), LSFT_T(KC_F8), KC_NO,             KC_NO,          RSFT_T(KC_4),  RCTL_T(KC_5),  LALT_T(KC_6),  RGUI_T(KC_PAST), KC_PSLS,
    KC_NO,           KC_F1,         KC_F2,         KC_F3,         KC_F4,         KC_NO,             KC_NO,          KC_1,          KC_2,          KC_3,          KC_PDOT,         KC_PEQL,
                                                 KC_LGUI,          KC_TRNS,         KC_TRNS,    KC_TRNS,     KC_0,     KC_0
  ),

  [_RAISE] = LAYOUT_split_3x6_3(
    KC_GRV,          KC_APP,        KC_NO,         KC_LPRN,         KC_RPRN,         KC_EQL,         KC_NO,          KC_NO,         KC_UP,         KC_NO,         KC_PAUS,         KC_DEL,
    KC_CLCK,        LGUI_T(KC_NO), LALT_T(KC_NO), LCTL_T(KC_LBRC), LSFT_T(KC_RBRC), KC_MINS,        KC_NO,          RSFT_T(KC_LEFT),RCTL_T(KC_DOWN),LALT_T(KC_RGHT),RGUI_T(KC_NO),KC_NO,
    KC_INSERT,       KC_LT,         KC_GT,         KC_LCBR,       KC_RCBR,           KC_UNDS,        KC_NO,          KC_HOME,       KC_PGDN,       KC_END,        KC_PGUP,         KC_NO,
                                                              KC_LGUI,    KC_SPC,    KC_TRNS,    KC_TRNS,    KC_TRNS,   KC_RALT
  ),

  [_ADJUST] = LAYOUT_split_3x6_3(
    RESET,           KC_NO,         KC_MPRV,       KC_MPLY,       KC_MNXT,       RGBRST,            RGBRST,          KC_NO,         KC_NO,         KC_NO,          KC_NO,           KC_NO,
    KC_NO,           RGB_TOG,       KC_VOLD,       KC_MUTE,       KC_VOLU,       KC_NO,             RGB_SPI,        RGB_VAI,       RGB_SAI,      RGB_HUI,        RGB_TOG,         KC_NO,
    KC_NO,           RGB_RMOD,      KC_NO,         KC_NO,         KC_NO,         KC_NO,             RGB_SPD,        RGB_VAD,       RGB_SAD,      RGB_HUD,        RGB_MOD,         KC_NO,
                                                      KC_LGUI,        KC_TRNS,       KC_TRNS,    KC_TRNS,       KC_TRNS,       KC_RALT
  )
};

bool get_tapping_force_hold(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        // Layer Keys also with switched keys
        case LT(1,KC_SPC):
        case LT(2,KC_ENT):
        case LT(1,KC_TAB):
        case LT(2,KC_BSPC):

        // Main Layer Home-RowMods
        case LGUI_T(KC_A):
        case LALT_T(KC_S):
        case LCTL_T(KC_D):
        case LSFT_T(KC_F):

        case RSFT_T(KC_J):
        case RCTL_T(KC_K):
        case RALT_T(KC_L):
        case RGUI_T(KC_SCLN):

            return true;
        default:
            return false;
    }
}

// avoid slow pinky to trigger unexpected actions while typing
bool get_permissive_hold(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case LGUI_T(KC_A):
            return false;
        default:
            return true;
    }
}

bool get_ignore_mod_tap_interrupt(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case LGUI_T(KC_A):
            return true;
        default:
            return true;
    }
}

#ifdef OLED_ENABLE
    oled_rotation_t oled_init_user(oled_rotation_t rotation) {
    if (!is_keyboard_master()) {
        return OLED_ROTATION_180;  // flips the display 180 degrees if offhand
    }
    return rotation;
    }

    #define L_BASE 0
    #define L_LOWER 2
    #define L_RAISE 4
    #define L_ADJUST 8

    void oled_render_layer_state(void) {
        oled_write_P(PSTR("Layer: "), false);
        switch (layer_state) {
            case L_BASE:
                oled_write_ln_P(PSTR("Default"), false);
                break;
            case L_LOWER:
                oled_write_ln_P(PSTR("Lower"), false);
                break;
            case L_RAISE:
                oled_write_ln_P(PSTR("Raise"), false);
                break;
            case L_ADJUST:
            case L_ADJUST|L_LOWER:
            case L_ADJUST|L_RAISE:
            case L_ADJUST|L_LOWER|L_RAISE:
                oled_write_ln_P(PSTR("Adjust"), false);
                break;
        }
    }


    char keylog_str[24] = {};

    const char code_to_name[60] = {
        ' ', ' ', ' ', ' ', 'a', 'b', 'c', 'd', 'e', 'f',
        'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p',
        'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
        '1', '2', '3', '4', '5', '6', '7', '8', '9', '0',
        'R', 'E', 'B', 'T', '_', '-', '=', '[', ']', '\\',
        '#', ';', '\'', '`', ',', '.', '/', ' ', ' ', ' '};

    void set_keylog(uint16_t keycode, keyrecord_t *record) {
    char name = ' ';
        if ((keycode >= QK_MOD_TAP && keycode <= QK_MOD_TAP_MAX) ||
            (keycode >= QK_LAYER_TAP && keycode <= QK_LAYER_TAP_MAX)) { keycode = keycode & 0xFF; }
    if (keycode < 60) {
        name = code_to_name[keycode];
    }

    // update keylog
    snprintf(keylog_str, sizeof(keylog_str), "%dx%d, k%2d : %c",
            record->event.key.row, record->event.key.col,
            keycode, name);
    }

    void oled_render_keylog(void) {
        oled_write(keylog_str, false);
    }

    void render_bootmagic_status(bool status) {
        /* Show Ctrl-Gui Swap options */
        static const char PROGMEM logo[][2][3] = {
            {{0x97, 0x98, 0}, {0xb7, 0xb8, 0}},
            {{0x95, 0x96, 0}, {0xb5, 0xb6, 0}},
        };
        if (status) {
            oled_write_ln_P(logo[0][0], false);
            oled_write_ln_P(logo[0][1], false);
        } else {
            oled_write_ln_P(logo[1][0], false);
            oled_write_ln_P(logo[1][1], false);
        }
    }

    void oled_render_logo(void) {
        static const char PROGMEM crkbd_logo[] = {
            0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8a, 0x8b, 0x8c, 0x8d, 0x8e, 0x8f, 0x90, 0x91, 0x92, 0x93, 0x94,
            0xa0, 0xa1, 0xa2, 0xa3, 0xa4, 0xa5, 0xa6, 0xa7, 0xa8, 0xa9, 0xaa, 0xab, 0xac, 0xad, 0xae, 0xaf, 0xb0, 0xb1, 0xb2, 0xb3, 0xb4,
            0xc0, 0xc1, 0xc2, 0xc3, 0xc4, 0xc5, 0xc6, 0xc7, 0xc8, 0xc9, 0xca, 0xcb, 0xcc, 0xcd, 0xce, 0xcf, 0xd0, 0xd1, 0xd2, 0xd3, 0xd4,
            0};
        oled_write_P(crkbd_logo, false);
    }

    void oled_task_user(void) {
        if (is_keyboard_master()) {
            oled_render_layer_state();
            oled_render_keylog();
        } else {
            oled_render_logo();
        }
    }
#endif // OLED_ENABLE

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
#if OLED_ENABLE
    if (record->event.pressed) {
        set_keylog(keycode, record);
    }
#endif // OLED_ENABLE

    switch (keycode) {
        case RGBRST:
            #ifdef RGBLIGHT_ENABLE
                if (record->event.pressed) {
                eeconfig_update_rgblight_default();
                rgblight_enable();
                RGB_current_mode = rgblight_config.mode;
                }
            #endif
            #ifdef RGB_MATRIX_ENABLE
                if (record->event.pressed) {
                eeconfig_update_rgb_matrix_default();
                rgb_matrix_enable();
                }
            #endif
            break;
    }
    return true;
}