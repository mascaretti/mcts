#include "../../src/monte_carlo_search_tree.hpp"
#include "../../src/oxo.hpp"

int main() {

// Namespaces
using Oxo = game::Oxo::OxoGame;
using Move = game::Oxo::OxoAction;

int outer_it1 = 1, inner_it1 = 1;
int outer_it2 = 100, inner_it2 = 100;

MPI_Init(nullptr, nullptr);

Oxo oxo_board;
Move current_player_move;
std::vector<Move> remaining_moves = oxo_board.get_actions();
MonteCarloSearchTree<Oxo, Move> mcst_player_1(outer_it1, inner_it1);
MonteCarloSearchTree<Oxo, Move> mcst_player_2(outer_it2, inner_it2);

oxo_board.print();
oxo_board.print_board();
// DEBUG
for(auto it = remaining_moves.cbegin(); it!=remaining_moves.cend(); ++it)
  std::cout << it->to_string() << " ";
std::cout << std::endl;

// Let two AIs play oxo one against the other!
while( !oxo_board.get_terminal_status() ) {
  if ( oxo_board.get_agent_id() == 1 ) {
    current_player_move = mcst_player_1.uct_search();
    mcst_player_2.change_current_status(current_player_move);
  }
  else {
    current_player_move = mcst_player_2.uct_search();
    mcst_player_1.change_current_status(current_player_move);
  }
  oxo_board.apply_action(current_player_move);
  std::cout << "Played move:" << current_player_move.to_string() << std::endl;
  oxo_board.print();
  oxo_board.print_board();
  remaining_moves = oxo_board.get_actions();
  // DEBUG
  for(auto it = remaining_moves.cbegin(); it!=remaining_moves.cend(); ++it)
    std::cout << it->to_string() << " ";
  std::cout << std::endl;
}

MPI_Finalize();

return 0;

}
