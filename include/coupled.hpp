#ifndef COUPLED_HPP_
#define COUPLED_HPP_

#include <cadmium/celldevs/grid/coupled.hpp>
#include "cells/cell.hpp"
#include "state.hpp"
#include "vicinity.hpp"

using namespace std;
using namespace cadmium::celldevs;
namespace RoomEvac{
	class CoupledModel: public cadmium::celldevs::GridCellDEVSCoupled<EvacState, double> {
	public:
		CoupledModel(const std::string& id, const std::string& configFilePath): cadmium::celldevs::GridCellDEVSCoupled<EvacState, double>(id, configFilePath) {}

		shared_ptr<GridCell<EvacState, double>> addGridCell(const std::vector<int>& cellId, const std::shared_ptr<const cadmium::celldevs::GridCellConfig<EvacState, double>>& cellConfig) override {
			auto cellModel = cellConfig->cellModel;
			if ((cellModel == "Evac")||(cellModel == "default")) {
				return make_shared<EvacCell>(cellId, cellConfig);
			} else {
				throw std::bad_typeid();
			}
		}
	};
}

#endif //COUPLED_HPP_
