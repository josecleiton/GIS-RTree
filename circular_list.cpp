#include "circular_list.hpp"

namespace CircularList{
Node::Node(): m_next(this), m_prev(this){
}

Node::~Node(){
}

Node* Node::Prev(){
    return m_prev;
}

Node* Node::Next(){
    return m_next;
}

Node* Node::Push(Node* in){
    Node* next = m_next;
    in->m_next = next;
    in->m_prev = this;
    m_next = in;
    next->m_prev = in;
    return this;
}

Node* Node::Pop(){
    m_prev->m_next = m_next;
    m_next->m_prev = m_prev;
    m_prev = m_next = this;
    return this;
}

void Node::Splice(Node* b){
    Node* a = this;
    Node *an = a->m_next;
    Node *bn = b->m_next;
    a->m_next = bn;
    b->m_next = an;
    an->m_prev = b;
    bn->m_prev = a;
}
} // NAMESPACE CIRCULAR LIST
