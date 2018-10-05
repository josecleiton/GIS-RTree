#ifndef CIRCULAR_LIST_HPP
#define CIRCULAR_LIST_HPP

namespace CircularList{
class Node{
protected:
    Node* m_next{};
    Node* m_prev{};
public:
    Node();
    virtual ~Node();
    Node* Next();
    Node* Prev();
    Node* Push(Node*);
    Node* Pop();
    void Splice(Node*);
};

}
#endif // CIRCULAR_LIST_HPP
