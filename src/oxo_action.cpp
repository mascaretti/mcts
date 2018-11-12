#include <utility>
#include "oxo_action.hpp"
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
			if ((row == rhs.row) && (column == rhs.column))
				return true;
			else
				return false;
		};

	}

}
