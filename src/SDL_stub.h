#pragma once

#include <stdint.h>

typedef uint8_t  Uint8;
typedef uint16_t Uint16;
typedef uint32_t Uint32;
typedef int16_t  Sint16;
typedef int32_t  Sint32;

typedef struct SDL_Color
{
    Uint8 r;
    Uint8 g;
    Uint8 b;
    Uint8 a;
} SDL_Color;

typedef struct SDL_Surface
{
    Uint32 flags;               /**< Read-only */
    //SDL_PixelFormat *format;    /**< Read-only */
    int w, h;                   /**< Read-only */
    int pitch;                  /**< Read-only */
    void *pixels;               /**< Read-write */

    /** Application data associated with the surface */
    void *userdata;             /**< Read-write */

    /** information needed for surfaces requiring locks */
    int locked;                 /**< Read-only */

    /** list of BlitMap that hold a reference to this surface */
    void *list_blitmap;         /**< Private */

    /** clipping information */
    //SDL_Rect clip_rect;         /**< Read-only */

    /** info for fast blit mapping to other surfaces */
    //SDL_BlitMap *map;           /**< Private */

    /** Reference count -- used when freeing surface */
    // int refcount;               /**< Read-mostly */
} SDL_Surface;

typedef enum SDL_Keycode  { Keycode_Empty = 0 } SDL_Keycode;
typedef enum SDL_Keymod   { Keymod_Empty = 0 } SDL_Keymod;
typedef enum SDL_Scancode { Scancode_Empty = 0 } SDL_Scancode;

typedef struct SDL_Keysym
{
    SDL_Scancode scancode;      /**< SDL physical key code - see SDL_Scancode for details */
    SDL_Keycode sym;            /**< SDL virtual key code - see SDL_Keycode for details */
    Uint16 mod;                 /**< current key modifiers - see SDL_Keymod for details */
    Uint32 unused;
} SDL_Keysym;

typedef Sint32 SDL_JoystickID;

typedef struct SDL_JoyButtonEvent
{
    Uint32 type;        /**< SDL_JOYBUTTONDOWN or SDL_JOYBUTTONUP */
    Uint32 timestamp;   /**< In milliseconds, populated using SDL_GetTicks() */
    SDL_JoystickID which; /**< The joystick instance id */
    Uint8 button;       /**< The joystick button index */
    Uint8 state;        /**< SDL_PRESSED or SDL_RELEASED */
    Uint8 padding1;
    Uint8 padding2;
} SDL_JoyButtonEvent;

typedef struct SDL_JoyAxisEvent
{
    Uint32 type;        /**< SDL_JOYAXISMOTION */
    Uint32 timestamp;   /**< In milliseconds, populated using SDL_GetTicks() */
    SDL_JoystickID which; /**< The joystick instance id */
    Uint8 axis;         /**< The joystick axis index */
    Uint8 padding1;
    Uint8 padding2;
    Uint8 padding3;
    Sint16 value;       /**< The axis value (range: -32768 to 32767) */
    Uint16 padding4;
} SDL_JoyAxisEvent;

typedef union SDL_Event
{
    Uint32 type;
} SDL_Event;

typedef struct SDL_Rect
{
    int x, y;
    int w, h;
} SDL_Rect;

typedef struct SDL_Window SDL_Window;
typedef Uint32 SDL_AudioDeviceID;
typedef struct SDL_Joystick SDL_Joystick;
typedef struct SDL_Renderer SDL_Renderer;
