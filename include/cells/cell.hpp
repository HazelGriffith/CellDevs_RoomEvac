#ifndef EVAC_CELL_HPP_
#define EVAC_CELL_HPP_

#include <cadmium/celldevs/grid/cell.hpp>
#include "../state.hpp"
#include <cmath>
#include <algorithm>
#include <vector>
#include <cassert>
#include <random>
#include <chrono>
#include <iostream>

using namespace std;
namespace RoomEvac{
	struct absNeighbor{
		int row;
		int col;
		shared_ptr<const RoomEvac::EvacState> neighborState;
	};
	
	struct emptyNeighbor{
		int direction;
		float exitProximity;
	};
	
	
	
	
	class EvacCell : public cadmium::celldevs::GridCell<EvacState, double> {
		
	public:
		EvacCell(const std::vector<int>& id, const std::shared_ptr<const cadmium::celldevs::GridCellConfig<EvacState, double>>& config):
			cadmium::celldevs::GridCell<EvacState, double>(id, config) {
				
		}

		 /**
		  * Implement the local computation function of the cell.
		  * @param state previous cell state. You can modify this variable and return it.
		  * @param neighborhood information about the cell neighborhood (neighboring cells, vicinity factor, states...).
		  * @param x input port set with any potential input messages.
		  * @return new cell state
		  */
		[[nodiscard]] EvacState localComputation(EvacState state, const std::unordered_map<std::vector<int>, cadmium::celldevs::NeighborData<EvacState, double>>& neighborhood) const override {
			 // This loop iterates over all the neighboring cells. neighborId is the neighboring cell ID
			bool move = false;
			bool wait = false;
			vector<float> eps;
			
			vector<int> incoming;
			vector<absNeighbor> neighbors;
			vector<emptyNeighbor> emptyNeighbors;
			int myRow;
			int myCol;
			double sum;
			for(const auto& [neighborId, neighborData]: neighborhood) {
				auto neighborState = neighborData.state;  // neighbor cell state
				auto neighborVicinity = neighborData.vicinity;  // vicinity factor of neighboring cell
				if (neighborVicinity == 1){
					myRow = neighborId[0];
					myCol = neighborId[1];
				} else if (neighborVicinity == 0.25){
					absNeighbor thisNeighbor = {neighborId[0], neighborId[1], neighborState};
					neighbors.push_back(thisNeighbor);
				} else {
					assert(("Not a possible neighbor Vicinity value", false));
				}
			}
			
			for (int i = 0; i < neighbors.size(); i++) {
				int relRow = neighbors[i].row - myRow;
				int relCol = neighbors[i].col - myCol;
				auto neighborState = neighbors[i].neighborState;
				if (state.type < 2){
					if (state.exitProximity == 0){
						if ((neighborState->type != 2)&&(neighborState->exitProximity != 0)){
							// The neighbor is not an obstacle
							sum = abs(relRow + relCol);
							if (sum == 1){
								// The neighbor is vertical or horizontal to the cell
								eps.push_back(neighborState->exitProximity + 1);
							} else {
							// Otherwise it is diagonal to the cell
								eps.push_back(neighborState->exitProximity + 1.5);
							}
						}
					} else if ((state.type == 1)&&(state.nextDestination == 0)){
						if (neighborState->type == 0){
							if (neighborState->exitProximity != 0){
								switch(relRow){
									case -1:
										switch(relCol){
											case -1:
												eps.push_back(neighborState->exitProximity);
												emptyNeighbors.push_back({1, neighborState->exitProximity});
												break;
											case 0:
												eps.push_back(neighborState->exitProximity);
												emptyNeighbors.push_back({2, neighborState->exitProximity});
												break;
											case 1:
												eps.push_back(neighborState->exitProximity);
												emptyNeighbors.push_back({3, neighborState->exitProximity});
												break;
											default:
												assert(("Not a possible neighbor ID value", false));
												break;
										}
										break;
									case 0:
										switch(relCol){
											case -1:
												eps.push_back(neighborState->exitProximity);
												emptyNeighbors.push_back({4, neighborState->exitProximity});
												break;
											case 1:
												eps.push_back(neighborState->exitProximity);
												emptyNeighbors.push_back({5, neighborState->exitProximity});
												break;
											default:
												assert(("Not a possible neighbor ID value", false));
												break;
										}
										break;
									case 1:
										switch(relCol){
											case -1:
												eps.push_back(neighborState->exitProximity);
												emptyNeighbors.push_back({6, neighborState->exitProximity});
												break;
											case 0:
												eps.push_back(neighborState->exitProximity);
												emptyNeighbors.push_back({7, neighborState->exitProximity});
												break;
											case 1:
												eps.push_back(neighborState->exitProximity);
												emptyNeighbors.push_back({8, neighborState->exitProximity});
												break;
											default:
												assert(("Not a possible neighbor ID value", false));
												break;
										}
										break;
									default:
										assert(("Not a possible neighbor ID value", false));
										break;
								}
							} else {
								eps.clear();
								emptyNeighbors.clear();
								break;
							}
						}
					} else if ((state.type == 1)&&(state.nextDestination != 0)){
						if (neighborState->type == 0){
							bool checkDirection = false;
							switch(state.nextDestination){
								case 1:
									if ((relRow == -1)&&(relCol == -1)){
										checkDirection=true;
									}
									break;
								case 2:
									if ((relRow == -1)&&(relCol == 0)){
										checkDirection=true;
									}
									break;
								case 3:
									if ((relRow == -1)&&(relCol == 1)){
										checkDirection=true;
									}
									break;
								case 4:
									if ((relRow == 0)&&(relCol == -1)){
										checkDirection=true;
									}
									break;
								case 5:
									if ((relRow == 0)&&(relCol == 1)){
										checkDirection=true;
									}
									break;
								case 6:
									if ((relRow == 1)&&(relCol == -1)){
										checkDirection=true;
									}
									break;
								case 7:
									if ((relRow == 1)&&(relCol == 0)){
										checkDirection=true;
									}
									break;
								case 8:
									if ((relRow == 1)&&(relCol == 1)){
										checkDirection=true;
									}
									break;
								default:
									assert(("Not a possible destination value", false));
									break;
								
							}
							if (checkDirection){
								if (state.nextDestination + neighborState->directionIncoming == 9){
									move = true;
									wait = false;
								} else if (neighborState->directionIncoming == 0){
									move = false;
									wait = true;
								} else if (state.nextDestination + neighborState->directionIncoming != 9){
									move = false;
									wait = false;
								}
								checkDirection = false;
								break;
							}
						}
					} else if ((state.type == 0)&&(state.directionIncoming == 0)){
						if (neighborState->type == 1) {
							if (neighborState->nextDestination != 0){
								switch(relRow){
									case -1:
										switch(relCol){
											case -1:
												if (neighborState->nextDestination == 8){
													incoming.push_back(1);
												}
												break;
											case 0:
												if (neighborState->nextDestination == 7){
													incoming.push_back(2);
												}
												break;
											case 1:
												if (neighborState->nextDestination == 6){
													incoming.push_back(3);
												}
												break;
											default:
												assert(("Not a possible neighbor ID value", false));
												break;
										}
										break;
									case 0:
										switch(relCol){
											case -1:
												if (neighborState->nextDestination == 5){
													incoming.push_back(4);
												}
												break;
											case 1:
												if (neighborState->nextDestination == 4){
													incoming.push_back(5);
												}
												break;
											default:
												assert(("Not a possible neighbor ID value", false));
												break;
										}
										break;
									case 1:
										switch(relCol){
											case -1:
												if (neighborState->nextDestination == 3){
													incoming.push_back(6);
												}
												break;
											case 0:
												if (neighborState->nextDestination == 2){
													incoming.push_back(7);
												}
												break;
											case 1:
												if (neighborState->nextDestination == 1){
													incoming.push_back(8);
												}
												break;
											default:
												assert(("Not a possible neighbor ID value", false));
												break;
										}
										break;
									default:
										assert(("Not a possible neighbor ID value", false));
										break;
								}
							} else {
								incoming.clear();
								break;
							}
						} 
					} else {
						
					}
				}
			}
			if (state.type < 2){
				if (state.exitProximity == 0){
					if (!eps.empty()){
						state.exitProximity = *min_element(eps.begin(), eps.end());
					} else {
						state.exitProximity = 0;
					}
					state.sigma = 0;
				} else if ((state.exitProximity == 1)&&(state.type == 1)){
					state.type = 0;
					state.sigma = 1;
				} else if ((state.type == 1)&&(state.nextDestination == 0)){
					state.directionIncoming = 0;
					if (!emptyNeighbors.empty()){
						vector<int> destinations;
						float minimumEP = *min_element(eps.begin(), eps.end());
						for(int i = 0; i < emptyNeighbors.size(); i++) {
							if (emptyNeighbors[i].exitProximity == minimumEP){
								destinations.push_back(emptyNeighbors[i].direction);
							}
						}
						if ((destinations.size() > 1)&&(destinations.size() < 9)){
							unsigned seed1 = chrono::system_clock::now().time_since_epoch().count();
							minstd_rand0 generator(seed1);
							uniform_int_distribution<> distrib(0, destinations.size()-1);
							state.nextDestination = destinations[distrib(generator)];
						} else if (destinations.size() == 1){
							state.nextDestination = destinations[0];
						} else {
							assert(("Not a possible number of destinations", false));
						}
					} else {
						state.nextDestination = 0;
					}
					state.sigma = 1;
				} else if ((state.type == 0)&&(state.directionIncoming == 0)){
					state.nextDestination = 0;
					if (!incoming.empty()){
						if ((incoming.size()>1)&&(incoming.size() < 9)){
							unsigned seed1 = chrono::system_clock::now().time_since_epoch().count();
							minstd_rand0 generator(seed1);
							uniform_int_distribution<> distrib(0, incoming.size()-1);
							state.directionIncoming = incoming[distrib(generator)];
						} else if (incoming.size() == 1){
							state.directionIncoming = incoming[0];
						} else {
							assert(("Not a possible number of destinations", false));
						}
						state.sigma = 0;
					} else {
						state.directionIncoming = 0;
						state.sigma = 0;
					}
					
				} else if ((state.type == 0)&&(state.directionIncoming != 0)){
					state.directionIncoming = 0;
					state.type = 1;
					state.sigma = 1;
				} else if ((state.type == 1)&&(state.nextDestination != 0)){
					if ((move == true)&&(wait == false)){
						state.type = 0;
						move = false;
						state.sigma = 1;
						state.nextDestination=0;
					} else if ((move == false)&&(wait == true)){
						state.type = 1;
						wait = false;
						state.sigma = 1;
					} else if ((move == false)&&(wait == false)){
						state.type = 1;
						state.sigma = 1;
						state.nextDestination=0;
					}
				}
			}
			return state;
		}

		/**
		 * Implement the output delay function of the cell.
		 * @param state new cell state.
		 * @return time to wait before notifying influenced cells about this new state.
		 */
		[[nodiscard]] double outputDelay(const EvacState& state) const override {
			return state.sigma;
		}
		 
		 
	};
}

#endif //EVAC_CELL_HPP_
