#ifndef HPP__NIM__ACTION__HEADER__HPP
#define HPP__NIM__ACTION__HEADER__HPP

#include <utility>
#include <string>
#include <cassert>

namespace game {
  namespace Nim {
    class NimAction {
      //The class NimAction implements the actions possible during a Nim play
    public:

      //Data member: the pile at which we subtract the cards and their number
      unsigned int pile{0u};
      unsigned int number{1u};

      //Constructors. Both a pair and two integers are accepted
      NimAction()= default;
      NimAction(std::pair<unsigned int, unsigned int> input);
      NimAction(const unsigned int& where, const unsigned int& what);

      //for debugging
      std::string to_string() const;

      }; //class NimAction

    //Equality operator, checking if the pile and number of different actions are the same
    bool operator==(const NimAction& lhs, const NimAction& rhs);
    
    } //namespace Nim
  } //namespace game

#endif
