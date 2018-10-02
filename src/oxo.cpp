#include "oxo.hpp"

namespace game {

Oxo::Oxo(): is_terminal{false}, agent_id{1} {
	board[0]= {{0, 0, 0}};
	board[1]= {{0, 0, 0}};
	board[2]= {{0, 0, 0}};
}

int Oxo::get_agent_id() {
	return agent_id;
}

bool Oxo::get_terminal_status() {
	return is_terminal;
}

std::string Oxo::to_string() {
	std::string status;
	/*status= " board[0][0] '\t' | board[0][1] '\t' | board[0][2] '\n'
			  ---------------------- '\n'
			  board[1][0] '\t' | board[1][1] '\t' | board[1][2] '\n'
			  ---------------------- '\n'
			  board[2][0] '\t' | board[2][1] '\t' | board[2][2] '\n' "*/;

	return status;
}

const std::array<int, 3>& Oxo::get_row(unsigned int i) const
{
	return board[i];
}

}