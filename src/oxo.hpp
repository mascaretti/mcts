#ifndef HPP__OXO__HEADER__HPP
#define HPP__OXO__HEADER__HPP

#include <string>
#include <vector>
#include <array>
#include <utility>
#include <cassert>


namespace game {

/*The following classes are used to throw errors when one occurs*/
class ActionAlreadyPlayed{}; //to throw an error if the action has been played
class NoRandomActions{}; //to throw an error if there are no random actions possible?
class NoActionsLeft{}; //if no actions are left

struct Action
{
	/*The class Action implements the action possible on the board and
	* defines to constructors*/
	int row{0};
	int column{0};

	Action()= default;
	Action(std::pair<int, int> input);
	Action(int first, int second);

};

bool operator==(const Action& lhs, const Action& rhs);



class Oxo
{
	/*The class Oxo implements the usual 3x3 tic-tac-toe board*/
private:
	bool is_terminal{false};
	int agent_id{1};

	std::array<std::array<int, 3>, 3> board;
	Action last_action;

	bool winner_exists{false};


	void update_terminal_status();
	int random_action_seed{0}; //TODO

public:

	Oxo();
	Oxo(int seed);
	Oxo(const Oxo& other)= default;
	Oxo& operator=(const Oxo& other)= default;

	bool get_terminal_status(); //true if current state is terminal

	int get_agent_id(); //returns the agent who is about to make a decision

	void apply_action(const Action& action); //returns void, but changes the internal state

	std::vector<Action> get_actions() const; //returns a vector of legal action at current state

	Action random_action() const; //returns a random action legal at the current state;

	int evaluate(); //returns the utility as an integer if state is terminal, otherwise throws error

	std::string to_string(); //print the value as string

	//todo
	void set_seed(); //sets new seed

	//just to check
	std::array<int, 3>& get_row(unsigned int i);
};

}

#endif