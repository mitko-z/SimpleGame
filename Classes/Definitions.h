#pragma once

#define PHYSICS_CATEGORY Definitions::PhysicsCategory
#define MODE Definitions::Mode

#define ALIEN_SHIP 100


namespace Definitions
{
	enum class PhysicsCategory
	{
		PHYSICS_CATEGORY_NONE		= 0,
		PHYSICS_CATEGORY_ALIEN_SHIP = (1 << 0),
		PHYSICS_CATEGORY_PROJECTILE = (1 << 1),
		PHYSICS_CATEGORY_PROJECTILE_ALL = PHYSICS_CATEGORY::PHYSICS_CATEGORY_ALIEN_SHIP | PHYSICS_CATEGORY::PHYSICS_CATEGORY_PROJECTILE
	};

	enum Mode
	{
		START_GAME_MODE,
		GAME_MODE,
		TOWARD_GAME_OVER_MODE,
		GAME_OVER_MODE
	};
}