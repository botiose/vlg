#include "gtest/gtest.h"

#include "eccentricity.hh"

TEST(Undirected, 1k) {
  // Experimental feature. Needs to be called prior to any other igraph call.
  igraph_set_attribute_table(&igraph_cattribute_table);

  igraph_t graph;
  igraph_bool_t isConnected;

  do {
    igraph_simple_interconnected_islands_game(&graph, 10, 100, 0.6, 10);
    igraph_is_connected(&graph, &isConnected, IGRAPH_STRONG);  
  } while (!isConnected);

  std::vector<long> eccentricities;
  boundingEccentricities(graph, eccentricities);

  igraph_vector_t ecc;
  igraph_vector_init(&ecc, 0);

  igraph_eccentricity(&graph, &ecc, igraph_vss_all(), IGRAPH_OUT);

  ASSERT_EQ(eccentricities.size(), igraph_vector_size(&ecc));

  for(size_t i = 0; i < eccentricities.size(); i++) {
    EXPECT_EQ(eccentricities[i], VECTOR(ecc)[i]);
  }
}
