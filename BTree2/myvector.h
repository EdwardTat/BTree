#ifndef MYVECTOR_H
#define MYVECTOR_H

const unsigned int minSize = 4, minCap = 4, desizeFactor = 3;

#include <iostream>
using namespace std;

template<typename T>
class myVec{
private:
    T* array;
    unsigned int cap;
    unsigned int sz;
    void resize();
    void desize();
    void copy(const myVec& other);
public:
    void deleteAll();
    myVec(myVec const &other);
    myVec(int cap = minSize);
    void push_back(const T val);
    T pop();
    unsigned int capacity() const;
    unsigned int size() const;
    bool full() const;
    bool empty() const;
    ~myVec();


    const T &operator[](const int index) const;
    T &operator[](const int index);
    myVec& operator=(const myVec& other);

    friend
    myVec<T>& operator +=(myVec<T>& list, const T& addme){
        list.push_back(addme);
//        cout << "addme returned\n";
        return list;
    }

    friend
    myVec<T>& operator +=(myVec<T>& rhs, const myVec<T>& lhs){
//        int newCap = rhs.cap + lhs.cap;
//        T *temp = new T[newCap];
//        int newSz = 0, i = 0;
//        while(rhs.sz > newSz){
//            temp[newSz] = rhs.array[newSz];
//            newSz++;
//        }
//        while(i < lhs.sz){
//            temp[newSz] = lhs.array[i];
//            i++;
//            newSz++;
//        }
//        rhs.sz = newSz;
//        rhs.cap = newCap;

//        delete[] rhs.array;

//        rhs.array = temp;
//        temp = NULL;

        for(int i = 0; i < lhs.sz; i++){
            rhs.push_back(lhs[i]);
        }

        return rhs;
    }

    friend
    ostream& operator <<(ostream& outs, const myVec<T>& list)  //print vector list
    {
        for(int i = 0; i < list.size(); i++){
//            outs << i << ". ";
            outs << " |" << list[i] << "| ";
        }
        return outs;
    }
};


template<typename T>
myVec<T>::myVec(myVec const &other)
{
    copy(other);
}

template<typename T>
myVec<T>& myVec<T>::operator=( const myVec& other)
{
    copy(other);
    return *this;
}

template<typename T>
void myVec<T>::copy(const myVec& other)
{
    sz = other.sz;
    cap = other.cap;
    array = new T[cap];
    for(int i = 0; i < other.sz; i++){
        array[i] = other.array[i];
    }
}


template<typename T>
void myVec<T>::deleteAll()
{
    for(int i = 0;i < cap; i++){
        array[i] = T();
    }
    delete[] array;
    cap = int();
    sz = int();
}

template<typename T>
myVec<T>::~myVec()
{
    deleteAll();
}


template<typename T>
void myVec<T>::resize()
{
    T* temp = new T[cap * 2];
    for(int i = 0; i < sz; i++){
        temp[i] = array[i];
    }
    delete[] array;
    array = temp;
    cap = cap * 2;
}

template<typename T>
void myVec<T>::desize()
{
    T* temp = new T[cap/desizeFactor];
    for(int i = 0; i < sz; i++){
        temp[i] = array[i];
    }
    delete[] array;
    array = temp;
    cap = cap/desizeFactor;
}




template<typename T>
myVec<T>::myVec(int capacity)
{
    if(capacity < minSize)
        capacity = minSize;
    cap = capacity;
    sz = 0;
    array = new T[capacity];
}

template <typename T>
const T& myVec<T>::operator[](const int index) const
{
    return array[index];
}

template<typename T>
T& myVec<T>::operator[](const int index)
{
    return array[index];
}


template<typename T>
void myVec<T>::push_back(const T val)
{
    if(sz >= cap){
        resize();
    }
    array[sz] = val;
    sz++;
}

template<typename T>
T myVec<T>::pop()
{
    sz--;
    if(sz < cap/minCap && cap/desizeFactor > minCap)
        desize();
    return array[sz];
}

template<typename T>
unsigned int myVec<T>::capacity() const
{
    return cap;
}

template<typename T>
unsigned int myVec<T>::size() const
{
    return sz;
}

template<typename T>
bool myVec<T>::full() const
{
    return (sz == cap);
}

template<typename T>
bool myVec<T>::empty() const
{
    return (!sz);
}

#endif // MYVECTOR_H
