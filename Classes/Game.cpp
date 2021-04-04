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

#include "AudioEngine.h"

#include "AlienShip.h"
#include "Definitions.h"
#include "EventsHolder.h"

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
	std::shared_ptr<EventsHolder> eventsHolder = EventsHolder::getInstnce();
	switch (eventsHolder->getMode())
	{
		case MODE::GAME_MODE:
		{
			_background = Sprite::create("res/surface-moon-background_1308-31766.jpg");
			_background->setPosition(winSize.width * 0.5, winSize.height * 0.5);
			_background->setScale(winSize.width / 200, winSize.height / 100);
			this->addChild(_background);

			_player = Sprite::create("res/Spaceman.png");
			_player->setScale(winSize.width / 800, winSize.height / 800);
			_player->setPosition(Vec2(winSize.width * 0.55, winSize.height * 0.1));
			this->addChild(_player);
			initPlayerProperties();

			AlienShip::initProperties();
			srand((unsigned int)time(nullptr));
			this->scheduleOnce(CC_SCHEDULE_SELECTOR(AlienShip::addShipToScene), AlienShip::calculateNextTimeOfAppearance());

			this->scheduleUpdate();

			auto eventListener = EventListenerTouchOneByOne::create();
			eventListener->onTouchBegan = CC_CALLBACK_2(Game::onTouchBegan, this);
			this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(eventListener, _player);

			auto contactListener = EventListenerPhysicsContact::create();
			contactListener->onContactBegin = CC_CALLBACK_1(Game::onContactBegan, this);
			this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(contactListener, this);

			_backgroundMusicID = AudioEngine::play2d("audio/Prism_-_Bobby_Richards_[trimmed].mp3", true);
		}
		break;
		case MODE::GAME_OVER_MODE:
		{
			_background = Sprite::create("res/Game Over Screen.png");
			_background->setPosition(winSize.width * 0.56, winSize.height * 0.5);
			_background->setScale(winSize.width / 405, winSize.height / 270);
			this->addChild(_background);
			_backgroundMusicID = AudioEngine::play2d("audio/Through and Through - Amulets.mp3");

			auto eventListener = EventListenerTouchOneByOne::create();
			eventListener->onTouchBegan = CC_CALLBACK_2(Game::onTouchBegan, this);
			this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(eventListener, this);
		}
		break;
	}

    return true;
}

bool Game::onTouchBegan(Touch *touch, Event *unused_event) 
{
	std::shared_ptr<EventsHolder> eventsHolder = EventsHolder::getInstnce();

	switch (eventsHolder->getMode())
	{
		case MODE::GAME_MODE:
		{
			if (_playerCanFire)
			{
				_playerCanFire = false;
				_timeRemainingToNextPlayerFire = _playerFiringRate;
				_playerFiringRate -= PLAYER_FIRING_RATE_STEP_DECREASING;
				if (_playerFiringRate < PLAYER_MIN_FIRING_RATE)
					_playerFiringRate = PLAYER_MIN_FIRING_RATE;

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

				auto actionMove = MoveTo::create(_projectileDuration, realDest);
				_projectileDuration -= PROJECTILE_DURATION_STEP_DECREASING;
				if (_projectileDuration < PROJECTILE_MIN_DURATION)
					_projectileDuration = PROJECTILE_MIN_DURATION;

				auto actionRemove = RemoveSelf::create();
				projectile->runAction(Sequence::create(actionMove, actionRemove, nullptr));
				AudioEngine::play2d("audio/Laser_Gun.mp3");
			}
		}
		break;
		case MODE::GAME_OVER_MODE:
		{
			eventsHolder->setMode(MODE::GAME_MODE);
			unregisterObjects();
			AudioEngine::stop(_backgroundMusicID);
			init();
		}
		break;
	}

	return true;
}

void Game::update(float dt)
{
	std::shared_ptr<EventsHolder> eventsHolder = EventsHolder::getInstnce();
	switch (eventsHolder->getMode())
	{
		case MODE::GAME_MODE:
		{
			if (!_playerCanFire)
			{
				_timeRemainingToNextPlayerFire -= dt;
				// check if time ran out
				if (_timeRemainingToNextPlayerFire <= 0.f)
				{
					_playerCanFire = true;
				}
			}

			if (!this->isScheduled(CC_SCHEDULE_SELECTOR(AlienShip::addShipToScene)))
			{
				this->scheduleOnce(CC_SCHEDULE_SELECTOR(AlienShip::addShipToScene), AlienShip::calculateNextTimeOfAppearance());
			}

			Vector<Node *> children = this->getChildren();
			auto winSize = Director::getInstance()->getVisibleSize();
			for (auto& child : children)
			{
				if (child->getTag() == ALIEN_SHIP)
				{
					Vec2 shipsPosition = child->getPosition();
					if (shipsPosition.y <= 0)
					{
						eventsHolder->setMode(MODE::TOWARD_GAME_OVER_MODE);
					}
				}
			}
		}
		break;
		case MODE::TOWARD_GAME_OVER_MODE:
		{
			unregisterObjects();
			eventsHolder->setMode(MODE::GAME_OVER_MODE);
			AudioEngine::stop(_backgroundMusicID);
			if (this->isScheduled(CC_SCHEDULE_SELECTOR(AlienShip::addShipToScene)))
			{
				this->unschedule(CC_SCHEDULE_SELECTOR(AlienShip::addShipToScene));
			}
			init();
		}
		break;
	}
}

bool Game::onContactBegan(PhysicsContact & contact)
{
	auto nodeA = contact.getShapeA()->getBody()->getNode();
	auto nodeB = contact.getShapeB()->getBody()->getNode();

	AudioEngine::play2d("audio/ship_destroyed.mp3");

	if(nodeA)  // check if nodeA and nodeB exist before trying to remove them - they might be removed by other events
		nodeA->removeFromParent();
	if(nodeB) 
		nodeB->removeFromParent();
	return true;
}

void Game::unregisterObjects()
{
	this->removeAllChildren();
}

void Game::initPlayerProperties()
{
	_playerCanFire = true;
	_playerFiringRate = 5.0f;
	_timeRemainingToNextPlayerFire = 0.0f;
	_projectileDuration = 10.0f;
}
