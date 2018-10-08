#include <utility>
#include "action.hpp"
#include <iostream>

namespace game {
namespace Oxo {

	OxoAction::OxoAction(std::pair<int, int> input): row{input.first}, column{input.second} {};

	OxoAction::OxoAction(const int& first, const int& second): row{first}, column{second} {};

	std::string OxoAction::to_string() {
		std::string data;
		data.append("(");
		data.append(std::to_string(row));
		data.append(",");
		data.append(std::to_string(column));
		data.append(")");
		return data;
	};

	bool OxoAction::operator==(const OxoAction& rhs) {
		if ((row == rhs.row) and (column == rhs.column))
			return true;
		else
			return false;
	};

}
}