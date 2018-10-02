#ifndef HPP__OXO__HEADER__HPP
#define HPP__OXO__HEADER__HPP

#include <string>
#include <vector>
#include <array>
#include <utility>


namespace game {

using Action= std::pair<int, int>;

class IllegalAction{}; //to throw an error

class Oxo
{
private:
	bool is_terminal{false};
	int agent_id{1};
	std::array<std::array<int, 3>, 3> board;

public:
//interfaccia pubblica

	Oxo();
	Oxo(const Oxo& other)= default;
	Oxo& operator=(const Oxo& other)= default;

	bool get_terminal_status(); //true if current state is terminal

	int get_agent_id(); //returns the agent who is about to make a decision

	void apply_action(const Action& action); //returns void, but changes the internal state

	std::vector<Action> get_actions() const; //returns a vector of legal action at current state

	Action random_action() const; //returns a random action legal at the current state;

	int evaluate(); //returns the utility as an integer if state is terminal, otherwise throws error

	std::string to_string() const; //print the value as string
};

}

#endif