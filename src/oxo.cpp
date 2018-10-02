#include "oxo.hpp"

namespace game {

//Constructors for action
Action::Action(std::pair<int, int> input): row{input.first}, column{input.second} {
	assert(row >= 0 and row <= 2);
	assert(column >= 0 and column <= 2);
}

Action::Action(const int& first, const int& second): row{first}, column{second} {
		assert(row >= 0 and row <= 2);
		assert(column >= 0 and column <= 2);
}

//for debugging
std::string Action::to_string() {
	std::string data;
	data.append("(");
	data.append(std::to_string(row));
	data.append(",");
	data.append(std::to_string(column));
	data.append(")");
	return data;
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

Oxo::Oxo(int seed): random_action_seed{seed}, gen{seed} {
	board[0]= {{0, 0, 0}};
	board[1]= {{0, 0, 0}};
	board[2]= {{0, 0, 0}};
};



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

Action Oxo::random_action() const
{
	if (is_terminal == true)
		throw NoRandomActions{};

	//std::random_device rd;
	//std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with random_action_seed
	int max_actions= get_actions().size() - 1;
    std::uniform_int_distribution<int> dis(0, max_actions);

    int random_index= dis(gen);

    return get_actions()[random_index];
}

int Oxo::evaluate()
{
	if ((is_terminal == false) and (winner_exists == false))
		throw GameNotOver{};
	if ((is_terminal == true) and (winner_exists == false))
		return 0;
	else
		return (agent_id == 1) ? -1 : 1;
}

void Oxo::set_seed(int new_seed)
{
	random_action_seed= new_seed;
	gen.seed(random_action_seed);
}

Action Oxo::get_last_action() const {
	return last_action;
}

//for debugging
void Oxo::print() {
		std::cout << "**************************************" << '\n';
		std::cout << "Now playing: " << 3 - get_agent_id() << '\n';
		std::cout << "It plays: " << get_last_action().to_string() << '\n';
		std::cout << "Game finished? " << get_terminal_status() << '\n';
		if (get_terminal_status() == true)
			std::cout << "Outcome: " << evaluate() << '\n';
		else
			std::cout << "Now up to: " << get_agent_id() << '\n';
		std::cout << "**************************************" << '\n';
	}


char Oxo::print_helper(int value)
	{
		char tmp;
		if (value == 0)
			tmp= ' ';

		if (value == 1)
			tmp= 'x';

		if (value == -1)
			tmp= 'o';
	return tmp;
	}

void Oxo::print_board() {
	std::cout << print_helper(board[0][0]) << " | " << print_helper(board[0][1]) << " | " << print_helper(board[0][2]) << '\n';
	std::cout << "---------" << '\n';
	std::cout << print_helper(board[1][0]) << " | " << print_helper(board[1][1]) << " | " << print_helper(board[1][2]) << '\n';
	std::cout << "---------" << '\n';
	std::cout << print_helper(board[2][0]) << " | " << print_helper(board[2][1]) << " | " << print_helper(board[2][2]) << '\n';
}

}