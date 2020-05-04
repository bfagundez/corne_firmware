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
  KC_CLOSE_TMUX_PANE,
  KC_TMUX_COPY_VISUAL_BLOCK,
  KC_SPCT_MAXIMIZE,
  KC_SPCT_LEFT_HALF,
  KC_SPCT_RIGHT_HALF,
  KC_SCREENSHOT,
  KC_CYCLE_FOCUS_CLOCKWISE,
  KC_CYCLE_FOCUS_COUNTER_CLOCKWISE,
  KC_CYCLE_FOCUS_WINDOW,
  KC_CYCLE_WINDOW_LAYOUT,
  KC_CYCLE_WINDOW_LOCATION,
  KC_MOVE_WINDOW_TO_NEXT_SCREEN
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
      LCTL,     Z,     X,     C,     V,     B,                      N,     M,  COMM,   DOT,  SLSH,  LALT,\
  //|------+------+------+------+------+------+------|  |------+------+------+------+------+------+------|
                                  RGUI, SPC, LOWER,      RAISE, ENT,  MO(_EXTRA) \
                              //`--------------------'  `--------------------'
  ),

  [_LOWER] = LAYOUT_kc( \
  //,-----------------------------------------.                                  ,-----------------------------------------.
      TAB,      1,     2,     3,     4,     5,                                        6,     7,     8,     9,     0,  BSPC,\
  //|--------+------+------+------+------+------|                                  |------+------+------+------+------+------|
      VIM_PREV_TAB, VIM_PREV_TAB,  VIM_NEW_TAB, VIM_H_SPLIT,  VIM_V_SPLIT,  VIM_SAVE, VIM_PANE_LEFT,  VIM_PANE_DOWN,  VIM_PANE_UP,  VIM_PANE_RIGHT,  VIM_NEXT_TAB, VIM_NEXT_TAB,\
  //|--------+------+------+------+------+------|                                  |------+------+------+------+------+------|
     TMUX_PREV_TAB,   TMUX_PREV_TAB, TMUX_ZOOM,   TMUX_H_SPLIT, TMUX_V_SPLIT, TMUX_EDIT_MODE,  TMUX_PANE_LEFT, TMUX_PANE_DOWN, TMUX_PANE_UP, TMUX_PANE_RIGHT, TMUX_NEXT_TAB, TMUX_NEXT_TAB,\
  //|------+------+------+------+------+------+------|  |------+------+------+------+------+------+------|
                                  RGUI, SPC, LOWER,      RAISE, ENT,  MO(_EXTRA) \
                              //`--------------------'  `--------------------'
  ),

  [_RAISE] = LAYOUT_kc( \
  //,-----------------------------------------.                ,-----------------------------------------.
      TAB,  XX   ,  AT  , LCBR , RCBR , PIPE ,                 GRAVE, EXCLAIM, MINUS, EQUAL,  PLUS, BSPC, \
  //|------------+------+------+------+------|                |------+------+------+------+------+------|
      ESC, LSFT  , TILD ,  LPRN, RPRN, UNDS,                    LEFT,  DOWN,  UP,    RGHT,  QUOTE , XX, \
  //|------------+------+------+------+------|                |------+------+------+------+------+------|
      LCTL, ASTERISK , HASH, LBRC, RBRC, AMPR,                  PERCENT, BSLASH,  DOLLAR, PLUS ,  COLON,XX, \
  //|------+------+------+------+------+------+------|  |------+------+------+------+------+------+------|
                                  RGUI, SPC, LOWER,      RAISE, ENT,  MO(_EXTRA) \
                              //`--------------------'  `--------------------'
  ),

  [_EXTRA] = LAYOUT_kc( \
  //,-----------------------------------------.                ,-----------------------------------------.
      SPCT_LEFT_HALF, XX, XX, SPCT_MAXIMIZE,  XX, SPCT_RIGHT_HALF,VIM_OPEN_FOLDS, VIM_PASTE_LAST_REGISTER, VIM_COPY_FILE_PATH, VIM_EQUALIZE_PANES, VIM_TOGGLE_COMMENT,  XX,\
  //|------+------+------+------+------+------|                |------+------+------+------+------+------|
      CLOSE_TMUX_PANE , XX , XX , XX, XX, XX  ,                  SCREENSHOT  ,  XX  ,  XX  ,  XX  , XX , TMUX_NEW_TAB ,\
  //|------+------+------+------+------+------|                |------+------+------+------+------+h------|
      XX   , XX  , XX   , TMUX_COPY_VISUAL_BLOCK , XX   ,  XX  ,  XX  ,  XX  ,  XX  ,  XX  , XX , XX ,\
  //|------+------+------+------+------+------+------|  |------+------+------+------+------+------+------|
                                  RGUI, SPC, LOWER,      RAISE, ENT,  MO(_EXTRA) \
                              //`--------------------'  `--------------------'
  ),

  [_ADJUST] = LAYOUT_kc( \
  //,-----------------------------------------.                ,-----------------------------------------.
      MOVE_WINDOW_TO_NEXT_SCREEN  ,  F1  ,  F2  ,  F3  ,  F4  ,  F5  ,                  F6,    F7,   F8,   F9,    F10, XX,\
  //|------+------+------+------+------+------|                |------+------+------+------+------+------|
      CYCLE_FOCUS_CLOCKWISE   ,  F11 ,  F12 , _MUTE, _VOLDOWN, _VOLUP,                 MS_L,  MS_D, MS_U, MS_R,  WH_U, CYCLE_FOCUS_COUNTER_CLOCKWISE,\
  //|------+------+------+------+------+------|                |------+------+------+------+------+------|
      CYCLE_WINDOW_LAYOUT , XX , XX ,XX, XX,  CYCLE_FOCUS_WINDOW  ,  MRWD, MFFD, MPLY, SCREENSHOT , WH_D, CYCLE_WINDOW_LOCATION,\
  //|------+------+------+------+------+------+------|  |------+------+------+------+------+------+------|
                                 RST,      XX,   XX,        XX,     XX,    XX \
                              //`--------------------'  `--------------------'
  )
};

void persistent_default_layer_set(uint16_t default_layer) {
  eeconfig_update_default_layer(default_layer);
  default_layer_set(default_layer);
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
    case KC_TMUX_COPY_VISUAL_BLOCK:
      if (record->event.pressed) {
        SEND_STRING(SS_LCTRL("v")SS_TAP(X_SPACE));
      }
      return false;
      break;
    case KC_CLOSE_TMUX_PANE:
      if (record->event.pressed) {
        SEND_STRING("`xy"SS_TAP(X_ENTER));
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
    case KC_CYCLE_FOCUS_CLOCKWISE:
      if (record->event.pressed) {
        SEND_STRING(SS_LALT(SS_LSFT("p")));
      }
      return false;
      break;
    case KC_CYCLE_FOCUS_COUNTER_CLOCKWISE:
      if (record->event.pressed) {
        SEND_STRING(SS_LALT(SS_LSFT("n")));
      }
      return false;
      break;
    case KC_CYCLE_FOCUS_WINDOW:
      if (record->event.pressed) {
        SEND_STRING(SS_LALT(SS_LSFT("k")));
      }
      return false;
      break;
    case KC_CYCLE_WINDOW_LAYOUT:
      if (record->event.pressed) {
        SEND_STRING(SS_LALT(SS_LSFT(SS_TAP(X_SPACE))));
      }
      return false;
      break;
    case KC_CYCLE_WINDOW_LOCATION:
      if (record->event.pressed) {
        SEND_STRING(SS_LALT(SS_LSFT(SS_LCTRL("h"))));
      }
      return false;
      break;
    case KC_MOVE_WINDOW_TO_NEXT_SCREEN:
      if (record->event.pressed) {
        SEND_STRING(SS_LALT(SS_LSFT(SS_LCTRL("h"))));
      }
      return false;
      break;
  }
  return true;
}

/////////////////
// OLED STUFF
/////////////////

void render_mod_status(uint8_t modifiers) {
    oled_write_P(PSTR("Sft"), (modifiers & MOD_MASK_SHIFT));
    oled_write_P(PSTR(" "), false);
    oled_write_P(PSTR("Ctl"), (modifiers & MOD_MASK_CTRL));
    oled_write_P(PSTR(" "), false);
    oled_write_P(PSTR("Alt"), (modifiers & MOD_MASK_ALT));
    oled_write_P(PSTR(" "), false);
    oled_write_P(PSTR("GUI"), (modifiers & MOD_MASK_GUI));
}


static void render_status(void){
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
}


static void render_logo(void) {
  static const char PROGMEM qmk_logo[] = {
    0x80,0x81,0x82,0x83,0x84,0x85,0x86,0x87,0x88,0x89,0x8a,0x8b,0x8c,0x8d,0x8e,0x8f,0x90,0x91,0x92,0x93,0x94,
    0xa0,0xa1,0xa2,0xa3,0xa4,0xa5,0xa6,0xa7,0xa8,0xa9,0xaa,0xab,0xac,0xad,0xae,0xaf,0xb0,0xb1,0xb2,0xb3,0xb4,
    0xc0,0xc1,0xc2,0xc3,0xc4,0xc5,0xc6,0xc7,0xc8,0xc9,0xca,0xcb,0xcc,0xcd,0xce,0xcf,0xd0,0xd1,0xd2,0xd3,0xd4,0};

  oled_write_P(qmk_logo, false);
}

oled_rotation_t oled_init_user(oled_rotation_t rotation) {
  oled_clear();
  if (!is_master)
    return OLED_ROTATION_180;  // flips the display 180 degrees if offhand
  return rotation;
}

void oled_task_user(void) {
  // Host Keyboard Layer Status
  if (is_master) {
    oled_set_cursor(5, 0);
    render_status();     // Renders the current keyboard state (layer, lock, caps, scroll, etc)
    oled_set_cursor(0, 2);
    oled_write_P(PSTR("--------------------"), false);
    oled_set_cursor(3, 3);
    render_mod_status(get_mods() | get_oneshot_mods()); // Renders pressed modifier
  } else {
    render_logo();       // Renders a statuc logo
    oled_scroll_left();  // Turns on scrolling
  }
}
