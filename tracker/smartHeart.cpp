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

SmartHeart::SmartHeart(const std::string& name, const Vector2f& pos, int w, int h) :
  MultiSprite(name),
  explosion(nullptr),
  bikerPos(pos),
  bikerWidth(w),
  bikerHeight(h),
  currentMode(NORMAL),
  safeDistance(90), //find way to calculate, hard coded for now
  color(name)
{}


SmartHeart::SmartHeart(const SmartHeart& s) :
  MultiSprite(s),
  explosion(s.explosion),
  bikerPos(s.bikerPos),
  bikerWidth(s.bikerWidth),
  bikerHeight(s.bikerHeight),
  currentMode(s.currentMode),
  safeDistance(s.safeDistance),
  color(s.color)
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
}

void SmartHeart::notify(Vector2f newPos){
  bikerPos = newPos;
}

void SmartHeart::update(Uint32 ticks) {
  if(explosion){
    explosion->update(ticks);
    if(explosion->chunkCount() == 0){
      delete explosion;
      explosion = NULL;
    }
    return;
  }
  //x1 and y1 of heart (center)
  float x1= getX()+getImage()->getWidth()/2;
  float y1= getY()+getImage()->getHeight()/2;

  //biker x2 y2
  float x2= bikerPos[0]+bikerWidth/2;
  float y2= bikerPos[1]+bikerHeight/2;

  float distanceToEnemy = ::distance( x1, y1, x2, y2 ); //the :: scope puts it in THIS file

  if(currentMode == NORMAL){
    if(distanceToEnemy <= safeDistance) currentMode = EVADE;
  }
  else if(currentMode == EVADE && color != "blackHeart"){
    if(distanceToEnemy > safeDistance)
      currentMode=NORMAL;
    else{
      if ( x1 < x2 ) left();
      if ( x1 > x2 ) right();
      if ( y1 < y2 ) up();
      //if ( y1 > y2 ) down(); //might be uneccessary
    }
  }

  if(getY() > 600 && color == "blackHeart"){ //lower than bridge
    setVelocityY(-1*getVelocityY());
  }

  MultiSprite::update(ticks); //need to call to update normally as well
}

SmartHeart& SmartHeart::operator=(const SmartHeart& s) {
  MultiSprite::operator=(s);
  bikerPos = (s.bikerPos);
  bikerWidth = (s.bikerWidth);
  bikerHeight = (s.bikerHeight);
  currentMode = (s.currentMode);
  safeDistance = (s.safeDistance);
  return *this;
}

void SmartHeart::explode() {
	if ( !explosion ) {
		Sprite sprite(getName(),getPosition(), getVelocity(), images[currentFrame]);
    sprite.setScale( getScale() );
		explosion = new ExplodingSprite( sprite );
  }
}

void SmartHeart::draw() const{
  if ( explosion ) {
    explosion->draw();
  }
  else images[currentFrame]->draw(getX(), getY(), getScale());
}
