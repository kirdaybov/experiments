#pragma once

#include "TArray.h"

#include <cassert>

template<class T>
class TGraph
{
  struct TVertice
  {
    T data;
  };
  struct TEdge
  {
    TEdge(TVertice* start, TVertice* end) : _start(start), _end(end) {}
    TVertice* _start;
    TVertice* _end;
  };

  TArray<TVertice> _vertices;
  TArray<TEdge> _edges;

  TArray<TEdge> get_edges_starting_with(TVertice* vertice)
  {
    TArray<TEdge> edges;
    for (const auto &edge : _edges)
      if (edge._start == vertice)
        edges.add(edge);
    return edges;
  }


public:

  inline void add_v(const T &data) { _vertices.add({ data }); }
  inline void add_e(size_t i, size_t j) { assert(i < _vertices.size() && j < _vertices.size()); _edges.add({&_vertices[i], &_vertices[j] }); }

  bool check_no_exit(TArray<int> &idxs)
  { 
    TArray<bool> has_end(_vertices.size(), false);    
    for (int i = 0; i < _vertices.size(); i++)
    {
      auto &vertice = _vertices[i];
      auto edges = get_edges_starting_with(&vertice);
      if (edges.size() == 0)
        has_end[i] = true;
      else
        for(auto edge : edges)
          //if (edge._end
    }
  }
};

struct GraphTest
{
  GraphTest()
  {
    TGraph<int> graph;
    const int N = 10;
    for (int i = 0; i < N; i++)
      graph.add_v(i);
    for (int i = 0; i < N-1; i++)
      graph.add_e(i, i + 1);
  }
} _TGraph;