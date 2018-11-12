#ifndef HPP__ACTION__HEADER__HPP
#define HPP__ACTION__HEADER__HPP

#include <utility>
#include <string>
#include <cassert>

//The class Action is an empty class, aimed at maintaing the code tidier.
class Action {};

namespace game {
	namespace Oxo {
	class OxoAction: public Action {
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

	namespace Nim {
		class NimAction: public Action {
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

			//Equality operator, checking if the pile and number of different actions are the same
			bool operator==(const NimAction& rhs) const;
		};
	}
}

#endif
