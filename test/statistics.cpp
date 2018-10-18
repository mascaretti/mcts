#include "../src/monte_carlo_search_tree.hpp"
#include "../src/oxo.hpp"
#include "../src/nim.hpp"

#include <fstream>

// Testing how well the algorithm behave against a purely random player

struct data_packet {
  std::string game_name = "";
  int inner_iter = 0;
  int outer_iter = 0;
  int player_tag = 0;
  int n_match = 0;
  int n_win = 0;
  int n_draw = 0;
  void print(void) const;
};

int main() {

using Oxo = game::Oxo::OxoGame;
using OxoAction = game::Oxo::OxoAction;
using Nim = game::Nim::NimGame<>;
using NimAction = game::Nim::NimAction;

int n_sample = 100;
std::vector<data_packet> collected_data;

// OXO first

// Two players
for (int k = 1; k<3; ++k) {

  // Outer iterations
  for (int i = 1; i<1000; i=10*i) {

    // Inner iterations
    for (int j = 1; j<1000; j=10*j) {

      data_packet temp_data;
      temp_data.game_name = "oxo";
      temp_data.inner_iter = j;
      temp_data.outer_iter = i;
      temp_data.player_tag = k;
      temp_data.n_match = n_sample;

      for (int n = 0; n<n_sample; ++n) {

        Oxo oxo_board;
        OxoAction current_oxoplayer_move;
        MonteCarloSearchTree<Oxo, OxoAction> mcst_oxo_player(i, j);

        while( !oxo_board.get_terminal_status() ) {
          if ( oxo_board.get_agent_id() == k )
            current_oxoplayer_move = mcst_oxo_player.uct_search();
          else {
            current_oxoplayer_move = oxo_board.random_action();
            mcst_oxo_player.change_current_status(current_oxoplayer_move);
          }
          oxo_board.apply_action(current_oxoplayer_move);
        }

        if( (oxo_board.evaluate()==1 && k==1) || (oxo_board.evaluate()==-1 && k==2) )
          temp_data.n_win++;
        else if( oxo_board.evaluate()==0 )
          temp_data.n_draw++;

      }

      temp_data.print();

      collected_data.push_back(temp_data);

    }

  }

}

// OXO first

// Two players
for (int k = 1; k<3; ++k) {

  // Outer iterations
  for (int i = 1; i<1000; i=10*i) {

    // Inner iterations
    for (int j = 1; j<1000; j=10*j) {

      data_packet temp_data;
      temp_data.game_name = "nim";
      temp_data.inner_iter = j;
      temp_data.outer_iter = i;
      temp_data.player_tag = k;
      temp_data.n_match = n_sample;

      for (int n = 0; n<n_sample; ++n) {

        Nim nim_board;
        NimAction current_nimplayer_move;
        MonteCarloSearchTree<Nim, NimAction> mcst_nim_player(i, j);

        while( !nim_board.get_terminal_status() ) {
          if ( nim_board.get_agent_id() == k )
            current_nimplayer_move = mcst_nim_player.uct_search();
          else {
            current_nimplayer_move = nim_board.random_action();
            mcst_nim_player.change_current_status(current_nimplayer_move);
          }
          nim_board.apply_action(current_nimplayer_move);
        }

        if( (nim_board.evaluate()==1 && k==1) || (nim_board.evaluate()==-1 && k==2) )
          temp_data.n_win++;
        else if( nim_board.evaluate()==0 )
          temp_data.n_draw++;

      }

      temp_data.print();

      collected_data.push_back(temp_data);

    }

  }

}

std::ofstream data_set;
data_set.open ("data_games.csv");
data_set << "GAME" << "," << "IN_ITER" << "," << "OUT_ITER"
<< "," << "PLAYER" << "," << "N_MATCH" << "," << "N_WIN" << ","
<< "N_DRAW" << std::endl;
for (auto it = collected_data.cbegin(); it!=collected_data.cend(); ++it) {
  data_set << it->game_name << "," << it->inner_iter << "," << it->outer_iter
  << "," << it->player_tag << "," << it->n_match << "," << it->n_win << ","
  << it->n_draw << std::endl;
}
data_set.close();

}

void data_packet::print() const {
  std::cout << "********************************" << std::endl;
  std::cout << "Following dataset simulated:" << std::endl;
  std::cout << "game : \t" << game_name << std::endl;
  std::cout << "inner_iter = \t" << inner_iter << std::endl;
  std::cout << "outer_iter = \t" << outer_iter << std::endl;
  std::cout << "player_tag = \t" << player_tag << std::endl;
  std::cout << "n_match = \t" << n_match << std::endl;
  std::cout << "n_win = \t" << n_win << std::endl;
  std::cout << "n_draw = \t" << n_draw << std::endl;
  std::cout << "********************************" << std::endl;
}
