#include <cadmium/celldevs/grid/coupled.hpp>
#include <cadmium/core/logger/csv.hpp>
#include <cadmium/core/simulation/root_coordinator.hpp>
#include <fstream>
#include <string>
#include <iostream>
#include "include/cells/cell.hpp"
using namespace std;
using namespace RoomEvac;
using namespace cadmium::celldevs;

shared_ptr<GridCell<EvacState, double>> addGridCell(const std::vector<int>& cellId, const std::shared_ptr<const cadmium::celldevs::GridCellConfig<EvacState, double>>& cellConfig) {
	auto cellModel = cellConfig->cellModel;
	
	if ((cellModel == "Evac")||(cellModel == "default")) {
		
		return make_shared<EvacCell>(cellId, cellConfig);
	} else {
		throw std::bad_typeid();
	}
}

int main(int argc, char ** argv) {
	
	if (argc < 2) {
		std::cout << "Program used with wrong parameters. The program must be invoked as follows:";
		std::cout << argv[0] << " SCENARIO_CONFIG.json [MAX_SIMULATION_TIME (default: 500)]" << std::endl;
		return -1;
	}
	std::string configFilePath = argv[1];
	double simTime = (argc > 2)? atof(argv[2]) : 500;
	auto model = std::make_shared<GridCellDEVSCoupled<EvacState, double>>("evac", addGridCell, configFilePath);
	model->buildModel();
	auto rootCoordinator = cadmium::RootCoordinator(model);
	auto logger = std::make_shared<cadmium::CSVLogger>("../simulation_results/evac.csv", ",");
	rootCoordinator.setLogger(logger);
	rootCoordinator.start();
	rootCoordinator.simulate(simTime);
	rootCoordinator.stop();
}
