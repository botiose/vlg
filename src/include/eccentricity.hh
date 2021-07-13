#pragma once
#include <igraph.h>
#include <vector>

/**
 * @brief Computes the eccentricity of each vertex in the given graph.
 * @details The graph can be either directed or undirected. Directed graphs will
 * be converted to an undirected version through the use of
 * igraph_to_undirected(...) function with the IGRAPH_TO_UNDIRECTED_COLLAPSE
 * options. For further information on the workings of this function visit the
 * respective section in the Igraph manual.
 */
void
boundingEccentricities(const igraph_t& graph,
                       std::vector<long>& eccentricities);
