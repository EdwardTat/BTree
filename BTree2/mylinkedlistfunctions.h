#ifndef MYLINKEDLISTFUNCTIONS_H
#define MYLINKEDLISTFUNCTIONS_H

#include <iostream>
#include <cassert>
using namespace std;

template <class T>
struct node{
    T _item;
    node<T> *_next, *_last;
    node(const T& item = T(), node<T>* next = NULL,
         node<T>* last = NULL):_item(item), _next(next), _last(last){}
    friend ostream& operator << (ostream& outs, const node<T>& print_me){
        outs<<"["<<print_me._item<<"]<->";
        return outs;
    }
    T operator *();

};

template <class T>
T node<T>::operator *(){
    return _item;
}

//initializes head to NULL
template <class T>
node<T>* init_head(node<T>* &head)
{
    head = NULL;
    return head;
}

//deletes all the nodes in the list
template<class T>
void delete_all(node<T>*&head)
{
    for(node<T>* temp = head; head;){
        temp = head->_next;
        delete head;
        head = temp;
    }
}

//true if head is NULL, false otherwise.
template <class T>
bool empty(const node<T>* head)
{
    return !head;
}

//makes a copy of the list, returns a pointer to the last node:
template <class T>
node<T>* copy_list(const node<T>* head, node<T>* & cpy)
{
    if(head){
        cpy = new node<T>(head->_item);
        node<T> *out = cpy, *headTemp = head->_next;
        while(headTemp){
            out->_next = new node<T>(headTemp->_item, NULL, out);
            headTemp = headTemp->_next;
            out = out->_next;
        }
        return out;
    }
    else{
        return NULL;
    }
}

//insert at the beginning of the list:
template <class T>
node<T>* insert_head(node<T>* &head, T item)
{
    node<T>* temp = new node<T>(item, head);
    if(head)
    head->_last = temp;
    head = temp;
    return head;
}

template <class T>
node<T>* insert_tail(node<T>* &tail, T item){
    node<T>* temp = new node<T>(item, NULL, tail);
    if(tail)
    tail->_next = temp;
    temp->_last = tail;
    tail = temp;
    return tail;
}

//insert_after: if after is NULL, inserts at head

template <class T>
node<T>* insert_after(node<T>* &head, node<T>* after, const T& item)
{
    if(after){
        node<T> *temp = new node<T>(item, after->_next, after);
        after->_next = temp;
        if(temp->_next){
            temp->_next->_last = temp;
        }
        return temp;
    }
    else {
        node<T> *temp = new node<T>(item, head->_next, head);
        if(head)
            head->_next = head;
        if(temp->_next){
            temp->_next->_last = temp;
        }
        return temp;
    }
    return NULL;

}

template<typename T>
node<T>* insert_before(node<T>* &head, node<T>* before, const T& item){
    if(before){
        node<T> *temp = new node<T>(item, before, before->_last);
        if(temp->_last)
        temp->_last->next = temp;
        if(before)
            before->_next = temp;
        return temp;
    }
    else {
        return insert_head(head, item);
    }
    return NULL;
}


//delete the node at the head of the list, reuturn the item:
template <class T>
T delete_head(node<T>* &head)
{
    assert(head);
    T output = head->_item;
    node<T> *temp;
    temp = head;
    head = head->_next;
    delete temp;
    if(head)
    head->_last = NULL;
    return output;
}

template <class T>
T delete_tail(node<T>* &tail){
    assert(tail);
    T output = tail->_item;
    node<T> *temp = tail;
    tail = tail->_last;
    delete temp;
    if(tail){
    tail->_next = NULL;
    }
    return output;
}

//print the list and return outs
template<class T>
ostream& print_list(node<T>* head, ostream& outs)
{
    outs<<"H->";
    for(node<T>* temp = head; temp; temp = temp->_next){
        outs << *temp;
    }
    outs <<"NULL";
    return outs;
}

template<typename T>
T _peek_data(const node<T>* point){
    return point->_item;
}

#endif // MYLINKEDLISTFUNCTIONS_H
