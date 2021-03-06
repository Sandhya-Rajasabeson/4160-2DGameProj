#ifndef TWOWAYMULTISPRITE__H
#define TWOWAYMULTISPRITE__H
#include "multisprite.h"

class TwoWayMultiSprite : public MultiSprite {
public:
  TwoWayMultiSprite(const std::string&);
  TwoWayMultiSprite(const TwoWayMultiSprite&);
  virtual void update(Uint32 ticks);

protected:
  std::vector<Image *> leftImages;
  std::vector<Image *> rightImages;
  TwoWayMultiSprite& operator=(const TwoWayMultiSprite&);

};
#endif
