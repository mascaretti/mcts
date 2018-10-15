#ifndef MONTE_CARLO_SEARCH_TREE
#define MONTE_CARLO_SEARCH_TREE

#include "node.hpp"
#include <random>
#include <mpi.h>
#include <ctime>
#include <algorithm>

// DEBUG
#include <iostream>

/* - - - - - - - - - - - - - - - - - */
// Declaration of the template class //
/* - - - - - - - - - - - - - - - - - */

// NB! THE LIST OF POSSIBLE MOVES IS NOW A VECTOR OF MOVES, NOT A VECTOR OF POINTERS!!!!!

template<class Game, class Move>
// template<class Game>
class MonteCarloSearchTree {

public:

  typedef typename Node<Game,Move>::NodePointerType NodePointerType;
  // typedef typename Node<Game>::Move Move;

  /* Methods */

  double compute_ucb(const NodePointerType&) const;
  NodePointerType best_child_ucb(const NodePointerType&) const;

  NodePointerType select(void) const;
  NodePointerType expand(const NodePointerType);
  double rollout(const NodePointerType) const;
  void back_propagation(const NodePointerType, double);

  Move uct_search(void);

  // method to update the pointer to the curret game state
  void change_current_status(const Move&);

  /* Setters */

  void set_outer_iter(unsigned it) { outer_iter = it; }
  void set_inner_iter(unsigned it) { inner_iter = it; }
  void set_ucb_constant(double c) { ucb_constant = c; }

  /* Constructors: do a little refactoring! Need also a game to be passed as arg. */
  /*
  MonteCarloSearchTree();
  MonteCarloSearchTree(int);
  */
  MonteCarloSearchTree(unsigned, unsigned);
  MonteCarloSearchTree(unsigned, unsigned, double);
  MonteCarloSearchTree(int, unsigned, unsigned, double);

  /* Otherwise: default (for now... we would really need a copy constr/assign for deep copy) */

  /* Methods for debug */
  void print_current_status_info(void) const;

private:

  NodePointerType root;
  NodePointerType current_game_node;

  int seed;
  int is_parallel = 0;  /* 0 = false, 1 = true */
  std::default_random_engine rng;
  void set_rand_seed(void);

  unsigned outer_iter = 1;
  unsigned inner_iter = 1;

  double ucb_constant = sqrt(2.0);

};


/* - - - - - - - - - - - - - - - - - */
// Definition of methods             //
/* - - - - - - - - - - - - - - - - - */


// *************
// Constructors:  TO BE REVIEWED!
// *************

/*
template<class Game, class Move>
MonteCarloSearchTree<Game,Move>::MonteCarloSearchTree():
  root(), current_game_node(root)
  {
    set_rand_seed();
    rng.seed(seed);
    MPI_Initialized( &is_parallel );
    root = std::make_shared< Node<Game, Move> >();
    current_game_node = root;
  }

template<class Game, class Move>
MonteCarloSearchTree<Game,Move>::MonteCarloSearchTree(int s):
  root(), current_game_node(root),
  seed(s), rng(s)
  {
    MPI_Initialized( &is_parallel );
    root = std::make_shared< Node<Game, Move> >();
    current_game_node = root;
  }
*/

template<class Game, class Move>
MonteCarloSearchTree<Game,Move>::MonteCarloSearchTree(unsigned oi, unsigned ii):
  root(), current_game_node(root),
  outer_iter(oi), inner_iter(ii)
  {
    set_rand_seed();
    rng.seed(seed);
    MPI_Initialized( &is_parallel );
    root = std::make_shared< Node<Game, Move> >();
    current_game_node = root;
  }

template<class Game, class Move>
MonteCarloSearchTree<Game,Move>::MonteCarloSearchTree(unsigned oi, unsigned ii, double c):
  root(), current_game_node(root),
  outer_iter(oi), inner_iter(ii), ucb_constant(c)
  {
    set_rand_seed();
    rng.seed(seed);
    MPI_Initialized( &is_parallel );
    root = std::make_shared< Node<Game, Move> >();
    current_game_node = root;
  }

template<class Game, class Move>
MonteCarloSearchTree<Game,Move>::MonteCarloSearchTree(int s, unsigned oi, unsigned ii, double c):
  root(), current_game_node(root),
  seed(s), rng(s), outer_iter(oi), inner_iter(ii), ucb_constant(c)
  {
    MPI_Initialized( &is_parallel );
    root = std::make_shared< Node<Game, Move> >();
    current_game_node = root;
  }

// ********
// Methods:
// ********

template<class Game, class Move>
double
MonteCarloSearchTree<Game,Move>::compute_ucb(const NodePointerType& target_node) const
{
  return ( target_node->get_wins() / (double)(target_node->get_visits()) ) +
    ucb_constant * sqrt( log( (double)(target_node->get_parent()->get_visits()) )
    / (double)(target_node->get_visits()) );
}

// THIS IS INCOMPLETE, ISN'T IT?
template<class Game, class Move>
typename Node<Game,Move>::NodePointerType  /* ??? */
MonteCarloSearchTree<Game,Move>::best_child_ucb(const NodePointerType& target_parent) const
{
  std::vector<NodePointerType> children = target_parent->get_children();
  NodePointerType select = nullptr;
  double temp_ucb = 0.0;
  for (auto it = children.begin(); it != children.end(); ++it) {
    if( compute_ucb(*it) >= temp_ucb ) {
      select = *it;
      temp_ucb = compute_ucb(*it);
    }
  }
  return select;
}

template<class Game, class Move>
typename Node<Game,Move>::NodePointerType  /* ??? */
MonteCarloSearchTree<Game,Move>::select() const
{
  NodePointerType selected_node = current_game_node;
  while( selected_node->all_moves_tried() && selected_node->has_children() )
    selected_node = best_child_ucb(selected_node);
  return selected_node;
}

template<class Game, class Move>
typename Node<Game,Move>::NodePointerType  /* ??? */
MonteCarloSearchTree<Game,Move>::expand(const NodePointerType current_parent)
{
  NodePointerType expanded_node = nullptr;
  // If there are still moves that have not been tried:
  if( !(current_parent->all_moves_tried()) ) {
    // DEBUG
    // std::cout << "I can expand, and I will!" << std::endl;
    std::vector<Move> available_moves = current_parent->get_moves();
    std::size_t tot_moves = available_moves.size();
    std::uniform_int_distribution<> choose(0, tot_moves-1);
    int idx = 0;
    if ( is_parallel ) {
      // MPI_Init(nullptr, nullptr);
      int rank;
      MPI_Comm_rank(MPI_COMM_WORLD, &rank);
      if (rank==0) idx = choose(rng);
      MPI_Bcast(&idx, 1, MPI_INT, 0, MPI_COMM_WORLD);
      // MPI_Finalize();
    }
    else
      idx = choose(rng);
    expanded_node = current_parent->make_child(available_moves[idx]);
    return expanded_node;
  }
  // Else, if all moves have been tried: (CHECK!)
  /*
  else {
    std::vector<NodePointerType> available_children = current_parent->get_children();
    std::size_t tot_moves = available_children.size();
    std::uniform_int_distribution<> choose(0, tot_moves-1);
    int idx = 0;
    if ( is_parallel ) {
      // MPI_Init(nullptr, nullptr);
      int rank;
      MPI_Comm_rank(MPI_COMM_WORLD, &rank);
      if (rank==0) idx = choose(rng);
      MPI_Bcast(&idx, 1, MPI_INT, 0, MPI_COMM_WORLD);
      // MPI_Finalize();
    }
    else
      idx = choose(rng);
    expanded_node = available_children[idx];
    return expanded_node;
  }
  */
  else {
    // DEBUG
    // std::cout << "Alas, I can't expand..." << std::endl;
    return current_parent;
  }
}

template<class Game, class Move>
double
MonteCarloSearchTree<Game,Move>::rollout(const NodePointerType current_leaf) const
{
  // set_rand_seed();
  double total_score(0.0);
  // DEBUG
  // int move_counter = 0;
  // int game_counter = 0;
  if ( is_parallel ) {
    // MPI_Init(nullptr, nullptr);
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    unsigned local_iter = inner_iter/size;
    unsigned reminder = inner_iter%size;
    if (rank<reminder) local_iter++;
    for (unsigned i = 0; i<local_iter; ++i) {
      Game temp_game = current_leaf->get_game();
      temp_game.set_seed(seed);
      while ( !temp_game.get_terminal_status() )
        temp_game.apply_action(temp_game.random_action());
      double result = (double)temp_game.evaluate();
      result = result*(temp_game.get_agent_id()==current_leaf->get_player()) -
        result*(temp_game.get_agent_id()!=current_leaf->get_player()) +
        0.5*(result==0);
      total_score += result;
    }
    MPI_Allreduce(MPI_IN_PLACE, &total_score, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
    // MPI_Finalize();
  }
  else {
    for (unsigned i = 0; i<inner_iter; ++i) {
      Game temp_game = current_leaf->get_game();
      temp_game.set_seed(seed);
      while ( !temp_game.get_terminal_status() ) {
        temp_game.apply_action(temp_game.random_action());
        // DEBUG
        // move_counter++;
        // std::cout << "Move " << move_counter <<  " has been simulated..." << std::endl;
      }
      // DEBUG
      // move_counter = 0;
      // game_counter++;
      // std::cout << "Game " << game_counter << " has been simulated..." << std::endl;
      double result = (double)temp_game.evaluate();
      result = result*(temp_game.get_agent_id()==current_leaf->get_player()) -
        result*(temp_game.get_agent_id()!=current_leaf->get_player()) +
        0.5*(result==0);
      total_score += result;
    }
  }
  return total_score;
}

template<class Game, class Move>
void
MonteCarloSearchTree<Game,Move>::back_propagation(NodePointerType current_leaf, double score)
{
  current_leaf->update(score, inner_iter);
  Node<Game,Move>* temp_ptr = current_leaf->get_parent();
  while ( temp_ptr!=nullptr ) {
    temp_ptr->update(score, inner_iter);
    temp_ptr = temp_ptr->get_parent();
  }
}

template<class Game, class Move>
Move
MonteCarloSearchTree<Game,Move>::uct_search()
{
  // DEBUG
  // Game current_game_state = current_game_node->get_game();
  // current_game_state.print_board();
  for (int i = 0; i<outer_iter; ++i) {
    // Select
    NodePointerType selected_parent = select();
    // Expand
    NodePointerType selected_leaf = expand(selected_parent);
    // MC simulate
    double score = rollout(selected_leaf);  // THIS ONE HAS TO BE DEBUGGED (SERIAL VERSION!!!)
    // Back propagate
    back_propagation(selected_leaf, score);
  }
  // Select best move
  std::vector<NodePointerType> candidate_nodes = current_game_node->get_children();
  // Class node has to store the move where it come from!
  auto best_node_it = candidate_nodes.begin();
  for (auto it = candidate_nodes.begin(); it != candidate_nodes.end(); it++) {
    if ( ( (*it)->get_wins() )/(double)( (*it)->get_visits() ) >
      ( (*best_node_it)->get_wins() )/(double)( (*best_node_it)->get_visits() ) ) {
        best_node_it = it;
      }
  }
  current_game_node = *best_node_it;
  // DEBUG
  // current_game_node->print_node();
  return (*best_node_it)->get_last_move();
}

// HERE WE HAVE ONE OF THE PROBLEMS...
template<class Game, class Move>
void
MonteCarloSearchTree<Game,Move>::change_current_status(const Move& opponent_move)
{
  std::vector<Move> possible_opponent_moves = current_game_node->get_moves();
  auto it_out = std::find( possible_opponent_moves.cbegin(), possible_opponent_moves.cend(), opponent_move );
  if( it_out == (current_game_node->get_moves().end()) ) {
    // DEBUG
    std::cout << "Ah ah! I predicted my opponent..." << std::endl;
    std::vector<NodePointerType> possible_opponent_games = current_game_node->get_children();
    for (auto it_in = possible_opponent_games.cbegin(); it_in != possible_opponent_games.cend(); ++it_in) {
      if ( (*it_in)->get_last_move() == opponent_move ) {
        current_game_node = *it_in;
        return;
      }
    }
  }
  else {
    // DEBUG
    std::cout << "Oh nous! I couldn't predict this move..." << std::endl;
    current_game_node = current_game_node->make_child(opponent_move);
  }
}

template<class Game, class Move>
void MonteCarloSearchTree<Game,Move>::set_rand_seed()
{
  time_t rawtime;
  struct tm * ptm;
  time ( &rawtime );
  ptm = gmtime ( &rawtime );
  if ( is_parallel ) {
    int rank;
    // MPI_Init(nullptr, nullptr);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    // MPI_Finalize();
    seed = ptm->tm_sec + 10*(ptm->tm_min) + 100*(ptm->tm_hour) + 1000*(rank+1);
  }
  else
    seed = ptm->tm_sec + 10*(ptm->tm_min) + 100*(ptm->tm_hour) + 1000;
}

// DEBUG
template<class Game, class Move>
void MonteCarloSearchTree<Game,Move>::print_current_status_info() const {
  current_game_node->print_node();
  std::cout << "Moves left: ";
  for( auto it = (current_game_node->get_moves()).cbegin(); it!=(current_game_node->get_moves()).cend(); it++ )
    std::cout << (*it).to_string() << " ";
  std::cout << std::endl;
  Game tmp = current_game_node->get_game();
  tmp.print_board();
}

#endif /* MONTE_CARLO_SEARCH_TREE */
