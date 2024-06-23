#pragma once

#include <stdint.h>
#include <keyboard.h>

using namespace nov::keyboard;

static const Key scan_code_set_1_key[256]
{
    K_NONE,         K_ESCAPE,       K_1,            K_2,            
    K_3,            K_4,            K_5,            K_6,
    K_7,            K_8,            K_9,            K_0,            
    K_DASH,         K_EQUALS,       K_BACKSPACE,    K_TAB,
    K_Q,            K_W,            K_E,            K_R,            
    K_T,            K_Y,            K_U,            K_I,
    K_O,            K_P,            K_OSQBRACKET,   K_CSQBRACKET,   
    K_ENTER,        K_L_CTRL,       K_A,            K_S,
    K_D,            K_F,            K_G,            K_H,            
    K_J,            K_K,            K_L,            K_SEMICOLON,
    K_QUOTE,        K_BACKTICK,     K_L_SHIFT,      K_BACKSLASH,    
    K_Z,            K_X,            K_C,            K_V,
    K_B,            K_N,            K_M,            K_COMMA,        
    K_DOT,          K_FORWSLASH,    K_R_SHIFT,      K_N_STAR,
    K_L_ALT,        K_SPACEBAR,     K_CAPSLOCK,     K_F_1,          
    K_F_2,          K_F_3,          K_F_4,          K_F_5,
    K_F_6,          K_F_7,          K_F_8,          K_F_9,          
    K_F_10,         K_NUMLOCK,      K_SCROLLLOCK,   K_N_7,
    K_N_8,          K_N_9,          K_N_DASH,       K_N_4,          
    K_N_5,          K_N_6,          K_N_PLUS,       K_N_1,
    K_N_2,          K_N_3,          K_N_0,          K_N_DOT,        
    K_NONE,         K_NONE,         K_NONE,         K_F_11,
    K_F_12,         K_NONE,         K_NONE,         K_NONE,
    K_NONE,         K_NONE,         K_NONE,         K_NONE,         
    K_NONE,         K_NONE,         K_NONE,         K_NONE,         
    K_NONE,         K_NONE,         K_NONE,         K_NONE,         
    K_NONE,         K_NONE,         K_NONE,         K_NONE,         
    K_NONE,         K_NONE,         K_NONE,         K_NONE,         
    K_NONE,         K_NONE,         K_NONE,         K_NONE,         
    K_NONE,         K_NONE,         K_NONE,         K_NONE,         
    K_NONE,         K_NONE,         K_NONE,         K_NONE,         
    K_NONE,         K_NONE,         K_NONE,         K_NONE,         
    K_NONE,         K_ESCAPE,       K_1,            K_2,
    K_3,            K_4,            K_5,            K_6,            
    K_7,            K_8,            K_9,            K_0,
    K_DASH,         K_EQUALS,       K_BACKSPACE,    K_TAB,          
    K_Q,            K_W,            K_E,            K_R,
    K_T,            K_Y,            K_U,            K_I,
    K_O,            K_P,            K_OSQBRACKET,   K_CSQBRACKET,   
    K_ENTER,        K_L_CTRL,       K_A,            K_S,
    K_D,            K_F,            K_G,            K_H,            
    K_J,            K_K,            K_L,            K_SEMICOLON,
    K_QUOTE,        K_BACKTICK,     K_L_SHIFT,      K_BACKSLASH,    
    K_Z,            K_X,            K_C,            K_V,
    K_B,            K_N,            K_M,            K_COMMA,        
    K_DOT,          K_FORWSLASH,    K_R_SHIFT,      K_N_STAR,
    K_L_ALT,        K_SPACEBAR,     K_CAPSLOCK,     K_F_1,          
    K_F_2,          K_F_3,          K_F_4,          K_F_5,
    K_F_6,          K_F_7,          K_F_8,          K_F_9,          
    K_F_10,         K_NUMLOCK,      K_SCROLLLOCK,   K_N_7,
    K_N_8,          K_N_9,          K_N_DASH,       K_N_4,          
    K_N_5,          K_N_6,          K_N_PLUS,       K_N_1,
    K_N_2,          K_N_3,          K_N_0,          K_N_DOT,        
    K_NONE,         K_NONE,         K_NONE,         K_F_11,
    K_F_12,         K_NONE,         K_NONE,         K_NONE,
};

static const bool scan_code_set_1_down[256]
{
    0, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0
};

static const Key scan_code_set_extended_1_key[256]
{
    K_NONE,         K_NONE,         K_NONE,         K_NONE,
    K_NONE,         K_NONE,         K_NONE,         K_NONE,
    K_NONE,         K_NONE,         K_NONE,         K_NONE,
    K_NONE,         K_NONE,         K_NONE,         K_NONE,
    K_M_PREV,       K_NONE,         K_NONE,         K_NONE,
    K_NONE,         K_NONE,         K_NONE,         K_NONE,
    K_NONE,         K_M_NEXT,       K_NONE,         K_NONE,
    K_N_ENTER,      K_R_CTRL,       K_NONE,         K_NONE,
    K_M_MUTE,       K_M_CALC,       K_M_PAUSE,      K_NONE,
    K_M_STOP,       K_NONE,         K_NONE,         K_NONE,
    K_NONE,         K_NONE,         K_NONE,         K_NONE,
    K_NONE,         K_NONE,         K_M_VOLDOWN,    K_NONE,
    K_M_VOLUP,      K_NONE,         K_W_HOME,       K_NONE,
    K_NONE,         K_N_SLASH,      K_NONE,         K_NONE,
    K_R_ALT,        K_NONE,         K_NONE,         K_NONE,
    K_NONE,         K_NONE,         K_NONE,         K_NONE,
    K_NONE,         K_NONE,         K_NONE,         K_NONE,
    K_NONE,         K_NONE,         K_NONE,         K_HOME,
    K_UP,           K_PAGEUP,       K_NONE,         K_LEFT,
    K_NONE,         K_RIGHT,        K_NONE,         K_END,
    K_DOWN,         K_PAGEDOWN,     K_INSERT,       K_DELETE,
    K_NONE,         K_NONE,         K_NONE,         K_NONE,
    K_NONE,         K_NONE,         K_NONE,         K_L_GUI,
    K_R_GUI,        K_APPS,         K_POWER,        K_SLEEP,
    K_NONE,         K_NONE,         K_NONE,         K_WAKE,
    K_NONE,         K_W_SEARCH,     K_W_FAVOURITES, K_W_REFRESH,
    K_W_STOP,       K_W_FORWARD,    K_W_BACK,       K_M_COMPUTER,
    K_M_EMAIL,      K_M_SELECT,     K_NONE,         K_NONE,
    K_NONE,         K_NONE,         K_NONE,         K_NONE,
    K_NONE,         K_NONE,         K_NONE,         K_NONE,
    K_NONE,         K_NONE,         K_NONE,         K_NONE,
    K_NONE,         K_NONE,         K_NONE,         K_NONE,
    K_NONE,         K_NONE,         K_NONE,         K_NONE,
    K_NONE,         K_NONE,         K_NONE,         K_NONE,
    K_NONE,         K_NONE,         K_NONE,         K_NONE,
    K_NONE,         K_NONE,         K_NONE,         K_NONE,
    K_M_PREV,       K_NONE,         K_NONE,         K_NONE,
    K_NONE,         K_NONE,         K_NONE,         K_NONE,
    K_NONE,         K_M_NEXT,       K_NONE,         K_NONE,
    K_N_ENTER,      K_R_CTRL,       K_NONE,         K_NONE,
    K_M_MUTE,       K_M_CALC,       K_M_PAUSE,      K_NONE,
    K_M_STOP,       K_NONE,         K_NONE,         K_NONE,
    K_NONE,         K_NONE,         K_NONE,         K_NONE,
    K_NONE,         K_NONE,         K_M_VOLDOWN,    K_NONE,
    K_M_VOLUP,      K_NONE,         K_W_HOME,       K_NONE,
    K_NONE,         K_N_SLASH,      K_NONE,         K_NONE,
    K_R_ALT,        K_NONE,         K_NONE,         K_NONE,
    K_NONE,         K_NONE,         K_NONE,         K_NONE,
    K_NONE,         K_NONE,         K_NONE,         K_NONE,
    K_NONE,         K_NONE,         K_NONE,         K_HOME,
    K_UP,           K_PAGEUP,       K_NONE,         K_LEFT,
    K_NONE,         K_RIGHT,        K_NONE,         K_END,
    K_DOWN,         K_PAGEDOWN,     K_INSERT,       K_DELETE,
    K_NONE,         K_NONE,         K_NONE,         K_NONE,
    K_NONE,         K_NONE,         K_NONE,         K_L_GUI,
    K_R_GUI,        K_APPS,         K_POWER,        K_SLEEP,
    K_NONE,         K_NONE,         K_NONE,         K_WAKE,
    K_NONE,         K_W_SEARCH,     K_W_FAVOURITES, K_W_REFRESH,
    K_W_STOP,       K_W_FORWARD,    K_W_BACK,       K_M_COMPUTER,
    K_M_EMAIL,      K_M_SELECT,     K_NONE,         K_NONE,
};

static const bool scan_code_set_extended_1_down[256]
{
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    1, 0, 0, 0, 0, 0, 0, 0,
    0, 1, 0, 0, 1, 1, 0, 0,
    1, 1, 1, 0, 1, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 1, 0,
    1, 0, 1, 0, 0, 1, 0, 0,
    1, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 1,
    1, 1, 0, 1, 0, 1, 0, 1,
    1, 1, 1, 1, 0, 0, 0, 0,
    0, 0, 0, 1, 1, 1, 1, 1,
    0, 0, 0, 1, 0, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
};