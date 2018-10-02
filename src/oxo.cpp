#include "oxo.hpp"

namespace game {

Oxo::Oxo(): is_terminal{false}, agent_id{1} {
	for (auto i= std::begin(board); i != std::end(board); ++i)
	{
		for (auto j= std::begin(*i); j != std::end(*i); ++j)
			*j= 0;
	}
}

int Oxo::get_agent_id() {
	return agent_id;
}

bool Oxo::get_terminal_status() {
	return is_terminal;
}

}