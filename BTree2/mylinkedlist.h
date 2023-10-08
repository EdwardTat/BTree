#ifndef MYLINKEDLIST_H
#define MYLINKEDLIST_H
#include "mylinkedlistfunctions.h"


template<typename T>
class List{
    node<T> *head;

public:
    class Iterator{
    public:
        friend class List;                  //this will allow List
                                            //  to access the private
                                            //  members of Iterator
        Iterator next()                     //return an iterator to the
        {                                   //  next location in the list
            assert(_ptr);
            return Iterator(_ptr->_next);
        }

        //      casting operator: true if _ptr not NULL
        //      this turned out to be a pain!
        operator bool()
        {
            return _ptr;
        }

        //true if _ptr is NULL
        bool is_null(){
            return !_ptr;
        }

        Iterator(node<T>* p = NULL):_ptr(p){}     //ctor
        T& operator *(){                    //dereference oprator
            assert(_ptr);
            return _ptr->_item;
        }

        Iterator last()                     //return an iterator to the
        {                                   //  next location in the list
            assert(_ptr);
            return Iterator(_ptr->_last);
        }

        friend Iterator operator --(Iterator& it,       //it++
                                    int unused){
            assert(it._ptr);
            Iterator hold;
            hold = it;
            it._ptr = it._ptr->_last;
            return hold;
        }
        Iterator& operator --(){                        //++it
            assert(_ptr);
            _ptr = _ptr->_last;
            return *this;
        }

        friend Iterator operator ++(Iterator& it,       //it++
                                    int unused){
            assert(it._ptr);
            Iterator hold;
            hold = it;
            it._ptr = it._ptr->_next;
            return hold;
        }
        Iterator& operator ++(){                        //++it
            assert(_ptr);
            _ptr = _ptr->_next;
            return *this;
        }

        friend bool operator !=(const Iterator& left,   //it != it
                                const Iterator& right){
            return left._ptr != right._ptr;
        }

        friend bool operator ==(const Iterator& left,   //it != it
                                const Iterator& right){
            return left._ptr == right._ptr;
        }



    private:
        node<T>* _ptr;                    //pointer being
        //  encapsulated
    };

    List();
    List(const List<T>& other);
    ~List();
    void push_back(T data);
    T pop_front();
    void empty();

    Iterator InsertHead(T i);                           //insert at the head of list
    Iterator InsertAfter(T i, Iterator iMarker);        //insert after marker
    Iterator InsertBefore(T i, Iterator iMarker);       //insert before marker
    Iterator InsertSorted(T i);                         //insert in a sorted list

    T Delete(List<T>::Iterator iMarker);        //delete node pointed to by marker
    void Print() const;
    Iterator Search(const T &key);                      //return Iterator to node [key]
    Iterator Prev(Iterator iMarker);                            //previous node to marker

    T& operator[](int index);                           //return item at position index
    Iterator Begin() const;                                     //Iterator to the head node
    Iterator End() const;                                       //Iterator to NULL
    Iterator LastNode() const;                                  //Iterator to the last node

    template<typename U>
    friend std::ostream& operator<<( std::ostream& out, List<U> const & other);

    Num& operator+=(const Num& rhs){

          this->m_iNumber += rhs.m_iNumber;
          return *this;
    }

};

template<typename U>
std::ostream& operator<<( std::ostream& out, List<U> const & other)
{
    print_list(other.head, out);
    return out;
}


template<typename T>
List<T>::List()
{
    init_head(head);
}

template<typename T>
List<T>::List(const List<T>& other)
{
    copy_list(other.head, head);
}

template<typename T>
List<T>::~List()
{
    delete_all(head);
}

template<typename T>
void List<T>::push_back(T data)
{
    insert_head(head, data);
}


template<typename T>
T List<T>::pop_front()
{
    return delete_head(head);
}

template<typename T>
void List<T>::empty()
{
    delete_all(head);
}

//insert at the head of list
template<typename T>
typename List<T>::Iterator List<T>::InsertHead(T i)
{
    insert_head(head, i);
    return Iterator(head);
}

//insert after marker
template<typename T>
typename List<T>::Iterator List<T>::InsertAfter(T i, Iterator iMarker)
{
    insert_after(head, iMarker, i);
}

//insert before marker
template<typename T>
typename List<T>::Iterator List<T>::InsertBefore(T i, Iterator iMarker)
{
    insert_before(head, iMarker, i);
}

//insert in a sorted list
template<typename T>
typename List<T>::Iterator List<T>::InsertSorted(T i)
{
    for(node<T>* temp = head; temp; temp = temp->_next){
        if(temp->_item < i){
            return Iterator(insert_after(head, temp, i));
        } else if (!temp->next){
            return Iterator(insert_after(head, temp, i));
        }
    }
    return nullptr;

}

//delete node pointed to by marker
template<typename T>
T List<T>::Delete(List<T>::Iterator iMarker)
{
    return delete_node(*iMarker);
}

template<typename T>
void List<T>::Print() const
{
    cout << *this;
}

//return Iterator to node [key]
template<typename T>
typename List<T>::Iterator List<T>::Search(const T &key)
{
    for(node<T>* temp = head; temp; temp = temp->_next){
        if(temp->_item == key)
            return Iterator(temp);
    }
    return Iterator();
}

//previous node to marker
template<typename T>
typename List<T>::Iterator List<T>::Prev(Iterator iMarker)
{
return Iterator(iMarker._ptr->_last);
}

//return item at position index
template<typename T>
T& List<T>::operator[](int index)
{
    Iterator temp(head);
    for(int i = 0; i < index; i++){
        temp++;
    }
    return *temp;
}

//Iterator to the head node
template<typename T>
typename List<T>::Iterator List<T>::Begin() const
{
    return Iterator(head);
}

//Iterator to NULL
template<typename T>
typename List<T>::Iterator List<T>::End() const
{
    return Iterator();
}

//Iterator to the last node
template<typename T>
typename List<T>::Iterator List<T>::LastNode() const
{
    if(!head)
        return Iterator(nullptr);

    node<T>* temp = head;
    while(temp->_next)
        temp = temp->_next;
    return Iterator(temp);

}

#endif // MYLINKEDLIST_H
