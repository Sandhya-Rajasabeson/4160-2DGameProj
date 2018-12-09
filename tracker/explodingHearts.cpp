#include <iostream>
#include <cmath>
#include "explodingHearts.h"

ExplodingHearts::ExplodingHearts(const MultiSprite& s) :
  MultiSprite(s)
{}

void ExplodingHearts::draw() const {
  images[currentFrame]->draw(getX(), getY(), getScale());
}

void ExplodingHearts::update(Uint32 ticks) {
  advanceFrame(ticks);
}

bool ExplodingHearts::reachedEnd() const{
  return  (currentFrame == numberOfFrames-1);
}
