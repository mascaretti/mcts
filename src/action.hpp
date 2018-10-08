#ifndef HPP__ACTION__HEADER__HPP
#define HPP__ACTION__HEADER__HPP

#include <utility>
#include <string>

class Action {
public:
	bool operator==(const Action& rhs);
};

namespace game {
namespace Oxo {
class OxoAction: public Action {
	public:
		/*The class OxoAction implements the action possible on the Oxo board and
		* defines the constructors*/

		int row{0};
		int column{0};

		OxoAction()= default;
		OxoAction(std::pair<int, int> input);
		OxoAction(const int& first, const int& second);

		//for debugging
		std::string to_string();

		bool operator==(const OxoAction& rhs);
	};

}
}

#endif


