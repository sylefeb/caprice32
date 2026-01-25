#include "cap32.h"
// #include <SDL_main.h>
extern "C" {
#include "../libs/sdcard.h"
#include "../libs/fat_io_lib/src/fat_filelib.h"
}

// -----------------------

extern "C" void* __dso_handle = (void*) &__dso_handle;

int swprintf(wchar_t *s, size_t n, const wchar_t *format, ...) {
    return -1; // not implemented
}

extern "C" {
    wint_t getwc(FILE *f) { return 0; }
    wint_t putwc(wchar_t c,FILE *f) { return 0; }
    wint_t ungetwc(wint_t i, FILE *f) { return 0; }
}

// -----------------------

int main(int argc, char **argv)
{
  // initialise file IO Library
  printf("fl_init ... ");
  fl_init();
  fl_attach_locks(NULL,NULL);
  printf("done.\n");

  // attach media access functions to library
  printf("fl_attach_media ... ");
  int iter_max = 128;
  while ( fl_attach_media(sdcard_readsector, sdcard_writesector) != FAT_INIT_OK
       && iter_max-- > 0) {
    // try again, we need this
    printf(".");
  }
  if (iter_max == 0) {
    printf("ERROR: could not initialize SDcard\n");
    return -1;
  }
  printf("done.\n");

  printf("cap32_main\n");
  return cap32_main(argc, argv);
}
