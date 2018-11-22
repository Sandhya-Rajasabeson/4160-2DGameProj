#include "player.h"
#include "gameData.h"
#include "imageFactory.h"

Player::Player( const std::string& name) :
  TwoWayMultiSprite(name),
  initialVelocity(getVelocity()),
  acceleration(-9.8)
{ }

Player::Player(const Player& s) :
  TwoWayMultiSprite(s),
  initialVelocity(s.getVelocity()),
  acceleration(s.getAcceleration())
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

void Player::getAcceleration(){
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
  setVelocityY(-12.0);
}

void Player::update(Uint32 ticks){
  advanceFrame(ticks);

  Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
  setPosition(getPosition() + incr);

  incr = getAcceleration() * static_cast<float>(ticks) * 0.001;
  setVelocity(getVelocity() + incr);

  //setVelocityY(getVelocityY() + .5);

  if ( getX() < 0) {
    setVelocityX( fabs( getVelocityX() ) );
    images = rightImages;
  }
  if ( getX() > worldWidth-getScaledWidth()) {
    setVelocityX( -fabs( getVelocityX() ) );
    images = leftImages;
  }
}
