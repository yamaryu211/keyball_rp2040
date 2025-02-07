/*
Copyright 2022 @Yowkees
Copyright 2022 MURAOKA Taro (aka KoRoN, @kaoriya)

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include QMK_KEYBOARD_H

#include "lib/keyball/keyball.h"

// OS判別用に追加
#include "os_detection.h"

// レイヤー定義
#define _WINDOWS 0
#define _LOWER_W 1
#define _RAISE_W 2
#define _ADJUST_W 3
#define _KEYBOARD_W 4
#define _MAC 5
#define _LOWER_M 6
#define _RAISE_M 7
#define _ADJUST_M 8
#define _KEYBOARD_M 9

/* tap dance */
typedef enum {
    TD_NONE,
    TD_UNKNOWN,
    TD_SINGLE_TAP,
    TD_SINGLE_HOLD,
    TD_DOUBLE_TAP,
    TD_DOUBLE_HOLD,
    TD_DOUBLE_SINGLE_TAP, // Send two single taps
    TD_TRIPLE_TAP,
    TD_TRIPLE_HOLD
} td_state_t;

typedef struct {
    bool is_press_action;
    td_state_t state;
} td_tap_t;

// QK_TAP_DANCE                   = 0x5700,
// QK_TAP_DANCE_MAX               = 0x57FF,

// Tap dance enums
enum {
  TD_Q,
};

td_state_t cur_dance(tap_dance_state_t *state);

// For the x tap dance. Put it here so it can be used in any keymap
void dance_q_finished(tap_dance_state_t *state, void *user_data);
void dance_q_reset(tap_dance_state_t *state, void *user_data);
void dance_fn_finished(tap_dance_state_t *state, void *user_data);
void dance_fn_reset(tap_dance_state_t *state, void *user_data);

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  // keymap for default (yamaryu211)
  [_WINDOWS] = LAYOUT_universal(
    KC_Q           , KC_W         , KC_E           , KC_R            , KC_T           ,                                  KC_Y          , KC_U            , KC_I        , KC_O           , KC_P            ,
    LCTL_T(KC_A)   , LALT_T(KC_S) , LT(2, KC_D)    , LT(1, KC_F)     , KC_G           ,                                  KC_H          , LT(1, KC_J)     , LT(2, KC_K) , LALT_T(KC_L)   , LCTL_T(KC_MINS) ,
    LSFT_T(KC_Z)   , LGUI_T(KC_X) , KC_C           , KC_V            , KC_B           ,                                  KC_N          , KC_M            , KC_COMM     , LGUI_T(KC_DOT) , LSFT_T(KC_SLSH) ,
    LT(1, KC_LNG2) , KC_ESC       , LGUI_T(KC_TAB) , KC_LALT         , LCTL_T(KC_DEL) , LSFT_T(KC_SPC) , LT(1, KC_ENT) , LT(2,KC_BSPC) , _______         , _______     , _______        , LT(3, KC_LNG1)
  ),

  [_LOWER_W] = LAYOUT_universal(
    KC_KP_SLASH , KC_7    , KC_8       , KC_9       , KC_KP_ASTERISK ,                      KC_ESC  , KC_NO   , KC_NO    , KC_NO            , LALT(KC_PSCR) ,
    KC_KP_MINUS , KC_4    , KC_5       , KC_6       , KC_KP_PLUS     ,                      KC_LEFT , KC_DOWN , KC_UP    , LALT_T(KC_RIGHT) , _______       ,
    KC_0        , KC_1    , KC_2       , KC_3       , KC_EQUAL       ,                      KC_HOME , KC_PGDN , KC_PGUP  , KC_END           , _______       ,
    MO(4)       , _______ , S(KC_LGUI) , S(KC_LALT) , S(KC_LCTL)     , _______  , _______ , _______ , _______ , _______  , _______          , MO(4)
  ),

  [_RAISE_W] = LAYOUT_universal(
    KC_QUOT , S(KC_QUOT) , S(KC_1)    , S(KC_SLSH) , S(KC_GRAVE) ,                     KC_BSLS , S(KC_LBRC) , S(KC_RBRC) , KC_NO      , S(KC_5)  ,
    S(KC_2) , KC_SCLN    , S(KC_4)    , S(KC_COMM) , S(KC_DOT)   ,                     S(KC_3) , S(KC_9)    , S(KC_0)    , S(KC_BSLS) , KC_EQUAL ,
    KC_NO   , S(KC_8)    , S(KC_SCLN) , S(KC_6)    , KC_GRAVE    ,                     S(KC_7) , KC_LBRC    , KC_RBRC    , KC_NO      , KC_SLSH  ,
    _______ , _______    , _______    , _______    , _______     , _______ , _______ , _______ , _______    , _______    , _______    , _______
  ),

  [_ADJUST_W] = LAYOUT_universal(
    KC_F10  , KC_F7       , KC_F8   , KC_F9   , KC_NO   ,                     LGUI(KC_TAB) , LCTL(KC_PGUP)   , LCTL(KC_PGDN)  , KC_NO      , LSG(KC_S) ,
    KC_F11  , KC_F4       , KC_F5   , KC_F6   , KC_NO   ,                     LCTL(KC_W)   , LCTL(S(KC_TAB)) , LCTL(KC_TAB)   , LCTL(KC_T) , KC_NO     ,
    KC_F12  , LCTL(KC_F1) , KC_F2   , KC_F3   , KC_NO   ,                     KC_NO        , LALT(KC_LEFT)   , LALT(KC_RIGHT) , KC_NO      , KC_NO     ,
    _______ , _______     , _______ , _______ , _______ , _______ , _______ , _______      , _______         , _______        , _______    , _______
  ),

  [_KEYBOARD_W] = LAYOUT_universal(
    KC_NO    , KC_NO    , KC_NO   , DT_PRNT , SSNP_VRT ,                     KC_BRIU , KC_BRID , KC_MUTE , KC_VOLD , KC_VOLU ,
    CPI_I100 , SCRL_DVI , AML_I50 , DT_UP   , SSNP_HOR ,                     KC_NO   , KC_NO   , KC_NO   , KC_NO   , _______ ,
    CPI_D100 , SCRL_DVD , AML_D50 , DT_DOWN , SSNP_FRE ,                     KC_NO   , KC_NO   , KC_NO   , KC_NO   , _______ ,
    _______  , _______  , _______ , _______ , _______  , _______ , _______ , _______ , _______ , _______ , _______ , _______  
  ),
  [_MAC] = LAYOUT_universal(
    KC_Q           , KC_W         , KC_E           , KC_R            , KC_T           ,                                  KC_Y          , KC_U            , KC_I        , KC_O           , KC_P            ,
    LCTL_T(KC_A)   , LALT_T(KC_S) , LT(2, KC_D)    , LT(1, KC_F)     , KC_G           ,                                  KC_H          , LT(1, KC_J)     , LT(2, KC_K) , LALT_T(KC_L)   , LCTL_T(KC_MINS) ,
    LSFT_T(KC_Z)   , LGUI_T(KC_X) , KC_C           , KC_V            , KC_B           ,                                  KC_N          , KC_M            , KC_COMM     , LGUI_T(KC_DOT) , LSFT_T(KC_SLSH) ,
    LT(1, KC_LNG2) , KC_ESC       , LGUI_T(KC_TAB) , KC_LALT         , LCTL_T(KC_DEL) , LSFT_T(KC_SPC) , LT(1, KC_ENT) , LT(2,KC_BSPC) , _______         , _______     , _______        , LT(3, KC_LNG1)
  ),

  [_LOWER_M] = LAYOUT_universal(
    KC_KP_SLASH , KC_7    , KC_8       , KC_9       , KC_KP_ASTERISK ,                      KC_ESC  , KC_NO   , KC_NO    , KC_NO            , LALT(KC_PSCR) ,
    KC_KP_MINUS , KC_4    , KC_5       , KC_6       , KC_KP_PLUS     ,                      KC_LEFT , KC_DOWN , KC_UP    , LALT_T(KC_RIGHT) , _______       ,
    KC_0        , KC_1    , KC_2       , KC_3       , KC_EQUAL       ,                      KC_HOME , KC_PGDN , KC_PGUP  , KC_END           , _______       ,
    MO(4)       , _______ , S(KC_LGUI) , S(KC_LALT) , S(KC_LCTL)     , _______  , _______ , _______ , _______ , _______  , _______          , MO(4)
  ),

  [_RAISE_M] = LAYOUT_universal(
    KC_QUOT , S(KC_QUOT) , S(KC_1)    , S(KC_SLSH) , S(KC_GRAVE) ,                     KC_BSLS , S(KC_LBRC) , S(KC_RBRC) , KC_NO      , S(KC_5)  ,
    S(KC_2) , KC_SCLN    , S(KC_4)    , S(KC_COMM) , S(KC_DOT)   ,                     S(KC_3) , S(KC_9)    , S(KC_0)    , S(KC_BSLS) , KC_EQUAL ,
    KC_NO   , S(KC_8)    , S(KC_SCLN) , S(KC_6)    , KC_GRAVE    ,                     S(KC_7) , KC_LBRC    , KC_RBRC    , KC_NO      , KC_SLSH  ,
    _______ , _______    , _______    , _______    , _______     , _______ , _______ , _______ , _______    , _______    , _______    , _______
  ),

  [_ADJUST_M] = LAYOUT_universal(
    KC_F10  , KC_F7       , KC_F8   , KC_F9   , KC_NO   ,                     LGUI(KC_TAB) , LCTL(KC_PGUP)   , LCTL(KC_PGDN)  , KC_NO      , LSG(KC_S) ,
    KC_F11  , KC_F4       , KC_F5   , KC_F6   , KC_NO   ,                     LCTL(KC_W)   , LCTL(S(KC_TAB)) , LCTL(KC_TAB)   , LCTL(KC_T) , KC_NO     ,
    KC_F12  , LCTL(KC_F1) , KC_F2   , KC_F3   , KC_NO   ,                     KC_NO        , LALT(KC_LEFT)   , LALT(KC_RIGHT) , KC_NO      , KC_NO     ,
    _______ , _______     , _______ , _______ , _______ , _______ , _______ , _______      , _______         , _______        , _______    , _______
  ),

  [_KEYBOARD_M] = LAYOUT_universal(
    KC_NO    , KC_NO    , KC_NO   , DT_PRNT , SSNP_VRT ,                     KC_BRIU , KC_BRID , KC_MUTE , KC_VOLD , KC_VOLU ,
    CPI_I100 , SCRL_DVI , AML_I50 , DT_UP   , SSNP_HOR ,                     KC_NO   , KC_NO   , KC_NO   , KC_NO   , _______ ,
    CPI_D100 , SCRL_DVD , AML_D50 , DT_DOWN , SSNP_FRE ,                     KC_NO   , KC_NO   , KC_NO   , KC_NO   , _______ ,
    _______  , _______  , _______ , _______ , _______  , _______ , _______ , _______ , _______ , _______ , _______ , _______  
  ),
};

// clang-format off

/*

color.h

#define HSV_AZURE       132, 102, 255
#define HSV_BLACK         0,   0,   0
#define HSV_BLUE        170, 255, 255
#define HSV_CHARTREUSE   64, 255, 255
#define HSV_CORAL        11, 176, 255
#define HSV_CYAN        128, 255, 255
#define HSV_GOLD         36, 255, 255
#define HSV_GOLDENROD    30, 218, 218
#define HSV_GREEN        85, 255, 255
#define HSV_MAGENTA     213, 255, 255
#define HSV_ORANGE       21, 255, 255
#define HSV_PINK        234, 128, 255
#define HSV_PURPLE      191, 255, 255
#define HSV_RED           0, 255, 255
#define HSV_SPRINGGREEN 106, 255, 255
#define HSV_TEAL        128, 255, 128
#define HSV_TURQUOISE   123,  90, 112
#define HSV_WHITE         0,   0, 255
#define HSV_YELLOW       43, 255, 255
#define HSV_OFF         HSV_BLACK
*/
// clang-format on

// デフォルトレイヤーの切り替え
void keyboard_post_init_user() {
  wait_ms(400);
  os_variant_t os = detected_host_os();
  switch (os) {
    case OS_WINDOWS:
      default_layer_set(1UL << _WINDOWS);
      break;
    case OS_MACOS:
    case OS_IOS:
      default_layer_set(1UL << _MAC);
      break;
    case OS_LINUX:
      default_layer_set(1UL << _WINDOWS);
      break;
    default:
      default_layer_set(1UL << _WINDOWS);
  }
}

layer_state_t layer_state_set_user(layer_state_t state) {
    // Auto enable scroll mode when the highest layer is 3
    keyball_set_scroll_mode(get_highest_layer(state) == _ADJUST_W || get_highest_layer(state) == _ADJUST_M);

    uint8_t layer = biton32(state);
    switch (layer) {
        case 0:
            rgblight_sethsv_noeeprom(HSV_SPRINGGREEN);
            break;
        case 1:
            rgblight_sethsv_noeeprom(HSV_WHITE);
            break;
        case 2:
            rgblight_sethsv_noeeprom(HSV_GOLD);
            break;
        case 3:
            rgblight_sethsv_noeeprom(HSV_BLUE);
            break;
        case 4:
            rgblight_sethsv_noeeprom(HSV_MAGENTA);
            break;            
    }

    return state;
}


#ifdef DYNAMIC_MACRO_ENABLE
// Dynamic Macros: Record and Replay Macros in Runtime
// https://github.com/qmk/qmk_firmware/blob/master/docs/feature_dynamic_macros.md

  bool isRecording = false;
  // Triggered when you start recording a macro.
  void dynamic_macro_record_start_user(int8_t direction) {
      isRecording = true;
  }

  // Triggered when you play back a macro.
  void dynamic_macro_play_user(int8_t direction) {
  }

  // Triggered on each keypress while recording a macro.
  void dynamic_macro_record_key_user(int8_t direction, keyrecord_t *record) {
  }

  // Triggered when the macro recording is stopped.
  void dynamic_macro_record_end_user(int8_t direction) {
      isRecording = false;
  }
#endif

// TAP DANCE
td_state_t cur_dance(tap_dance_state_t *state) {
    if (state->count == 1) {
      if (state->interrupted || !state->pressed) {
        return TD_SINGLE_TAP;
      } else {
        return TD_SINGLE_HOLD;
      }
    } else if (state->count == 2) {
      if (state->interrupted) {
        return TD_DOUBLE_SINGLE_TAP;
      } else if (state->pressed) {
        return TD_DOUBLE_HOLD;
      } else {
        return TD_DOUBLE_TAP;
      }
    } else {
      return TD_UNKNOWN;
    }
}

td_state_t cur_dance2(tap_dance_state_t *state) {
    if (state->count == 1) {
        if (!state->pressed) {
          return TD_SINGLE_TAP;
        } else {
          return TD_SINGLE_HOLD;
        }
    } else if (state->count == 2) {
      return TD_DOUBLE_TAP;
    } else {
      return TD_UNKNOWN;
    }
}

// TD_Q
static td_tap_t TD_Q_tap_state = {
    .is_press_action = true,
    .state = TD_NONE
};

void dance_q_finished(tap_dance_state_t *state, void *user_data) {
    TD_Q_tap_state.state = cur_dance(state);
    switch (TD_Q_tap_state.state) {
        case TD_SINGLE_TAP: register_code(KC_Q); break;
        case TD_SINGLE_HOLD: register_code(KC_LGUI); break;
        case TD_DOUBLE_TAP: register_code(KC_ESC); break;
        case TD_DOUBLE_HOLD: break;
        case TD_DOUBLE_SINGLE_TAP: tap_code(KC_Q); register_code(KC_Q); break;
        default: break;
    }
}

void dance_q_reset(tap_dance_state_t *state, void *user_data) {
    switch (TD_Q_tap_state.state) {
        case TD_SINGLE_TAP: unregister_code(KC_Q); break;
        case TD_SINGLE_HOLD: unregister_code(KC_LGUI); break;
        case TD_DOUBLE_TAP: unregister_code(KC_ESC); break;
        case TD_DOUBLE_HOLD: break;
        case TD_DOUBLE_SINGLE_TAP: unregister_code(KC_Q); break;
        default: break;
    }
    TD_Q_tap_state.state = TD_NONE;
}


tap_dance_action_t tap_dance_actions[] = {
  [TD_Q] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, dance_q_finished, dance_q_reset),
};

// COMBO
// 以下はyamaryu211バージョン
#ifdef COMBO_ENABLE
  const uint16_t PROGMEM combo_esc[] = {KC_Q, KC_W, COMBO_END};
  const uint16_t PROGMEM combo_tab[] = {KC_A, KC_S, COMBO_END};
  const uint16_t PROGMEM combo_tab_alt[] = {KC_S, KC_D, COMBO_END};
  const uint16_t PROGMEM combo_left_click[] = {KC_J, KC_K, COMBO_END};
  const uint16_t PROGMEM combo_right_click[] = {KC_K, KC_L, COMBO_END};
  const uint16_t PROGMEM combo_middle_click[] = {KC_J, KC_L, COMBO_END};
  // レイヤー・モディファイアキーを含むコンボキーの設定
  const uint16_t PROGMEM combo_esc2[] = {MT(MOD_LGUI, KC_Q), KC_W, COMBO_END};
  const uint16_t PROGMEM combo_tab2[] = {MT(MOD_LCTL, KC_A), MT(MOD_LALT, KC_S), COMBO_END};
  const uint16_t PROGMEM combo_tab2_alt[] = {MT(MOD_LALT, KC_S), LT(2, KC_D), COMBO_END};
  const uint16_t PROGMEM combo_left_click2[] = {LT(1, KC_J), LT(2, KC_K), COMBO_END};
  const uint16_t PROGMEM combo_right_click2[] = {LT(2, KC_K), MT(MOD_LALT, KC_L), COMBO_END};
  const uint16_t PROGMEM combo_middle_click2[] = {LT(1, KC_J), MT(MOD_LALT, KC_L), COMBO_END};
  // 大西配列用のコンボキーの設定
  const uint16_t PROGMEM combo_esc3[] = {KC_Q, KC_L, COMBO_END}; 
  const uint16_t PROGMEM combo_tab3[] = {MT(MOD_LCTL, KC_E), MT(MOD_LALT, KC_I), COMBO_END};
  const uint16_t PROGMEM combo_tab3_alt[] = {MT(MOD_LALT, KC_I), LT(2, KC_A), COMBO_END};
  const uint16_t PROGMEM combo_left_click3[] = {LT(1, KC_T), LT(2, KC_N), COMBO_END};
  const uint16_t PROGMEM combo_right_click3[] = {LT(2, KC_N), MT(MOD_LALT, KC_S), COMBO_END};
  const uint16_t PROGMEM combo_middle_click3[] = {LT(1, KC_T), MT(MOD_LALT, KC_S), COMBO_END};

  combo_t key_combos[] = {
      COMBO(combo_esc, KC_ESC),
      COMBO(combo_tab, KC_TAB),
      COMBO(combo_left_click, KC_BTN1),
      COMBO(combo_right_click, KC_BTN2),
      COMBO(combo_middle_click, KC_BTN3),
      COMBO(combo_esc2, KC_ESC),
      COMBO(combo_tab2, KC_TAB),
      COMBO(combo_left_click2, KC_BTN1),
      COMBO(combo_right_click2, KC_BTN2),
      COMBO(combo_middle_click2, KC_BTN3),
      COMBO(combo_esc3, KC_ESC),
      COMBO(combo_tab3, KC_TAB),
      COMBO(combo_left_click3, KC_BTN1),
      COMBO(combo_right_click3, KC_BTN2),
      COMBO(combo_middle_click3, KC_BTN3),
      COMBO(combo_tab_alt, KC_TAB),
      COMBO(combo_tab2_alt, KC_TAB),
      COMBO(combo_tab3_alt, KC_TAB),
  };
#endif

#ifdef OLED_ENABLE

#    include "lib/oledkit/oledkit.h"

void oledkit_render_info_user(void) {
    keyball_oled_render_keyinfo();
    keyball_oled_render_ballinfo();
    keyball_oled_render_layerinfo();
}
#endif