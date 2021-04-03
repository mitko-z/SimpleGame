#include "AlienShip.h"
#include "Definitions.h"

#define OFSET_FROM_VERTICAL_BORDERS 0.05 // 5 percents

void AlienShip::addShipToScene(float dt)
{
	size_t spriteIndex = rand() % _spritesList.size();

	auto alienShip = Sprite::create(_spritesList[spriteIndex]);

	auto shipSize = alienShip->getContentSize();
	auto physicsBody = PhysicsBody::createBox(Size(shipSize.width, shipSize.height),
		PhysicsMaterial(0.1f, 1.0f, 0.0f));
	physicsBody->setDynamic(true);
	physicsBody->setCategoryBitmask((int)PHYSICS_CATEGORY::PHYSICS_CATEGORY_ALIEN_SHIP);
	physicsBody->setCollisionBitmask((int)PHYSICS_CATEGORY::PHYSICS_CATEGORY_NONE);
	physicsBody->setContactTestBitmask((int)PHYSICS_CATEGORY::PHYSICS_CATEGORY_PROJECTILE);

	alienShip->setPhysicsBody(physicsBody);

	auto shipContentSize = alienShip->getContentSize();
	auto selfContentSize = this->getContentSize();
	float percentageOfset = selfContentSize.width * OFSET_FROM_VERTICAL_BORDERS;
	int minX = percentageOfset;
	int maxX = selfContentSize.width - percentageOfset;
	int rangeX = maxX - minX;
	int randomX = (rand() % rangeX) + minX;

	alienShip->setPosition(Vec2(randomX, selfContentSize.height + shipContentSize.height / 2));
	this->addChild(alienShip);

	int rangeDuration = _maxDuration - _minDuration;
	int randomDuration = (rand() % rangeDuration) + _minDuration;

	auto actionMove = MoveTo::create(randomDuration, Vec2(randomX, -shipContentSize.height / 2));
	auto actionRemove = RemoveSelf::create();
	alienShip->runAction(Sequence::create(actionMove, actionRemove, nullptr));

	// making next ships a little bit faster
	_minDuration -= MIN_DURATION_STEP_DECREASING;
	_maxDuration -= MAX_DURATION_STEP_DECREASING;
	if (_minDuration < 0) _minDuration = 0;
	if (_maxDuration < 0) _maxDuration = 0;
}

float AlienShip::calculateNextTimeOfAppearance()
{
	float result = _timeOfAppearance;
	_timeOfAppearance -= TIME_OF_APPEARANCE_STEP_DECREASING;
	if (_timeOfAppearance < MIN_TIME_OF_APPEARANCE) _timeOfAppearance = MIN_TIME_OF_APPEARANCE;
	return result;
}

float AlienShip::_timeOfAppearance{ 10 };
float AlienShip::_minDuration{ 15 };
float AlienShip::_maxDuration{ 30 };
std::vector<std::string> AlienShip::_spritesList
											{
												"res/Ship-1.png",
												"res/Ship-2.png",
												"res/Ship-3.png",
												"res/Ship-4.png",
												"res/Ship-5.png",
												"res/Ship-6.png",
												"res/Ship-6b.png",
												"res/Ship-7.png",
												"res/Ship-8.png",
												"res/Ship-9.png",
												"res/Ship-10.png",
												"res/Ship-11.png",
												"res/Ship-jelly.png",
												"res/Ship-jelly-2.png"
											};