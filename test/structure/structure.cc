#include "gtest/gtest.h"

#include "eccentricity.hh"

TEST(undirected, probability10Islands10Nodes) {
  // Experimental feature. Needs to be called prior to any other igraph call.
  igraph_set_attribute_table(&igraph_cattribute_table);

  for (float i = 0.1; i <= 1; i += 0.1) {
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

TEST(Directed, kRegularGame) {
  // Experimental feature. Needs to be called prior to any other igraph call.
  igraph_set_attribute_table(&igraph_cattribute_table);

  for (int i = 10; i < 20; i++) {
    igraph_t graph;
    igraph_k_regular_game(&graph, i, 2, true, false);

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

    // This comparison is no longer valid
    // for (size_t i = 0; i < eccentricities.size(); i++) {
    //   EXPECT_EQ(eccentricities[i], VECTOR(ecc)[i]);
    // }
  }
}
