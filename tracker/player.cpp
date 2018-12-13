#include "player.h"
#include "gameData.h"
#include "imageFactory.h"
#include "ioMod.h"
#include <iostream>
#include <sstream>

Player::Player( const std::string& name) :
  TwoWayMultiSprite(name),
  explosion(nullptr),
  initialVelocity(Gamedata::getInstance().getXmlInt(name+"/speedX")),
  acceleration(12),
  observingHearts(),
  bulletName( Gamedata::getInstance().getXmlStr(name+"/bullet") ),
  bullets(new BulletPool(bulletName)),
  bulletSpeed(Gamedata::getInstance().getXmlInt(bulletName+"/speedX")),
  lives(Gamedata::getInstance().getXmlInt(name+"/lives")),
  points(0)
  //soundIndex(),
  //sounds()
{
  //sounds.addMusicWAV("shoot");
  //soundIndex.insert(std::make_pair<string, int>("shoot", 0));
}

Player::~Player() {
  if(explosion)
    delete explosion;
  delete bullets;
  observingHearts.clear();
}

Player::Player(const Player& s) :
  TwoWayMultiSprite(s),
  explosion(s.explosion),
  initialVelocity(s.initialVelocity),
  acceleration(s.acceleration),
  observingHearts(s.observingHearts),
  bulletName(s.bulletName),
  bullets(s.bullets),
  bulletSpeed(s.bulletSpeed),
  lives(s.lives),
  points(s.points)
  //soundIndex(s.soundIndex),
  //sounds(s.sounds)
{ }

/*Player& Player::operator=(const Player& s) {
  Drawable::operator=(s);
  explosion = (s.explosion);
  initialVelocity = (s.initialVelocity);
  acceleration = (s.acceleration);
  observingHearts = (s.observingHearts);
  bulletName = (s.bulletName);
  bullets = (s.bullets);
  bulletSpeed = (s.bulletSpeed);
  return *this;
}*/

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
  if(!explosion){
    if(getY() == 480){
      setVelocityY(-350);
    }
  }
}

void Player::update(Uint32 ticks){
  if(explosion){
    explosion->update(ticks);
    if(explosion->reachedEnd()){
      delete explosion;
      explosion = NULL;
    }
    return;
  }
  advanceFrame(ticks);
  bullets->update(ticks);

  float incr = getVelocityY() * static_cast<float>(ticks) * 0.001;
  setY(getY() + incr);

  //incr = getAcceleration() * static_cast<float>(ticks) * 0.001;
  setVelocityY(getVelocityY() + getAcceleration());

  if(getY() > 480){ // constrain to bridge jumping
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

  notifyHearts(); //observer pattern
}

//observer pattern
void Player::attach(SmartHeart* h){
  observingHearts.push_back(h);
}

void Player::notifyHearts(){
  std::vector<SmartHeart*>::iterator it = observingHearts.begin();
  while(it != observingHearts.end()){
    (*it)->notify(getPosition());
    it++;
  }
}

void Player::detach(SmartHeart* toDetach){
  std::vector<SmartHeart*>::iterator it = observingHearts.begin();
  while(it != observingHearts.end()){
    if(*it == toDetach){
      it = observingHearts.erase(it);
    }
    else{
      it++;
    }
  }
}

/*Vector2f Player::getExplosionPosition(){
  float x = ((explosion->getImage()->getWidth())/2.0) + this->getImage()->getWidth();
  x += (this->getImage()->getWidth())/2.0;
  float y = ((explosion->getImage()->getHeight())/2.0) - this->getImage()->getHeight();
  y += (this->getImage()->getHeight())/2.0;
  return Vector2f(this->getX()-x, this->getY()+y);
}*/

void Player::explode(){
  if(!explosion){
    MultiSprite sprite("explodingHearts");
    explosion = new ExplodingHearts(sprite);
    explosion->setX(getX()-115); //centering biker sprite and explosion frames
    explosion->setY(getY()-55);
    setY(480);
    setVelocityY(0);
    lives--;
  }
}

void Player::draw() const{
  if(explosion) {
    explosion->draw();
  }
  else {
    TwoWayMultiSprite::draw();
    bullets->draw();
  }

  /*if(lives > 0){
    std::stringstream stream;
    stream << "Lives";
    IoMod::getInstance().
      writeText(stream.str(), 15, 15);
    stream.clear();
    stream.str("");
    for(int i = 0; i < lives; i++)
      stream << "<3  ";
    IoMod::getInstance().
      writeText(stream.str(), 85, 15);

    std::stringstream strm;
    strm << "Points";
    IoMod::getInstance().
      writeText(strm.str(), 15, 45);
    strm.clear();
    strm.str("");
    strm << points;
    IoMod::getInstance().
      writeText(strm.str(), 95, 45);
  }*/


}

void Player::shoot(){
  //std::cout << soundIndex["shoot"] << std::endl;
  //sounds[soundIndex["shoot"]];
  if(getVelocityX() > 0){ //going right
    bullets->shoot(Vector2f(getX()+getImage()->getWidth(), getY()+((getImage()->getHeight()/2)-20)),
      Vector2f(bulletSpeed+getVelocityX(),0));
  }
  else {
    bullets->shoot(Vector2f(getX(), getY()+((getImage()->getHeight()/2)-20)),
      Vector2f(-1*bulletSpeed+getVelocityX(),0));
  }

}

bool Player::collidedWith(const Drawable* obj) const {
  return bullets->collidedWith( obj );
}

void Player::energy(std::string color){
  if(color == "pinkHeart")
    points++;
  else if(color == "rainbowHeart")
    lives++;
}

void Player::energyPlus(std::string color){
  if(color == "pinkHeart")
    points+=2;
  else if(color == "rainbowHeart")
    lives++;
}
