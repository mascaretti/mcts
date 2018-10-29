#include <utility>
#include "action.hpp"
#include <iostream>

namespace game {

	namespace Oxo {

		//Constructor taking a pair as an input
		OxoAction::OxoAction(std::pair<int, int> input): row{input.first}, column{input.second} {};

		//Constructor taking two integers as an input
		OxoAction::OxoAction(const int& first, const int& second): row{first}, column{second} {};

		//This is to build a string with the information regarding the action
		std::string OxoAction::to_string() const {
			std::string data;
			data.append("(");
			data.append(std::to_string(row));
			data.append(",");
			data.append(std::to_string(column));
			data.append(")");
			return data;
		};

		//Equality operator, checking if actions have the same properties
		bool OxoAction::operator==(const OxoAction& rhs) const {
			if ((row == rhs.row) and (column == rhs.column))
				return true;
			else
				return false;
		};
	}

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
		bool NimAction::operator==(const NimAction& rhs) const {
			if ((pile == rhs.pile) && (number == rhs.number))
				return true;
			else
				return false;
		};
	}
}
