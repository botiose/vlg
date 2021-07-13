#pragma once
#include <igraph.h>
#include <vector>
#include <map>

/**
 * @brief Prunes vertices with degree equal one from the graph.
 * @details Pruned vertices are stored in the given map by their respective
 * neighborhood for future eccentricity computations.
 */
void
pruneGraph(igraph_t& graph,
           std::map<long, std::vector<long>>& prunedNeighborBuckets);
