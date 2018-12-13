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
  lights(),
  sprites(),
  cStrategy(new MidPointCollisionStrategy()),
  makeVideo( false ),
  sounds(),
  godMode(false),
  nightMode(false)
  //soundIndex()
{
  //need to replace with hearts. NEED MORE THINKING HERE

  SmartHeart* temp = new SmartHeart("rainbowHeart", player->getPosition(), player->getImage()->getWidth(), player->getImage()->getHeight());
  sprites.emplace_back(temp);
  player->attach(temp);

  for(int i = 0; i < 4; i++){
    temp = new SmartHeart("pinkHeart", player->getPosition(), player->getImage()->getWidth(), player->getImage()->getHeight());
    sprites.emplace_back(temp);
    player->attach(temp);

  }
  for(int i = 0; i < 4; i++){
    temp = new SmartHeart("blackHeart", player->getPosition(), player->getImage()->getWidth(), player->getImage()->getHeight());
    sprites.emplace_back(temp);
    player->attach(temp);

  }

  //soundIndex = sounds.getSoundIndex();
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
  if(nightMode) {
    for(unsigned int i = 0; i < sprites.size(); i++){
      sprites[i]->draw();
    }
    lights.draw();
  }
  bridge.draw();

  if(player->getLives() > 0){
    std::stringstream stream;
    stream << "Lives";
    IoMod::getInstance().
      writeText(stream.str(), 15, 15);
    stream.clear();
    stream.str("");
    for(int i = 0; i < player->getLives(); i++)
      stream << "<3  ";
    IoMod::getInstance().
      writeText(stream.str(), 85, 15);

    std::stringstream strm;
    strm << "Points";
    IoMod::getInstance().
      writeText(strm.str(), 15, 45);
    strm.clear();
    strm.str("");
    strm << player->getPoints();
    IoMod::getInstance().
      writeText(strm.str(), 95, 45);
  }

  if(!nightMode){
    for(unsigned int i = 0; i < sprites.size(); i++){
      sprites[i]->draw();
    }
  }

  if(player->getLives() == 0){
    // Set the hud background color:
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor( renderer, 0, 0, 15, 255/2 );
    // Draw the  background
    SDL_Rect rect = {0, 0, 800, 600};
    SDL_RenderFillRect( renderer, &rect);

    io.writeText("You lost :(\nScore: "+std::to_string(player->getPoints())+"pts\nRestart?(r)", 200, 250,SDL_Color({0, 255, 255, 255}));
    clock.pause();
  }

  io.writeText("Sandhya Rajasabeson", 30, Gamedata::getInstance().getXmlInt("view/height") - Gamedata::getInstance().getXmlInt("city1/factor") - Gamedata::getInstance().getXmlInt("font/size") - 5, SDL_Color({255, 204, 255, 255}));
  if(godMode){
    io.writeText("!!!!!GOD MODE!!!!!!", 30, 100, SDL_Color({255, 204, 255, 255}));

  }
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
  player->update(ticks);
  if(nightMode) {
    for(auto& sp : sprites)
      sp->update(ticks);
    lights.update();
  }
  bridge.update();

  if(!nightMode){
    for(auto& sp : sprites)
      sp->update(ticks);
  }

  viewport.update(); // always update viewport last
}

void Engine::checkForCollisions(){

  std::vector<Drawable*>::iterator it = sprites.begin();

  while(it != sprites.end()){
    Drawable* dHeart = *it;
    if(!static_cast<SmartHeart*>(dHeart)->isExploding()){
      if(cStrategy->execute(*player, **it) && !player->isExploding()){ //if player collides with heart
        if(static_cast<SmartHeart*>(dHeart)->getColor() == "blackHeart"){
          if(!godMode){
            sounds[2];
            player->explode();
          }
        }
        static_cast<SmartHeart*>(dHeart)->explode();
        dHeart->setX(rand()%player->getWorldWidth());
        dHeart->setY(rand()%player->getWorldHeight());
        player->energyPlus(static_cast<SmartHeart*>(dHeart)->getColor());
      }
      if(player->collidedWith(*it)){ //if bullet collides with something
        sounds[1];
        static_cast<SmartHeart*>(dHeart)->explode();
        //player->detach(static_cast<SmartHeart*>(dHeart));
        //it = sprites.erase(it); //will point to next after deleting
        dHeart->setX(rand()%player->getWorldWidth());
        dHeart->setY(rand()%player->getWorldHeight());
        player->energy(static_cast<SmartHeart*>(dHeart)->getColor());
      }
    }
    it++;
  }
}

bool Engine::play() {
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
        if ( keystate[SDL_SCANCODE_R] && clock.isPaused() && player->getLives() == 0) {
          if ( clock.isPaused() ) clock.unpause();
          return true;
        }
        if (keystate[SDL_SCANCODE_F4] && !makeVideo) {
          std::cout << "Initiating frame capture" << std::endl;
          makeVideo = true;
        }
        else if (keystate[SDL_SCANCODE_F4] && makeVideo) {
          std::cout << "Terminating frame capture" << std::endl;
          makeVideo = false;
        }

        if ( keystate[SDL_SCANCODE_SPACE] && !clock.isPaused()) {
          sounds[0];
          player->shoot();
        }

        if(keystate[SDL_SCANCODE_F1]) {
          hud.toggle();
        }

        if(keystate[SDL_SCANCODE_N]) {
          nightMode = !nightMode;
        }

        if(keystate[SDL_SCANCODE_G]) {
          godMode = !godMode;
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

      draw();

      update(ticks);
      if ( makeVideo ) {
        frameGen.makeFrame();
      }
    }
  }

  return false;
}
