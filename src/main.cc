#include <iostream>
#include <vector>
#include <igraph.h>
#include <chrono>
#include <string>

#include "eccentricity.hh"

void
loadGreatestConnectedComponent(const std::string& filepath, igraph_t*& graph) {
  igraph_t rawGraph;  

  FILE* f;
  if ((f = fopen(filepath.c_str(), "r"))) {
    igraph_read_graph_edgelist(&rawGraph, f, 0, false);  
    pclose(f);
  } else {
    std::cerr << "main.cc: Error occured while attempting to read from "
              << filepath << std::endl;
    return;
  }

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

  graph = (igraph_t *)VECTOR(components)[greatestComponentIndex];
  igraph_simplify(graph, true, true, 0);

  for (size_t i = 0; i < componentCount; i++) {
    if (i != greatestComponentIndex) {
      igraph_destroy((igraph_t *)VECTOR(components)[i]);
      igraph_free(VECTOR(components)[i]);
    }
  }
}

int
main(int argc, char *argv[]) {
  // Experimental feature. Needs to be called prior to any other igraph call.
  igraph_set_attribute_table(&igraph_cattribute_table);

  for (int i = 1; i < argc; i++) {
    if (i != 1) {
      std::cout << std::endl; 
    }

    std::cout << argv[i] << ":" << std::endl;

    igraph_t* graph = nullptr;
    loadGreatestConnectedComponent(argv[i], graph);

    if (graph == nullptr) {
      continue;
    }

    std::cout << "nodes: " << igraph_vcount(graph) << std::endl; 
    std::cout << "edges: " << igraph_ecount(graph) << std::endl; 

    std::vector<long> eccentricities; 
    boundingEccentricities(*graph, eccentricities);

    igraph_destroy(graph);
    igraph_free(graph);
  }

  return 0;
}
