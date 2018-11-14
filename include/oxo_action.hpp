#ifndef HPP__OXO__ACTION__HEADER
#define HPP__OXO__ACTION__HEADER

#include <utility>
#include <string>
#include <cassert>

namespace game {
	namespace Oxo {
  	class OxoAction {
		private:
  			/*The class OxoAction implements the action that are possible on the Oxo board and
  			* defines the constructors*/

  			//Data members of the action: row and column of the action
  			int row{0};
  			int column{0};

		public:
  			//Defining the constructors, accepting both a pair and two integers as input
  			OxoAction()= default;
  			OxoAction(std::pair<int, int> input);
  			OxoAction(const int& first, const int& second);

				//getters
				int get_row() const;
				int get_column() const;

  			//for debugging
  			std::string to_string() const;

				friend bool operator==(const OxoAction& lhs, const OxoAction& rhs);
  		}; //class OxoAction

		//Equality operator defined as a free function, comparing if rows and columns match between different actions
		bool operator==(const OxoAction& lhs, const OxoAction& rhs);
	} //namespace Oxo
} //namespace game

#endif
