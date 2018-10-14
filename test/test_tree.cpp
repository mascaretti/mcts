#include "../src/monte_carlo_search_tree.hpp"
#include "../src/oxo.hpp"

int main() {

// Namespaces
using Oxo = game::Oxo::OxoGame;
using Move = game::Oxo::OxoAction;

// Test init (serial)
int seed = 1234;
unsigned outer_it = 1, inner_it = 1;
double mcst_constant = 1.4;
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

// Let two AIs play oxo one against the other! (First, serial)

Oxo oxo_board;
Move current_player_move;
MonteCarloSearchTree<Oxo, Move> mcst_player_1(seed, outer_it, inner_it, mcst_constant);
MonteCarloSearchTree<Oxo, Move> mcst_player_2(seed, outer_it, inner_it, mcst_constant);
// DEBUG
// std::cout << current_player_move.to_string() << std::endl;
// oxo_board.print();
// oxo_board.print_board();

while( !oxo_board.get_terminal_status() ) {
  if ( oxo_board.get_agent_id() == 1 ) {
    current_player_move = mcst_player_1.uct_search();
    mcst_player_2.change_current_status(current_player_move);
  }
  else {
    current_player_move = mcst_player_2.uct_search();
    mcst_player_1.change_current_status(current_player_move);
  }
  // DEBUG
  // std::cout << current_player_move.to_string() << std::endl;
  oxo_board.apply_action(current_player_move);
  // oxo_board.print();
  // oxo_board.print_board();
}

return 0;

}
