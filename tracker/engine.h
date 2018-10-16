#include <vector>
#include <SDL.h>
#include "ioMod.h"
#include "renderContext.h"
#include "clock.h"
#include "world.h"
#include "viewport.h"

class Engine {
public:
  Engine ();
  ~Engine ();
  void play();
  void switchSprite();

private:
  const RenderContext& rc;
  const IoMod& io;
  Clock& clock;

  SDL_Renderer * const renderer;

  World sky;
  World bridge;
  World city1;
  World city2;
  World city3;
  World city4;

  Viewport& viewport;

  //Drawable* star;
  //Drawable* spinningStar;
  std::vector<Drawable*> sprites;
  int currentSprite;

  bool makeVideo;

  void draw() const;
  void update(Uint32);

  Engine(const Engine&) = delete;
  Engine& operator=(const Engine&) = delete;
  void printScales() const;
  void checkForCollisions();
};
