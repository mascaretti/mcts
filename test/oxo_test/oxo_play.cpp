#include "oxo.hpp"

int main(int argc, char const *argv[])
{

	game::Oxo::OxoGame test_game;

	do {

		std::cout << "Play a move" << '\n';
		std::cout << "Insert row: ";
		int row;
		std::cin >> row;
		std::cout << "Insert column: ";
		int column;
		std::cin >> column;

		test_game.apply_action({row, column});

		test_game.print();

	} while (test_game.get_terminal_status() == false);

	return 0;
}
