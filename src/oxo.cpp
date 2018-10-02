#include "oxo.hpp"

namespace game {

//Constructors for action
Action::Action(std::pair<int, int> input): row{input.first}, column{input.second} {
	assert(row >= 0 and row <= 2);
	assert(column >= 0 and column <= 2);
}

Action::Action(int first, int second): row{first}, column{second} {
		assert(row >= 0 and row <= 2);
		assert(column >= 0 and column <= 2);
}


//Equality operator between actions
bool operator==(const Action& lhs, const Action& rhs)
	{
		if ((lhs.row == rhs.row) and (lhs.column == rhs.column))
			return true;
		else
			return false;
	}


//Constructors of Oxo
Oxo::Oxo() {
	board[0]= {{0, 0, 0}};
	board[1]= {{0, 0, 0}};
	board[2]= {{0, 0, 0}};
}

Oxo::Oxo(int seed): random_action_seed{seed} {};



void Oxo::update_terminal_status() {
	//check if there is a winner
	//row-wise
	for (int j= 0; j != 3; ++j)
	{
		int row_sum= board[0][j] + board[1][j] + board[2][j];
		if (row_sum == 3 or row_sum == -3)
		{
			winner_exists= true;
			is_terminal= true;
		}
	}

	//column-wise
	for (int i= 0; i != 3; ++i)
	{
		int col_sum= board[i][0] + board[i][1] + board[i][2];
		if (col_sum == 3 or col_sum == -3)
		{
			winner_exists= true;
			is_terminal= true;
		}
	}

	//diagonals
	int first_diag_sum= board[0][0] + board[1][1] + board[2][2];
	if (first_diag_sum == 3 or first_diag_sum == -3)
		{
			winner_exists= true;
			is_terminal= true;
		}


	int second_diag_sum= board[0][2] + board[1][1] + board[2][0];
	if (second_diag_sum == 3 or second_diag_sum == -3)
		{
			winner_exists= true;
			is_terminal= true;
		}


	//checking if there are any actions left
	bool zeros_left{false};
	for (int i= 0; i != 3; ++i) {
		for (int j= 0; j != 3; ++j)
		{
			if (board[i][j] == 0)
				zeros_left= true;
		}
	}

	if (zeros_left == false)
		is_terminal= true;
}

int Oxo::get_agent_id() {
	return agent_id;
}

bool Oxo::get_terminal_status() {
	return is_terminal;
}

std::array<int, 3>& Oxo::get_row(unsigned int i) {
	return board[i];
}

void Oxo::apply_action(const Action& action) {
	//Checking if the action has not been played before
	if (board[action.row][action.column] != 0)
		throw ActionAlreadyPlayed{};


	//Action has effect depending on whose turn it is
	if (agent_id == 1)
		board[action.row][action.column]= 1;

	else
		board[action.row][action.column]= -1;

	//control if terminal
	update_terminal_status();


	//Changing the agent_id
	agent_id= ((agent_id == 1) ? 2 : 1);

	//setting the action the brought here
	last_action= action;

}

std::vector<Action> Oxo::get_actions() const {

	//check if actions available
	if (is_terminal == true)
		throw NoActionsLeft{};

	std::vector<Action> action_vector;
	for (int i= 0; i != 3; ++i) {
		for (int j= 0; j != 3; ++j) {
			if (board[i][j] == 0)
				action_vector.emplace_back(std::make_pair(i, j));
		}
	}

	return action_vector;
}

}