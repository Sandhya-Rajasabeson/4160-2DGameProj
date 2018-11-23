#include <cmath>
#include <random>
#include <functional>
#include "smartHeart.h"
#include "gameData.h"

//helper to find distance
float distance(float x1, float y1, float x2, float y2) {
  float x = x1-x2;
  float y = y1-y2;
  return sqrt((x*x) + (y*y));
}

SmartHeart::SmartHeart(const std::string& name, Drawable* player) :
  MultiSprite(name),
  biker(player),
  bikerPos(player->getPosition()),
  currentMode(NORMAL),
  safeDistance(200) //find way to calculate, hard coded for now
{}


SmartHeart::SmartHeart(const SmartHeart& s) :
  MultiSprite(s),
  biker(s.biker),
  bikerPos(s.bikerPos),
  currentMode(s.currentMode),
  safeDistance(s.safeDistance)
{}

void SmartHeart::left()  {
  setVelocityX( -abs(getVelocityX()) );
}

void SmartHeart::right() {
  setVelocityX( fabs(getVelocityX()) );
}

void SmartHeart::up()    {
  setVelocityY( -fabs(getVelocityY()) );
}

void SmartHeart::down()  {
  setVelocityY( fabs(getVelocityY()) );
  if(getY() > 480){ //lower than bridge
    setY(480);
  }
}

void SmartHeart::notify(Vector2f newPos){
  bikerPos = newPos;
}

void SmartHeart::update(Uint32 ticks) {
  //x1 and y1 of heart (center)
  float x1= getX()+getImage()->getWidth()/2;
  float y1= getY()+getImage()->getHeight()/2;

  //biker x2 y2
  float x2= bikerPos[0]+biker->getImage()->getWidth()/2;
  float y2= bikerPos[1]+biker->getImage()->getHeight()/2;

  float distanceToEnemy = ::distance( x1, y1, x2, y2 ); //the :: scope puts it in THIS file

  if(currentMode == NORMAL){
    if(distanceToEnemy <= safeDistance) currentMode = EVADE;
  }
  else if(currentMode == EVADE){
    if(distanceToEnemy > safeDistance)
      currentMode=NORMAL;
    else{
      if ( x1 < x2 ) left();
      if ( x1 > x2 ) right();
      if ( y1 < y2 ) up();
      if ( y1 > y2 ) down(); //might be uneccessary
    }
  }
  MultiSprite::update(ticks); //need to call to update normally as well
}


SmartHeart& SmartHeart::operator=(const SmartHeart& s) {
  MultiSprite::operator=(s);
  biker = (s.biker);
  bikerPos = (s.bikerPos);
  currentMode = (s.currentMode);
  safeDistance = (s.safeDistance);
  return *this;
}
