#include "node.hpp"
#include "oxo.hpp"

#include <string>
#include <iostream>

int main() {

// Change the blow calls to include the 2 template arg.

using Oxo = game::Oxo::OxoGame<>;
using Move = game::Oxo::OxoAction;
using NodePointerType = Node<Oxo, Move>::NodePointerType;


Node<Oxo, Move> myNode;

myNode.update(1.5);
myNode.update(4.5, 3);
std::string str = "";
myNode.all_moves_tried() ? str = "Yes" : str = "No";
std::cout << "All moves tried? " << str << std::endl;
myNode.has_children() ? str = "Yes" : str = "No";
std::cout << "Has children? " << str << std::endl;
Oxo myGame = myNode.get_game();

myNode.print_node();

Move myMove = myGame.random_action();   // Why is it always the very same action ?????
std::cout << "Selected move: " << myMove.to_string() << std::endl;
myNode.make_child(myMove);
myNode.has_children() ? str = "Yes" : str = "No";
std::cout << "Move made; has children? " << str << std::endl;

double wins = myNode.get_wins();
unsigned visits = myNode.get_visits();
std::vector< NodePointerType > children_ptrs = myNode.get_children();
Node<Oxo, Move>* parent_ptr = children_ptrs[0]->get_parent();
std::vector< Move > left_moves = children_ptrs[0]->get_moves();
std::cout << "Parent has " << wins << " score" << std::endl;
std::cout << "Parent has been visited " << visits << " times" << std::endl;

myNode.print_node();
children_ptrs[0]->print_node();

return 0;

}
