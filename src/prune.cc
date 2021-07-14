#include <iostream>
#include "prune.hh"

void
bucketOutliers(igraph_t& graph,
               std::map<long, std::vector<long>>& prunedNeighborBuckets) {
  igraph_vs_t vs = igraph_vss_all();
  igraph_vit_t vit;

  igraph_vit_create(&graph, vs, &vit);

  igraph_vs_destroy(&vs);

  while (!IGRAPH_VIT_END(vit)) {
    igraph_integer_t vid = IGRAPH_VIT_GET(vit);

    // Stores the original vertex id in a vertex attribute
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

      igraph_vector_ptr_destroy_all(&neighborVecPtr);
    }

    igraph_vector_destroy(&degree);
    igraph_vs_destroy(&curVs);

    IGRAPH_VIT_NEXT(vit);
  }

  igraph_vit_destroy(&vit);
}

void
removeOutliers(igraph_t& graph,
               std::map<long, std::vector<long>>& prunedNeighborBuckets,
               long& prunedVertexCount) {
  std::vector<long> prunedVertices;

  for (const auto& p : prunedNeighborBuckets) {
    const std::vector<long>& bucket = p.second;
    prunedVertexCount += bucket.size() - 1;
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
  igraph_vector_destroy(&v);
  igraph_vs_destroy(&prunedVs);
}

void
pruneGraph(igraph_t& graph,
           std::map<long, std::vector<long>>& prunedNeighborBuckets,
           long& prunedVertexCount) {
  bucketOutliers(graph, prunedNeighborBuckets);
  removeOutliers(graph, prunedNeighborBuckets, prunedVertexCount);
}
