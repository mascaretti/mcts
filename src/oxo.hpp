#ifndef HPP__OXO__HEADER__HPP
#define HPP__OXO__HEADER__HPP

#include <string>
#include <vector>
#include <array>
#include <utility>
#include <cassert>
#include <iostream>
#include <random>


namespace game {

/*The following classes are used to throw errors when one occurs*/
class ActionAlreadyPlayed{}; //to throw an error if the action has been played
class NoRandomActions{}; //to throw an error if there are no random actions possible?
class NoActionsLeft{}; //if no actions are left
class GameNotOver{}; //if the game is not over and we want a utility value

/*
struct Action
{
	// The class Action implements the action possible on the board and
	// defines to constructors
	int row{0};
	int column{0};

	Action()= default;
	Action(std::pair<int, int> input);
	Action(const int& first, const int& second);

	//for debugging
	std::string to_string();

};

bool operator==(const Action& lhs, const Action& rhs);
*/

class Oxo
{
	/*The class Oxo implements the usual 3x3 tic-tac-toe board*/
public:
	struct Action
	{
		/*The class Action implements the action possible on the board and
		* defines to constructors*/
		int row{0};
		int column{0};

		Action()= default;
		Action(std::pair<int, int> input);
		Action(const int& first, const int& second);

		//for debugging
		std::string to_string();

		bool operator==(const Action& rhs);

	};

private:
	bool is_terminal{false};
	int agent_id{1};

	std::array<std::array<int, 3>, 3> board;
	Action last_action;

	bool winner_exists{false};

	int random_action_seed{0};
	std::default_random_engine gen;

	void update_terminal_status();


	char print_helper(int value);

public:

	Oxo();
	Oxo(int seed);
	Oxo(const Oxo& other)= default;
	Oxo& operator=(const Oxo& other)= default;

	bool get_terminal_status(); //true if current state is terminal

	int get_agent_id(); //returns the agent who is about to make a decision

	void apply_action(const Action& action); //returns void, but changes the internal state

	std::vector<Action> get_actions() const; //returns a vector of legal action at current state

	Action random_action(); //returns a random action legal at the current state;

	int evaluate(); //returns the utility as an integer if state is terminal, otherwise throws error

	void print_board(); //print the value as string

	void set_seed(int new_seed); //sets new seed

	//for debugging
	void print();

	//for debugging
	Action get_last_action() const;
};

}

#endif
