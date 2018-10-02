#ifndef MONTE_CARLO_SEARCH_TREE
#define MONTE_CARLO_SEARCH_TREE

#include "node.hpp"
#include <random>
// #include <mpi.h>

// Declaration of the template class

template<class Game>
class MonteCarloSearchTree {

public:

  typedef typename Node<Game>::NodePointerType NodePointerType;

  /* Methods */

  double compute_ucb(const NodePointerType&) const;
  NodePointerType best_child_ucb(const NodePointerType&) const;
  Game uct_search();
  // SELECT
  // EXPAND
  // ROLLOUT
  // BACKPROPAGATION

  /* Setters */

  void set_outer_iter(unsigned it) { outer_iter = it; }
  void set_inner_iter(unsigned it) { inner_iter = it; }
  void set_ucb_constant(double c) { ucb_constant = c; }

private:

  Node<Game> root;
  Node<Game>* current_game_state;

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
Game
MonteCarloSearchTree<Game>::uct_search()
{
  /* ... */
}

// To be continued ...

#endif /* MONTE_CARLO_SEARCH_TREE */
