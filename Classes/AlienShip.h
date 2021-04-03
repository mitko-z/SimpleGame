#pragma once

#include <vector>
#include <string>

#include "cocos2d.h"

#define MIN_DURATION_STEP_DECREASING 0.1
#define MAX_DURATION_STEP_DECREASING 0.1
#define TIME_OF_APPEARANCE_STEP_DECREASING 0.1
#define MIN_TIME_OF_APPEARANCE 0.5

using namespace cocos2d;

class AlienShip : public cocos2d::Scene
{
public:
	void addShipToScene(float dt);

	static float calculateNextTimeOfAppearance();
private:
	static float _timeOfAppearance;
	static float _minDuration;
	static float _maxDuration;
	static std::vector<std::string> _spritesList;
};