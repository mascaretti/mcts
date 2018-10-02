#ifndef HPP__OXO__HEADER__HPP
#define HPP__OXO__HEADER__HPP

#include <string>
#include <vector>
#include <array>
#include <utility>


namespace game {

using Action= std::pair<int, int>;


class Oxo
{
private:
	std::array<std::array<int, 3>> board;

public:
//interfaccia pubblica
	//constructors (default)
	Oxo();
	Oxo(const Oxo& other);
	Oxo& operator=(const Oxo& other);

	bool is_terminal(); //true if current state is terminal

	int agent_id(); //returns the agent who is about to make a decision

	void apply_action(const Action& action); //returns void, but changes the internal state

	std::vector<Action> get_actions(); //returns a vector of legal action at current state

	Action random_action(); //returns a random action legal at the current state;

	int evaluate(); //returns the utility as an integer if state is terminal, otherwise throws error

	std::string to_string(); //print the value as string
};

}

#endif
