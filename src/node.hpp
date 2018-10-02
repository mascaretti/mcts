#ifndef NODE_HPP
#define NODE_HPP

/* ??? */
// Vittorie da qui in poi?
// UCT: deve essere per forza immagazzinato
// Nodo foglia?
/* ??? */

#include <vector>
#include <memory>
#include <cmath>

// Declaration of the template class

template<class Game>
class Node {

public:

  typedef typename Game::Action Move;
  typedef typename std::shared_ptr< Node<Game> > NodePointerType;
  typedef typename std::shared_ptr< Move > MovePointerType;

  /* Costructors */

  Node<Game>();
  Node<Game>(const Game&);
  Node<Game>(const Node<Game>&) = default;

  /* Copy-assigment */

  Node<Game>& operator = (const Node<Game>&) = default;

  /* Methods */

  void add_child(const Node<Game>&);
  void update(double);
  bool all_moves_tried(void);
  bool has_children(void);

  /* Getters */

  double get_wins(void) const { return wins; }
  unsigned get_visits(void) const { return visits; }
  const NodePointerType get_parent(void) const { return parent; }
  NodePointerType get_parent(void) { return parent; }
  std::vector< NodePointerType > get_children(void) const { return children; }

  /*
  FOLLIA
  typedef typename std::vector< NodePointerType >::iterator NodeIterator;
  typedef typename std::vector< MovePointerType >::iterator MoveIterator;
  typedef typename std::vector< NodePointerType >::const_iterator ConstNodeIterator;
  typedef typename std::vector< MovePointerType >::const_iterator ConstMoveIterator;
  NodeIterator get_first_child(void) { return children.begin(); }
  ConstNodeIterator get_first_child(void) const { return children.cbegin(); }
  NodeIterator get_last_child(void) { return children.end(); }
  ConstNodeIterator get_last_child(void) const { return children.cend(); }
  */

  /* Destructor */
  ~Node<Game>() = default;

private:

  Game game_state;

  NodePointerType parent;

  std::vector< NodePointerType > children;
  std::vector< MovePointerType > possible_moves;

  int player;

  // Move move_to_here;
  // unsigned depth;

  double wins;
  unsigned visits;

};

// Definition of methods

template<class Game>
Node<Game>():
  game_state(), parent(nullptr), wins(0.0)
  {
    player = game_state.get_agent_id();
    visits = 0;
  }

template<class Game>
Node<Game>(const Game& input_game):
  game_state(input_game), parent(nullptr), wins(0.0)
  {
    player = game_state.get_agent_id();
    visits = 0;
  }

template<class Game>
void
Node<Game>::add_child(const Node<Game>& child_play)
{
  children.emplace_back(std::make_shared<Node<Game>>(child_play));
}

template<class Game>
void
Node<Game>::update(double result)
{
  wins += result;
  visits += 1;
}

template<class Game>
bool
Node<Game>::all_moves_tried()
{
  return possible_moves.empty();
}

template<class Game>
bool
Node<Game>::has_children()
{
  return !(children.empty());
}

/* Need definition for constructor, copy assignment and destructor... */


#endif /* NODE_HPP */
