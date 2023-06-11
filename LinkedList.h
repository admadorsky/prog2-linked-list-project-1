#ifndef PROG2_LINKED_LISTS_PROJECT_1_LINKEDLIST_H
#define PROG2_LINKED_LISTS_PROJECT_1_LINKEDLIST_H
#include <iostream>
#include <vector>

using namespace std;

template <class T> class LinkedList {
public:
    // nested class for Node objects
    struct Node {
        // node-level variables
        T data ;
        Node *prev = nullptr ;
        Node *next = nullptr ;
    };

    // constructors & big three
    // part1: DONE
    // part2:
    // part3:
    // default constructor
    LinkedList() ;
    // copy constructor
    LinkedList(const LinkedList<T> &to_copy) ;
    // copy assignment operator; operator overload for "="
    LinkedList operator=(LinkedList<T> &rhs) ;
    // operator overload for "[]"
    T &operator[](unsigned int target_index) ;
    // operator overload for "=="
    bool operator==(const LinkedList <T> &rhs) const ;
    // destructor
    ~LinkedList() ;

    // add individual items to the list
    void AddHead(const T &data) ;
    void AddTail(const T &data) ;

    // add several items to the list
    void AddNodesHead(const T *data, unsigned int count) ;
    void AddNodesTail(const T *data, unsigned int count) ;
    void InsertAfter(Node *node, const T &data) ;
    void InsertBefore(Node *node, const T &data) ;
    void InsertAt(const T &data, unsigned int index) ;

    // remove items
    bool RemoveHead() ;
    bool RemoveTail() ;
    bool RemoveAt(unsigned int index) ;
    unsigned int Remove(const T& data) ;
    void Clear() ;

    // print the list
    void PrintForward() ;
    void PrintReverse() ;
    void PrintForwardRecursive(const Node *node) const ;
    void PrintReverseRecursive(const Node *node) const ;

    // simple accessors
    unsigned int NodeCount() {return size ; }
    Node *Head() {return head ; }
    Node *Tail() {return tail ; }

    // complex accessors
    Node *GetNode(unsigned int target_index) ;
    Node *Find(T value) ;
    void FindAll(vector<Node*> &nodes, const T &value) ;

    // const versions
    const T &operator[](unsigned int target_index) const ;
    const Node *Find(T value) const ;
    const Node *GetNode(unsigned int target_index) const ;
    const Node *Head() const {return head ; }
    const Node *Tail() const {return tail ; }

private:
    // list-level variables
    Node *head = nullptr ;
    Node *tail = nullptr ;
    unsigned int size ;  // easier than iterating to track down nullptrs
};

// default constructor
template <class T> LinkedList <T>::LinkedList() {
    head = nullptr ;
    tail = nullptr ;
    size = 0 ;
}

//copy constructor
template <class T> LinkedList <T>::LinkedList(const LinkedList<T> &to_copy) {
    // create a new head node using to_copy's head data
    AddHead(to_copy.head->data) ;
    // create a new object for the 2nd - tail items
    Node *to_copy_iterator = to_copy.head->next ;
    // iterate through nodes in to_copy, adding a new tail to "this" for each one, using data from to_copy
    while (to_copy_iterator != nullptr) {
        AddTail(to_copy_iterator->data) ;
        // iterate
        to_copy_iterator = to_copy_iterator->next ;
    }

    // constructed list has the same size as to_copy
    this->size = to_copy.size ;
}

// copy assignment operator
template <class T> LinkedList<T> LinkedList<T>::operator=(LinkedList<T> &rhs) {
    Node *iterator = this->head ;
    // iterate through the existing list and de-allocate all memory
    while(iterator->next != nullptr) {
        iterator = iterator->next ;
        delete iterator->prev ;
    }
    delete iterator ;
    this->head = nullptr ;
    this->tail = nullptr ;

    // create a new head node using rhs's head data
    AddHead(rhs.head->data) ;
    // create a new object for the 2nd - tail items
    Node *to_copy_iterator = rhs.head->next ;
    // iterate through nodes in rhs, adding a new tail to "this" for each one, using data from rhs
    while (to_copy_iterator != nullptr) {
        AddTail(to_copy_iterator->data) ;
        // iterate
        to_copy_iterator = to_copy_iterator->next ;
    }

    // lhs and rhs have the same size
    this->size = rhs.size ;

    return *this ;
}

// operator overload for []: used to access data at a certain index of the list
template <class T> T& LinkedList<T>::operator[](unsigned int target_index) {
    // access the node at the target index, and return its data
    return GetNode(target_index)->data ;
}

// operator overload for ==: used to compare two linked list's values
template <class T> bool LinkedList<T>::operator==(const LinkedList<T> &rhs) const {
    // the lists must be the same size for us to begin checking if they are equivalent
    if (this->size == rhs.size) {
        // start at the beginning of both lists
        Node *this_current_node = this->head ;
        Node *rhs_current_node = rhs.head ;
        // iterate to the end of the list
        while (this_current_node != nullptr) {
            // check if the current data differs between lists
            if (this_current_node->data != rhs_current_node->data) {
                // if it is different, then the lists are not equivalent
                return false ;
            } else {
                // if the data matches, then we iterate to the next node for each list
                this_current_node = this_current_node->next ;
                rhs_current_node = rhs_current_node->next ;
            }
        }
        // if we made it through the whole loop without returning false, the lists must be equivalent
        return true ;
    }
    // if the lists are different sizes, they cannot be equivalent
    return false ;
}

// destructor
template <class T> LinkedList <T>::~LinkedList() {
    Node *temp = this->head ;
    while (temp != nullptr) {
        this->head = this->head->next ;
        delete temp ;
        temp = this->head ;
    }
}

template <class T> void LinkedList <T>::AddHead(const T &data) {
    // initialize a new head with all associated pointers
    Node *new_head = new Node ;
    new_head->data = data ;
    // as head, there is no prev item
    new_head->prev = nullptr ;

    // make sure there is an ex-head to update, and to point to as the next item
    if (head != nullptr) {
        // point to the ex-head as next
        new_head->next = this->head;
        // update the ex-head's "prev" pointer
        this->head->prev = new_head ;
    } else { // otherwise, the node we're adding is the only node in the list
        // new node is head and tail; i.e. no prev or next items
        new_head->next = nullptr ;
        this->tail = new_head ;
    }

    // update list-level variables
    this->head = new_head ;
    this->size++ ;
}

template <class T> void LinkedList <T>::AddTail(const T &data) {
    // initialize a new tail with all associated pointers
    Node *new_tail = new Node ;
    new_tail->data = data ;
    // as tail, there is no next item
    new_tail->next = nullptr ;

    // make sure there is an ex-tail to update, and to point to as the prev item
    if (tail != nullptr) {
        // point to the ex-tail as prev
        new_tail->prev = this->tail ;
        // as tail, there is no next
        new_tail->next = nullptr ;
        // update the ex-tail's "next" pointer
        this->tail->next = new_tail ;
    } else { // otherwise, the node we're adding is the only node in the list
        // new node is head and tail; i.e. no prev or next items
        new_tail->prev = nullptr ;
        this->head = new_tail ;
    }

    // update list-level variables
    this->tail = new_tail ;
    this->size++ ;
}

// add several nodes to the head based on an array of data, iterating backwards to maintain their order
template <class T> void LinkedList <T>::AddNodesHead(const T *data, unsigned int count) {
    for (unsigned int i = count; i > 0; i--) {
        this->AddHead(data[i - 1]) ;
    }
}

// add several nodes to the tail based on an array of data
template <class T> void LinkedList <T>::AddNodesTail(const T *data, unsigned int count) {
    for (unsigned int i = 0; i < count; i++) {
        this->AddTail(data[i]) ;
    }
}

template <class T> void LinkedList <T>::InsertAfter(LinkedList::Node *node, const T &data) {
    // check if the new node is the tail
    if (node == this->tail) {
        AddTail(data) ;
    // otherwise, we are inserting elsewhere in the list, so the new node has prev AND next
    } else {
        // create a new node with all associated pointers
        Node *new_node = new Node ;
        new_node->data = data ;
        new_node->prev = node ;
        new_node->next = node->next ;
        // readdress pointers for objects on either side of the new node
        node->next = new_node ;
        new_node->next->prev = new_node;
        // increment size
        this->size ++ ;
    }
}

template <class T> void LinkedList <T>::InsertBefore(LinkedList::Node *node, const T &data) {
    // check if the new node is the head
    if (node == this->head) {
        AddHead(data) ;
    // otherwise, we are inserting elsewhere in the list, so the new node has prev AND next
    } else {
        // create a new node with all associated pointers
        Node *new_node = new Node ;
        new_node->data = data ;
        new_node->next = node ;
        new_node->prev = node->prev ;
        // readdress pointers for objects on either side of the new node
        node->prev = new_node ;
        new_node->prev->next = new_node ;
        // increment size
        this->size ++ ;
    }
}

template <class T> void LinkedList <T>::InsertAt(const T &data, unsigned int index) {
    // check if we are inserting at the end of the list
    if (index == size) {
        AddTail(data) ;
    // otherwise, we are inserting somewhere in the middle of the list, or at the head; both handled by InsertBefore()
    } else {
        // find what is currently stored at the target index
        Node *node = GetNode(index) ;
        // insert a new node before it with the passed data... this "takes over" the target index
        InsertBefore(node, data) ;
    }
}

template <class T> bool LinkedList <T>::RemoveHead() {
    // check if the list is empty
     if (this->size < 1) {
        // can't remove the head of an empty list
        return false ;
    }
    // store pointers that will be affected
    Node* new_head = this->head->next ;
    Node* old_head = this->head ;
    // readdress the list's head
    delete old_head ;
    // check if we are removing the last item
    if (new_head == nullptr) {
        // send the list to hell
        this->size -- ;
        this->head = nullptr ;
        this->tail = nullptr ;
        return true ;
    }
    // make sure the new head doesn't point to anything previous
    new_head->prev = nullptr ;
    this->head = new_head ;
    // we successfully removed the head
    this->size -- ;
    return true ;
}

template <class T> bool LinkedList <T>::RemoveTail() {
    // check if the list is empty
    if (this->tail == nullptr) {
        // can't remove the tail of an empty list
        return false ;
    }
    // store pointers that will be affected
    Node* new_tail = this->tail->prev ;
    Node* old_tail = this->tail ;
    // readdress the list's head
    delete old_tail ;
    // check if we are removing the last item
    if (new_tail == nullptr) {
        // send the list to hell
        this->size -- ;
        this->head = nullptr ;
        this->tail = nullptr ;
        return true ;
    }
    // make sure the new tail doesn't point to anything next
    new_tail->next = nullptr ;
    this->tail = new_tail ;
    // we successfully removed the tail
    this->size -- ;
    return true ;
}

template <class T> bool LinkedList<T>::RemoveAt(unsigned int index) {
    // make sure the index of the object to remove is in-bounds
    if (index + 1 > size) {
        return false ;
    }
    // cannot remove from an empty list
    if (size < 1) {
        return false ;
    }
    Node* node = GetNode(index) ;
    // edge case if the node to remove is the head
    if (node == this->head) {
        RemoveHead() ;
        return true ;
    // edge case if the node to remove is the tail
    } else if (node == this->tail) {
        RemoveTail() ;
        return true ;
    }
    /* otherwise, the node to remove is somewhere else in the list
    readdress the surrounding nodes' pointers */
    node->prev->next = node->next ;
    node->next->prev = node->prev ;
    delete node ;
    // we successfully removed the target node
    this->size -- ;
    return true ;
}

template <class T> unsigned int LinkedList <T>::Remove(const T &data) {
    unsigned int removed_count = 0 ;
    // check if the list is empty
    if (this->size < 1) {
        // if so, we did not find any items with data to remove
        return removed_count ;
    }
    // if the list is not empty, we iterate through every node
    Node *current_node = this->head ;
    while (current_node != nullptr) {
        // save the next pointer, b/c we will lose it with de-allocation
        Node *temp = current_node->next ;
        // check if the current node's data matches the data we're looking to remove
        if (current_node->data == data) {
            // test for edge cases
            if (current_node == this->head) {
                RemoveHead() ;
            } else if (current_node == this->tail) {
                RemoveTail() ;
            // otherwise, the node for deletion is not first or last in the list
            } else {
                // "skip" the current node with its neighbors' pointers
                current_node->prev->next = current_node->next;
                current_node->next->prev = current_node->prev;
                // get rid of the current node
                delete current_node ;
                this->size -- ;
            }
            removed_count++;
        }
        // iterate to the next node
        current_node = temp ;
    }
    // return the number of nodes removed
    return removed_count ;
}

template <class T> void LinkedList<T>::Clear() {
    while (RemoveHead()) {}
    this->size = 0 ;
    this->head = nullptr ;
    this->tail = nullptr ;
}

template <class T> void LinkedList <T>::PrintForward() {
    // start printing from the head
    Node *current_node = this->head ;
    // iterate through all nodes forwards, by using next
    while (current_node != nullptr) {
        // print the current node
        cout << current_node->data << endl ;
        current_node = current_node->next ;
    }
}

template <class T> void LinkedList <T>::PrintReverse() {
    // start printing from the tail
    Node *current_node = tail ;
    // iterate through all nodes in reverse, by using prev
    while (current_node != nullptr) {
        // print the current node
        cout << current_node->data << endl ;
        current_node = current_node->prev ;
    }
}

template <class T> void LinkedList <T>::PrintForwardRecursive(const Node *node) const {
    // whack ass const hijack to get an editable pointer
    Node* current_node = const_cast <Node*> (node) ;
    // make sure we aren't at the end of the list
    if (current_node != nullptr) {
        // print current data
        cout << current_node->data << endl ;
    } else {return ;}
    // "iterate" to the next node
    PrintForwardRecursive(current_node->next) ;
}

template <class T> void LinkedList <T>::PrintReverseRecursive(const Node *node) const {
    // whack ass const hijack to get an editable pointer
    Node* current_node = const_cast <Node*> (node) ;
    // make sure we aren't at the beginning of the list
    if (current_node != nullptr) {
        // print current data
        cout << current_node->data << endl ;
    } else {return ;}
    // "iterate" to the previous node
    PrintReverseRecursive(current_node->prev) ;
}

// accessor function for a node's data by iterating through the list
template <class T> typename LinkedList<T>::Node* LinkedList <T>::GetNode(unsigned int target_index) {
    // check if the passed index is within bounds
    if (target_index + 1 > size) {
        throw out_of_range("Target index is out of range for the linked list") ;
    }
    // traverse the list to find the right index
    unsigned int i = 0 ;
    Node *current_pointer = this->head ;
    while (current_pointer != nullptr) {
        // check if we reached the target index
        if (i == target_index) {
            // return the pointer to the node we want
            return current_pointer;
        } else {
            // iterate to the next node in the list
            i++;
            current_pointer = current_pointer->next;
        }
    }
    return nullptr ;
}

template <class T> typename LinkedList<T>::Node* LinkedList <T>::Find(T value) {
    // start with the first node; head
    Node *current_node = this->head ;
    while (current_node != nullptr) {
        // check if the current node has the value we are looking for
        if (current_node->data == value) {
            return current_node ;
        } else {
            // iterate to the next node
            current_node = current_node->next ;
        }
    }
    // if we end up here, no node with the passed value was found
    return nullptr ;
}

template <class T> void LinkedList<T>::FindAll(vector<Node*> &nodes, const T &value) {
    // iterate through the list
    Node *current_node = this->head ;
    while (current_node != nullptr) {
        // check if the current node's data matches the target data
        if (current_node->data == value) {
            // add the node's pointer to output vector
            nodes.push_back(current_node) ;
        }
        // iterate to the next node
        current_node = current_node->next ;
    }
}

// ----------------------------------------------- CONST VERSIONS ------------------------------------------------------

// const version of operator overload for []
template <class T> const T& LinkedList<T>::operator[](unsigned int target_index) const {
    // access the node at the target index, and return its data
    return GetNode(target_index)->data ;
}

template <class T> const typename LinkedList<T>::Node* LinkedList <T>::Find(T value) const {
    // start with the first node; head
    Node *current_node = this->head ;
    while (current_node != nullptr) {
        // check if the current node has the value we are looking for
        if (current_node->data == value) {
            return current_node ;
        } else {
            // iterate to the next node
            current_node = current_node->next ;
        }
    }
    // if we end up here, no node with the passed value was found
    return nullptr ;
}

// accessor function for a node's data by iterating through the list
template <class T> const typename LinkedList<T>::Node* LinkedList <T>::GetNode(unsigned int target_index) const {
    // check if the passed index is within bounds
    if (target_index + 1 > size) {
        throw out_of_range("Target index is out of range for the linked list") ;
    }
    // traverse the list to find the right index
    unsigned int i = 0 ;
    Node *current_pointer = this->head ;
    while (current_pointer != nullptr) {
        // check if we reached the target index
        if (i == target_index) {
            // return the pointer to the node we want
            return current_pointer;
        } else {
            // iterate to the next node in the list
            i++;
            current_pointer = current_pointer->next;
        }
    }
}

#endif //PROG2_LINKED_LISTS_PROJECT_1_LINKEDLIST_H