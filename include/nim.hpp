#ifndef HPP__NIM__HEADER__HPP
#define HPP__NIM__HEADER__HPP

#include "game.hpp"
#include "nim_action.hpp"
#include <iostream>
#include <array>
#include <vector>
#include <limits>


namespace game {
	namespace Nim {

		template <unsigned int N1= 7u, unsigned N2= 7u, unsigned N3= 7u, class Action= NimAction>
		class NimGame {
		//The NimGame class is a template class, creating a 3-pile Nim game
		//in which the number of cards per pile is given by the template parameters.

		using NimBoard= std::array<unsigned int, 3>;

		private:
			//to check if the game is over
			bool is_terminal{false};

			//to check if the game has begun
			bool no_move_played{true};

			//Agent that moves next
			int agent_id{1};

			//Last action played
			Action last_action;

			//Seed for the random action method
			int random_action_seed{0};

			//Random engine for the random action method
			std::default_random_engine gen;

			//Board of the game
			NimBoard board{{N1, N2, N3}};

		public:

			//Default constructor
			NimGame()= default;

			//Constructor taking a seed
			NimGame(int seed): random_action_seed{seed} {};

			//Copy constructor
			NimGame(const NimGame& Other)= default;

			//Assignment operator
			NimGame& operator=(const NimGame& Other)= default;

			//Method returning if the game is over
			bool get_terminal_status() {
				return is_terminal;
			}; //true if current state is terminal

			//Method returning the player moving *next*
			int get_agent_id() {
				return agent_id;
			}; //returns the agent who is about to make a decision


			//Method taking an action as an input and updating the status of the board
			void apply_action(const Action& action) {
				//Check if the action to be played is legal
				assert(action.get_number() <= std::numeric_limits<int>::max());
				if (board[action.get_pile()] < (unsigned int) action.get_number())
					throw IllegalAction{};

				//update the action played status
				if (no_move_played)
					no_move_played= false;

				//Update the status of the board
				board[action.get_pile()]-= action.get_number();

				//control if terminal and update
				((board[0] == 0u) && (board[1] == 0u) && (board[2] == 0u)) ? is_terminal= true : is_terminal= false;

				//Changing the agent_id
				agent_id= ((agent_id == 1) ? 2 : 1);

				//setting the action the brought here
				last_action= action;
			};

			//Method returning the set of viable action given the current board status
			std::vector<Action> get_actions() const {

				//check if actions available
				/*
				if (is_terminal == true)
					throw NoActionsLeft{};
				*/

				std::vector<Action> action_vector;
				for (unsigned int position{0u}; position != 3; ++position) {
					if (board[position] > 0) {
						for (unsigned int left{1u}; left < board[position] + 1; ++left)
							action_vector.emplace_back(std::make_pair(position, left));
					}
				}

				return action_vector;
			}

			//Method returning a legal random action
			Action random_action() {
				if (is_terminal)
					throw NoRandomActions{};

				int max_actions= get_actions().size() - 1;
		    	std::uniform_int_distribution<int> dis(0, max_actions);

		    	int random_index= dis(gen);

		   		return get_actions()[random_index];
			}; //returns a random action legal at the current state

			//Method returning the utility at the terminal node if we are at a terminal node
			int evaluate() {
				if (!is_terminal)
					throw GameNotOver{};
				else
					return (agent_id == 1) ? -1 : 1;
			}; //returns the utility as an integer if state is terminal, otherwise throws error

			//Method to set a new seed for random actions
			void set_seed(int new_seed) {
				random_action_seed= new_seed;
				gen.seed(random_action_seed);
			}; //sets new seed


			//Method to print the status of the game at this action
			void print() {
				std::cout << "**************************************" << '\n';
				std::cout << "Now playing: " << 3 - get_agent_id() << '\n';

				if (no_move_played)
					std::cout << "First move.";
				else
					std::cout << "It plays: " << get_last_action().to_string() << '\n';

				std::cout << "Game finished? " << get_terminal_status() << '\n';
				if (get_terminal_status())
					std::cout << "Outcome: " << evaluate() << '\n';
				else
					std::cout << "Now up to: " << get_agent_id() << '\n';
				std::cout << "**************************************" << '\n';
			};


			//Method to get the last action played
			Action get_last_action() const {
				if (no_move_played)
					throw NoActionPlayed{};
				return last_action;
			};


			//Method to print the status of the board
			void print_board() {
				std::cout << "Position 0: " << " " << board[0] << '\n';
				std::cout << "Poistion 1: " << " " << board[1] << '\n';
				std::cout << "Poistion 2: " << " " << board[2] << '\n';
			}; //print the value as string

			//Method to input a move from a human player
			int human_input() {
				auto actions = get_actions();
				bool correct_move{false};

				unsigned input;

				while (!correct_move) {
				std::cout << "Human: this are the moves left for you." << std::endl;
				int j{1};
				for (auto i = std::begin(actions); i != std::end(actions); ++i) {
					std::cout << "Move " << j << ": " << (*i).to_string() << std::endl;
					j+= 1;
				}
				std::cout << "Pick a move! Select the number next to it.: ";

				std::cin >> input;

				if ((input >= 1) && (input <= actions.size()))
					correct_move= true;
				else
					std::cout << "Move not allowed, human. Try again.";
				}

				//casting to int for later MPI usage
				assert(input <= (unsigned int) std::numeric_limits<int>::max());
				return (int)(input - 1);
			}
		};
	}
}

#endif
