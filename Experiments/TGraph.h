#pragma once

template<class T>
class TGraph
{
  class TVertice
  {
    T data;
  };
  class TEdge
  {
    TVertice* _start;
    TVertice* _end;
  };

  TArray<TVertice> _vertices;
  TArray<TEdge> _edges;
public:
};

struct GraphTest
{
  GraphTest()
  {
    TGraph<int> graph;
  }
} _TGraph;