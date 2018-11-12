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
int outer_it1, inner_it1;
int outer_it2, inner_it2;
int game_tag, difficulty_tag1, difficulty_tag2;

if (rank==0) {
  std::cout << std::endl;
  std::cout << "#######################" << std::endl;
  std::cout << "Monte Carlo search Tree\nAI vs. AI" << std::endl;
  std::cout << "#######################" << std::endl;
  std::cout << std::endl;
  std::cout << "Select game:\n[1]: Oxo\n[2]: Nim" << std::endl;
  std::cin >> game_tag;
  std::cout << "Select player 1 difficulty:\n[1]: Easy\n[2]: Medium\n[3]: Hard\n[4]: Very hard" << std::endl;
  std::cin >> difficulty_tag1;
  std::cout << "Select player 2 difficulty:\n[1]: Easy\n[2]: Medium\n[3]: Hard\n[4]: Very hard" << std::endl;
  std::cin >> difficulty_tag2;
  assert ( (game_tag==1 || game_tag==2) &&
    (difficulty_tag1==1 || difficulty_tag1==2 || difficulty_tag1==3 || difficulty_tag1==4) &&
    (difficulty_tag2==1 || difficulty_tag2==2 || difficulty_tag2==3 || difficulty_tag2==4) );
}

MPI_Bcast(&game_tag, 1, MPI_INT, 0, MPI_COMM_WORLD);
MPI_Bcast(&difficulty_tag1, 1, MPI_INT, 0, MPI_COMM_WORLD);
MPI_Bcast(&difficulty_tag2, 1, MPI_INT, 0, MPI_COMM_WORLD);

switch (difficulty_tag1) {
  case 1:
    outer_it1 = 1; inner_it1 = 1;
    break;
  case 2:
    outer_it1 = 10; inner_it1 = 10;
    break;
  case 3:
    outer_it1 = 100; inner_it1 = 100;
    break;
  case 4:
    outer_it1 = 1000; inner_it1 = 1000;
    break;
  default:
    std::cerr << "Invalid difficulty tag" << std::endl;
}
switch (difficulty_tag2) {
  case 1:
    outer_it2 = 1; inner_it2 = 1;
    break;
  case 2:
    outer_it2 = 10; inner_it2 = 10;
    break;
  case 3:
    outer_it2 = 100; inner_it2 = 100;
    break;
  case 4:
    outer_it2 = 1000; inner_it2 = 1000;
    break;
  default:
    std::cerr << "Invalid difficulty tag" << std::endl;
}

// Play OXO
if (game_tag==1) {
  Oxo oxo_board;
  OxoMove current_player_move;
  MonteCarloSearchTree<Oxo, OxoMove> mcst_player_1(outer_it1, inner_it2);
  MonteCarloSearchTree<Oxo, OxoMove> mcst_player_2(outer_it1, inner_it2);
  if (rank == 0) {
    oxo_board.print();
    oxo_board.print_board();
  }
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
  MonteCarloSearchTree<Nim, NimMove> mcst_player_1(outer_it1, inner_it1);
  MonteCarloSearchTree<Nim, NimMove> mcst_player_2(outer_it2, inner_it2);
  if (rank == 0) {
    nim_board.print();
    nim_board.print_board();
  }
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
