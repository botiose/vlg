#include <iostream>
#include <vector>
#include <igraph.h>
#include <chrono>
#include <string>

// #include <valgrind/callgrind.h>

#include "eccentricity.hh"

void
loadGreatestConnectedComponent(const std::string& filepath, igraph_t& graph) {
  igraph_t rawGraph;  

  FILE* f = fopen(filepath.c_str(), "r"); // TODO: handle errors
  igraph_read_graph_edgelist(&rawGraph, f, 0, false);  
  pclose(f);

  igraph_vector_ptr_t components;
  igraph_vector_ptr_init(&components, 0);

  igraph_decompose(&rawGraph, &components, IGRAPH_STRONG, -1, 1);  

  size_t componentCount = igraph_vector_ptr_size(&components);

  size_t greatestComponentIndex = 0;
  size_t greatestComponentSize = 0;

  for (size_t i = 0; i < componentCount; i++) {
    igraph_t* component = (igraph_t *)VECTOR(components)[i];

    size_t componentSize = igraph_vcount(component);
    if (componentSize > greatestComponentSize) {
      greatestComponentIndex = i;
      greatestComponentSize = componentSize;
    }
  }

  graph = *(igraph_t *)VECTOR(components)[greatestComponentIndex];

  for (size_t i = 0; i < componentCount; i++) {
    if (i != greatestComponentIndex) {
      igraph_destroy((igraph_t *)VECTOR(components)[i]);
      igraph_free(VECTOR(components)[i]);
    }
  }
}

int
main() {
  // Experimental feature. Needs to be called prior to any other igraph call.
  igraph_set_attribute_table(&igraph_cattribute_table);

  igraph_t graph;
  loadGreatestConnectedComponent("etc/data/ca-HepTh.txt", graph);

  std::vector<long> eccentricities;

  auto start = std::chrono::high_resolution_clock::now();
  // CALLGRIND_START_INSTRUMENTATION;
  boundingEccentricities(graph, eccentricities);
  // CALLGRIND_STOP_INSTRUMENTATION;
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
