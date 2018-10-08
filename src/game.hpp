#ifndef HPP__GAME__HEADER__HPP
#define HPP__GAME__HEADER__HPP

#include <string>
#include <vector>
#include <array>
#include <cassert>
#include <random>

#include "action.hpp"

namespace game {

	/*The following classes are used to throw errors when one occurs*/
	class ActionAlreadyPlayed{}; //to throw an error if the action has been played
	class NoRandomActions{}; //to throw an error if there are no random actions possible?
	class NoActionsLeft{}; //if no actions are left
	class GameNotOver{}; //if the game is not over and we want a utility value

	template <class Action>
	class Game {
		/*The class Game implements the interface of game*/
	public:

		virtual bool get_terminal_status(); //true if current state is terminal

		virtual int get_agent_id(); //returns the agent who is about to make a decision

		virtual void apply_action(const Action& action); //returns void, but changes the internal state

		virtual std::vector<Action> get_actions() const; //returns a vector of legal action at current state

		virtual Action random_action(); //returns a random action legal at the current state

		virtual int evaluate(); //returns the utility as an integer if state is terminal, otherwise throws error

		virtual void set_seed(int new_seed); //sets new seed
	};
}

#endif