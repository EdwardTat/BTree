#ifndef MYPAIR_H
#define MYPAIR_H

#include <iostream>
#include <iomanip>

using namespace std;

template<typename K, typename V>
struct Pair
{
    K key;
    V value;
    Pair(const K& key, const V& value){
        this->key = key;
        this->value = value;
    }
    Pair(){}

    friend ostream& operator <<(ostream& out, const Pair<K,V>& print){
        out << "Key = " << print.key << " Value = " << print.value;
        return out;
    }

    friend bool operator ==(const Pair<K, V>& lhs, const Pair<K, V>& rhs)
        {
            return (lhs.key == rhs.key);
        }

    friend bool operator<(const Pair<K, V>& lhs, const Pair<K, V>& rhs)
        {
            return (lhs.key < rhs.key);
        }

    friend bool operator>(const Pair<K, V>& lhs, const Pair<K, V>& rhs)
        {
            return (lhs.key > rhs.key);
        }

    friend bool operator<=(const Pair<K, V>& lhs, const Pair<K, V>& rhs)
        {
            return ((lhs.key < rhs.key) || (lhs.key == rhs.key));
        }

    friend bool operator>=(const Pair<K, V>& lhs, const Pair<K, V>& rhs)
        {
            return ((lhs.key > rhs.key) || (lhs.key == rhs.key));
        }

    friend Pair<K, V> operator+(const Pair<K, V>& lhs, const Pair<K, V>& rhs)
        {
//            Pair<K,V> temp;
            return Pair(lhs.key, rhs.value);
        }
    friend Pair<K, V> operator+=(const Pair<K, V>& lhs, const Pair<K, V>& rhs)
        {
//            Pair<K,V> temp;
            return Pair(lhs.key, rhs.value);
        }

};

#endif // MYPAIR_H
