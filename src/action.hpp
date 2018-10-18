#ifndef HPP__ACTION__HEADER__HPP
#define HPP__ACTION__HEADER__HPP

#include <utility>
#include <string>
#include <cassert>

class Action {};

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
			std::string to_string() const;

			bool operator==(const OxoAction& rhs) const;

		};

	}

	namespace Nim {
		class NimAction: public Action {
			/*The class NimAction implements the actions possible during a Nim play*/
		public:
			unsigned int pile{0u};
			unsigned int number{1u};

			NimAction()= default;
			NimAction(std::pair<unsigned int, unsigned int> input);
			NimAction(const unsigned int& where, const unsigned int& what);

			//for debugging
			std::string to_string() const;

			bool operator==(const NimAction& rhs) const;
		};
	}
}

#endif
