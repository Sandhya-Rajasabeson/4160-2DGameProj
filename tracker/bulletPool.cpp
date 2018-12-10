#include <iostream>
#include <sstream>
#include <cmath>
#include "ioMod.h"
#include "gameData.h"
#include "bulletPool.h"

CollisionStrategy* getStrategy(const string& name) {
  std::string sName = Gamedata::getInstance().getXmlStr(name+"/strategy");
  if ( sName == "midpoint" ) return new MidPointCollisionStrategy;
  if ( sName == "rectangular" ) return new RectangularCollisionStrategy;
  if ( sName == "perpixel" ) return new PerPixelCollisionStrategy;
  throw std::string("No strategy in getStrategy");
}

BulletPool::~BulletPool() {
  delete strategy;
  bulletList.clear();
  freeList.clear();
}

BulletPool::BulletPool(const std::string& n) :
  name(n),
  strategy( getStrategy(name) ),
  frameInterval(Gamedata::getInstance().getXmlInt(name+"/interval")),
  timeSinceLastFrame( 0 ),
  bulletList(),
  freeList()
{ }

BulletPool::BulletPool(const BulletPool& b) :
  name(b.name),
  strategy(b.strategy),
  frameInterval(b.frameInterval),
  timeSinceLastFrame( b.timeSinceLastFrame ),
  bulletList(b.bulletList),
  freeList(b.freeList)
{ }

bool BulletPool::collidedWith(const Drawable* obj) const {
  std::list<Arrow>::iterator ptr = bulletList.begin();
  while (ptr != bulletList.end()) {
    if ( strategy->execute(*ptr, *obj) ) {
      freeList.push_back(*ptr);
      ptr = bulletList.erase(ptr);
      return true;
    }
    ++ptr;
  }
  return false;
}

void BulletPool::shoot(const Vector2f& position, const Vector2f& velocity) {
  //std::cout << timeSinceLastFrame << " " << frameInterval << std::endl;

	if (timeSinceLastFrame > frameInterval) {
    //std::cout << "position: " << position[0] << " " << position[1] << std::endl;


    // If no bullets in pool, make one:
    if ( freeList.empty() ) {
      Arrow b(name);
      b.setPosition( position );
      b.setVelocity( velocity );
      bulletList.push_back( b );
    }
    else {
      Arrow b = freeList.front();
      freeList.pop_front();
      b.reset();
      b.setVelocity(velocity);
      b.setPosition(position);
      bulletList.push_back( b );
    }
		timeSinceLastFrame = 0;
	}
}

void BulletPool::draw() const {
  std::stringstream stream;
  stream << "Active bullets: " << bulletList.size();
  IoMod::getInstance().
    writeText(stream.str(), 500, 30);
  stream.clear();
  stream.str("");
  stream << "Bullet pool: " << freeList.size();
  IoMod::getInstance().
    writeText(stream.str(), 500, 60);
  for ( Arrow bullet : bulletList ) {
    bullet.draw();
  }
}

void BulletPool::update(Uint32 ticks) {
  //std::cout << "hiinupdate" << std::endl;
	timeSinceLastFrame += ticks;
  std::list<Arrow>::iterator ptr = bulletList.begin();
  while (ptr != bulletList.end()) {
    ptr->update(ticks);
    if (ptr->goneTooFar()) {  // Check to see if we should free a Bullet
      freeList.push_back(*ptr);
      ptr = bulletList.erase(ptr);
    }
    else ++ptr;
  }
}
