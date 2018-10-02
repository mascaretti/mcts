#include "../../src/oxo.hpp"
#include <iostream>

int main(int argc, char const *argv[])
{
	game::Oxo first;
	std::cout << "Agent id: " << first.get_agent_id() << '\n';
	std::cout << "Terminal status: " << first.get_terminal_status() << '\n';

	auto test= first.to_string();
	std::cout << test << '\n';

	const auto first_row= first.get_row(0);
	for (auto i : first_row) { 
		std::cout << i << '\t';
	}
	std::cout << '\n';
	return 0;
}