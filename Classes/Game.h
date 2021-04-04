/****************************************************************************
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#ifndef __GAME_H__
#define __GAME_H__

#include "cocos2d.h"

#define PLAYER_FIRING_RATE_STEP_DECREASING 0.1
#define PLAYER_MIN_FIRING_RATE 0.5
#define PROJECTILE_DURATION_STEP_DECREASING 0.1
#define PROJECTILE_MIN_DURATION 1.0

using namespace cocos2d;

class Game : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    
    // implement the "static create()" method manually
    CREATE_FUNC(Game);

	bool onTouchBegan(Touch *touch, Event *unused_event);

	void update(float dt);

	bool onContactBegan(PhysicsContact &contact);

private:
	void unregisterObjects();

	void initPlayerProperties();

#pragma region members
	Sprite* _background;

	/// player
	Sprite* _player;
	bool _playerCanFire;
	float _playerFiringRate;
	float _timeRemainingToNextPlayerFire;
	float _projectileDuration;

	/// audio
	int _backgroundMusicID;
#pragma endregion members
};

#endif // __GAME_H__
