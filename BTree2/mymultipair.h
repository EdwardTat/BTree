#ifndef MYMULTIPAIR_H
#define MYMULTIPAIR_H

#include <iostream>
#include "myvector.h"

using namespace std;

// Maps, they don't love you like I love you.
template <typename K, typename V>
struct MPair
{
    K key;                   // Key to search for
    myVec<V> value_list;  // Values stored in myVec


    MPair(const K& k = K())
    {
        key = k;
        value_list = myVec<V>();
    }

    MPair(const K& k, const V& v)
    {
        key = k;
        value_list = myVec<V>();
        value_list.push_back(v);
    }

    MPair(const K& k, const myVec<V>& vlist)
    {
        key = k;
        value_list = vlist;
    }

    friend ostream& operator<<(ostream& outs, const MPair<K, V>& print_me)
    {

        outs <<  print_me.key << print_me.value_list;


        return outs;
    }


    friend bool operator==(const MPair<K, V>& lhs, const MPair<K, V>& rhs)
    {
        return (lhs.key == rhs.key);
    }


    friend bool operator<(const MPair<K, V>& lhs, const MPair<K, V>& rhs)
    {
        return (lhs.key < rhs.key);
    }


    friend bool operator>(const MPair<K, V>& lhs, const MPair<K, V>& rhs)
    {
        return (lhs.key > rhs.key);
    }

    friend bool operator<=(const MPair<K, V>& lhs, const MPair<K, V>& rhs)
    {
        return ((lhs.key < rhs.key) || (lhs.key == rhs.key));
    }


    friend bool operator>=(const MPair<K, V>& lhs, const MPair<K, V>& rhs)
    {
        return ((lhs.key > rhs.key) || (lhs.key == rhs.key));
    }


    friend MPair<K, V> operator+(const MPair<K, V>& lhs, const MPair<K, V>& rhs)
        {
            assert(lhs.key == rhs.key);

            myVec<V> temp = rhs.value_list;

            temp += lhs.value_list;

            return MPair(lhs.key, temp);
        }
};

#endif // MYMULTIPAIR_H
