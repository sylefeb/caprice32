#include "cap32.h"
// #include <SDL_main.h>

// In your main.cpp or a support file
extern "C" void* __dso_handle = (void*) &__dso_handle;

int swprintf(wchar_t *s, size_t n, const wchar_t *format, ...) {
    return -1; // not implemented
}

int main(int argc, char **argv)
{
  printf("cap32_main\n");
  return cap32_main(argc, argv);
}
