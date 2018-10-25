#ifndef MONTE_CARLO_SEARCH_TREE
#define MONTE_CARLO_SEARCH_TREE

#include "node.hpp"
#include <random>
#include <mpi.h>
#include <chrono>
#include <algorithm>
#include <cmath>

// DEBUG
#include <cassert>

/* - - - - - - - - - - - - - - - - - */
// Declaration of the template class //
/* - - - - - - - - - - - - - - - - - */

template<class Game, class Move>
class MonteCarloSearchTree {

public:

  // Shared pointer to Node class type
  typedef typename Node<Game,Move>::NodePointerType NodePointerType;

  /* List of public methods */

  // Performs the UCT search and find the best next move
  Move uct_search(void);

  // Updates the pointer to the curret game state
  void change_current_status(const Move&);

  /* Setters */
  void set_outer_iter(unsigned it) { outer_iter = it; }
  void set_inner_iter(unsigned it) { inner_iter = it; }
  void set_ucb_constant(double c) { ucb_constant = c; }

  /* Constructors */
  MonteCarloSearchTree(unsigned, unsigned);
  MonteCarloSearchTree(unsigned, unsigned, double);
  MonteCarloSearchTree(int, unsigned, unsigned);
  MonteCarloSearchTree(int, unsigned, unsigned, double);

  // For the moment copy assignment and costructors are prevented
  // In future deep-copy costructor and assignment may be implemented
  MonteCarloSearchTree& operator = (const MonteCarloSearchTree&) = delete;
  MonteCarloSearchTree(const MonteCarloSearchTree&) = delete;

  // Print info (DEBUG)
  void print_current_status_info(void) const;

  /* Destructor */
  ~MonteCarloSearchTree() = default;

private:

  /* List of private methods */

  // Given a node, computes its UCB
  double compute_ucb(const NodePointerType&) const;

  // Given a parent node, returns the child with best UCB
  NodePointerType best_child_ucb(const NodePointerType&) const;

  // Steps of the MCST algorithm
  // (1) selects the best UCB node and returns it, via pointer
  // (2) produces a leaf from the selected node and returns it
  // (3) performs multiple MC simulations and returns the summed score
  // (4) update visits and scores of the nodes all way back to the root
  NodePointerType select(void) const;
  NodePointerType expand(const NodePointerType);
  double rollout(const NodePointerType);
  void back_propagation(const NodePointerType, double);

  // RNG management
  int gen_rand_seed(void);
  void set_rand_seed(void);

  /* List of private members */

  NodePointerType root;
  NodePointerType current_game_node;
  int seed;
  std::chrono::steady_clock rng_time;
  int seed_increment = 0;
  std::default_random_engine rng;
  int is_parallel = 0;
  unsigned outer_iter;
  unsigned inner_iter;
  double ucb_constant = sqrt(2.0);

};

/* - - - - - - - - - - - - - - - - - */
// Definition of methods             //
/* - - - - - - - - - - - - - - - - - */

// *************
// Constructors:
// *************

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
MonteCarloSearchTree<Game,Move>::MonteCarloSearchTree(int s, unsigned oi, unsigned ii):
  root(), current_game_node(root),
  seed(s), rng(s), outer_iter(oi), inner_iter(ii)
  {
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
  assert ( target_node->get_visits()>0 );
  return ( target_node->get_wins() / (double)(target_node->get_visits()) ) +
    ucb_constant * sqrt( log( (double)(target_node->get_parent()->get_visits()) )
    / (double)(target_node->get_visits()) );
}

template<class Game, class Move>
typename Node<Game,Move>::NodePointerType  /* ??? */
MonteCarloSearchTree<Game,Move>::best_child_ucb(const NodePointerType& target_parent) const
{
  std::vector<NodePointerType> children = target_parent->get_children();
  NodePointerType select = (*(children.cbegin()));
  double best_ucb = compute_ucb(select);
  double temp_ucb = 0.0;
  for (auto it = children.cbegin()+1; it != children.cend(); ++it) {
    temp_ucb = compute_ucb(*it);
    if( temp_ucb >= best_ucb ) {
      select = (*it);
      best_ucb = temp_ucb;
    }
  }
  return select;
}

template<class Game, class Move>
typename Node<Game,Move>::NodePointerType  /* ??? */
MonteCarloSearchTree<Game,Move>::select() const
{
  NodePointerType selected_node = current_game_node;
  while( selected_node->all_moves_tried() && selected_node->has_children() ) {
    assert( best_child_ucb(selected_node)!=nullptr );
    selected_node = best_child_ucb(selected_node);
  }
  return selected_node;
}

template<class Game, class Move>
typename Node<Game,Move>::NodePointerType  /* ??? */
MonteCarloSearchTree<Game,Move>::expand(const NodePointerType current_parent)
{
  // NodePointerType expanded_node = nullptr;
  // If there are still moves that have not been tried:
  if( !(current_parent->all_moves_tried()) ) {
    std::vector<Move> available_moves = current_parent->get_moves();
    std::size_t tot_moves = available_moves.size();
    std::uniform_int_distribution<> choose(0, tot_moves-1);
    int idx = 0;
    if ( is_parallel ) {
      int rank;
      MPI_Comm_rank(MPI_COMM_WORLD, &rank);
      if (rank==0) idx = choose(rng);
      MPI_Bcast(&idx, 1, MPI_INT, 0, MPI_COMM_WORLD);
    }
    else
      idx = choose(rng);
    return current_parent->make_child(available_moves[idx]);
  }
  // If all moves have been tried and it's not a leaf
  else if ( current_parent->has_children() ) {
    std::vector<NodePointerType> available_children = current_parent->get_children();
    std::size_t tot_moves = available_children.size();
    std::uniform_int_distribution<> choose(0, tot_moves-1);
    int idx = 0;
    if ( is_parallel ) {
      int rank;
      MPI_Comm_rank(MPI_COMM_WORLD, &rank);
      if (rank==0) idx = choose(rng);
      MPI_Bcast(&idx, 1, MPI_INT, 0, MPI_COMM_WORLD);
    }
    else
      idx = choose(rng);
    return available_children[idx];
  }
  // If it is a leaf
  else {
    return current_parent;
  }
}

template<class Game, class Move>
double
MonteCarloSearchTree<Game,Move>::rollout(const NodePointerType current_leaf)
{
  double total_score(0.0);
  if ( is_parallel ) {
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    unsigned local_iter = inner_iter/size;
    unsigned reminder = inner_iter%size;
    if (rank<reminder) local_iter++;
    for (unsigned i = 0; i<local_iter; ++i) {
      Game temp_game = current_leaf->get_game();
      temp_game.set_seed(gen_rand_seed());
      while ( !temp_game.get_terminal_status() )
        temp_game.apply_action(temp_game.random_action());
      double result = (double)temp_game.evaluate();
      result = 0.5*(result+1) * ( current_game_node->get_player()==1 )
        + 0.5*(1-result) * ( current_game_node->get_player()==2 );
      total_score += result;
    }
    MPI_Allreduce(MPI_IN_PLACE, &total_score, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
  }
  else {
    for (unsigned i = 0; i<inner_iter; ++i) {
      Game temp_game = current_leaf->get_game();
      temp_game.set_seed(gen_rand_seed());
      while ( !temp_game.get_terminal_status() )
        temp_game.apply_action( temp_game.random_action() );
      double result = (double)temp_game.evaluate();
      result = 0.5*(result+1) * ( current_game_node->get_player()==1 )
        + 0.5*(1-result) * ( current_game_node->get_player()==2 );
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

  for (unsigned i = 0; i<outer_iter; ++i) {

    // Select
    NodePointerType selected_parent = select();

    // Expand
    NodePointerType selected_leaf = expand(selected_parent);

    // MC simulate
    double score = rollout(selected_leaf);

    // Back propagate
    back_propagation(selected_leaf, score);

  }

  // Select best move
  std::vector<NodePointerType> candidate_nodes = current_game_node->get_children();
  // Class node has to store the move where it come from!
  auto best_node_it = candidate_nodes.begin();
  for (auto it = candidate_nodes.begin(); it != candidate_nodes.end(); it++) {
    if ( ( (*it)->get_wins() )/(double)( (*it)->get_visits() ) >
      ( (*best_node_it)->get_wins() )/(double)( (*best_node_it)->get_visits() ) )
      {
        best_node_it = it;
      }
  }
  current_game_node = *best_node_it;
  return (*best_node_it)->get_last_move();

}

template<class Game, class Move>
void
MonteCarloSearchTree<Game,Move>::change_current_status(const Move& opponent_move)
{
  std::vector<Move> possible_opponent_moves = current_game_node->get_moves();
  auto it_out = std::find( possible_opponent_moves.cbegin(), possible_opponent_moves.cend(), opponent_move );
  if( it_out == (current_game_node->get_moves().end()) ) {
    std::vector<NodePointerType> possible_opponent_games = current_game_node->get_children();
    for (auto it_in = possible_opponent_games.cbegin(); it_in != possible_opponent_games.cend(); ++it_in) {
      if ( (*it_in)->get_last_move() == opponent_move ) {
        current_game_node = *it_in;
        return;
      }
    }
  }
  else {
    current_game_node = current_game_node->make_child(opponent_move);
  }
}

template<class Game, class Move>
void MonteCarloSearchTree<Game,Move>::set_rand_seed()
{
  auto now_time = rng_time.now();
  std::chrono::duration<double> diff = now_time.time_since_epoch();
  if ( is_parallel ) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    seed = ( (int)((diff.count())*100) + seed_increment );
  }
  else
    seed = ( (int)((diff.count())*100) + seed_increment );
  rng.seed(seed);
  seed_increment++;
}

template<class Game, class Move>
int MonteCarloSearchTree<Game,Move>::gen_rand_seed()
{
  seed_increment++;
  if ( is_parallel ) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    return ( seed + seed_increment );
  }
  else
    return ( seed + seed_increment );
}

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
