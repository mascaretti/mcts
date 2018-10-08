#ifndef MONTE_CARLO_SEARCH_TREE
#define MONTE_CARLO_SEARCH_TREE

#include "node.hpp"
#include <random>
#include <mpi.h>
#include <ctime>
#include <algorithm>


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
  MonteCarloSearchTree();
  MonteCarloSearchTree(int);
  MonteCarloSearchTree(unsigned, unsigned, double);
  MonteCarloSearchTree(int, unsigned, unsigned, double);

  /* Otherwise: default (for now... we would realli need a copy constr/assign for deep copy) */

private:

  NodePointerType root;
  NodePointerType current_game_node;

  int seed;
  int is_parallel = 0;  /* 0 = false, 1 = true */
  std::default_random_engine rng;
  void set_rand_seed(void);

  unsigned outer_iter = 1000;
  unsigned inner_iter = 100;

  double ucb_constant = sqrt(2.0);

};


/* - - - - - - - - - - - - - - - - - */
// Definition of methods             //
/* - - - - - - - - - - - - - - - - - */

// Constructors:

template<class Game, class Move>
MonteCarloSearchTree<Game,Move>::MonteCarloSearchTree():
  root(), current_game_node(root)
  {
    set_rand_seed();
    rng.seed(seed);
    MPI_Initialized( &is_parallel );
  }

template<class Game, class Move>
MonteCarloSearchTree<Game,Move>::MonteCarloSearchTree(int s):
  root(), current_game_node(root),
  seed(s), rng(s)
  {
    MPI_Initialized( &is_parallel );
  }

template<class Game, class Move>
MonteCarloSearchTree<Game,Move>::MonteCarloSearchTree(unsigned oi, unsigned ii, double c):
  root(), current_game_node(root),
  outer_iter(oi), inner_iter(ii), ucb_constant(c)
  {
    set_rand_seed();
    rng.seed(seed);
    MPI_Initialized( &is_parallel );
  }

template<class Game, class Move>
MonteCarloSearchTree<Game,Move>::MonteCarloSearchTree(int s, unsigned oi, unsigned ii, double c):
  root(), current_game_node(root),
  seed(s), rng(s), outer_iter(oi), inner_iter(ii), ucb_constant(c)
  {
    MPI_Initialized( &is_parallel );
  }

// Methods:

template<class Game, class Move>
double
MonteCarloSearchTree<Game,Move>::compute_ucb(const NodePointerType& target_node) const
{
  return ( target_node->get_wins() / (double)(target_node->get_visits()) ) +
    ucb_constant * sqrt( log( (double)(target_node->get_parent()->get_visits()) )
    / (double)(target_node->get_visits()) );
}

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
  if( !(current_parent->all_moves_tried()) ) {
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
    expanded_node = current_parent.make_child(available_moves[idx]);
  }
}

template<class Game, class Move>
double
MonteCarloSearchTree<Game,Move>::rollout(const NodePointerType current_leaf) const
{
  set_rand_seed();
  double total_score(0.0);
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
      while ( temp_game.get_terminal_status() )
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
      while ( temp_game.get_terminal_status() )
        temp_game.apply_action(temp_game.random_action());
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
  Node<Game,Move>* temp_ptr = current_leaf.get_parent();
  while ( temp_ptr!=nullptr ) {
    temp_ptr->update(score, inner_iter);
    temp_ptr = temp_ptr->get_parent();
  }
}

template<class Game, class Move>
Move
MonteCarloSearchTree<Game,Move>::uct_search()
{
  for (int i = 0; i<outer_iter; ++i) {
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
      ( (*best_node_it)->get_wins() )/(double)( (*best_node_it)->get_visits() ) ) {
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
  auto it_out = std::find( (current_game_node->get_moves()).begin(), (current_game_node->get_moves().end()) );
  if( it_out == (current_game_node->get_moves().end()) ) {
    std::vector<NodePointerType> possible_opponenet_moves = current_game_node->get_children();
    for (auto it_in = possible_opponenet_moves.begin(); it_in != possible_opponenet_moves.end(); ++it_in) {
      if ( (*it_in)->get_last_move()==opponent_move ) {
        current_game_node = *it_in;
        return;
      }
    }
  }
  else {
    current_game_node = current_game_node.make_child(opponent_move);
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

// To be continued ...

#endif /* MONTE_CARLO_SEARCH_TREE */
