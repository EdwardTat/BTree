#ifndef MYMAP_H
#define MYMAP_H

#include "bptree.h"
#include "mypair.h"
#include <iostream>
using namespace std;

template<typename K, typename V>
class Map{
public:
    Map(){keyCount = 0;}
    int size() const;
    bool empty() const;
    bool contains(Pair<K, V> &target);
    bool is_valid() const;

    V& operator[](const K& key);
    V& at(const K &key);
    const V& operator[](const K& key) const;
    const V& at(const K& key) const;

    V &get(const K& key);
    void insert(const K& k, const V& v);
    void erase(K& key);
    void clear();

    bool erase(const K& key);


    friend ostream& operator<<(ostream& out, const Map<K, V>& source)
    {
        out << source.map << endl;
        return out;
    }

private:
    int keyCount;
    BPTree<Pair<K, V>>map;
};

template<typename K, typename V>
bool Map<K, V>::erase(const K& key)
{
    Pair<K,V> temp(key, V());
    if(map.contains(temp)){
        keyCount--;
        map.remove(temp);
        return true;
    }
    return false;
}

template<typename K, typename V>
int Map<K, V>::size() const
{
    return keyCount;
}

template<typename K, typename V>
bool Map<K, V>::empty() const
{
    return (keyCount==0);
}

template<typename K, typename V>
bool Map<K, V>::contains(Pair<K, V>& target)
{
    return map.contains(target);
}

template<typename K, typename V>
bool Map<K, V>::is_valid() const
{
    return map.valid();
}


template<typename K, typename V>
V& Map<K, V>::operator[](const K& key)
{
    return get(key);
}

template<typename K, typename V>
V &Map<K, V>::at(const K& key)
{
    return get(key);
}

template<typename K, typename V>
const V& Map<K, V>::operator[](const K& key) const
{
    return get(key);
}

template<typename K, typename V>
const V& Map<K, V>::at(const K& key) const
{
    return get(key);
}

template<typename K, typename V>
V& Map<K, V>::get(const K& key)
{
    Pair<K, V> temp (key, V());
    return map.get(temp).value;
}

template<typename K, typename V>
void Map<K, V>::insert(const K& k, const V& v)
{
    Pair<K, V> temp(k, v);
    map.insert(temp);
    keyCount++;
}

template<typename K, typename V>
void Map<K, V>::erase(K& key)
{
    map.remove(key);
    keyCount--;
}

template<typename K, typename V>
void Map<K, V>::clear()
{
    map.clear_tree();
    keyCount = 0;
}








#endif // MYMAP_H
