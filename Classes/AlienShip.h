#pragma once

#include <vector>
#include <string>

#include "cocos2d.h"

using namespace cocos2d;

class AlienShip : public cocos2d::Scene
{
public:
	void addShipToScene(float dt);
private:

	static int m_minDuration;
	static int m_maxDuration;
	static std::vector<std::string> m_spritesList;
};