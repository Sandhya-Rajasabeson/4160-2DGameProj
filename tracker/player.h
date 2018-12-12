#ifndef PLAYER__H
#define PLAYER__H
#include "twoWayMultisprite.h"
#include "smartHeart.h"
#include "explodingHearts.h"
#include "bulletPool.h"

class Player : public TwoWayMultiSprite {
public:
  Player(const std::string&);
  Player(const Player&);
  virtual ~Player();
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

  bool collidedWith(const Drawable* obj) const;

  virtual void explode();
  void shoot();


protected:
  ExplodingHearts* explosion;

private:
  Player& operator=(const Player&);
  Vector2f initialVelocity;
  float acceleration;
  std::vector<SmartHeart*> observingHearts;
  std::string bulletName;
  BulletPool* bullets;
  float bulletSpeed;
};
#endif
