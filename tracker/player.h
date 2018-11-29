#ifndef PLAYER__H
#define PLAYER__H
#include "twoWayMultisprite.h"
#include "smartHeart.h"

class Player : public TwoWayMultiSprite {
public:
  Player(const std::string&);
  Player(const Player&);
  virtual void update(Uint32 ticks);

  void right();
  void left();
  void jump();
  float getAcceleration();

  //this is where observers will go
  void attach(SmartHeart*);
  void notifyHearts();
  void detach(SmartHeart*);

protected:
  Player& operator=(const Player&);

private:
  Vector2f initialVelocity;
  float acceleration;
  std::vector<SmartHeart*> observingHearts;
};
#endif
