#ifndef CADMIUM_TEMPLATE_CELLDEVS_ASYMM_STATE_HPP_
#define CADMIUM_TEMPLATE_CELLDEVS_ASYMM_STATE_HPP_

#include <iostream>
#include "nlohmann/json.hpp"

namespace RoomEvac{
	/// State of each Cell
	struct EvacState {
		float exitProximity; // Relative distance to exit
		int type;	// 0 = Empty, 1 = Person, 2 = Exit, 3 = Obstacle
		int nextDestination; // 1 = UP_LEFT, 2 = UP, 3 = UP_RIGHT, 4 = LEFT, 5 = RIGHT, 6 = DOWN_LEFT, 
							// 7 = DOWN, 8 = DOWN_RIGHT
		/// The default is an empty cell with no relative distance to the exit
		State() : exitProximity(0), type(0), nextDestination(0){}
	};

	/**
	 * @param x first cell state.
	 * @param y second cell state.
	 * @return true if the first cell state is not equal to the second cell state.
	 */
	inline bool operator != (const EvacState& x, const EvacState& y) {
		if ((x.exitProximity != y.exitProximity) || 
			(x.type != y.type) ||
			(x.nextDestination != y.nextDestination)){
			return true;
		} else {
			return false;
		}
	}

	/**
	 * @param os output stream.
	 * @param s cell state.
	 * @return output stream with the inserted cell state.
	 */
	std::ostream &operator << (std::ostream& os, const EvacState& s) {
		os << "<" << s.exitProximity << "," << s.type << "," << s.nextDestination << ">";
		return os;
	}

	/**
	 * @param j JSON object representing a cell state.
	 * @param s cell state structure to be filled with the configuration shown in the JSON file.
	 */
	[[maybe_unused]] void from_json(const nlohmann::json& j, EvacState& s) {
		j.at("exitProsimity").get_to(s.exitProximity);
		j.at("type").get_to(s.type);
		j.at("nextDestination").get_to(s.nextDestination);
	}
	
} // namespace RoomEvac
#endif //CADMIUM_TEMPLATE_CELLDEVS_ASYMM_HPP_
