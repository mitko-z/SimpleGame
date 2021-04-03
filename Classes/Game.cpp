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

#include "Game.h"
#include "AlienShip.h"
#include "Definitions.h"
#include "AudioEngine.h"

USING_NS_CC;

Scene* Game::createScene()
{
	auto scene = Scene::createWithPhysics();
	scene->getPhysicsWorld()->setGravity(Vec2(0, 0));
	//scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
	
	auto layer = Game::create();
	scene->addChild(layer);

    return scene;
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool Game::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }

    auto winSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

	_background = Sprite::create("res/surface-moon-background_1308-31766.jpg");
	_background->setPosition(winSize.width * 0.5, winSize.height * 0.5);
	_background->setScale(winSize.width / 200, winSize.height / 100);
	this->addChild(_background);

	_player = Sprite::create("res/Spaceman.png");
	_player->setScale(winSize.width / 800, winSize.height / 800);
	_player->setPosition(Vec2(winSize.width * 0.55, winSize.height * 0.1));
	this->addChild(_player);


	srand((unsigned int)time(nullptr));
	this->schedule(CC_SCHEDULE_SELECTOR(AlienShip::addShipToScene), 10);
	
	this->scheduleUpdate();

	auto eventListener = EventListenerTouchOneByOne::create();
	eventListener->onTouchBegan = CC_CALLBACK_2(Game::onTouchBegan, this);
	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(eventListener, _player);

	auto contactListener = EventListenerPhysicsContact::create();
	contactListener->onContactBegin = CC_CALLBACK_1(Game::onContactBegan, this);
	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(contactListener, this);

	AudioEngine::play2d("audio/Prism_-_Bobby_Richards_[trimmed].mp3", true);

    return true;
}

bool Game::onTouchBegan(Touch *touch, Event *unused_event) 
{
	if (_playerCanFire)
	{
		_playerCanFire = false;
		_timeRemainingToNextFire = _playerFiringRate;

		Vec2 touchLocation = touch->getLocation();
		Vec2 offset = touchLocation - _player->getPosition();

		auto projectile = Sprite::create("res/shoot-4.png");
		projectile->setPosition(_player->getPosition());

		auto projectileSize = projectile->getContentSize();
		auto physicsBody = PhysicsBody::createCircle(projectileSize.width / 2);
		physicsBody->setDynamic(true);
		physicsBody->setCategoryBitmask((int)PHYSICS_CATEGORY::PHYSICS_CATEGORY_PROJECTILE);
		physicsBody->setCollisionBitmask((int)PHYSICS_CATEGORY::PHYSICS_CATEGORY_NONE);
		physicsBody->setContactTestBitmask((int)PHYSICS_CATEGORY::PHYSICS_CATEGORY_ALIEN_SHIP);
		projectile->setPhysicsBody(physicsBody);

		this->addChild(projectile);

		offset.normalize();
		auto shootAmount = offset * 1000;

		auto realDest = shootAmount + projectile->getPosition();

		auto actionMove = MoveTo::create(10.0f, realDest);
		auto actionRemove = RemoveSelf::create();
		projectile->runAction(Sequence::create(actionMove, actionRemove, nullptr));
		AudioEngine::play2d("audio/Laser_Gun.mp3");
	}

	return true;
}

void Game::update(float dt)
{
	if (!_playerCanFire) 
	{
		_timeRemainingToNextFire -= dt;
		// check if time ran out
		if (_timeRemainingToNextFire <= 0.f) 
		{
			_playerCanFire = true;
		}
	}

}

bool Game::onContactBegan(PhysicsContact & contact)
{
	auto nodeA = contact.getShapeA()->getBody()->getNode();
	auto nodeB = contact.getShapeB()->getBody()->getNode();

	nodeA->removeFromParent();
	nodeB->removeFromParent();
	return true;
}
