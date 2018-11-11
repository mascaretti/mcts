#ifndef HPP__OXO__ACTION__HEADER
#define HPP__OXO__ACTION__HEADER

#include <utility>
#include <string>
#include <cassert>

namespace game {
	namespace Oxo {
  	class OxoAction {
  		public:
  			/*The class OxoAction implements the action that are possible on the Oxo board and
  			* defines the constructors*/

  			//Data members of the action: row and column of the action
  			int row{0};
  			int column{0};


  			//Defining the constructors, accepting both a pair and two integers as input
  			OxoAction()= default;
  			OxoAction(std::pair<int, int> input);
  			OxoAction(const int& first, const int& second);

  			//for debugging
  			std::string to_string() const;

  			//Equality operator defined comparing if rows and columns match between different actions
  			bool operator==(const OxoAction& rhs) const;
  		};
	}
}

#endif
