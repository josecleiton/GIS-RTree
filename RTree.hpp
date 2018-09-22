#ifndef RTREE_HPP
#define RTREE_HPP
#include "stdlib.hpp"

class RTree{
protected:
    struct Point{
        float x, y;
    };
    struct Line{

    };
    struct Rect{

    };
    struct Box{

    };
    struct Node{
        bool IsInternalNode() {return (m_level);}
        bool IsLeaf() {return (!m_level);}
        size_t m_count{};
        size_t m_level{};
    };
public:
    RTree();
};

#endif // RTREE_HPP
