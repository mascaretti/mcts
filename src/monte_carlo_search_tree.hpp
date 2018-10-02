#ifndef MONTE_CARLO_SEARCH_TREE
#define MONTE_CARLO_SEARCH_TREE

#include "node.hpp"
#include <random>
#include <mpi.h>
#include <ctime>

// Declaration of the template class

template<class Game>
class MonteCarloSearchTree {

public:

  typedef typename Node<Game>::NodePointerType NodePointerType;
  typedef typename Node<Game>::Move Move;

  /* Methods */

  double compute_ucb(const NodePointerType&) const;
  NodePointerType best_child_ucb(const NodePointerType&) const;
  void set_rand_seed(void);

  NodePointerType select(void) const;
  NodePointerType expand(NodePointerType);
  double rollout(NodePointerType) const;
  void back_propagation(NodePointerType);

  Move uct_search(void);

  /* Setters */

  void set_outer_iter(unsigned it) { outer_iter = it; }
  void set_inner_iter(unsigned it) { inner_iter = it; }
  void set_ucb_constant(double c) { ucb_constant = c; }

  /* Constructors */
  MonteCarloSearchTree<Game>();
  MonteCarloSearchTree<Game>(const MonteCarloSearchTree<Game>&);
  MonteCarloSearchTree<Game>(int, unsigned, unsigned, double);

  /* Copy-assignment */
  // ...

  /* Destructor */
  // ...

private:

  NodePointerType root;
  NodePointerType current_game_node;

  int seed;
  std::default_random_engine rng;

  unsigned outer_iter;
  unsigned inner_iter;

  double ucb_constant;

};

// Definition of methods

template<class Game>
double
MonteCarloSearchTree<Game>::compute_ucb(const NodePointerType& target_node) const
{
  return ( target_node->get_wins() / (double)(target_node->get_visits()) ) +
    ucb_constant * sqrt( log( (double)((target_node->get_parent())->get_visits()) )
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
MonteCarloSearchTree<Game>::select() const {
  NodePointerType selected_node = current_game_node;
  while( selected_node->all_moves_tried() && selected_node->has_children() )
    selected_node = best_child_ucb(selected_node);
  return selected_node;
}

template<class Game>
typename Node<Game>::NodePointerType  /* ??? */
MonteCarloSearchTree<Game>::expand(NodePointerType current_parent) {
  /* ... */
}

template<class Game>
double
MonteCarloSearchTree<Game>::rollout(NodePointerType current_leaf) const {
  /* ... */
}

template<class Game>
void
MonteCarloSearchTree<Game>::back_propagation(NodePointerType current_leaf) {
  /* ... */
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
