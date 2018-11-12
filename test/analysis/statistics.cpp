#include "monte_carlo_search_tree.hpp"
#include "oxo.hpp"
#include "nim.hpp"

#include <fstream>
#include <ctime>

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

struct seed_generator {
  unsigned state = 0;
  int operator () (void);
};

int main() {

using Oxo = game::Oxo::OxoGame;
using OxoAction = game::Oxo::OxoAction;
using Nim = game::Nim::NimGame<>;
using NimAction = game::Nim::NimAction;

seed_generator gen_seed;
int n_sample = 100;
std::vector<data_packet> collected_data;


// OXO

// Test: two random players (player 1 persp.)
data_packet temp_data_oxo;
temp_data_oxo.game_name = "oxo_rand";
temp_data_oxo.n_match = n_sample;
for (int n = 0; n<n_sample; ++n) {
  Oxo oxo_board;
  oxo_board.set_seed(gen_seed());
  while( !oxo_board.get_terminal_status() )
    oxo_board.apply_action(oxo_board.random_action());
  if( oxo_board.evaluate()==1 )
    temp_data_oxo.n_win++;
  else if( oxo_board.evaluate()==0 )
    temp_data_oxo.n_draw++;
}
temp_data_oxo.print();
collected_data.push_back(temp_data_oxo);

// Two players
for (int k = 1; k<3; ++k) {

  // Outer iterations
  //for (int i = 1; i<10000; i+= 500) {
  	int i{100};

    // Inner iterations
    for (int j = 100; j<=2000; j+= 100) {

      data_packet temp_data;
      temp_data.game_name = "oxo";
      temp_data.inner_iter = j;
      temp_data.outer_iter = i;
      temp_data.player_tag = k;
      temp_data.n_match = n_sample;

      for (int n = 0; n<n_sample; ++n) {

        Oxo oxo_board;
        oxo_board.set_seed(gen_seed());
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

  //}

}


// NIM

// Test: two random players (player 1 persp.)
data_packet temp_data_nim;
temp_data_nim.game_name = "nim_rand";
temp_data_nim.n_match = n_sample;
for (int n = 0; n<n_sample; ++n) {
  Nim nim_board;
  nim_board.set_seed(gen_seed());
  while( !nim_board.get_terminal_status() )
    nim_board.apply_action(nim_board.random_action());
  if( nim_board.evaluate()==1 )
    temp_data_nim.n_win++;
  else if( nim_board.evaluate()==0 )
    temp_data_nim.n_draw++;
}
temp_data_nim.print();
collected_data.push_back(temp_data_nim);

// Two players
for (int k = 1; k<3; ++k) {

  // Outer iterations
  //for (int i = 1; i<10000; i+= 500) {
  	int i{100};

    // Inner iterations
    for (int j = 100; j<=2000; j+= 100) {

      data_packet temp_data;
      temp_data.game_name = "nim";
      temp_data.inner_iter = j;
      temp_data.outer_iter = i;
      temp_data.player_tag = k;
      temp_data.n_match = n_sample;

      for (int n = 0; n<n_sample; ++n) {

        Nim nim_board;
        nim_board.set_seed(gen_seed());
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

  //}

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

int seed_generator::operator () () {
  time_t rawtime;
  struct tm * ptm;
  time ( &rawtime );
  ptm = gmtime ( &rawtime );
  state = (state+1);
  return state + ptm->tm_sec + 10*(ptm->tm_min) + 100*(ptm->tm_hour) + 1000;
}
