#ifndef HPP__GAME__HEADER__HPP
#define HPP__GAME__HEADER__HPP

#include <string>
#include <vector>
#include <array>
#include <cassert>
#include <random>

namespace game {

	//The following classes are used to throw errors when one occurs
	class ActionAlreadyPlayed{}; //to throw an error if the action has been played
	class NoRandomActions{}; //to throw an error if there are no random actions possible?
	class NoActionsLeft{}; //if no actions are left
	class GameNotOver{}; //if the game is not over and we want a utility value
	class IllegalAction{}; //if the action is not legal
	class NoActionPlayed{}; //if no action has been played

}

#endif
