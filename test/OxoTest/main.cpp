#include "../../src/oxo.hpp"
#include <iostream>

int main(int argc, char const *argv[])
{
	game::Oxo first;

	//testing agent id
	std::cout << "Agent id: " << first.get_agent_id() << '\n';

	//testing default first row
	auto first_row= first.get_row(0);
	for (auto i : first_row) { 
		std::cout << i << '\t';
	}
	std::cout << '\n';

	//testing inbound apply action
	first.apply_action(std::make_pair(0, 2));
	std::cout << "Agent id: " << first.get_agent_id() << '\n';

	first_row= first.get_row(0);
	for (auto i : first_row) { 
		std::cout << i << '\t';
	}
	std::cout << '\n';

	
	//testing outbound apply action
	/*first.apply_action(std::make_pair(0, 3));
	std::cout << "Agent id: " << first.get_agent_id() << '\n';

	first_row= first.get_row(0);
	for (auto i : first_row) { 
		std::cout << i << '\t';
	}
	std::cout << '\n';*/

	
	//testing same action played twice
	/*first.apply_action(std::make_pair(0, 2));
	std::cout << "Agent id: " << first.get_agent_id() << '\n';

	first_row= first.get_row(0);
	for (auto i : first_row) { 
		std::cout << i << '\t';
	}
	std::cout << '\n';*/

	//check equality between actions
	game::Action one{1, 2}, two{1, 0}, three{1, 2};
	std::cout << "One equals two? " << (one == two) << std::endl;
	std::cout << "One equals three? " << (one == three) << std::endl;

	return 0;
}