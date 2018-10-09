#include "../../../src/oxo.hpp"
#include "../../../src/game.hpp"
#include "../../../src/action.hpp"

int main(int argc, char const *argv[])
{
	game::Oxo::OxoGame new_game{19};
	game::Oxo::OxoAction new_action;

	do {
		new_action= new_game.random_action();
		new_game.apply_action(new_action);
		new_game.print();
		new_game.print_board();
	} while (new_game.get_terminal_status() == false);

	return 0;
}