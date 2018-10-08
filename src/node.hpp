#ifndef NODE_HPP
#define NODE_HPP

#include <vector>
#include <memory>
#include <cmath>


/* - - - - - - - - - - - - - - - - - */
// Declaration of the template class //
/* - - - - - - - - - - - - - - - - - */

template<class Game, class Move>
// template<class Game>
class Node {

public:

  // typedef typename Game::Action Move;
  // typedef typename std::shared_ptr< Node<Game> > NodePointerType;
  typedef typename std::shared_ptr< Node<Game,Move> > NodePointerType;

  /* Costructors */

  Node(void);
  Node(const Game&);
  Node(const Node&) = default;

  /* Copy-assigment */

  // Node<Game>& operator = (const Node<Game>&) = default;
  Node<Game,Move>& operator = (const Node<Game,Move>&) = default;

  /* Methods */

  void update(double);
  void update(double, unsigned);
  bool all_moves_tried(void) const;
  bool has_children(void) const;
  NodePointerType make_child(const Move&);

  /* Getters */

  double get_wins(void) const { return wins; }
  unsigned get_visits(void) const { return visits; }
  const Node* get_parent(void) const { return parent; }
  Node* get_parent(void) { return parent; }
  std::vector< NodePointerType > get_children(void) const { return children; }
  std::vector< Move > get_moves(void) const { return possible_moves; }
  Game get_game(void) const { return game_state; }
  int get_player(void) const { return player; }
  Move get_last_move(void) const { return last_move; }

  /* Destructor */
  ~Node() = default;

private:

  Game game_state;
  Node* parent;
  Move last_move;     // Which method has to be modified? Can it stay undefined?
                      // Should define a null move? Should we specify is it's a starting config?

  std::vector< NodePointerType > children;
  std::vector< Move > possible_moves;
  void erase_move(const Move&);

  int player;

  double wins;
  unsigned visits;

};


/* - - - - - - - - - - - - - - - - - */
// Definition of methods             //
/* - - - - - - - - - - - - - - - - - */

template<class Game, class Move>
Node<Game,Move>::Node():
  game_state(), parent(nullptr), wins(0.0)
  {
    player = game_state.get_agent_id();
    possible_moves = game_state.get_actions();
    visits = 0;
  }

template<class Game, class Move>
Node<Game,Move>::Node(const Game& input_game):
  game_state(input_game), parent(nullptr), wins(0.0)
  {
    player = game_state.get_agent_id();
    possible_moves = game_state.get_actions();
    visits = 0;
  }

template<class Game, class Move>
void
Node<Game,Move>::update(double result)
{
  wins += result;
  visits += 1;
}

template<class Game, class Move>
void
Node<Game,Move>::update(double result, unsigned new_visits)
{
  wins += result;
  visits += new_visits;
}

template<class Game, class Move>
bool
Node<Game,Move>::all_moves_tried() const
{
  return possible_moves.empty();
}

template<class Game, class Move>
bool
Node<Game,Move>::has_children() const
{
  return !(children.empty());
}

template<class Game, class Move>
typename std::shared_ptr<Node<Game,Move>>  /* ??? */
Node<Game,Move>::make_child(const Move& next_move)
{
  Game next_game(game_state);
  next_game.apply_action(next_move);
  children.emplace_back(std::make_shared<Node<Game,Move>>(next_game));
  children[children.size()-1]->parent = this;
  erase_move(next_move);
  children[children.size()-1]->last_move = next_move;
  return children[children.size()-1];
}

template<class Game, class Move>
void
Node<Game,Move>::erase_move(const Move& next_move)
{
  for (auto it = possible_moves.begin(); it!=possible_moves.end(); ++it) {
    if ( *it == next_move)
      possible_moves.erase(it);
  }
}


#endif /* NODE_HPP */
