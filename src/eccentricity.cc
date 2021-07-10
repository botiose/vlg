#include <iostream>
#include <climits>
#include <set>

#include "eccentricity.hh"

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
computeBounds(const long& srcVertexEccentricity,
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

void getDegrees(const igraph_t& graph, igraph_vector_t& degrees) {
  igraph_vector_init(&degrees, 0);
  igraph_degree(
      &graph, &degrees, igraph_vss_all(), IGRAPH_OUT, IGRAPH_NO_LOOPS);
}

long
selectCandidate(const std::set<long>& candidates,
                const long& maxUpperVertex,
                const long& minLowerVertex,
                bool& chooseUpper) {
  UNUSED(maxUpperVertex);
  UNUSED(minLowerVertex);
  UNUSED(chooseUpper);
  if (maxUpperVertex == -1) {
    return *candidates.begin(); // TODO compute this
  }

  long selection = 0;

  if (chooseUpper == true) {
    selection = maxUpperVertex;
  } else {
    selection = minLowerVertex;
  }

  chooseUpper = !chooseUpper;
  std::cout << selection << std::endl;
  return selection;
}

void
computeBoundVertex(const std::vector<long>& boundVector,
                   const igraph_vector_t& degrees,
                   const long& candidate,
                   long& curBoundVertex) {
  if (curBoundVertex == -1) {
    curBoundVertex = candidate;
  } else if (boundVector[candidate] == boundVector[curBoundVertex] &&
             VECTOR(degrees)[candidate] > VECTOR(degrees)[curBoundVertex]) {
    curBoundVertex = candidate; 
  } else if (boundVector[candidate] < boundVector[curBoundVertex]) {
    curBoundVertex = candidate;
  }  
}

void
boundingEccentricities(const igraph_t& graph,
                       std::vector<long>& eccentricities) {
  igraph_integer_t vertexCount = igraph_vcount(&graph);
  eccentricities.resize(vertexCount);

  std::vector<long> lowerBounds(vertexCount, LONG_MIN);
  std::vector<long> upperBounds(vertexCount, LONG_MAX);

  std::set<long> candidates;
  copyCandidates(graph, candidates);

  igraph_vector_t degrees;
  getDegrees(graph, degrees);

  long maxUpperVertex = -1;
  long minLowerVertex = -1;
  bool chooseUpper = true;

  igraph_matrix_t shortestPaths;
  igraph_matrix_init(&shortestPaths, 1, vertexCount);

  if (igraph_matrix_max(&shortestPaths) == IGRAPH_INFINITY) {
    throw std::invalid_argument("Graph is not connected");
  }

  int i = 0;
  while (!candidates.empty()) {
    i += 1;
    long vertex = selectCandidate(
        candidates, maxUpperVertex, minLowerVertex, chooseUpper);
    long maxUpperVertex = -1;
    long minLowerVertex = -1;

    computeShortestPaths(graph, vertex, shortestPaths);
    eccentricities[vertex] = computeEccentricity(shortestPaths);

    std::set<long>::iterator candidateIterator = candidates.begin();
    while (candidateIterator != candidates.end()) {
      long candidate = *candidateIterator;
      computeBounds(eccentricities[vertex],
                   candidate,
                   shortestPaths,
                   lowerBounds,
                   upperBounds);

      if (lowerBounds[candidate] == upperBounds[candidate]) {
        eccentricities[candidate] = lowerBounds[candidate];

        candidateIterator = candidates.erase(candidateIterator);
      } else {
        computeBoundVertex(lowerBounds, degrees, candidate, minLowerVertex);
        computeBoundVertex(upperBounds, degrees, candidate, maxUpperVertex);

        candidateIterator++;
      }
    }
  }
  std::cout << i << std::endl; 
}
