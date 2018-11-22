#include <iostream>
#include <algorithm>
#include <sstream>
#include <string>
#include <random>
#include <iomanip>
#include "sprite.h"
#include "twoWayMultisprite.h"
#include "player.h"
#include "gameData.h"
#include "engine.h"
#include "frameGenerator.h"

/* in charge of creating the sprites and updating them and allowing for
  game play. contains the destructor for the sprite calls, play command,
  and update commands */

Engine::~Engine() {
  std::cout << "here" << std::endl;

  for(auto& sp : sprites){
    delete sp;
  }
  sprites.clear();
  std::cout << "Terminating program" << std::endl;
}

Engine::Engine() :
  rc( RenderContext::getInstance() ),
  io( IoMod::getInstance() ),
  clock( Clock::getInstance() ),
  renderer( rc.getRenderer() ),
  sky("sky", Gamedata::getInstance().getXmlInt("sky/factor") ),
  bridge("bridge", Gamedata::getInstance().getXmlInt("bridge/factor") ),
  city1("city1", Gamedata::getInstance().getXmlInt("city1/factor") ),
  city2("city2", Gamedata::getInstance().getXmlInt("city2/factor") ),
  city3("city3", Gamedata::getInstance().getXmlInt("city3/factor") ),
  city4("city4", Gamedata::getInstance().getXmlInt("city4/factor") ),
  viewport( Viewport::getInstance() ),
  sprites(),
  currentSprite(0),
  makeVideo( false )
{
  sprites.emplace_back(new Player("bikerSprite"));

  //need to replace with hearts. NEED MORE THINKING HERE
  /*for(int i = 0; i < 7; i++){
    sprites.emplace_back(new Sprite("petSprite"));
  }

  //why doesn't this wwork? i have a feeling it has to do with world(world&) being private. (bc no reserve) but how to set up reserve with this
  //also why does moving constructor to publlic break too?
  background.emplace_back("sky", Gamedata::getInstance().getXmlInt("sky/factor")); //0
  background.emplace_back("city1", Gamedata::getInstance().getXmlInt("city1/factor"));
  background.emplace_back("city2", Gamedata::getInstance().getXmlInt("city2/factor"));
  background.emplace_back("city3", Gamedata::getInstance().getXmlInt("city3/factor"));
  background.emplace_back("city4", Gamedata::getInstance().getXmlInt("city4/factor"));
  background.emplace_back("bridge", Gamedata::getInstance().getXmlInt("bridge/factor")); //5 */

  Viewport::getInstance().setObjectToTrack(sprites[0]);

  std::cout << "Loading complete" << std::endl;
}

void Engine::draw() const {
  sky.draw();
  city4.draw();
  city3.draw();
  city2.draw();
  city1.draw();
  sprites[0]->draw();
  bridge.draw();

  /*for(unsigned int i = 1; i < sprites.size(); i++){
    sprites[i]->draw();
  }*/ //old sprites


  std::stringstream str;
  str << "fps: " << clock.getFps();
  io.writeText(str.str(), 30, 60);
  io.writeText("Sandhya Rajasabeson", 30, Gamedata::getInstance().getXmlInt("view/height") - Gamedata::getInstance().getXmlInt("city1/factor") - Gamedata::getInstance().getXmlInt("font/size") - 5, SDL_Color({255, 204, 255, 255}));

  viewport.draw();

  SDL_RenderPresent(renderer);
}

void Engine::update(Uint32 ticks) {

  sky.update();
  city4.update();
  city3.update();
  city2.update();
  city1.update();
  bridge.update();

  for(auto& sp : sprites){
    sp->update(ticks);
  }


  viewport.update(); // always update viewport last
}

void Engine::switchSprite(){
  ++currentSprite;
  currentSprite = currentSprite % sprites.size();
  Viewport::getInstance().setObjectToTrack(sprites[currentSprite]);

}

void Engine::play() {
  SDL_Event event;
  const Uint8* keystate;
  bool done = false;
  Uint32 ticks = clock.getElapsedTicks();
  FrameGenerator frameGen;

  while ( !done ) {
    // The next loop polls for events, guarding against key bounce:
    while ( SDL_PollEvent(&event) ) {
      keystate = SDL_GetKeyboardState(NULL);
      if (event.type ==  SDL_QUIT) { done = true; break; }
      if(event.type == SDL_KEYDOWN) {
        if (keystate[SDL_SCANCODE_ESCAPE] || keystate[SDL_SCANCODE_Q]) {
          done = true;
          break;
        }
        if ( keystate[SDL_SCANCODE_P] ) {
          if ( clock.isPaused() ) clock.unpause();
          else clock.pause();
        }
        if ( keystate[SDL_SCANCODE_T] ) {
          switchSprite();
        }
        if (keystate[SDL_SCANCODE_F4] && !makeVideo) {
          std::cout << "Initiating frame capture" << std::endl;
          makeVideo = true;
        }
        else if (keystate[SDL_SCANCODE_F4] && makeVideo) {
          std::cout << "Terminating frame capture" << std::endl;
          makeVideo = false;
        }
      }
    }

    // In this section of the event loop we allow key bounce:

    ticks = clock.getElapsedTicks();
    if ( ticks > 0 ) {
      clock.incrFrame();

      if(keystate[SDL_SCANCODE_A]) {
        static_cast<Player*>(sprites[0])->left();
      }

      if(keystate[SDL_SCANCODE_D]) {
        static_cast<Player*>(sprites[0])->right();
      }


      draw();

      update(ticks);
      if ( makeVideo ) {
        frameGen.makeFrame();
      }
    }
  }
}
