#include <iostream>
#include <climits>
#include <set>
#include <map>

#include "eccentricity.hh"

#define UNUSED(x) (void)(x)

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

long
toOriginalId(const igraph_t& graph, const long& prunedId) {
  return igraph_cattribute_VAN(&graph, "originalId", prunedId);
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

void
getDegrees(const igraph_t& graph, igraph_vector_t& degrees) {
  igraph_vector_init(&degrees, 0);
  igraph_degree(
      &graph, &degrees, igraph_vss_all(), IGRAPH_OUT, IGRAPH_NO_LOOPS);
}

long
selectCandidate(const std::set<long>& candidates,
                const long& maxUpperVertex,
                const long& minLowerVertex,
                bool& chooseUpper) {
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

void
initContainers(const igraph_t& originalGraph,
               std::vector<long>& eccentricities,
               igraph_t& graph) {
  igraph_copy(&graph, &originalGraph);

  igraph_bool_t isDirected = igraph_is_directed(&graph);

  if (isDirected == true) {
    igraph_to_undirected(&graph, IGRAPH_TO_UNDIRECTED_COLLAPSE, 0);
  }

  igraph_integer_t vertexCount = igraph_vcount(&graph);
  eccentricities.resize(vertexCount);
}

void
pruneGraph(igraph_t& graph,
           std::map<long, std::vector<long>>& prunedNeighborBuckets) {
  igraph_vs_t vs = igraph_vss_all();
  igraph_vit_t vit;

  igraph_vit_create(&graph, vs, &vit);

  while (!IGRAPH_VIT_END(vit)) {
    igraph_integer_t vid = IGRAPH_VIT_GET(vit);

    igraph_cattribute_VAN_set(&graph, "originalId", vid, vid);

    igraph_vector_t degree;
    igraph_vector_init(&degree, 0);

    igraph_vs_t curVs;
    igraph_vs_1(&curVs, vid);

    igraph_degree(&graph, &degree, curVs, IGRAPH_OUT, false);

    if (VECTOR(degree)[0] == 1) {
      igraph_vector_ptr_t neighborVecPtr;
      igraph_vector_ptr_init(&neighborVecPtr, 0);

      igraph_neighborhood(&graph, &neighborVecPtr, curVs, 1, IGRAPH_OUT, 1);

      igraph_vector_t* neighborVec =
          (igraph_vector_t*)igraph_vector_ptr_e(&neighborVecPtr, 0);

      long neighbor = VECTOR(*neighborVec)[0];

      prunedNeighborBuckets[neighbor].push_back(vid);
    }

    IGRAPH_VIT_NEXT(vit);
  }

  std::vector<long> prunedVertices;

  for (const auto& p : prunedNeighborBuckets) {
    const std::vector<long>& bucket = p.second;
    for (auto ite = bucket.begin(); ite < bucket.end() - 1; ite++) {
      prunedVertices.push_back(*ite);
    }
  }

  igraph_vector_t v;
  std::vector<igraph_real_t> prunedVerticesReal(prunedVertices.begin(),
                                                prunedVertices.end());

  igraph_vector_init_copy(
      &v, prunedVerticesReal.data(), prunedVerticesReal.size());

  igraph_vs_t prunedVs;
  igraph_vs_vector(&prunedVs, &v);

  igraph_delete_vertices(&graph, prunedVs);
}

void
computePrunedEccentricities(
    const std::map<long, std::vector<long>>& prunedNeighborBuckets,
    std::vector<long>& eccentricities) {

  for (const auto& p : prunedNeighborBuckets) {
    long neighbor = p.first;
    const std::vector<long>& bucket = p.second;
    for (const auto& vertex : bucket) {
      eccentricities[vertex] = eccentricities[neighbor] + 1;
    }
  }
}

void
boundingEccentricities(const igraph_t& originalGraph,
                       std::vector<long>& eccentricities) {
  igraph_t graph;
  std::vector<long> lowerBounds;
  std::vector<long> upperBounds;

  initContainers(originalGraph, eccentricities, graph);

  std::map<long, std::vector<long>> prunedNeighborBuckets;
  pruneGraph(graph, prunedNeighborBuckets);

  std::set<long> candidates;
  igraph_vector_t degrees;
  igraph_matrix_t shortestPaths;

  // TODO start encapsulate
  copyCandidates(graph, candidates);

  getDegrees(graph, degrees);

  igraph_integer_t vertexCount = igraph_vcount(&graph);
  igraph_matrix_init(&shortestPaths, 1, vertexCount);

  lowerBounds.resize(vertexCount, LONG_MIN);
  upperBounds.resize(vertexCount, LONG_MAX);
  // TODO end encapsulate

  // std::cout << "pruned bucked: " << prunedNeighborBuckets.size() <<
  // std::endl;

  if (igraph_matrix_max(&shortestPaths) == IGRAPH_INFINITY) {
    throw std::invalid_argument("Graph is not connected");
  }

  long maxUpperVertex = -1;
  long minLowerVertex = -1;
  bool chooseUpper = true;

  // int i = 0;
  while (!candidates.empty()) {
    // i += 1;
    long vertex = selectCandidate(
        candidates, maxUpperVertex, minLowerVertex, chooseUpper);
    long maxUpperVertex = -1;
    long minLowerVertex = -1;

    computeShortestPaths(graph, vertex, shortestPaths);

    eccentricities[toOriginalId(graph, vertex)] =
        computeEccentricity(shortestPaths);

    std::set<long>::iterator candidateIterator = candidates.begin();
    while (candidateIterator != candidates.end()) {
      long candidate = *candidateIterator;
      computeBounds(eccentricities[toOriginalId(graph, vertex)],
                    candidate,
                    shortestPaths,
                    lowerBounds,
                    upperBounds);

      if (lowerBounds[candidate] == upperBounds[candidate]) {
        eccentricities[toOriginalId(graph, candidate)] = lowerBounds[candidate];

        candidateIterator = candidates.erase(candidateIterator);
      } else {
        computeBoundVertex(lowerBounds, degrees, candidate, minLowerVertex);
        computeBoundVertex(upperBounds, degrees, candidate, maxUpperVertex);

        candidateIterator++;
      }
    }
  }
  // std::cout << "iterations: " << i << std::endl;

  computePrunedEccentricities(prunedNeighborBuckets, eccentricities);
}
