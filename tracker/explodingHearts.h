// This is the "pointers" example in the Brian example
#ifndef EXPLODINGHEARTS__H
#define EXPLODINGHEARTS__H
#include <vector>
#include <list>
#include "multisprite.h"

class ExplodingHearts : public MultiSprite {
public:
  ExplodingHearts(const MultiSprite& s);
  //virtual ~ExplodingHearts();
  virtual void draw() const;
  virtual void update(Uint32 ticks);
  bool reachedEnd() const;
private:
  ExplodingHearts(const ExplodingHearts&); // Explicit disallow (Item 6)
  ExplodingHearts& operator=(const ExplodingHearts&); // (Item 6)
};
#endif
