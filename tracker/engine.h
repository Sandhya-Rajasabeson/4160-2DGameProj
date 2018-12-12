#include <vector>
#include <SDL.h>
#include "renderContext.h"
#include "hud.h"
#include "world.h"
#include "viewport.h"
#include "player.h"
#include "collisionStrategy.h"
#include "sound.h"

class Engine {
public:
  Engine ();
  ~Engine ();
  void play();

private:
  const RenderContext& rc;
  const IoMod& io;
  Clock& clock;

  SDL_Renderer * const renderer;
  Hud hud;
  bool hudToggle;

  World sky;
  World bridge;
  World city1;
  World city2;
  World city3;
  World city4;
  Player* player;
  //std::vector<World> background;
  Viewport& viewport;


  std::vector<Drawable*> sprites; //0 is player NEED TO SPLIT. NEEDS to be Drawable for collision to work

  CollisionStrategy* cStrategy;

  bool makeVideo;

  SDLSound sounds;
  bool godMode;
  //std::map<std::string, int> soundIndex;

  void draw() const;
  void update(Uint32);

  Engine(const Engine&) = delete;
  Engine& operator=(const Engine&) = delete;
  void printScales() const;
  void checkForCollisions();
};
