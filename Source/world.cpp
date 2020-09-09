#include <cmath>
#include "../Header/sprite_node.h"
#include "../Header/world.h"

World::World(sf::RenderWindow & window) :
  mWindow(window), mWorldView(window.getDefaultView()), mTextures(), mSceneGraph(), mSceneLayers(), mScrollSpeed(-50.f),
  mWorldBounds(0.f,0.f,mWorldView.getSize().x,2000.f),
  mSpawnPosition(mWorldView.getSize().x/2.f,
                 mWorldBounds.height - mWorldView.getSize().y/2.f),
  mPlayerAircraft(nullptr){
  loadTextures();
  buildScene();
  mWorldView.setCenter(mSpawnPosition);
}


void World::loadTextures(){
  mTextures.load(Textures::Eagle, "Media/Textures/Eagle.png");
  mTextures.load(Textures::Raptor, "Media/Textures/Raptor.png");
  mTextures.load(Textures::Desert, "Media/Textures/Desert.png");
}

void World::buildScene(){

	for (std::size_t i = 0; i < LayerCount; ++i)
	{
		SceneNode::Ptr layer(new SceneNode());
		mSceneLayers[i] = layer.get();

		mSceneGraph.attachChild(std::move(layer));
	}

	sf::Texture& texture = mTextures.get(Textures::Desert);
	sf::IntRect textureRect(mWorldBounds);
	texture.setRepeated(true);

	std::unique_ptr<SpriteNode> backgroundSprite(new SpriteNode(texture, textureRect));
	backgroundSprite->setPosition(mWorldBounds.left, mWorldBounds.top);
	mSceneLayers[Background]->attachChild(std::move(backgroundSprite));

	std::unique_ptr<Aircraft> leader(new Aircraft(Aircraft::Eagle, mTextures));
	mPlayerAircraft = leader.get();
	mPlayerAircraft->setPosition(mSpawnPosition);
	mSceneLayers[Air]->attachChild(std::move(leader));
}

void World::draw(){
  mWindow.setView(mWorldView);
  mWindow.draw(mSceneGraph);
}

void World::update(sf::Time dt){

	mWorldView.move(0.f, mScrollSpeed * dt.asSeconds());
	mPlayerAircraft->setVelocity(0.f, 0.f);

	while (!mCommandQueue.isEmpty()){
    mSceneGraph.onCommand(mCommandQueue.pop(), dt);
  }

	adaptPlayerVelocity();

	mSceneGraph.update(dt);
	adaptPlayerPosition();
}

CommandQueue & World::getCommandQueue(){
  return mCommandQueue;
}

void World::adaptPlayerPosition(){
	sf::FloatRect viewBounds(mWorldView.getCenter() - mWorldView.getSize() / 2.f, mWorldView.getSize());
	const float borderDistance = 40.f;

	sf::Vector2f position = mPlayerAircraft->getPosition();
	position.x = std::max(position.x, viewBounds.left + borderDistance);
	position.x = std::min(position.x, viewBounds.left + viewBounds.width - borderDistance);
	position.y = std::max(position.y, viewBounds.top + borderDistance);
	position.y = std::min(position.y, viewBounds.top + viewBounds.height - borderDistance);
	mPlayerAircraft->setPosition(position);
}

void World::adaptPlayerVelocity(){
	sf::Vector2f velocity = mPlayerAircraft->getVelocity();

	if (velocity.x != 0.f && velocity.y != 0.f){
    mPlayerAircraft->setVelocity(velocity / std::sqrt(2.f));
  }

	mPlayerAircraft->accelerate(0.f, mScrollSpeed);
}