#include <utility>
#include "action.hpp"
#include <iostream>

namespace game {

	namespace Oxo {

		OxoAction::OxoAction(std::pair<int, int> input): row{input.first}, column{input.second} {};

		OxoAction::OxoAction(const int& first, const int& second): row{first}, column{second} {};

		std::string OxoAction::to_string() const {
			std::string data;
			data.append("(");
			data.append(std::to_string(row));
			data.append(",");
			data.append(std::to_string(column));
			data.append(")");
			return data;
		};

		bool OxoAction::operator==(const OxoAction& rhs) const {
			if ((row == rhs.row) and (column == rhs.column))
				return true;
			else
				return false;
		};
	}

	namespace Nim {

		NimAction::NimAction(std::pair<unsigned int, unsigned int> input): pile{input.first}, number{input.second} {
			assert(number > 0);
		};

		NimAction::NimAction(const unsigned int& where, const unsigned int& what): pile{where}, number{what} {
			assert(number > 0);
		};

		std::string NimAction::to_string() const {
			std::string data;
			data.append("(");
			data.append(std::to_string(pile));
			data.append(",");
			data.append(std::to_string(number));
			data.append(")");
			return data;
		};

		bool NimAction::operator==(const NimAction& rhs) const {
			if ((pile == rhs.pile) and (number == rhs.number))
				return true;
			else
				return false;
		};
	}
}