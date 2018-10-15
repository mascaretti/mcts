#include "../src/monte_carlo_search_tree.hpp"
#include "../src/oxo.hpp"

int main() {

// Namespaces
using Oxo = game::Oxo::OxoGame;
using Move = game::Oxo::OxoAction;

int seed = 1234;
unsigned outer_it = 1, inner_it = 1;

// Test init (serial)
/*
MonteCarloSearchTree<Oxo, Move> mcst_s1();
MonteCarloSearchTree<Oxo, Move> mcst_s2(seed);
MonteCarloSearchTree<Oxo, Move> mcst_s3(outer_it, inner_it, mcst_constant);
MonteCarloSearchTree<Oxo, Move> mcst_s4(seed, outer_it, inner_it, mcst_constant);
*/

// Test init (parallel)
/*
MPI_Init(nullptr, nullptr);
MonteCarloSearchTree<Oxo, Move> mcst_p1();
MonteCarloSearchTree<Oxo, Move> mcst_p2(seed);
MonteCarloSearchTree<Oxo, Move> mcst_p3(outer_it, inner_it, mcst_constant);
MonteCarloSearchTree<Oxo, Move> mcst_p4(seed, outer_it, inner_it, mcst_constant);
MPI_Finalize();
*/

Oxo oxo_board;
Move current_player_move;
std::vector<Move> remaining_moves = oxo_board.get_actions();
MonteCarloSearchTree<Oxo, Move> mcst_player_1(outer_it, inner_it);
MonteCarloSearchTree<Oxo, Move> mcst_player_2(outer_it, inner_it);

// DEBUG
// mcst_player_1.print_current_status_info();
// mcst_player_2.print_current_status_info();
std::cout << "Default move:" << current_player_move.to_string() << std::endl;
oxo_board.print();
oxo_board.print_board();
for(auto it = remaining_moves.cbegin(); it!=remaining_moves.cend(); ++it)
  std::cout << it->to_string() << " ";
std::cout << std::endl;

// Let two AIs play oxo one against the other! (First, serial)
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
  // DEBUG
  // mcst_player_1.print_current_status_info();
  // mcst_player_2.print_current_status_info();
  std::cout << "Played move:" << current_player_move.to_string() << std::endl;
  oxo_board.print();
  oxo_board.print_board();
  remaining_moves = oxo_board.get_actions();
  for(auto it = remaining_moves.cbegin(); it!=remaining_moves.cend(); ++it)
    std::cout << it->to_string() << " ";
  std::cout << std::endl;
}

return 0;

}
