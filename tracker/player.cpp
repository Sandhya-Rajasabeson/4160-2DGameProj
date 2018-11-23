#include "player.h"
#include "gameData.h"
#include "imageFactory.h"

Player::Player( const std::string& name) :
  TwoWayMultiSprite(name),
  initialVelocity(getVelocity()),
  acceleration(9.8)
{ }

Player::Player(const Player& s) :
  TwoWayMultiSprite(s),
  initialVelocity(s.initialVelocity),
  acceleration(s.acceleration)
{ }

Player& Player::operator=(const Player& s) {
  Drawable::operator=(s);
  images = (s.images);
  leftImages = (s.leftImages);
  rightImages = (s.rightImages);
  currentFrame = (s.currentFrame);
  numberOfFrames = ( s.numberOfFrames );
  frameInterval = ( s.frameInterval );
  timeSinceLastFrame = ( s.timeSinceLastFrame );
  worldWidth = ( s.worldWidth );
  worldHeight = ( s.worldHeight );
  initialVelocity = (s.initialVelocity);
  acceleration = (s.acceleration);
  return *this;
}

float Player::getAcceleration(){
  return acceleration;
}

void Player::right() {
  if ( getX() < worldWidth-getScaledWidth()) {
    setVelocityX(initialVelocity[0]);
    images = rightImages;
  }
}

void Player::left()  {
  if ( getX() > 0) {
    setVelocityX(-initialVelocity[0]);
    images = leftImages;
  }
}

void Player::jump(){
  if(getY() == 480){
    setVelocityY(-200);
  }
}

void Player::update(Uint32 ticks){
  advanceFrame(ticks);

  float incr = getVelocityY() * static_cast<float>(ticks) * 0.001;
  setY(getY() + incr);

  //incr = getAcceleration() * static_cast<float>(ticks) * 0.001;
  setVelocityY(getVelocityY() + getAcceleration());

  if(getY() > 480){ // lower than it should be
    setY(480);
    setVelocityY(0);
  }

  incr = getVelocityX() * static_cast<float>(ticks) * 0.001;
  setX(getX() + incr);


  if ( getX() < 0) {
    setVelocityX( fabs( getVelocityX() ) );
    images = rightImages;
  }
  if ( getX() > worldWidth-getScaledWidth()) {
    setVelocityX( -fabs( getVelocityX() ) );
    images = leftImages;
  }
}
