/* Caprice32 - Amstrad CPC Emulator
   (c) Copyright 1997-2004 Ulrich Doewich

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

/*
   This file includes video filters from the SMS Plus/SDL
   sega master system emulator :
   (c) Copyright Gregory Montoir
   http://membres.lycos.fr/cyxdown/smssdl/
*/

/*
   This file includes video filters from MAME
   (Multiple Arcade Machine Emulator) :
   (c) Copyright The MAME Team
   http://www.mame.net/
*/

#include "video.h"
#include "cap32.h"
#include "log.h"
//#include "glfuncs.h"
//#ifdef HAVE_GL
//#include "SDL_opengl.h"
//#endif
#include <math.h>
#include <memory>
#include <iostream>

extern "C" {
#include "../libs/gpu.h"
}

#if 0
SDL_Window* mainSDLWindow = nullptr;
SDL_Renderer* renderer = nullptr;
SDL_Texture* texture = nullptr;
SDL_GLContext glcontext;
#endif

// the video surface ready to display
SDL_Surface* vid = nullptr;
// the video surface scaled with same format as pub
// SDL_Surface* scaled = nullptr;
// the video surface shown by the plugin to the application
SDL_Surface* pub = nullptr;

extern t_CPC CPC;

#ifndef min
#define min(a,b) ((a)<(b) ? (a) : (b))
#endif

#ifndef max
#define max(a,b) ((a)>(b) ? (a) : (b))
#endif

// checks for an OpenGL extension
#ifdef HAVE_GL
static bool have_gl_extension (const char *nom_ext)
{
   const char *ext;
   ext = reinterpret_cast<const char *> (eglGetString (GL_EXTENSIONS));
   const char *f;
   if (ext == nullptr)
      return false;
   f = ext + strlen (ext);
   while (ext < f)
   {
      unsigned int n = strcspn (ext, " ");
      if ((strlen (nom_ext) == n) && (strncmp (nom_ext, ext, n) == 0))
         return true;
      ext += (n + 1);
   }
   return false;
}
#endif

// Returns a bpp compatible with the renderer
int renderer_bpp(SDL_Renderer *sdl_renderer)
{
  return 8;
#if 0
  SDL_RendererInfo infos;
  SDL_GetRendererInfo(sdl_renderer, &infos);
  return SDL_BITSPERPIXEL(infos.texture_formats[0]);
#endif
}

// TODO: Cleanup sw_scaling if really not needed
void compute_scale(video_plugin* t, int w, int h)
{
#if 0
  int win_width, win_height;
  SDL_GetWindowSize(mainSDLWindow, &win_width, &win_height);
  if (CPC.scr_preserve_aspect_ratio != 0) {
    float win_x_scale, win_y_scale;
    win_x_scale = w/static_cast<float>(win_width);
    win_y_scale = h/static_cast<float>(win_height);
    float scale = max(win_x_scale, win_y_scale);
    t->width=w/scale;
    t->height=h/scale;
    float x_offset = 0.5*(win_width-t->width);
    float y_offset = 0.5*(win_height-t->height);
    t->x_offset=x_offset;
    t->y_offset=y_offset;
    t->x_scale=scale;
    t->y_scale=scale;
  } else {
    t->x_offset=0;
    t->y_offset=0;
    t->x_scale=w/static_cast<float>(win_width);
    t->y_scale=h/static_cast<float>(win_height);
    t->width = win_width;
    t->height = win_height;
  }
#endif
}

/* ------------------------------------------------------------------------------------ */
/* Half size video plugin ------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------ */

static SDL_Surface *make_surface(int w,int h)
{
  SDL_Surface *srf = new SDL_Surface;
  srf->flags = 0;
  srf->w = w;
  srf->h = h;
  srf->pitch = srf->w;
  printf("surface: allocating %dx%d pixels\n",srf->w,srf->h);
  srf->pixels = new Uint8[srf->w*srf->h];
  memset(srf->pixels,0x00,srf->w*srf->h);
  srf->userdata = 0;
  srf->list_blitmap = nullptr;
  srf->locked = 0;
  return srf;
}

SDL_Surface* direct_init(video_plugin*t, int scale, bool fs)
{
  printf("direct_init [1]\n");
  int surface_width, surface_height;
  if (scale > 1) {
    t->half_pixels = 0;
    surface_width = CPC_VISIBLE_SCR_WIDTH * 2;
    surface_height = CPC_VISIBLE_SCR_HEIGHT * 2;
  } else {
    t->half_pixels = 1;
    surface_width = CPC_VISIBLE_SCR_WIDTH;
    surface_height = CPC_VISIBLE_SCR_HEIGHT;
  }
#if 0
  SDL_CreateWindowAndRenderer(CPC_VISIBLE_SCR_WIDTH*scale, CPC_VISIBLE_SCR_HEIGHT*scale, (fs?SDL_WINDOW_FULLSCREEN_DESKTOP:SDL_WINDOW_SHOWN), &mainSDLWindow, &renderer);
  if (!mainSDLWindow || !renderer) return nullptr;
  SDL_SetWindowTitle(mainSDLWindow, "Caprice32 " VERSION_STRING);
  vid = SDL_CreateRGBSurface(0, surface_width, surface_height, renderer_bpp(renderer), 0, 0, 0, 0);
  if (!vid) return nullptr;
  texture = SDL_CreateTextureFromSurface(renderer, vid);
  if (!texture) return nullptr;
  SDL_FillRect(vid, nullptr, SDL_MapRGB(vid->format,0,0,0));
  compute_scale(t, surface_width, surface_height);
  return vid;
#else
  vid = make_surface(surface_width,surface_height);
  pub = vid;
  return vid;
#endif
  return nullptr;
}

void direct_setpal(SDL_Color* c)
{
  gpu_pal_start();
  for (int i=0 ; i<256 ; i++) {
    int r = c[i&31].r;
    int g = c[i&31].g;
    int b = c[i&31].b;
    // printf("color %d rgb:%d,%d,%d\n",i&31,r,g,b);
    gpu_pal_rgb(r,g,b);
  }
  gpu_pal_end();
#if 0
  SDL_SetPaletteColors(vid->format->palette, c, 0, 32);
#endif
}

#include "tga.h"

void direct_flip(video_plugin*)
{
#if 0
  SDL_UpdateTexture(texture, nullptr, vid->pixels, vid->pitch);
  SDL_RenderClear(renderer);
  if (CPC.scr_preserve_aspect_ratio != 0) {
    SDL_Rect dest_rect = { t->x_offset, t->y_offset, t->width, t->height };
    SDL_RenderCopy(renderer, texture, nullptr, &dest_rect);
  } else {
    SDL_RenderCopy(renderer, texture, nullptr, nullptr);
  }
  SDL_RenderPresent(renderer);
#else

#if 0
  static int divider = 0;
  ++divider;
  if ((divider&31) == 31) {
    std::cerr << "saving ...\n";
    t_image_nfo tga;
    tga.pixels = (Uint8*)vid->pixels;
    tga.width  = vid->w;
    tga.height = vid->h;
    tga.depth  = 24;
    char str[] = "/video_.tga";
    static char cnt = '0';
    str[6] = cnt;
    ++cnt;
    SaveTGAFile(str,&tga);
    std::cerr << " done.\n";
  }
#else
  gpu_frame_start();
  const int W = 320;
  const int H = 200;
  const int *row = ((int*)vid->pixels) + (vid->w*32/4);
  for (int j=0;j<H;j++) {
    const int *ptr = row + (28/4);
    for (int i=0;i<W/4;i++) {
      *GPU = *(ptr++);
      GPU_COM_WAIT;
    }
    row += vid->w/4;
  }
  gpu_frame_end();
#endif
#endif
}

void direct_close()
{
#if 0
  if (texture) SDL_DestroyTexture(texture);
  if (vid) SDL_FreeSurface(vid);
  if (renderer) SDL_DestroyRenderer(renderer);
  if (mainSDLWindow) SDL_DestroyWindow(mainSDLWindow);
#endif
}

std::vector<video_plugin> video_plugin_list =
{
  // Hardware flip version are the same as software ones since switch to SDL2. Kept for compatibility of config, would be nice to not display them in the UI.
  /* Name                     Hidden Init func      Palette func     Flip func      Close func      Half size  X, Y offsets   X, Y scale  width, height */
  {"Direct",                  false, direct_init,   direct_setpal,   direct_flip,   direct_close,   1,         0, 0,          0, 0, 0, 0 },
};
