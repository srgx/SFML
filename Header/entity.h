#ifndef ENTITY__H__
#define ENTITY__H__


#include "scene_node.h"
#include "command_queue.h"

class Entity : public SceneNode{
private:
  sf::Vector2f mVelocity;
  int mHitpoints;
public:

  explicit Entity(int);
  void repair(int);
  void damage(int);
  void destroy();
  virtual void remove();
  int getHitpoints() const;
  bool isDestroyed() const;

  void setVelocity(sf::Vector2f);
  void setVelocity(float, float);
  sf::Vector2f getVelocity() const;
  void accelerate(sf::Vector2f velocity);
  void accelerate(float vx, float vy);
protected:
  virtual void updateCurrent(sf::Time dt, CommandQueue& commands);
};

#endif
