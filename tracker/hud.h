#include "SDL.h"
#include "gameData.h"
#include "clock.h"
#include "ioMod.h"

class Hud {
public:
  Hud(SDL_Renderer*);
  void draw() const;
  void update();
  Hud(const Hud&) = delete;
  Hud& operator=(const Hud&) = delete;
private:
  SDL_Renderer* renderer;
  Gamedata& gdata;
  SDL_Rect hudFrame;
  Clock& clock;
  IoMod& io;
};
