#include "../../src/monte_carlo_search_tree.hpp"
#include "../../src/nim.hpp"

int main() {

// Namespaces
using Nim = game::Nim::NimGame<>;
using Move = game::Nim::NimAction;

int outer_it1 = 100, inner_it1 = 100;
int outer_it2 = 100, inner_it2 = 100;

// MPI_Init(nullptr, nullptr);

Nim nim_board;
Move current_player_move;
std::vector<Move> remaining_moves = nim_board.get_actions();
MonteCarloSearchTree<Nim, Move> mcst_player_1(outer_it1, inner_it1);
MonteCarloSearchTree<Nim, Move> mcst_player_2(outer_it2, inner_it2);

nim_board.print();
nim_board.print_board();
// DEBUG
for(auto it = remaining_moves.cbegin(); it!=remaining_moves.cend(); ++it)
  std::cout << it->to_string() << " ";
std::cout << std::endl;

// Let two AIs play nim one against the other!
while( !nim_board.get_terminal_status() ) {
  if ( nim_board.get_agent_id() == 1 ) {
    current_player_move = mcst_player_1.uct_search();
    mcst_player_2.change_current_status(current_player_move);
  }
  else {
    current_player_move = mcst_player_2.uct_search();
    mcst_player_1.change_current_status(current_player_move);
  }
  nim_board.apply_action(current_player_move);
  std::cout << "Played move:" << current_player_move.to_string() << std::endl;
  nim_board.print();
  nim_board.print_board();
  remaining_moves = nim_board.get_actions();
  // DEBUG
  for(auto it = remaining_moves.cbegin(); it!=remaining_moves.cend(); ++it)
    std::cout << it->to_string() << " ";
  std::cout << std::endl;
}

// MPI_Finalize();

return 0;

}
