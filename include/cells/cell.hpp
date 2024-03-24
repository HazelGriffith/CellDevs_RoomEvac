#ifndef CADMIUM_TEMPLATE_CELLDEVS_GRID_CELL_HPP_
#define CADMIUM_TEMPLATE_CELLDEVS_GRID_CELL_HPP_

#include "cadmium/celldevs/grid/cell.hpp"
#include "../state.hpp"
#include "../vicinity.hpp"

 class CellTemplate : public cadmium::celldevs::GridCell<State, Vicinity> {
 public:
	 CellTemplate(const std::vector<int>& id, const std::shared_ptr<const cadmium::celldevs::GridCellConfig<State, Vicinity>>& config):`
	   cadmium::celldevs::GridCell<State, Vicinity>(id, config), cellConfig() {
	 }

	 /**
	  * Implement the local computation function of the cell.
	  * @param state previous cell state. You can modify this variable and return it.
	  * @param neighborhood information about the cell neighborhood (neighboring cells, vicinity factor, states...).
	  * @param x input port set with any potential input messages.
	  * @return new cell state
	  */
	[[nodiscard]] State localComputation(State state, const std::unordered_map<std::vector<int>, cadmium::celldevs::NeighborData<State, Vicinity>>& neighborhood, const cadmium::PortSet& x) const override {
		 // This loop iterates over all the neighboring cells. neighborId is the neighboring cell ID
		for(const auto& [neighborId, neighborData]: neighborhood) {
			 auto neighborState = neighborData.state;  // neighbor cell state
			 auto neighborVicinity = neighborData.vicinity;  // vicinity factor of neighboring cell
		 }
		return state;
	}

	/**
	 * Implement the output delay function of the cell.
	 * @param state new cell state.
	 * @return time to wait before notifying influenced cells about this new state.
	 */
	 [[nodiscard]] double outputDelay(const State& state) const override {
		 return 1.;
	 }
};

#endif //CADMIUM_TEMPLATE_CELLDEVS_GRID_CELL_HPP_
