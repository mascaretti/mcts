#include "nim.hpp"

int main(int argc, char const *argv[])
{

	game::Nim::NimGame<> test_game;

	do {

		std::cout << "Play a move" << '\n';
		std::cout << "Insert pile: ";
		unsigned int pile;
		std::cin >> pile;
		std::cout << "Insert number: ";
		unsigned int number;
		std::cin >> number;

		test_game.apply_action({pile, number});

		test_game.print();

	} while (test_game.get_terminal_status() == false);

	return 0;
}
