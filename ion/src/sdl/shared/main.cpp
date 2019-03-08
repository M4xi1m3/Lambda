#include "main.h"
#include "display.h"
#include "images.h"
#include "layout.h"

#include <ion.h>
#include <ion/timing.h>
#include <ion/events.h>
#include <SDL.h>

void Ion::Timing::msleep(uint32_t ms) {
  SDL_Delay(ms);
}

int main(int argc, char * argv[]) {
  Ion::SDL::Main::init();
  ion_main(argc, argv);
  Ion::SDL::Main::quit();
  return 0;
}

namespace Ion {
namespace SDL {
namespace Main {

static SDL_Window * sWindow = nullptr;
static SDL_Renderer * sRenderer = nullptr;
static SDL_Texture * sBackgroundTexture = nullptr;

void init() {
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    SDL_Log("Could not init video");
    return;
  }

  sWindow = SDL_CreateWindow(
    "Epsilon",
    SDL_WINDOWPOS_CENTERED,
    SDL_WINDOWPOS_CENTERED,
    800, 600,
    SDL_WINDOW_ALLOW_HIGHDPI
#if EPSILON_SDL_FULLSCREEN
    | SDL_WINDOW_FULLSCREEN
#else
    | SDL_WINDOW_RESIZABLE
#endif
  );

  SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");

  sRenderer = SDL_CreateRenderer(sWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

  Display::init(sRenderer);

  sBackgroundTexture = loadImage(sRenderer, "background.jpg");

  relayout();
}

void relayout() {
  int windowWidth = 0;
  int windowHeight = 0;
  SDL_GetWindowSize(sWindow, &windowWidth, &windowHeight);
  SDL_RenderSetLogicalSize(sRenderer, windowWidth, windowHeight);

  Layout::recompute(windowWidth, windowHeight);
  SDL_Rect backgroundRect;
  Layout::getBackgroundRect(&backgroundRect);

  SDL_RenderCopy(sRenderer, sBackgroundTexture, nullptr, &backgroundRect);
  SDL_RenderPresent(sRenderer);

  refresh();
}

void refresh() {
  SDL_Rect screenRect;
  Layout::getScreenRect(&screenRect);
  SDL_Rect backgroundRect;
  Layout::getBackgroundRect(&backgroundRect);

  SDL_RenderCopy(sRenderer, sBackgroundTexture, nullptr, &backgroundRect);
  Display::draw(sRenderer, &screenRect);
  SDL_RenderPresent(sRenderer);
}

void quit() {
  SDL_DestroyWindow(sWindow);
  SDL_Quit();
}

}
}
}
