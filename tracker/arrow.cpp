#include <iostream>
#include <cmath>
#include "arrow.h"
#include "collisionStrategy.h"

void Arrow::update(Uint32 ticks) {
  Vector2f pos = getPosition();
  Sprite::update(ticks);
  distance += ( hypot(getX()-pos[0], getY()-pos[1]) );
  if (distance > maxDistance) tooFar = true;
}
