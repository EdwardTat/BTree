// Clean up multimap iterator implementation, base it off BPTree iterator

#ifndef MYMULTIMAP_H
#define MYMULTIMAP_H

#include "myvector.h"
#include "bptree.h"
#include "mymultipair.h"

template <typename K, typename V>
class MMap
{
public:

    class Iterator{
    public:
        friend class MMap;
//        Iterator(typename BPTree<MPair<K, V>>::Iterator _it=NULL, int _key_ptr = 0):key_ptr(_key_ptr)
//        {
//            it = typename BPTree<MPair<K, V>>::Iterator(_it);
//        }

        Iterator(MMap* _it=NULL, int _key_ptr = 0):key_ptr(_key_ptr)
        {
            if(_it){
                it = NULL;
                return;
            }
            it = typename BPTree<MPair<K, V>>::Iterator(&_it->mmap);
        }
//        Iterator(){
//            it = NULL;
//            key_ptr = 0;
//        }



        V operator *(){
            //            assert(!it.is_null());
            return (*it).value_list[key_ptr];
        }

        //        MPair<K,V> at(int i)
        //        {
        //            assert(key_ptr<it->data_count);
        //            return it->data[key_ptr]->value_list[i];
        //        }


        Iterator operator++(int)
        {

            if(it.is_null())
                return *this;

            if((*it).value_list.size() <= key_ptr + 1){
                it++;
                key_ptr = 0;
            }
            else
                key_ptr++;

            return *this;
        }

        // TEST THIS
        Iterator operator++()
        {
            if(it.is_null())
                return *this;

            if((*it).value_list.size() <= key_ptr + 1){
                it++;
                key_ptr = 0;
            }
            else
                key_ptr++;

            return *this;
        }

        friend bool operator ==(const Iterator& lhs, const Iterator& rhs){
            return (lhs.it == rhs.it && lhs.key_ptr == rhs.key_ptr);
        }

        friend bool operator !=(const Iterator& lhs, const Iterator& rhs){
            return (!(lhs.it == rhs.it && lhs.key_ptr == rhs.key_ptr));
        }
        void print_Iterator(){
            cout << *it;
        }
        bool is_null(){return it.is_null();}






    private:
        typename BPTree<MPair<K, V>>::Iterator it;
        int key_ptr;
    };

    Iterator lower_bound(const K& key)
    {
        Iterator out;
        out.it = mmap.lower_bound(key);
        out.key_ptr = 0;
        return out;
    }

    Iterator upper_bound(const K& key)
    {
        Iterator out;
        out.it = mmap.upper_bound(key);
        out.key_ptr = 0;
        return out;
    }

    Iterator begin(){

        MMap::Iterator out;
        out.it = mmap.begin();
        out.key_ptr = 0;
        //        cout << mmap << endl;
        return out;
    }

    Iterator end(){
        MMap::Iterator it(mmap.end());
        return it;
    }


    MMap(){keyCount = 0; mmap = BPTree<MPair<K, V> >(true);}
    const myVec<V>& operator[](const K& key) const;
    myVec<V>& operator[](const K& key);
    void insert(const K& key, const V& value);
    bool contains(const K& key);
    myVec<V> &get(const K& key);
    const myVec<V> &get(const K& key) const;
    bool erase(const K& key);

    BPTree<V> greaterThanKey(K key);
    BPTree<V> greaterThanOrEqualKey(K key);

    BPTree<V> lessThanKey(K key);
    BPTree<V> lessThanOrEqualKey(K key);

    friend ostream& operator<<(ostream& out, const MMap<K, V>& print)
    {
        out << print.mmap << endl;
        return out;
    }




private:
    int keyCount;
    BPTree<MPair<K, V> > mmap;
};

template <typename K, typename V>
BPTree<V> MMap<K, V>::lessThanKey(K key)
{
    BPTree<V> out;
    typename BPTree<MPair<K, V>>::Iterator it = mmap.begin();

    while((!it.is_null()) && *it < key){
        for(int i = 0, j = (*it).value_list.size(); i < j; i++){
            out.insert((*it).value_list[i]);
        }
        it++;
    }
    return out;
}

template <typename K, typename V>
BPTree<V> MMap<K, V>::lessThanOrEqualKey(K key)
{
    BPTree<V> out;
    typename BPTree<MPair<K, V>>::Iterator it = mmap.begin();

    while((!it.is_null()) && *it < key){
        for(int i = 0, j = (*it).value_list.size(); i < j; i++){
            out.insert((*it).value_list[i]);
        }
        it++;
    }
    return out;
}



template <typename K, typename V>
BPTree<V> MMap<K, V>::greaterThanKey(K key)
{
    BPTree<V> out;
    //    cout << "MMAP IS EQUAL TO:\n" << mmap << endl;
    //    cout << "LARGEST = " << mmap.get_largest() << endl;
    //    cout << "key = " << key << endl;
    //    cout << "(mmap.get_largest() <= key) = " << (mmap.get_largest() <= key) << endl;
    if(mmap.get_largest() <= key)
        return out;
    typename BPTree<MPair<K, V>>::Iterator it = mmap.upper_bound(key);
    if(it.is_null()){
        //        cout << "from mmap it is null\n";
        return out;
    }
    while(it != mmap.end() && (!it.is_null()))
    {
        //        cout << "*it = " << *it << endl;
        //        cout << "(*it).value_list.size() = " << (*it).value_list.size() << endl;
        for(int i = 0, j = (*it).value_list.size(); i < j; i++){
            out.insert((*it).value_list[i]);
        }
        it++;
    }
    if(!it.is_null())
        for(int i = 0, j = (*it).value_list.size(); i < j; i++){
            out.insert((*it).value_list[i]);
        }


    return out;
}

template <typename K, typename V>
BPTree<V> MMap<K, V>::greaterThanOrEqualKey(K key)
{
    BPTree<V> out;
    typename BPTree<MPair<K, V>>::Iterator it = mmap.lower_bound(key);
    while(!it.is_null())
    {
        for(int i = 0, j = (*it).value_list.size(); i < j; i++){
            out.insert((*it).value_list[i]);
        }
        it++;
    }


    return out;

}

//template <typename K, typename V>
//BPTree<V> MMap<K, V>::greaterThanOrEqualKey(K key)
//{
//    BPTree<V>out;
//    BPTree<V>::Iterator it = this->lower_bound(key);
//    while(!it.is_null())
//    {
//        out.insert(*it);
//        it++;
//    }
//    return out;
//}

template <typename K, typename V>
bool MMap<K, V>::erase(const K& key)
{
    MPair<K,V> temp(key, myVec<V>());
    if(mmap.contains(temp)){
        mmap.remove(temp);
        keyCount--;
        return true;
    }
    return false;

}


template <typename K, typename V>
const myVec<V>& MMap<K, V>::operator[](const K& key) const
{
    return get(key);
}

template <typename K, typename V>
myVec<V>& MMap<K, V>::operator[](const K& key)
{

    return get(key);
}

template <typename K, typename V>
void MMap<K, V>::insert(const K& key, const V& value)
{
    MPair<K, V> temp(key, value);

    if(mmap.contains(key)){
        keyCount++;
        mmap.get(key).value_list.push_back(value);
    }
    else
        mmap.insert(temp);
}

template <typename K, typename V>
bool MMap<K, V>::contains(const K& key)
{
    return mmap.contains(key);
}

template <typename K, typename V>
myVec<V>& MMap<K, V>::get(const K& key)
{
    if(mmap.contains(key)){
        return mmap.get(key).value_list;
    }
    else{
        MPair<K,V> temp(key, myVec<V>());
        mmap.insert(temp);
        keyCount++;
    }
    return mmap.get(key).value_list;
}






#endif // MYMULTIMAP_H
