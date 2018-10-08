#include <utility>
#include "action.hpp"
#include <iostream>

Action::Action(std::pair<int, int> input): row{input.first}, column{input.second} {};

Action::Action(const int& first, const int& second): row{first}, column{second} {};

std::string to_string() {
	std::cout << "(" << row << "," << column << ")" << std::endl;
};

bool operator==(const Action& rhs) override {
	if ((row == rhs.row) and (column == rhs.column))
		return true;
	else
		return false;
};