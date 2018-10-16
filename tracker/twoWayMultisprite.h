#ifndef TWOWAYMULTISPRITE__H
#define TWOWAYMULTISPRITE__H
#include <string>
#include <vector>
#include <cmath>
#include "multisprite.h"

class TwoWayMultiSprite : public MultiSprite {
public:
  TwoWayMultiSprite(const std::string&);
  TwoWayMultiSprite(const TwoWayMultiSprite&);
  virtual void update(Uint32 ticks);

protected:
  std::vector<Image *> leftImages;
  std::vector<Image *> rightImages;
  MultiSprite& operator=(const TwoWayMultiSprite&);

};
#endif
