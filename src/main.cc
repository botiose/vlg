#include <iostream>
#include <vector>
#include <igraph.h>
#include <chrono>

#include <valgrind/callgrind.h>

#include "eccentricity.hh"

int
main() {
  // Experimental feature. Needs to be called prior to any other igraph call.
  igraph_set_attribute_table(&igraph_cattribute_table);

  igraph_t graph;

  igraph_bool_t isConnected;

  do {
    igraph_simple_interconnected_islands_game(&graph, 10, 100, 0.1, 2);
    // igraph_k_regular_game(&graph, 10, 2, true, false);
    igraph_is_connected(&graph, &isConnected, IGRAPH_STRONG);
  } while (!isConnected);

  std::vector<long> eccentricities;

  auto start = std::chrono::high_resolution_clock::now();
  CALLGRIND_START_INSTRUMENTATION;
  boundingEccentricities(graph, eccentricities);
  CALLGRIND_STOP_INSTRUMENTATION;
  auto stop = std::chrono::high_resolution_clock::now();

  auto duration =
      std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
  std::cout << "boundingEccentricities(...) : " << duration.count()
            << " microseconds" << std::endl;

  // for (long eccentricity : eccentricities) {
  //   std::cout << eccentricity << " ";
  // }
  // std::cout <<  std::endl;

  igraph_vector_t ecc;
  igraph_vector_init(&ecc, 0);
  igraph_vs_t vs = igraph_vss_all();

  start = std::chrono::high_resolution_clock::now();
  igraph_eccentricity(&graph, &ecc, vs, IGRAPH_OUT);
  stop = std::chrono::high_resolution_clock::now();

  duration =
      std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
  std::cout << "igraph_eccentricity(......) : " << duration.count()
            << " microseconds" << std::endl;

  // igraph_vector_print(&ecc);

  igraph_vs_destroy(&vs);
  igraph_vector_destroy(&ecc);
  igraph_destroy(&graph);
  return 0;
}
