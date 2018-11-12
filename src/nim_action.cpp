#include <utility>
#include "nim_action.hpp"
#include <iostream>

namespace game {
<<<<<<< HEAD

=======
>>>>>>> master
  namespace Nim {

    //Constructor taking a pair as an input. Optionally asserting if the number of cards taken is positive
    NimAction::NimAction(std::pair<unsigned int, unsigned int> input): pile{input.first}, number{input.second} {
      assert(number > 0);
    };

    //Constructor taking two integers as input. Optionally asserting if the number of cards taken is positive
    NimAction::NimAction(const unsigned int& where, const unsigned int& what): pile{where}, number{what} {
      assert(number > 0);
    };

    //Method to return a string with the properties of the action
    std::string NimAction::to_string() const {
      std::string data;
      data.append("(");
      data.append(std::to_string(pile));
      data.append(",");
      data.append(std::to_string(number));
      data.append(")");
      return data;
    };

    //Equality operator, checking if two actions share the same properties
<<<<<<< HEAD
    bool operator==(const NimAction& lhs, const NimAction& rhs) {
      if ((lhs.pile == rhs.pile) && (lhs.number == rhs.number))
=======
    bool NimAction::operator==(const NimAction& rhs) const {
      if ((pile == rhs.pile) && (number == rhs.number))
>>>>>>> master
        return true;
      else
        return false;
    };
<<<<<<< HEAD
  } //namespace Nim
} //namespace game
=======
  }
  }


}
>>>>>>> master
