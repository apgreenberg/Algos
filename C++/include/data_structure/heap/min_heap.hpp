/*
    Min heap
    ------------------
    A minheap is an organizational tool, structured like a complete binary tree,
    where the current node has the smallest value and it's right and left children must be equal to or larger than it.
    Eg        2
            /   \
           4      6
          / \    / \
         8   5  9   12
        / \
       13  10
    The structure is represented as a vector where the 0th index is the root,
    the 1st and 2nd indeces are the left and right children respectively.
    The left child of any node (if it exists) is at index 2x+1 where x is the index of the current node.
    the right child of any node (if it exists) is at index 2x+2 where x is the index of the current node.
    <2, 4, 6, 8, 5, 9, 12, 13, 10> is the vector representation of the min heap above.

    The benefit of a min_heap is that the smallest element can always be found in O(1) time.
*/

#ifndef MIN_HEAP_HPP
#define MIN_HEAP_HPP

#include <climits>
#include <vector>
#include <iostream>

using std::cout;
using std::endl;

using std::vector;

const int ERROR_VAL = INT_MIN;

/*
    ===========================================================================
    Node
    ===========================================================================
    Helper class for the nodes in the list.
*/

template<class T>
class Node {
    T value;
    Node(void);

public:
    Node(const T&);
    Node(const Node&);
    Node<T>& operator=(const Node<T>&);
    ~Node();
    T& get_value(void);
    void set_value(const T&);

    // inline friend functions:
    friend bool operator==(const Node& lhs, const Node& rhs) {
        return lhs.value == rhs.value;
    }
    friend bool operator!=(const Node& lhs, const Node& rhs) {
        return !operator==(lhs, rhs);
    }
    friend bool operator>=(const Node& lhs, const Node& rhs) {
        return !operator<(lhs, rhs);
    }
    friend bool operator<=(const Node& lhs, const Node& rhs) {
        return !operator>(lhs, rhs);
    }
    friend bool operator>(const Node& lhs, const Node& rhs) {
        return lhs.value > rhs.value;
    }
    friend bool operator<(const Node& lhs, const Node& rhs) {
        return operator>(rhs,lhs);
    }
};

/*
    Constructor
    -----------
*/

template<class T>
Node<T>::Node(const T& value) {
    this->value = value;
}


/*
    Copy constructor
    ----------------
*/

template<class T>
Node<T>::Node(const Node<T>& n) {
    this->value = n.value;
}


/*
    Assignment operator
    -------------------
    This copies the RHS (right-hand-side value) to 'this' instance
*/

template<class T>
Node<T>& Node<T>::operator=(const Node<T>& rhs) {
    this->value = rhs.value;
    return *this;
}


/*
    Destructor
    ----------
*/

template<class T>
Node<T>::~Node() {
}


/*
    Getters and setters
    -------------------
*/

template<class T>
T& Node<T>::get_value(void) {
    return this->value;
}

template<class T>
void Node<T>::set_value(const T& other) {
    this->value = other;
}

/*
    ===========================================================================
    MinHeap
    ===========================================================================
*/
template<class T>
class MinHeap {
private:
  vector<Node<T>*>* heap;
  size_t findIndex(const T&);
  bool hasChildren(size_t index);
  size_t maxPriorityChild(size_t index);
  void heapifyDown(size_t index);
  void heapifyUp(size_t index);
public:
  MinHeap();
  MinHeap(vector<T> toInsert);
  ~MinHeap();
  void insertValue(const T&);
  void removeValue(const T&);
  T returnMin();
  size_t size() {return heap->size();}
  vector<T> returnElems();
};

/*
    Constructors
    -------------------
*/

template<class T>
MinHeap<T>::MinHeap() {
  heap = new vector<Node<T>*>();
}

/*
  Build a heap from an existing vector of ints
  Heapify down is called on all non-leaf nodes.

  Time Complexity:

  Worst Case: O(n) + O(n/2)
  Average Case: O(n) + O(n/2)
*/
template<class T>
MinHeap<T>::MinHeap(vector<T> toInsert) {
  heap = new vector<Node<T>*>();
  for (T value : toInsert) {
    Node<T>* newNode = new Node<T>(value);
    heap->push_back(newNode);
  }
  for (size_t i = (heap->size() / 2) - 1; i > 0; i--) {
    heapifyDown(i);
  }
  heapifyDown(0);
}

/*
    Destructors
    -------------------
*/
template<class T>
MinHeap<T>::~MinHeap() {
  for (auto elem : *heap) {
    delete elem;
  }
  delete heap;
}

/*
    Helper function to find the index of a node in the array.
    If multiple nodes with the same value exist, it will return the one with the lowest index.
    If node does not exist, function returns the size of the heap (value out of bounds)
    This algorithm uses linear search

    Time Complexity:

    Worst time: O(n)
    Average time: O(n)
    -------------------
*/
template<class T>
size_t MinHeap<T>::findIndex(const T& search) {
  for (size_t i = 0; i < heap->size(); i++) {
    if (heap->at(i)->get_value() == search) {
      return i;
    }
  }
  return heap->size();
}

/*
  hasChildren
  Returns true if the node at the current index has at least one child, false if not
*/
template<class T>
bool MinHeap<T>::hasChildren(size_t index) {
  if (index * 2 + 1 < heap->size()) {
    return true;
  }
  return false;
}


/*
  Helper function to return the maxPriorityChild, the child of the current node that has
  the smallest value, eg the max priority.
  If the current node only has one child, it returns that child.
  Assumes that the current node has at least one child.
*/
template<class T>
size_t MinHeap<T>::maxPriorityChild(size_t index) {
  size_t leftChild = index * 2 + 1;
  size_t rightChild = index * 2 + 2;
  if (rightChild == heap->size()) {
    return leftChild; //if node has only one child (the left child) return it
  }
  if (*(heap->at(leftChild)) < *(heap->at(rightChild))) {
    return leftChild;
  } else {
    return rightChild;
  }
}

/*
    heapifyDown
    Recursively sends the top node down the tree and swaps places until it is in the correct place.

    Time Complexity:

    Worst Case: O(log(n))
    Average Case: O(log(n))
    -------------------
*/
template<class T>
void MinHeap<T>::heapifyDown(size_t index) {
  if (hasChildren(index)) {
    size_t child = maxPriorityChild(index);
    if (*(heap->at(index)) > *(heap->at(child))) {
      std::swap(heap->at(index), heap->at(child));
      heapifyDown(child);
    }
  }
}

/*
    heapifyUp
    Recursively builds the heap by sending values upward.

    Time Complexity:

    Worst Case: O(log(n))
    Average Case: O(log(n))
    -------------------
*/
template<class T>
void MinHeap<T>::heapifyUp(size_t index) {
  if (index == 0) {
    return;
  }
  size_t parent = (index - 1)/ 2;
  if (*(heap->at(index)) < *(heap->at(parent))) {
    std::swap(heap->at(index), heap->at(parent));
    heapifyUp(parent);
  }
}

/*
  insertValue
  inserts a new value into the heap by adding it to the end and calling heapifyUp to
  maintain heap property

  Time Complexity:

  Worst Case: O(log(n))
  Average Case: O(log(n))
*/
template<class T>
void MinHeap<T>::insertValue(const T& value) {
  Node<T>* newNode = new Node<T>(value);
  heap->push_back(newNode);
  heapifyUp(heap->size() - 1);
}

/*
  removeValue
  Removes value from the heap while keeping the heap property by swapping it with the last item
  and calling heapifyDown and heapifyUp on that index.
  If there are duplicates of values, the function only removes the one with the lower
  index.
  If the value does not exist in the heap, this function does nothing.
  HeapifyUp and HeapifyDown are called but only one (or possibly neither) will actually result in any changes, depending on
  whether the current value is too far down or up the tree.

  Time Complexity:

  Worst Case: O(n) + O(log(n))
  Average Case: O(n) + O(log(n))
*/
template<class T>
void MinHeap<T>::removeValue(const T& value) {
  size_t indexToRemove = findIndex(value);
  if (indexToRemove == heap->size()) {
    return;
  }
  std::swap(heap->at(indexToRemove), heap->at(heap->size() - 1));
  delete heap->at(heap->size() - 1);
  heap->erase(heap->end() - 1);
  heapifyDown(indexToRemove);
  heapifyUp(indexToRemove);
}

/* returnMin
  return the minimum value of the heap (aka the root)

  Time Complexity:

  Worst Case: O(1)
  Average Case: O(1)
*/
template<class T>
T MinHeap<T>::returnMin() {
  return heap->at(0)->get_value();
}

/* returnElems
  Returns a vector of just the elements in order.
  Used for testing
*/
template<class T>
vector<T> MinHeap<T>::returnElems() {
  vector<T> elems;
  for (auto elem : *heap) {
    elems.push_back(elem->get_value());
  }
  return elems;
}
#endif  // MIN_HEAP_HPP
