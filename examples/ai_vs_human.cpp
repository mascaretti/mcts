#include "monte_carlo_search_tree.hpp"
#include "oxo.hpp"
#include "nim.hpp"

int main(int argc, char** argv) {

using Oxo = game::Oxo::OxoGame<>;
using OxoMove = game::Oxo::OxoAction;
using Nim = game::Nim::NimGame<>;
using NimMove = game::Nim::NimAction;

MPI_Init(&argc, &argv);

int rank, size;
MPI_Comm_rank(MPI_COMM_WORLD, &rank);
MPI_Comm_size(MPI_COMM_WORLD, &size);
int outer_it, inner_it;
int game_tag, difficulty_tag;

if (rank==0) {
  std::cout << std::endl;
  std::cout << "#######################" << std::endl;
  std::cout << "Monte Carlo search Tree\nAI vs. human" << std::endl;
  std::cout << "#######################" << std::endl;
  std::cout << std::endl;
  std::cout << "Select game:\n[1]: Oxo\n[2]: Nim" << std::endl;
  std::cin >> game_tag;
  std::cout << "Select difficulty:\n[1]: Easy\n[2]: Medium\n[3]: Hard\n[4]: Very hard" << std::endl;
  std::cin >> difficulty_tag;
  assert ( (game_tag==1 || game_tag==2) && (difficulty_tag==1 || difficulty_tag==2 || difficulty_tag==3 || difficulty_tag==4) );
}

MPI_Bcast(&game_tag, 1, MPI_INT, 0, MPI_COMM_WORLD);
MPI_Bcast(&difficulty_tag, 1, MPI_INT, 0, MPI_COMM_WORLD);

switch (difficulty_tag) {
  case 1:
    outer_it = 1; inner_it = 1;
    break;
  case 2:
    outer_it = 10; inner_it = 10;
    break;
  case 3:
    outer_it = 100; inner_it = 100;
    break;
  case 4:
    outer_it = 1000; inner_it = 1000;
    break;
  default:
    std::cerr << "Invalid difficulty tag" << std::endl;
}

// Play OXO
if (game_tag==1) {
  Oxo oxo_board;
  OxoMove current_player_move;
  MonteCarloSearchTree<Oxo, OxoMove> mcst_player(outer_it, inner_it);
  int idx_move = 0;
  if (rank == 0) {
    oxo_board.print();
    oxo_board.print_board();
  }
  while( !oxo_board.get_terminal_status() ) {
    if ( oxo_board.get_agent_id() == 1 ) {
      if (rank == 0)
        idx_move = oxo_board.human_input();
      MPI_Bcast(&idx_move, 1, MPI_INT, 0, MPI_COMM_WORLD);
      current_player_move = oxo_board.get_actions()[idx_move];
      oxo_board.apply_action(current_player_move);
      mcst_player.change_current_status(oxo_board.get_last_action());
    }
    else {
      current_player_move = mcst_player.uct_search();
      oxo_board.apply_action(current_player_move);
    }
    if (rank == 0) {
      oxo_board.print();
      oxo_board.print_board();
    }
  }
  if (rank == 0) {
    if (oxo_board.evaluate()==1)
      std::cout << "Player 1 wins!" << std::endl;
    else if (oxo_board.evaluate()==-1)
      std::cout << "Player 2 wins!" << std::endl;
    else
      std::cout << "We call it a draw..." << std::endl;
  }
}

// Play NIM
else if (game_tag==2) {
  Nim nim_board;
  NimMove current_player_move;
  MonteCarloSearchTree<Nim, NimMove> mcst_player(outer_it, inner_it);
  int idx_move = 0;
  if (rank == 0) {
    nim_board.print();
    nim_board.print_board();
  }
  while( !nim_board.get_terminal_status() ) {
    if ( nim_board.get_agent_id() == 1 ) {
      if (rank == 0)
        idx_move = nim_board.human_input();
      MPI_Bcast(&idx_move, 1, MPI_INT, 0, MPI_COMM_WORLD);
      current_player_move = nim_board.get_actions()[idx_move];
      nim_board.apply_action(current_player_move);
      mcst_player.change_current_status(nim_board.get_last_action());
    }
    else {
      current_player_move = mcst_player.uct_search();
      nim_board.apply_action(current_player_move);
    }
    if (rank == 0) {
      nim_board.print();
      nim_board.print_board();
    }
  }
  if (rank == 0) {
    if (nim_board.evaluate()==1)
      std::cout << "Player 1 wins!" << std::endl;
    else if (nim_board.evaluate()==-1)
      std::cout << "Player 2 wins!" << std::endl;
    else
      std::cout << "We call it a draw..." << std::endl;
  }
}


else {
  std::cerr << "Invalid game tag" << std::endl;
}

MPI_Finalize();

return 0;

}
