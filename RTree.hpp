#ifndef RTREE_HPP
#define RTREE_HPP
#include "stdlib.hpp"
#include <vector>
#include "SpatialData.hpp"
using namespace SpatialData;
namespace SpatialIndex{
#define DEGREE 4

class Node{
    friend class RTree;
    size_t m_count;
    size_t m_level;
    union{
        vector<Node*> ChildPointers;
        //DataElement Element;
    };
    vector<string> Identifier;
    //vector<Bounding_Rectangle> Mbb;
public:
    Node(): m_count(0), m_level(0), Identifier(DEGREE-1){
        if(IsInternalNode()){
            ChildPointers.resize(DEGREE);
        }
    }
    bool IsInternalNode() {return (m_level);}
    bool IsLeaf() {return (!m_level);}
    bool IsFull() {return (m_count == DEGREE-1);}
};

class RTree{
private:
    Node* root{};
    size_t d_count;
public:
    RTree();
};
}

#endif // RTREE_HPP
