#ifndef PLAYER__H
#define PLAYER__H
#include "twoWayMultisprite.h"
#include "smartHeart.h"
#include "explodingHearts.h"

class Player : public TwoWayMultiSprite {
public:
  Player(const std::string&);
  Player(const Player&);
  ~Player();
  virtual void draw() const;
  virtual void update(Uint32 ticks);

  void right();
  void left();
  void jump();
  float getAcceleration();

  //this is where observers will go
  void attach(SmartHeart*);
  void notifyHearts();
  void detach(SmartHeart*);

  virtual void explode();


protected:
  Player& operator=(const Player&);
  ExplodingHearts* explosion;

private:
  Vector2f initialVelocity;
  float acceleration;
  std::vector<SmartHeart*> observingHearts;
  //Vector2f prePosition;
  //Vector2f getExplosionPosition();
};
#endif
