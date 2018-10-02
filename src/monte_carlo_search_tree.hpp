#ifndef MONTE_CARLO_SEARCH_TREE
#define MONTE_CARLO_SEARCH_TREE

#include "node.hpp"
#include <random>
#include <mpi.h>
#include <ctime>


/* - - - - - - - - - - - - - - - - - */
// Declaration of the template class //
/* - - - - - - - - - - - - - - - - - */

template<class Game>
class MonteCarloSearchTree {

public:

  typedef typename Node<Game>::NodePointerType NodePointerType;
  typedef typename Node<Game>::Move Move;

  /* Methods */

  double compute_ucb(const NodePointerType&) const;
  NodePointerType best_child_ucb(const NodePointerType&) const;

  NodePointerType select(void) const;
  NodePointerType expand(NodePointerType);
  double rollout(NodePointerType) const;
  void back_propagation(NodePointerType, double);

  Move uct_search(void);

  /* Setters */

  void set_outer_iter(unsigned it) { outer_iter = it; }
  void set_inner_iter(unsigned it) { inner_iter = it; }
  void set_ucb_constant(double c) { ucb_constant = c; }

  /* Constructors */
  MonteCarloSearchTree();
  MonteCarloSearchTree(int);
  MonteCarloSearchTree(unsigned, unsigned, double);
  MonteCarloSearchTree(int, unsigned, unsigned, double);

  /* Otherwise: default (for now...) */

private:

  NodePointerType root;
  NodePointerType current_game_node;

  int seed;
  std::default_random_engine rng;
  void set_rand_seed(void);

  unsigned outer_iter = 1000;
  unsigned inner_iter = 100;

  double ucb_constant = sqrt(2.0);

};


/* - - - - - - - - - - - - - - - - - */
// Definition of methods             //
/* - - - - - - - - - - - - - - - - - */

template<class Game>
MonteCarloSearchTree<Game>::MonteCarloSearchTree():
  root(), current_game_node(root)
  {
    set_rand_seed();
    rng.seed(seed);
  }

template<class Game>
MonteCarloSearchTree<Game>::MonteCarloSearchTree(int s):
  root(), current_game_node(root),
  seed(s), rng(s) { }

template<class Game>
MonteCarloSearchTree<Game>::MonteCarloSearchTree(unsigned oi, unsigned ii, double c):
  root(), current_game_node(root),
  outer_iter(oi), inner_iter(ii), ucb_constant(c)
  {
    set_rand_seed();
    rng.seed(seed);
  }

template<class Game>
MonteCarloSearchTree<Game>::MonteCarloSearchTree(int s, unsigned oi, unsigned ii, double c):
  root(), current_game_node(root),
  seed(s), rng(s), outer_iter(oi), inner_iter(ii), ucb_constant(c) { }

template<class Game>
double
MonteCarloSearchTree<Game>::compute_ucb(const NodePointerType& target_node) const
{
  return ( target_node->get_wins() / (double)(target_node->get_visits()) ) +
    ucb_constant * sqrt( log( (double)(target_node->get_parent()->get_visits()) )
    / (double)(target_node->get_visits()) );
}

template<class Game>
typename Node<Game>::NodePointerType  /* ??? */
MonteCarloSearchTree<Game>::best_child_ucb(const NodePointerType& target_parent) const
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

template<class Game>
typename Node<Game>::NodePointerType  /* ??? */
MonteCarloSearchTree<Game>::select() const
{
  NodePointerType selected_node = current_game_node;
  while( selected_node->all_moves_tried() && selected_node->has_children() )
    selected_node = best_child_ucb(selected_node);
  return selected_node;
}

template<class Game>
typename Node<Game>::NodePointerType  /* ??? */
MonteCarloSearchTree<Game>::expand(NodePointerType current_parent)
{
  NodePointerType expanded_node = nullptr;
  if( !(current_parent->all_moves_tried()) ) {
    std::vector<Move> available_moves = current_parent->get_moves();
    std::size_t tot_moves = available_moves.size();
    std::uniform_int_distribution<> choose(0, tot_moves-1);
    MPI_Init(nullptr, nullptr);
    int idx, rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank==0) idx = choose(rng);
    MPI_Bcast(&idx, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Finalize();
    expanded_node = current_parent.make_child(available_moves[idx]);
  }
}

template<class Game>
double
MonteCarloSearchTree<Game>::rollout(NodePointerType current_leaf) const
{
  set_rand_seed();
  double total_score(0.0);
  MPI_Init(nullptr, nullptr);
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
  MPI_Finalize();
  return total_score;
}

template<class Game>
void
MonteCarloSearchTree<Game>::back_propagation(NodePointerType current_leaf, double score)
{
  current_leaf->update(score, inner_iter);
  Node<Game>* temp_ptr = current_leaf.get_parent();
  while ( temp_ptr!=nullptr ) {
    temp_ptr->update(score, inner_iter);
    temp_ptr = temp_ptr->get_parent();
  }
}

template<class Game>
typename Node<Game>::Move             /* ??? */
MonteCarloSearchTree<Game>::uct_search()
{
  /* ... */
}

template<class Game>
void MonteCarloSearchTree<Game>::set_rand_seed()
{
  time_t rawtime;
  struct tm * ptm;
  time ( &rawtime );
  ptm = gmtime ( &rawtime );
  int rank;
  MPI_Init(nullptr, nullptr);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Finalize();
  seed = ptm->tm_sec + 10*(ptm->tm_min) + 100*(ptm->tm_hour) + 1000*(rank+1);
}

// To be continued ...

#endif /* MONTE_CARLO_SEARCH_TREE */
