#ifndef HPP__OXO__HEADER__HPP
#define HPP__OXO__HEADER__HPP

#include "action.hpp"
#include "game.hpp"
#include <iostream>

namespace game {
	namespace Oxo {

	class OxoGame: public Game<OxoAction> {

		using Action= OxoAction;
			//The class Oxo implements a 3x3 tic-tac-toe board

		private:
			//to check if the game is over
			bool is_terminal{false};

			//to check if the game has begun
			bool no_move_played{true};

			//to know who will play *next*
			int agent_id{1};

			//the board
			std::array<std::array<int, 3>, 3> board;

			//the last action played
			Action last_action;

			//to know if we have a winner already
			bool winner_exists{false};

			//seed for the random action generation
			int random_action_seed{0};

			//random engine
			std::default_random_engine gen;

			//method to updated the terminal status given the status of the board
			void update_terminal_status()  {
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
			};


			//method to help printing the status of the board
			char print_helper(int value) {
				char tmp;
				if (value == 0)
					tmp= ' ';

				if (value == 1)
					tmp= 'x';

				if (value == -1)
					tmp= 'o';
			return tmp;
			};

		public:

			//default constructor
			OxoGame() {
				board[0]= {{0, 0, 0}};
				board[1]= {{0, 0, 0}};
				board[2]= {{0, 0, 0}};
				};

			//constructor taking a seed
			OxoGame(int seed): random_action_seed{seed}, gen(seed)  {
				board[0]= {{0, 0, 0}};
				board[1]= {{0, 0, 0}};
				board[2]= {{0, 0, 0}};
			}

			//copy constructor
			OxoGame(const OxoGame& other)= default;

			//assignment operator
			OxoGame& operator=(const OxoGame& other)= default;

			//method to know whether the game is over
			virtual bool get_terminal_status() override {
				return is_terminal;
			}; //true if current state is terminal

			//method to know what agen will play *next*
			virtual int get_agent_id() override {
				return agent_id;
			}; //returns the agent who is about to make a decision

			//Method to apply an action and update the status of the board
			virtual void apply_action(const Action& action) override {
				//Checking if the action has not been played before
				if (board[action.row][action.column] != 0)
					throw ActionAlreadyPlayed{};


				//update the action played status
				if (no_move_played == true)
					no_move_played= false;


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

			}; //returns void, but changes the internal state

			//Method to know what legal actions it is possible to play
			virtual std::vector<Action> get_actions() const override {

				//check if actions available
				/*
				if (is_terminal == true)
					throw NoActionsLeft{};
				*/

				std::vector<Action> action_vector;
				for (int i= 0; i != 3; ++i) {
					for (int j= 0; j != 3; ++j) {
						if (board[i][j] == 0)
							action_vector.emplace_back(std::make_pair(i, j));
					}
				}

				return action_vector;
			}; //returns a vector of legal action at current state

			//Method returning a legal action
			virtual Action random_action() override {
				if (is_terminal == true)
					throw NoRandomActions{};

				//std::random_device rd;
				//std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with random_action_seed
				int max_actions= get_actions().size() - 1;
		    	std::uniform_int_distribution<int> dis(0, max_actions);

		    	int random_index= dis(gen);

		   		return get_actions()[random_index];
			}; //returns a random action legal at the current state

			//Method returning the utility if the status of the game is a terminal node
			virtual int evaluate() override {
				if ((is_terminal == false) and (winner_exists == false))
					throw GameNotOver{};
				if ((is_terminal == true) and (winner_exists == false))
					return 0;
				else
					return (agent_id == 1) ? -1 : 1;
			}; //returns the utility as an integer if state is terminal, otherwise throws error

			//Method to print the board of the game
			void print_board() {
				std::cout << print_helper(board[0][0]) << " | " << print_helper(board[0][1]) << " | " << print_helper(board[0][2]) << '\n';
				std::cout << "---------" << '\n';
				std::cout << print_helper(board[1][0]) << " | " << print_helper(board[1][1]) << " | " << print_helper(board[1][2]) << '\n';
				std::cout << "---------" << '\n';
				std::cout << print_helper(board[2][0]) << " | " << print_helper(board[2][1]) << " | " << print_helper(board[2][2]) << '\n';
			};

			//Method to set a new seed
			virtual void set_seed(int new_seed) override {
				random_action_seed= new_seed;
				gen.seed(random_action_seed);
			}; //sets new seed


			//Method to print the status of the current move of the game and the situation
			void print() {
				std::cout << "**************************************" << '\n';
				std::cout << "Now playing: " << 3 - get_agent_id() << '\n';

				if (no_move_played == true)
					std::cout << "First move.";
				else
					std::cout << "It plays: " << get_last_action().to_string() << '\n';

				std::cout << "Game finished? " << get_terminal_status() << '\n';
				if (get_terminal_status() == true)
					std::cout << "Outcome: " << evaluate() << '\n';
				else
					std::cout << "Now up to: " << get_agent_id() << '\n';
				std::cout << "**************************************" << '\n';
			}; //for debugging


			//Method to get the last action played
			Action get_last_action() const {
				if (no_move_played == true)
					throw NoActionPlayed{};
				return last_action;
			};

			//Method to allow input from a human player
			virtual int human_input() override {
				auto actions = get_actions();
				bool correct_move{false};

				int input;
				auto num_moves{actions.size()};

				while (correct_move == false) {
				std::cout << "Human: this are the moves left for you." << std::endl;
				int j{1};
				for (auto i = std::begin(actions); i != std::end(actions); ++i) {
					std::cout << "Move " << j << ": " << (*i).to_string() << std::endl;
					j+= 1;
				}
				std::cout << "Pick a move! Select the number next to it.: ";

				std::cin >> input;

				if (input >= 1 and input <= num_moves)
					correct_move= true;

				else
					std::cout << "Move not allowed, human. Try again.";
				}

				return (input - 1);
			}
	};

	}
}

#endif
