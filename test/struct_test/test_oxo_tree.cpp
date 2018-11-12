#include "monte_carlo_search_tree.hpp"
#include "oxo.hpp"

int main() {

// Namespaces
using Oxo = game::Oxo::OxoGame;
using Move = game::Oxo::OxoAction;

int outer_it1 = 10, inner_it1 = 10;
int outer_it2 = 10, inner_it2 = 10;

MPI_Init(nullptr, nullptr);
int rank, size;
MPI_Comm_rank(MPI_COMM_WORLD, &rank);
MPI_Comm_size(MPI_COMM_WORLD, &size);
if (rank == 0) {
  // DEBUG
  std::cout << size << std::endl;
}

Oxo oxo_board;
Move current_player_move;
std::vector<Move> remaining_moves = oxo_board.get_actions();
MonteCarloSearchTree<Oxo, Move> mcst_player_1(outer_it1, inner_it1);
MonteCarloSearchTree<Oxo, Move> mcst_player_2(outer_it2, inner_it2);

if(rank == 0)
{
oxo_board.print();
oxo_board.print_board();
// DEBUG
for(auto it = remaining_moves.cbegin(); it!=remaining_moves.cend(); ++it)
  std::cout << it->to_string() << " ";
std::cout << std::endl;
}

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
  if(rank == 0)
  {
  std::cout << "Played move:" << current_player_move.to_string() << std::endl;
  oxo_board.print();
  oxo_board.print_board();
  remaining_moves = oxo_board.get_actions();
  // DEBUG
  for(auto it = remaining_moves.cbegin(); it!=remaining_moves.cend(); ++it)
    std::cout << it->to_string() << " ";
  std::cout << std::endl;
  }
}

MPI_Finalize();

return 0;

}
