#ifndef HPP__NIM__HEADER__HPP
#define HPP__NIM__HEADER__HPP

#include "action.hpp"
#include "game.hpp"
#include <iostream>
#include <array>
#include <vector>


namespace game {
	namespace Nim {

		template <unsigned int N1= 7u, unsigned N2= 7u, unsigned N3= 7u>
		class NimGame: public Game<NimAction> {
			using Action= NimAction;
			using NimBoard= std::array<unsigned int, 3>;

		private:
			bool is_terminal{false};
			bool no_move_played{true};
			int agent_id{1};
			Action last_action;

			int random_action_seed{0};
			std::default_random_engine gen;

			NimBoard board{{N1, N2, N3}};

			void update_terminal_status()  {
				if ((board[0] == 0u) and (board[1] == 0u) and (board[2] == 0u))
					is_terminal= true;
				else
					is_terminal= false;
			};

		public:
			NimGame()= default;

			NimGame(int seed): random_action_seed{seed} {};

			NimGame(const NimGame& Other)= default;

			NimGame& operator=(const NimGame& Other)= default;

			virtual bool get_terminal_status() override {
				return is_terminal;
			}; //true if current state is terminal

			virtual int get_agent_id() override {
				return agent_id;
			}; //returns the agent who is about to make a decision

			virtual void apply_action(const Action& action) override {
				//Check if the action to be played is legal
				if (board[action.pile] < action.number)
					throw IllegalAction{};

				//update the action played status
				if (no_move_played == true)
					no_move_played= false;

				//Update the status of the board
				board[action.pile]-= action.number;

				//control if terminal
				update_terminal_status();

				//Changing the agent_id
				agent_id= ((agent_id == 1) ? 2 : 1);

				//setting the action the brought here
				last_action= action;
			};

			virtual std::vector<Action> get_actions() const override {

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

			virtual Action random_action() override {
				if (is_terminal == true)
					throw NoRandomActions{};

				int max_actions= get_actions().size() - 1;
		    	std::uniform_int_distribution<int> dis(0, max_actions);

		    	int random_index= dis(gen);

		   		return get_actions()[random_index];
			}; //returns a random action legal at the current state

			virtual int evaluate() override {
				if (is_terminal == false)
					throw GameNotOver{};
				else
					return (agent_id == 1) ? -1 : 1;
			}; //returns the utility as an integer if state is terminal, otherwise throws error

			virtual void set_seed(int new_seed) override {
				random_action_seed= new_seed;
				gen.seed(random_action_seed);
			}; //sets new seed


			void print() {
				std::cout << "**************************************" << '\n';
				std::cout << "Now playing: " << 3 - get_agent_id() << '\n';

				if (no_move_played == false)
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


			Action get_last_action() const {
				if (no_move_played == true)
					throw NoActionPlayed{};
				return last_action;
			};


			void print_board() {
				std::cout << "Position 0: " << " " << board[0] << '\n';
				std::cout << "Poistion 1: " << " " << board[1] << '\n';
				std::cout << "Poistion 2: " << " " << board[2] << '\n';
			}; //print the value as string

			virtual int human_input() override {
				auto actions = get_actions();
				bool correct_move{false};

				int input;
				
				while (correct_move == false) {
				std::cout << "Human: this are the moves left for you." << std::endl;
				int j{1};
				for (auto i = std::begin(actions); i != std::end(actions); ++i) {
					std::cout << "Move " << j << ": " << (*i).to_string() << std::endl;
					j+= 1;
				}
				std::cout << "Pick a move! Select the number next to it.: ";
				
				std::cin >> input;

				if (input >= 1 and input <= actions.size())
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
