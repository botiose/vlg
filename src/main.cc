#include <iostream>
#include <string>

#include <vector>
#include <set>
#include <climits>

#include <igraph.h>

#define UNUSED(x) (void)(x)

void
copyCandidates(const igraph_t& graph, std::set<long>& candidates) {
  igraph_vs_t vs = igraph_vss_all();
  igraph_vit_t vit;

  igraph_vit_create(&graph, vs, &vit);

  while (!IGRAPH_VIT_END(vit)) {
    candidates.insert(IGRAPH_VIT_GET(vit));
    IGRAPH_VIT_NEXT(vit);
  }
}

long
selectCandidate(const std::set<long>& candidates) {
  return *candidates.begin();
}

void
computeShortestPaths(const igraph_t& graph,
                     const long& vertex,
                     igraph_matrix_t& res) {
  igraph_vs_t fromVs;
  igraph_vs_1(&fromVs, vertex);

  igraph_vs_t toVs = igraph_vss_all();

  igraph_shortest_paths(&graph, &res, fromVs, toVs, IGRAPH_OUT);
}

long
computeEccentricity(const igraph_matrix_t& res) {
  return igraph_matrix_max(&res);
}

long
max(const long& a, const long& b) {
  return (a < b) ? b : a;
}

long
min(const long& a, const long& b) {
  return (a > b) ? b : a;
}

void
updateBounds(const long& srcVertexEccentricity,
             const long& dstVertex,
             const igraph_matrix_t& shortestPaths,
             std::vector<long>& lowerBounds,
             std::vector<long>& upperBounds) {
  long distance = MATRIX(shortestPaths, 0, dstVertex);
  long lowerBound = lowerBounds[dstVertex];
  lowerBounds[dstVertex] =
      max(lowerBound, max(srcVertexEccentricity - distance, distance));

  long upperBound = upperBounds[dstVertex];
  upperBounds[dstVertex] = min(upperBound, srcVertexEccentricity + distance);
}

void
boundingEccentricities(const igraph_t& graph,
                       std::vector<long>& eccentricities) {
  igraph_integer_t vertexCount = igraph_vcount(&graph);

  std::vector<long> lowerBounds(vertexCount, LONG_MIN);
  std::vector<long> upperBounds(vertexCount, LONG_MAX);

  std::set<long> candidates;
  copyCandidates(graph, candidates);

  while (!candidates.empty()) {
    long vertex = selectCandidate(candidates);

    igraph_matrix_t shortestPaths;
    igraph_matrix_init(&shortestPaths, 1, vertexCount);
    computeShortestPaths(graph, vertex, shortestPaths);
    eccentricities[vertex] = computeEccentricity(shortestPaths);

    std::set<long>::iterator candidateIterator = candidates.begin();
    while (candidateIterator != candidates.end()) {
      long candidate = *candidateIterator;
      updateBounds(eccentricities[vertex],
                   candidate,
                   shortestPaths,
                   lowerBounds,
                   upperBounds);
      if (lowerBounds[candidate] == upperBounds[candidate]) {
        eccentricities[candidate] = lowerBounds[candidate];

        candidateIterator = candidates.erase(candidateIterator);
      } else {
        candidateIterator++;
      }
    }
  }
}

int
main() {
  igraph_t graph;

  igraph_star(&graph, 10, IGRAPH_STAR_UNDIRECTED, 0);

  igraph_integer_t vertexCount = igraph_vcount(&graph);  
  std::vector<long> eccentricities(vertexCount);
  boundingEccentricities(graph, eccentricities);

  for(long eccentricity : eccentricities) {
    std::cout << eccentricity << " "; 
  }
  std::cout << std::endl; 
  
  igraph_vector_t ecc;
  igraph_vector_init(&ecc, 0);

  igraph_eccentricity(&graph, &ecc, igraph_vss_all(), IGRAPH_OUT);
  igraph_vector_print(&ecc);

  igraph_destroy(&graph);

  return 0;
}
