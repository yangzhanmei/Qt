#ifndef LEAFNODE_H
#define LEAFNODE_H

template <typename E>
// Leaf node subclass
class LeafNode : public HuffNode<E> {
private:
E it;
// Value
int wgt;
// Weight
public:
LeafNode(const E &val){ it = val;wgt = 0;}
LeafNode(const E& val, int freq)
// Constructor
{ it = val; wgt = freq; }
int weight() { return wgt; }
E val() { return it; }
bool isLeaf() { return true; }
};


#endif // LEAFNODE_H
