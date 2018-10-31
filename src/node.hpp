#ifndef NODE_HPP
#define NODE_HPP

#include <vector>
#include <memory>
#include <iostream>

/* - - - - - - - - - - - - - - - - - */
// Declaration of the template class //
/* - - - - - - - - - - - - - - - - - */

template<class Game, class Move>
class Node {

public:

  // Shared pointer to Node class type
  typedef typename std::shared_ptr< Node<Game,Move> > NodePointerType;

  /* Costructors */
  Node(void);
  Node(const Game&);

  // For the moment copy assignment and costructors are prevented
  // In future deep-copy costructor and assignment may be implemented
  Node<Game,Move>& operator = (const Node<Game,Move>&) = delete;
  Node(const Node&) = delete;

  /* List of public methods */

  //  Update the score and the visits of the node
  void update(double);
  void update(double, unsigned);

  // True if all moves have been tried (vector of moves is empty)
  bool all_moves_tried(void) const;

  // True if the node has children (vector of children not null)
  bool has_children(void) const;

  // Adds a new child, built applying the input move to the current game state
  NodePointerType make_child(const Move&);

  // Print info (DEBUG)
  void print_node(void) const;

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

  // Deletes the input move from the vector of available moves
  void erase_move(const Move&);

  /* List of private members */

  Game game_state;

  // Here a reference may have been used instead; however, we need to initialize
  // the parent of the root node to a "null" state (nullptr in this case),
  // which is not possible with a reference
  Node* parent;

  Move last_move;
  std::vector< NodePointerType > children;
  std::vector< Move > possible_moves;
  int player;
  double wins;
  unsigned visits;

};

/* - - - - - - - - - - - - - - - - - */
// Definition of methods             //
/* - - - - - - - - - - - - - - - - - */

// *************
// Constructors:
// *************

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

// ********
// Methods:
// ********

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
    if ( (*it) == next_move) {
      possible_moves.erase(it);
      return;
    }
  }
}

template<class Game, class Move>
void
Node<Game,Move>::print_node() const {
  std::cout << "Player: " << player <<
    " | W/V: " << wins << "/" << visits <<
    " #child: " << children.size() <<
    " #moves: " << possible_moves.size()
  << std::endl;
}

#endif /* NODE_HPP */
