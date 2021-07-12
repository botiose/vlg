#include <iostream>
#include <vector>
#include <igraph.h>
#include <chrono>

// #include <valgrind/callgrind.h>

#include "eccentricity.hh"

int
main() {
  // CALLGRIND_START_INSTRUMENTATION;

  // Experimental feature. Needs to be called prior to any other igraph call.
  igraph_set_attribute_table(&igraph_cattribute_table);

  igraph_t graph;

  igraph_bool_t isConnected;

  do {
    igraph_simple_interconnected_islands_game(&graph, 10, 10, 0.2, 1);
    // igraph_k_regular_game(&graph, 10, 2, true, false);
    igraph_is_connected(&graph, &isConnected, IGRAPH_STRONG);  
  } while (!isConnected);

  FILE *ofile;
  ofile = fopen("graph.dot", "w");
  igraph_write_graph_dot(&graph, ofile);
  pclose(ofile);

  std::vector<long> eccentricities;

  auto start = std::chrono::high_resolution_clock::now();
  boundingEccentricities(graph, eccentricities);
  auto stop = std::chrono::high_resolution_clock::now();

  auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
  std::cout << "\nTime taken by function : " << duration.count()
            << " microseconds" << std::endl;

  for (long eccentricity : eccentricities) {
    std::cout << eccentricity << " ";
  }
  std::cout <<  std::endl; 

  igraph_vector_t ecc;
  igraph_vector_init(&ecc, 0);

  start = std::chrono::high_resolution_clock::now();
  igraph_eccentricity(&graph, &ecc, igraph_vss_all(), IGRAPH_OUT);
  stop = std::chrono::high_resolution_clock::now();

  duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
  std::cout << "\nTime taken by function : " << duration.count()
            << " microseconds" << std::endl;

  igraph_vector_print(&ecc);

  igraph_destroy(&graph);
  // CALLGRIND_STOP_INSTRUMENTATION;
  return 0;
}
