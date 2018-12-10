#ifndef SMARTHEART__H
#define SMARTSPRITE__H
#include <string>
#include "multisprite.h"
#include "explodingSprite.h"

class SmartHeart : public MultiSprite {
public:
  SmartHeart(const std::string&, const Vector2f& pos, int w, int h);
  SmartHeart(const SmartHeart&);
  virtual ~SmartHeart() {if(explosion) delete explosion;}

  void notify(Vector2f);

  virtual void update(Uint32 ticks);
  virtual void draw() const;

  virtual void explode();
  bool isExploding(){return explosion;}


protected:
  SmartHeart& operator=(const SmartHeart&);
  ExplodingSprite* explosion;

private:
  enum MODE {NORMAL, EVADE};
  Vector2f bikerPos;
  int bikerWidth;
  int bikerHeight;
  MODE currentMode;
  float safeDistance;

  void left();
  void right();
  void up();
  void down();
};
#endif
