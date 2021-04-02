#include "AlienShip.h"

void AlienShip::addShipToScene(float dt)
{
	size_t spriteIndex = rand() % m_spritesList.size();

	auto alienShip = Sprite::create(m_spritesList[spriteIndex]);

	auto shipContentSize = alienShip->getContentSize();
	auto selfContentSize = this->getContentSize();
	int minX = shipContentSize.width / 2;
	int maxX = selfContentSize.width - shipContentSize.width / 2;
	int rangeX = maxX - minX;
	int randomX = (rand() % rangeX) + minX;

	alienShip->setPosition(Vec2(randomX, selfContentSize.height + shipContentSize.height / 2));
	this->addChild(alienShip);

	int rangeDuration = m_maxDuration - m_minDuration;
	int randomDuration = (rand() % rangeDuration) + m_minDuration;

	auto actionMove = MoveTo::create(randomDuration, Vec2(randomX, -shipContentSize.height / 2));
	auto actionRemove = RemoveSelf::create();
	alienShip->runAction(Sequence::create(actionMove, actionRemove, nullptr));
}

int AlienShip::m_minDuration{ 15 };
int AlienShip::m_maxDuration{ 30 };
std::vector<std::string> AlienShip::m_spritesList
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