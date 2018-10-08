#ifndef HPP__ACTION__HEADER__HPP
#define HPP__ACTION__HEADER__HPP

class Action {
public:
	virtual bool operator==(const Action& rhs);
};

namespace game::Oxo {
class OxoAction: public Action {
	public:
		/*The class OxoAction implements the action possible on the Oxo board and
		* defines the constructors*/

		int row{0};
		int column{0};

		Action()= default;
		Action(std::pair<int, int> input);
		Action(const int& first, const int& second);

		//for debugging
		std::string to_string();

		bool operator==(const Action& rhs) override;
	};

}

#endif


