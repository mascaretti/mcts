#include "../../src/oxo.hpp"
#include <iostream>

int main(int argc, char const *argv[])
{
	game::Oxo first;
	std::cout << "Agent id: " << first.get_agent_id() << '\n';
	std::cout << "Terminal status: " << first.get_terminal_status() << '\n';
	return 0;
}