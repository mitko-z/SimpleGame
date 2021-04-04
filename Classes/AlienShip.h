#pragma once

#include <vector>
#include <string>

#include "cocos2d.h"

using namespace cocos2d;

class AlienShip : public cocos2d::Scene
{
public:
	static void initProperties();

	void addShipToScene(float dt);

	static float calculateNextTimeOfAppearance();
private:
	static float _timeOfAppearance;
	static float _minDuration;
	static float _maxDuration;
	static std::vector<std::string> _spritesList;
};