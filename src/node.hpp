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


/* - - - - - - - - - - - - - - - - - */
// Declaration of the template class //
/* - - - - - - - - - - - - - - - - - */

template<class Game>
class Node {

public:

  typedef typename Game::Action Move;
  typedef typename std::shared_ptr< Node<Game> > NodePointerType;
  typedef typename std::shared_ptr< Move > MovePointerType;

  /* Costructors */

  Node(void);
  Node(const Game&);
  Node(const Node&) = default;

  /* Copy-assigment */

  Node<Game>& operator = (const Node<Game>&) = default;

  /* Methods */

  void add_child(const Node<Game>&);
  void update(double);
  bool all_moves_tried(void) const;
  bool has_children(void) const;
  NodePointerType make_child(const Move&);

  /* Getters */

  double get_wins(void) const { return wins; }
  unsigned get_visits(void) const { return visits; }
  const NodePointerType get_parent(void) const { return parent; }
  NodePointerType get_parent(void) { return parent; }
  std::vector< NodePointerType > get_children(void) const { return children; }
  std::vector< MovePointerType > get_moves(void) const { return possible_moves; }
  Game get_game(void) const { return game_state; }
  int get_player(void) const { return player; }

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
  ~Node() = default;

private:

  Game game_state;

  NodePointerType parent;

  std::vector< NodePointerType > children;
  std::vector< MovePointerType > possible_moves;
  void erase_move(const Move&);

  int player;

  // Move move_to_here;
  // unsigned depth;

  double wins;
  unsigned visits;

};


/* - - - - - - - - - - - - - - - - - */
// Definition of methods             //
/* - - - - - - - - - - - - - - - - - */

template<class Game>
Node<Game>::Node():
  game_state(), parent(nullptr), wins(0.0)
  {
    player = game_state.get_agent_id();
    possible_moves = game_state.get_actions();
    visits = 0;
  }

template<class Game>
Node<Game>::Node(const Game& input_game):
  game_state(input_game), parent(nullptr), wins(0.0)
  {
    player = game_state.get_agent_id();
    possible_moves = game_state.get_actions();
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
Node<Game>::all_moves_tried() const
{
  return possible_moves.empty();
}

template<class Game>
bool
Node<Game>::has_children() const
{
  return !(children.empty());
}

template<class Game>
typename std::shared_ptr<Node<Game>>  /* ??? */
Node<Game>::make_child(const Move& next_move)
{
  Game next_game(game_state);
  next_game.apply_action(next_move);
  children.emplace_back(std::make_shared<Node<Game>>(next_game));
  children[children.size()-1]->parent = this;
  erase_move(next_move);
  return children[children.size()-1];
}

template<class Game>
void
Node<Game>::erase_move(const Move& next_move)
{
  for (auto it = possible_moves.begin(); it!=possible_moves.end(); ++it) {
    if ( **it == next_move)
      possible_moves.erase(it);
  }
}

/* Need definition for constructor, copy assignment and destructor... */


#endif /* NODE_HPP */
