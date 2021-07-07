#include <iostream>
#include <string>

#include <vector>
#include <set>
#include <climits>

#include <igraph.h>


#define UNUSED(x)(void)(x)

void copyCandidates(const igraph_t &graph, std::set<int long>& candidates) { 
  igraph_vs_t vs = igraph_vss_all();
  igraph_vit_t vit;

  igraph_vit_create(&graph, vs, &vit);

  while (!IGRAPH_VIT_END(vit)) {
    candidates.insert(IGRAPH_VIT_GET(vit));
    IGRAPH_VIT_NEXT(vit);
  }
}

std::vector<int long> boundingEccentricities(const igraph_t &graph) {
  igraph_integer_t vertexCount = igraph_vcount(&graph);

  std::vector<int long> lowerBounds(vertexCount, LONG_MIN);
  std::vector<int long> upperBounds(vertexCount, LONG_MAX);
  std::vector<int long> eccentricities(vertexCount);

  std::set<int long> candidates;
  copyCandidates(graph, candidates);

  while(!candidates.empty()) {
    // TODO
  }

  return eccentricities;
}

int main() {
    igraph_t g;

    igraph_star(&g, 10, IGRAPH_STAR_UNDIRECTED, 0);

    boundingEccentricities(g);

    igraph_destroy(&g);

    return 0;
}
