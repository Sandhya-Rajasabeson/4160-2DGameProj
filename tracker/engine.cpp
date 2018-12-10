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
  std::cout << "Terminating program" << std::endl;

  delete player;
  for(auto sp : sprites){
    delete sp;
  }
  sprites.clear();
  delete cStrategy;
}

Engine::Engine() :
  rc( RenderContext::getInstance() ),
  io( IoMod::getInstance() ),
  clock( Clock::getInstance() ),
  renderer( rc.getRenderer() ),
  hud(renderer),
  hudToggle(false),
  sky("sky", Gamedata::getInstance().getXmlInt("sky/factor") ),
  bridge("bridge", Gamedata::getInstance().getXmlInt("bridge/factor") ),
  city1("city1", Gamedata::getInstance().getXmlInt("city1/factor") ),
  city2("city2", Gamedata::getInstance().getXmlInt("city2/factor") ),
  city3("city3", Gamedata::getInstance().getXmlInt("city3/factor") ),
  city4("city4", Gamedata::getInstance().getXmlInt("city4/factor") ),
  player(new Player("bikerSprite")),
  viewport( Viewport::getInstance() ),
  sprites(),
  cStrategy(new MidPointCollisionStrategy()),
  makeVideo( false )
{
  //need to replace with hearts. NEED MORE THINKING HERE
  for(int i = 0; i < 10; i++){
    SmartHeart* temp = new SmartHeart("blackHeart", player->getPosition(), player->getImage()->getWidth(), player->getImage()->getHeight());
    sprites.emplace_back(temp);
    player->attach(temp);

  }

  Viewport::getInstance().setObjectToTrack(player);

  std::cout << "Loading complete" << std::endl;
}

void Engine::draw() const {
  sky.draw();
  city4.draw();
  city3.draw();
  city2.draw();
  city1.draw();

  player->draw();
  bridge.draw();

  for(unsigned int i = 0; i < sprites.size(); i++){
    sprites[i]->draw();
  }

  io.writeText("Sandhya Rajasabeson", 30, Gamedata::getInstance().getXmlInt("view/height") - Gamedata::getInstance().getXmlInt("city1/factor") - Gamedata::getInstance().getXmlInt("font/size") - 5, SDL_Color({255, 204, 255, 255}));

  hud.draw();
  viewport.draw();

  SDL_RenderPresent(renderer);
}

void Engine::update(Uint32 ticks) {
  checkForCollisions();
  sky.update();
  city4.update();
  city3.update();
  city2.update();
  city1.update();
  bridge.update();
  player->update(ticks);

  for(auto& sp : sprites){

    sp->update(ticks);
  }
  viewport.update(); // always update viewport last
}

void Engine::checkForCollisions(){

  std::vector<Drawable*>::iterator it = sprites.begin();

  while(it != sprites.end()){
    Drawable* dHeart = *it;
    if(!static_cast<SmartHeart*>(dHeart)->isExploding()){
      if(cStrategy->execute(*player, **it)){ //if player collides with heart
        player->explode();
      }
      if(player->collidedWith(*it)){ //if bullet collides with something
        static_cast<SmartHeart*>(dHeart)->explode();
        //player->detach(static_cast<SmartHeart*>(dHeart));
        //it = sprites.erase(it); //will point to next after deleting
        dHeart->setX(rand()%player->getWorldWidth());
        dHeart->setY(rand()%player->getWorldHeight());
      }
    }
    it++;
  }
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
        if (keystate[SDL_SCANCODE_F4] && !makeVideo) {
          std::cout << "Initiating frame capture" << std::endl;
          makeVideo = true;
        }
        else if (keystate[SDL_SCANCODE_F4] && makeVideo) {
          std::cout << "Terminating frame capture" << std::endl;
          makeVideo = false;
        }

        if ( keystate[SDL_SCANCODE_SPACE] ) {
          player->shoot();
        }

        if(keystate[SDL_SCANCODE_F1]) {
          hud.toggle();
        }
      }
    }

    // In this section of the event loop we allow key bounce:

    ticks = clock.getElapsedTicks();
    if ( ticks > 0 ) {
      clock.incrFrame();

      if(keystate[SDL_SCANCODE_A]) {
        player->left();
      }

      if(keystate[SDL_SCANCODE_D]) {
        player->right();
      }

      if(keystate[SDL_SCANCODE_W]) {
        player->jump();
      }

      if(keystate[SDL_SCANCODE_E]) {
        for(unsigned int i = 0; i < sprites.size(); i++){
          static_cast<SmartHeart*>(sprites[i])->explode();
        }
      }

      draw();

      update(ticks);
      if ( makeVideo ) {
        frameGen.makeFrame();
      }
    }
  }
}
