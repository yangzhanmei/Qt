#ifndef INTLNODE_H
#define INTLNODE_H

template <typename E>
// Internal node subclass
class IntlNode : public HuffNode<E> {
private:
HuffNode<E>* lc;
// Left child
HuffNode<E>* rc;
// Right child
int wgt;
// Subtree weight
public:
IntlNode(){
    lc = NULL;
    rc = NULL;
    wgt = 0;
}

IntlNode(HuffNode<E>* l, HuffNode<E>* r)
{ wgt = l->weight() + r->weight(); lc = l; rc = r; }
int weight() { return wgt; }
bool isLeaf() { return false; }
HuffNode<E>* left() const { return lc; }
void setLeft(HuffNode<E>* b)
{ lc = (HuffNode<E>*)b; }
HuffNode<E>* right() const { return rc; }
void setRight(HuffNode<E>* b)
{ rc = (HuffNode<E>*)b; }
};

#endif // INTLNODE_H
