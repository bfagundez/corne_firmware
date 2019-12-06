#include "crkbd.h"
#include "bootloader.h"
#include "action_layer.h"
#include "eeconfig.h"
#ifdef PROTOCOL_LUFA
#include "lufa.h"
#include "split_util.h"
#endif
#include "LUFA/Drivers/Peripheral/TWI.h"

#ifdef OLED_DRIVER_ENABLE
#    define KEYLOGGER_LENGTH 5

oled_rotation_t oled_init_user(oled_rotation_t rotation) { return OLED_ROTATION_0; }

void add_keylog(uint16_t keycode);
#endif

extern keymap_config_t keymap_config;
extern uint8_t is_master;

// Each layer gets a name for readability, which is then used in the keymap matrix below.
// The underscores don't mean anything - you can have a layer called STUFF or any other name.
// Layer names don't all need to be of the same length, obviously, and you can also skip them
// entirely and just use numbers.
#define _QWERTY 0
#define _LOWER 2
#define _RAISE 3
#define _EXTRA 4
#define _ADJUST 16

enum custom_keycodes {
  QWERTY = SAFE_RANGE,
  LOWER,
  RAISE,
  ADJUST,
  KC_VIM_PASTE_LAST_REGISTER,
  KC_VIM_EQUALIZE_PANES,
  KC_VIM_NEXT_TAB,
  KC_VIM_PREV_TAB,
  KC_VIM_PANE_DOWN,
  KC_VIM_PANE_LEFT,
  KC_VIM_PANE_RIGHT,
  KC_VIM_PANE_UP,
  KC_VIM_NEW_TAB,
  KC_VIM_V_SPLIT,
  KC_VIM_H_SPLIT,
  KC_VIM_OPEN_FOLDS,
  KC_VIM_COPY_FILE_PATH,
  KC_VIM_TAB_CLOSE,
  KC_VIM_SAVE,
  KC_VIM_TOGGLE_COMMENT,
  KC_VIM_UNWRAP_WORD,
  KC_VIM_WRAP_WORD,
  KC_TMUX_NEXT_TAB,
  KC_TMUX_PREV_TAB,
  KC_TMUX_PANE_DOWN,
  KC_TMUX_PANE_UP,
  KC_TMUX_PANE_RIGHT,
  KC_TMUX_PANE_LEFT,
  KC_TMUX_V_SPLIT,
  KC_TMUX_H_SPLIT,
  KC_TMUX_EDIT_MODE,
  KC_TMUX_ZOOM,
  KC_TMUX_NEW_TAB,
  KC_SPCT_MAXIMIZE,
  KC_SPCT_LEFT_HALF,
  KC_SPCT_RIGHT_HALF,
  KC_SCREENSHOT
};


#define _____ KC_TRNS
#define KC_XX KC_NO
#define KC_LOWER LOWER
#define KC_RAISE RAISE
#define KC_QWERTY QWERTY
#define KC_RST   RESET
#define KC_MO   MO
#define KC_SFT_T   SFT_T
#define KC_CTL_T   CTL_T

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [_QWERTY] = LAYOUT_kc( \
  //,-----------------------------------------.                ,-----------------------------------------.
      TAB,      Q,     W,     E,     R,     T,                      Y,     U,     I,     O,     P,  BSPC,\
  //|------+------+------+------+------+------|                |------+------+------+------+------+------|
      SFT_T(KC_ESC),SFT_T(KC_A), S, D,F,  G,                      H,     J,     K,     L,  SFT_T(KC_SCLN),  SFT_T(KC_QUOTE),
  //|------+------+------+------+------+------|                |------+------+------+------+------+------|
      LCTL,     Z,     X,     C,     V,     B,                      N,     M,  COMM,   DOT,  SLSH,  RSFT,\
  //|------+------+------+------+------+------+------|  |------+------+------+------+------+------+------|
                                  RGUI, SPC, LOWER,      RAISE, ENT,  MO(_EXTRA) \
                              //`--------------------'  `--------------------'
  ),

  [_LOWER] = LAYOUT_kc( \
  //,-----------------------------------------.                                  ,-----------------------------------------.
      TAB,      1,     2,     3,     4,     5,                                        6,     7,     8,     9,     0,  BSPC,\
  //|--------+------+------+------+------+------|                                  |------+------+------+------+------+------|
      SFT_T(KC_ESC),    VIM_PREV_TAB,  VIM_NEW_TAB, VIM_H_SPLIT,  VIM_V_SPLIT,  VIM_SAVE, VIM_PANE_LEFT,  VIM_PANE_DOWN,  VIM_PANE_UP,  VIM_PANE_RIGHT,  VIM_NEXT_TAB, XX,\
  //|--------+------+------+------+------+------|                                  |------+------+------+------+------+------|
      LCTL,   TMUX_PREV_TAB, TMUX_ZOOM,   TMUX_H_SPLIT, TMUX_V_SPLIT, TMUX_EDIT_MODE,  TMUX_PANE_LEFT, TMUX_PANE_DOWN, TMUX_PANE_UP, TMUX_PANE_RIGHT, TMUX_NEXT_TAB, XX,\
  //|------+------+------+------+------+------+------|  |------+------+------+------+------+------+------|
                                  RGUI, SPC, LOWER,      RAISE, ENT,  MO(_EXTRA) \
                              //`--------------------'  `--------------------'
  ),

  [_RAISE] = LAYOUT_kc( \
  //,-----------------------------------------.                ,-----------------------------------------.
      TAB,  XX   ,  AT  , LCBR , RCBR , PIPE ,                 GRAVE, EXCLAIM, MINUS,  EQUAL,  XX, BSPC, \
  //|------------+------+------+------+------|                |------+------+------+------+------+------|
      SFT_T(KC_ESC), LSFT, TILD,  LPRN, RPRN, UNDS,                              LEFT,  DOWN,  UP,    RGHT,  QUOTE , XX, \
  //|------------+------+------+------+------|                |------+------+------+------+------+------|
      LCTL, ASTERISK , HASH, LBRC, RBRC, AMPR,                          PERCENT, BSLASH,  DOLLAR, PLUS ,  COLON,XX, \
  //|------+------+------+------+------+------+------|  |------+------+------+------+------+------+------|
                                  RGUI, SPC, LOWER,      RAISE, ENT,  MO(_EXTRA) \
                              //`--------------------'  `--------------------'
  ),

  [_EXTRA] = LAYOUT_kc( \
  //,-----------------------------------------.                ,-----------------------------------------.
      SPCT_LEFT_HALF, XX, XX, SPCT_MAXIMIZE,  XX, SPCT_RIGHT_HALF,VIM_OPEN_FOLDS, VIM_PASTE_LAST_REGISTER, VIM_COPY_FILE_PATH, VIM_EQUALIZE_PANES, VIM_TOGGLE_COMMENT,  XX,\
  //|------+------+------+------+------+------|                |------+------+------+------+------+------|
      XX   , XX   , XX   , XX   , XX   ,  XX  ,                   XX  ,  XX  ,  XX  ,  XX  , XX , XX ,\
  //|------+------+------+------+------+------|                |------+------+------+------+------+h------|
      XX   , XX   , XX   , XX   , XX   ,  XX  ,                   XX  ,  XX  ,  XX  ,  XX  , XX , XX ,\
  //|------+------+------+------+------+------+------|  |------+------+------+------+------+------+------|
                                  RGUI, SPC, LOWER,      RAISE, ENT,  MO(_EXTRA) \
                              //`--------------------'  `--------------------'
  ),

  [_ADJUST] = LAYOUT_kc( \
  //,-----------------------------------------.                ,-----------------------------------------.
      XX  ,  F1  ,  F2  ,  F3  ,  F4  ,  F5  ,                  F6,    F7,   F8,   F9,    F10, XX,\
  //|------+------+------+------+------+------|                |------+------+------+------+------+------|
      XX   ,  F11 ,  F12 , _MUTE, _VOLDOWN, _VOLUP,                 MS_L,  MS_D, MS_U, MS_R,  WH_U, XX,\
  //|------+------+------+------+------+------|                |------+------+------+------+------+------|
      XX   , XX   ,   XX ,  XX  ,  XX  ,  XX  ,                  MRWD, MFFD, MPLY, SCREENSHOT , WH_D, XX,\
  //|------+------+------+------+------+------+------|  |------+------+------+------+------+------+------|
                                 RST,      XX,   XX,        XX,     XX,    XX \
                              //`--------------------'  `--------------------'
  )
};

void persistent_default_layer_set(uint16_t default_layer) {
  eeconfig_update_default_layer(default_layer);
  default_layer_set(default_layer);
}

void oled_task_user(void) {
  // Host Keyboard Layer Status
  switch (get_highest_layer(layer_state)) {
    case 0:
      oled_write_P(PSTR("<< QWERTY >>"), false);
      break;
    case 2:
      oled_write_P(PSTR("<< LOWER >>"), false);
      break;
    case 3:
      oled_write_P(PSTR("<< RAISE >>"), false);
      break;
    case 4:
      oled_write_P(PSTR("<< EXTRA >>"), false);
      break;
    case 16:
      oled_write_P(PSTR("<< ADJUST >>"), false);
      break;
    default:
      // Or use the write_ln shortcut over adding '\n' to the end of your string
      oled_write_ln_P(PSTR("WTF?"), false);
  }

  // Host Keyboard LED Status
  uint8_t led_usb_state = host_keyboard_leds();
  oled_write_P(led_usb_state & (1<<USB_LED_NUM_LOCK) ? PSTR("NUMLCK ") : PSTR("       "), false);
  oled_write_P(led_usb_state & (1<<USB_LED_CAPS_LOCK) ? PSTR("CAPLCK ") : PSTR("       "), false);
  oled_write_P(led_usb_state & (1<<USB_LED_SCROLL_LOCK) ? PSTR("SCRLCK ") : PSTR("       "), false);
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
    case QWERTY:
      if (record->event.pressed) {
        persistent_default_layer_set(1UL<<_QWERTY);
      }
      return false;
      break;
    case LOWER:
      if (record->event.pressed) {
        layer_on(_LOWER);
        update_tri_layer(_LOWER, _RAISE, _ADJUST);
      } else {
        layer_off(_LOWER);
        update_tri_layer(_LOWER, _RAISE, _ADJUST);
      }
      return false;
      break;
    case RAISE:
      if (record->event.pressed) {
        layer_on(_RAISE);
        update_tri_layer(_LOWER, _RAISE, _ADJUST);
      } else {
        layer_off(_RAISE);
        update_tri_layer(_LOWER, _RAISE, _ADJUST);
      }
      return false;
      break;
    case ADJUST:
        if (record->event.pressed) {
          layer_on(_ADJUST);
        } else {
          layer_off(_ADJUST);
        }
        return false;
        break;
    // TMUX shortcuts
    case KC_TMUX_ZOOM:
      if (record->event.pressed) {
        SEND_STRING("`z");
      }
      return false;
      break;
    case KC_TMUX_EDIT_MODE:
      if (record->event.pressed) {
        SEND_STRING("`[");
      }
      return false;
      break;
    case KC_TMUX_NEW_TAB:
      if (record->event.pressed) {
        SEND_STRING("`c");
      }
      return false;
      break;
    case KC_TMUX_V_SPLIT:
      if (record->event.pressed) {
        SEND_STRING("`b");
      }
      return false;
      break;
    case KC_TMUX_H_SPLIT:
      if (record->event.pressed) {
        SEND_STRING("`v");
      }
      return false;
      break;
    case KC_TMUX_PREV_TAB:
      if (record->event.pressed) {
        SEND_STRING("`p");
      }
      return false;
      break;
    case KC_TMUX_NEXT_TAB:
      if (record->event.pressed) {
        SEND_STRING("`n");
      }
      return false;
      break;
    case KC_TMUX_PANE_LEFT:
      if (record->event.pressed) {
        SEND_STRING("`h");
      }
      return false;
      break;
    case KC_TMUX_PANE_RIGHT:
      if (record->event.pressed) {
        SEND_STRING("`l");
      }
      return false;
      break;
    case KC_TMUX_PANE_UP:
      if (record->event.pressed) {
        SEND_STRING("`k");
      }
      return false;
      break;
    case KC_TMUX_PANE_DOWN:
      if (record->event.pressed) {
        SEND_STRING("`j");
      }
      return false;
      break;
    // VIM shortcuts
    case KC_VIM_PASTE_LAST_REGISTER:
      if (record->event.pressed) {
        SEND_STRING(SS_TAP(X_ESCAPE)"\"0p");
      }
      return false;
      break;
    case KC_VIM_NEW_TAB:
      if (record->event.pressed) {
        SEND_STRING(SS_TAP(X_ESCAPE)":tabe"SS_TAP(X_ENTER));
      }
      return false;
      break;
    case KC_VIM_PANE_UP:
      if (record->event.pressed) {
        SEND_STRING(SS_TAP(X_ESCAPE)SS_LCTRL("w")"k");
      }
      return false;
      break;
    case KC_VIM_PANE_DOWN:
      if (record->event.pressed) {
        SEND_STRING(SS_TAP(X_ESCAPE)SS_LCTRL("w")"j");
      }
      return false;
      break;
    case KC_VIM_PANE_RIGHT:
      if (record->event.pressed) {
        SEND_STRING(SS_TAP(X_ESCAPE)SS_LCTRL("w")"l");
      }
      return false;
      break;
    case KC_VIM_PANE_LEFT:
      if (record->event.pressed) {
        SEND_STRING(SS_TAP(X_ESCAPE)SS_LCTRL("w")"h");
      }
      return false;
      break;
    case KC_VIM_H_SPLIT:
      if (record->event.pressed) {
        SEND_STRING(SS_TAP(X_ESCAPE)":sp"SS_TAP(X_ENTER));
      }
      return false;
      break;
    case KC_VIM_V_SPLIT:
      if (record->event.pressed) {
        SEND_STRING(SS_TAP(X_ESCAPE)":vsp"SS_TAP(X_ENTER));
      }
      return false;
      break;
    case KC_VIM_PREV_TAB:
      if (record->event.pressed) {
        SEND_STRING(SS_TAP(X_ESCAPE)"gT");
      }
      return false;
      break;
    case KC_VIM_NEXT_TAB:
      if (record->event.pressed) {
        SEND_STRING(SS_TAP(X_ESCAPE)"gt");
      }
      return false;
      break;
    case KC_VIM_SAVE:
      if (record->event.pressed) {
        SEND_STRING(SS_TAP(X_ESCAPE)":w"SS_TAP(X_ENTER));
      }
      return false;
      break;
    case KC_VIM_EQUALIZE_PANES:
      if (record->event.pressed) {
        SEND_STRING(SS_LCTRL("w")SS_TAP(X_EQUAL));
      }
      return false;
      break;
    case KC_VIM_TAB_CLOSE:
      if (record->event.pressed) {
        SEND_STRING(SS_TAP(X_ESCAPE)":tabclose"SS_TAP(X_ENTER));
      }
      return false;
      break;
    case KC_VIM_TOGGLE_COMMENT:
      if (record->event.pressed) {
        SEND_STRING(SS_TAP(X_SPACE)"ci");
      }
      return false;
      break;
    case KC_VIM_UNWRAP_WORD:
      if (record->event.pressed) {
        SEND_STRING("ysiy");
      }
      return false;
      break;
    case KC_VIM_WRAP_WORD:
      if (record->event.pressed) {
        SEND_STRING("ysiw");
      }
      return false;
      break;
    case KC_VIM_OPEN_FOLDS:
      if (record->event.pressed) {
        SEND_STRING(SS_TAP(X_ESCAPE)"zR");
      }
      return false;
      break;
    case KC_VIM_COPY_FILE_PATH:
      if (record->event.pressed) {
        SEND_STRING(SS_TAP(X_ESCAPE)":let @+ = expand(\"%\")"SS_TAP(X_ENTER));
      }
      return false;
      break;
    case KC_SPCT_MAXIMIZE:
      if (record->event.pressed) {
        SEND_STRING(SS_LCTRL(SS_LGUI("m")));
      }
      return false;
      break;
    case KC_SPCT_LEFT_HALF:
      if (record->event.pressed) {
        SEND_STRING(SS_LCTRL(SS_LGUI("h")));
      }
      return false;
      break;
    case KC_SPCT_RIGHT_HALF:
      if (record->event.pressed) {
        SEND_STRING(SS_LCTRL(SS_LGUI("l")));
      }
      return false;
      break;
    case KC_SCREENSHOT:
      if (record->event.pressed) {
        SEND_STRING(SS_LCTRL(SS_LGUI("s")));
      }
      return false;
      break;
  }
  return true;
}

