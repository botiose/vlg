#include "gtest/gtest.h"

#include "eccentricity.hh"

TEST(Undirected, 1b) {
  igraph_t graph;
  igraph_bool_t isConnected;

  do {
    igraph_simple_interconnected_islands_game(&graph, 1000, 1000000, 0.1, 1000);
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
