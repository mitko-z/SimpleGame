#pragma once

#include <memory>
#include <unordered_map>
#include "Definitions.h"

class EventsHolder
{
public:
	static std::shared_ptr<EventsHolder> getInstnce();

	MODE getMode() { return _mode; }
	void setMode(MODE modeTypeToSet) { _mode = modeTypeToSet; }
private:
	EventsHolder() {}

	MODE _mode{ MODE::GAME_MODE }; // TODO make this to START_GAME_MODE when this mode is ready
};