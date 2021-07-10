#include "gtest/gtest.h"

#include "eccentricity.hh"

TEST(Undirected, Probability10Islands10Nodes) {
  for (float i = 0.1; i <= 1; i += 0.1) {
    std::cout << i << std::endl;
    igraph_t graph;

    for (int j = 1; j < 10; j++) {
      igraph_simple_interconnected_islands_game(&graph, 10, 10, i, j);

      igraph_bool_t isConnected;
      igraph_is_connected(&graph, &isConnected, IGRAPH_STRONG);

      if (isConnected == false) {
        continue;
      }

      std::vector<long> eccentricities;
      boundingEccentricities(graph, eccentricities);

      igraph_vector_t ecc;
      igraph_vector_init(&ecc, 0);

      igraph_eccentricity(&graph, &ecc, igraph_vss_all(), IGRAPH_OUT);

      ASSERT_EQ(eccentricities.size(), igraph_vector_size(&ecc));

      for (size_t i = 0; i < eccentricities.size(); i++) {
        EXPECT_EQ(eccentricities[i], VECTOR(ecc)[i]);
      }
    }
  }
}
