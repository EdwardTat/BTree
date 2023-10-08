#ifndef BTREE_H
#define BTREE_H

#include <iostream>
#include <iomanip>
#include "tempfunctions.h"
using namespace std;

template <class T>
class BTree
{
public:
    BTree(bool dups = false);
    //big three:
    BTree(const BTree<T>& other);
    ~BTree();
    BTree<T>& operator =(const BTree<T>& RHS);
    BTree<T>& operator=(const T& other){
        if(!this->empty())
            this->clear_tree();
        copy_tree(other);
        return this;
    }

    void insert(const T& entry);                //insert entry into the tree
    void remove(const T& entry);                //remove entry from the tree

    void clear_tree();                          //clear this object (delete all nodes etc.)
    void copy_tree(const BTree<T>& other);      //copy other into this object

    bool contains(const T& entry);              //true if entry can be found in the array
    T& get(const T& entry);                     //return a reference to entry in the tree
    T* find(const T& entry);                    //return a pointer to this key. NULL if not there.

    int size() const;                           //count the number of elements in the tree
    bool empty() const{return (child_count + data_count == 0);}  //true if the tree is empty

    void print_tree(int level = 0, ostream &outs=cout) const; //print a readable version of the tree
    friend ostream& operator<<(ostream& outs, const BTree<T>& print_me){
        print_me.print_tree(0, outs);
        return outs;
    }
    bool is_valid();
    void alignEdges(BTree<T> *ptr);
private:
    static const int MINIMUM = 1;
    static const int MAXIMUM = 2 * MINIMUM;

    bool dups_ok;                                   //true if duplicate keys may be inserted
    int data_count;                                 //number of data elements
    T data[MAXIMUM + 1];                            //holds the keys
    int child_count;                                //number of children
    BTree* subset[MAXIMUM + 2];                     //subtrees

    bool is_leaf() const {return child_count==0;}   //true if this is a leaf node



    //insert element functions
    void loose_insert(const T& entry);              //allows MAXIMUM+1 data elements in the root
    void fix_excess(int i);                         //fix excess of data elements in child i

    //remove element functions:
    void loose_remove(const T& entry);              //allows MINIMUM-1 data elements in the root
    void fix_shortage(int i);                       //fix shortage of data elements in child i

    void remove_biggest(T& entry);                  //remove the biggest child of this tree->entry
    void rotate_left(int i);                        //transfer one element LEFT from child i
    void rotate_right(int i);                       //transfer one element RIGHT from child i
    void merge_with_next_subset(int i);             //merge subset i with subset i+1

};





template<typename T>
BTree<T>::BTree(const BTree<T>& other)
{
    this->copy_tree(other);
}


template<typename T>
bool BTree<T>::is_valid()
{
    bool temp;
    for(int i = 0; i < child_count; i++){
        temp = subset[i]->is_valid();
        if(!temp)
            return false;
    }
    if((!data_count) && (!child_count))
        return true;
    if(data_count > MAXIMUM)
        return false;
    if(data_count < MINIMUM){
        return false;
    }
    return true;
}


template<typename T>
void BTree<T>::merge_with_next_subset(int i)
{
    //    cout << "this is my tree:\n\n";
    //    print_tree();
    T temp;
    BTree<T> *btemp;
    delete_item(data, i, data_count, temp);

    attach_item(subset[i]->data, subset[i]->data_count, temp);
    merge(subset[i]->subset, subset[i]->child_count, subset[i + 1]->subset,
            subset[i + 1]->child_count);
    merge(subset[i]->data, subset[i]->data_count, subset[i + 1]->data,
            subset[i + 1]->data_count);
    delete_item(subset, i + 1, child_count, btemp);
    delete btemp;
}


template<typename T>
void BTree<T>::rotate_right(int i)
{
//    cout << "rotate right\n";
    T temp;
    ordered_insert(subset[i]->data, subset[i]->data_count, data[0]);
    delete_item(data, 0, data_count, temp);
    ordered_insert(data, data_count, subset[i - 1]->data[data_count - 1]);
    subset[i - 1]->data_count--;
    if (!subset[i]->is_leaf())
    {
        BTree<T>* temp;
        detach_item(subset[i]->subset, subset[i]->child_count, temp);
        insert_item(subset[i + 1]->subset, 0, subset[i + 1]->child_count, temp);
    }

    //    if(!subset[i]->data && !subset[i]-> child_count){
    //        BTree<T> *btemp;
    //        delete_item(subset, i, child_count, btemp);
    //        delete btemp;
    //    }

}

template<typename T>
void BTree<T>::rotate_left(int i)
{
//    cout << "rotate left\n";
    T temp;
    ordered_insert(subset[i]->data, subset[i]->data_count, data[data_count - 1]);
    data_count--;
    ordered_insert(data, data_count, subset[i + 1]->data[0]);
    delete_item(subset[i + 1]->data, 0, subset[i + 1]->data_count, temp);
    if (!subset[i]->is_leaf())
    {
        BTree<T>* temp;
        delete_item(subset[i]->subset, 0, subset[i]->child_count, temp);
        attach_item(subset[i - 1]->subset, subset[i - 1]->child_count, temp);
    }
    //    if(!subset[i]->data && !subset[i]-> child_count){
    //        BTree<T> *btemp;
    //        delete_item(subset, i, child_count, btemp);
    //        delete btemp;
    //    }
}

template<typename T>
void BTree<T>::fix_shortage(int i)
{
    if(subset[i]->data_count < MINIMUM){
        if(i - 1 > 0 && subset[i - 1]->data_count > MINIMUM){
            rotate_right(i);
        } else if(i + 1 < child_count && subset[i + 1]->data_count > MINIMUM){
            rotate_left(i);
        } else if(i > 0){
            merge_with_next_subset(i - 1);
        } else if (i < child_count - 1){
            merge_with_next_subset(i);
        }
    }

}


template<typename T>
void BTree<T>::remove_biggest(T& entry)
{
    if(is_leaf())
        detach_item(data, data_count, entry);
    else
    {
        subset[child_count - 1]->remove_biggest(entry);
        fix_shortage(child_count - 1);
    }
}


template <typename T>
void BTree<T>::remove(const T& entry)
{
    loose_remove(entry);
//    cout << "loose remove resolved\n";
//    print_tree();
//    cout << endl << endl;
    if(data_count == 0 && child_count == 1){
//        cout << "advanced called\n";
        BTree<T> *temp = subset[0];
        for(int i = 0; i < temp->child_count; i++)
            subset[i] = temp->subset[i];
        for(int i = 0; i < temp->data_count; i++)
            data[i] = temp->data[i];
        child_count = temp->child_count;
        data_count = temp->data_count;
        delete temp;
    }
}



template <typename T>
void BTree<T>::loose_remove(const T& entry)
{

    int i = first_ge(data, data_count, entry);
    bool found = (data[i] == entry);

    if(found && is_leaf()){
        delete_item(data, i, data_count, entry);
    }
    else if(found && (child_count))
    {
        T temp;
        delete_item(data, i, data_count, entry);
        subset[i]->remove_biggest(temp);
        ordered_insert(data, data_count, temp);
        fix_shortage(i);
    }
    else if(!found && is_leaf())
    {
        return;
    }
    else
    {
        subset[i]->loose_remove(entry);
        fix_shortage(i);
    }

}

template <typename T>
T& BTree<T>::get(const T& entry)
{
//    cout << "entry = " << entry << " end entry" << endl;
//    cout << "tree  = \n\n" << *this << endl;
    assert(contains(entry));

    int i = first_ge(data, data_count, entry);
    bool found = (data[i] == entry);
    if (found)
        return data[i];

    return subset[i]->get(entry);
}


template <typename T>
BTree<T>::~BTree()
{
    this->clear_tree();
}

template <typename T>
BTree<T>& BTree<T>::operator =(const BTree<T>& RHS)
{
    this->clear_tree();
    this->copy_tree(RHS);
    return *this;
}

template <typename T>
void BTree<T>::fix_excess(int i)
{

    BTree<T>* temp = new BTree<T>(dups_ok);
    insert_item(subset, i + 1, child_count, temp);

    split(subset[i]->data, subset[i]->data_count,
          temp->data, temp->data_count);

    split(subset[i]->subset, subset[i]->child_count,
          temp->subset, temp->child_count);

    T n;

    detach_item(subset[i]->data, subset[i]->data_count, n);
    ordered_insert(data, data_count, n);
}

template <typename T>
void BTree<T>::insert(const T& entry)
{
    loose_insert(entry);

    if (data_count > MAXIMUM)
    {
        BTree<T>* temp = new BTree<T>(dups_ok);
        temp->copy_tree(*this);
        this->clear_tree();
        subset[0] = temp;
        child_count = 1;
        data_count = 0;

        fix_excess(0);
    }

}



template <typename T>
void BTree<T>::loose_insert(const T& entry){

    int i = first_ge(data, data_count, entry);
    bool found = (i<data_count && data[i] == entry);
    if(found && dups_ok){
        data[i] = data[i] + entry;
    } else if(found && !dups_ok)
        return;

    if(!child_count){
        ordered_insert(data, data_count, entry);
    } else{
        subset[i]->loose_insert(entry);
        if (subset[i]->data_count > MAXIMUM)
            fix_excess(i);
    }

    /*
       int i = first_ge(data, data_count, entry);
       bool found = (i<data_count && data[i] == entry);

       three cases:
         a. found: deal with duplicates
         ! found:
         b. leaf : insert entry in data at position i
         c. !leaf: subset[i]->loose_insert(entry)
                   fix_excess(i) if there is a need
            |   found     |   !found        |
      ------|-------------|-----------------|-------
      leaf  |  a. Deal    | b: insert entry |
            |     with    |    at data[i]   |
      ------|  duplicates |-----------------|-------
            |             | d: subset[i]->  |
      !leaf |             |    loose_insert |
            |             |    fix_excess(i)|
      ------|-------------|-----------------|-------
    */
}



template <typename T>
void BTree<T>::copy_tree(const BTree<T>& other)
{

    data_count = other.data_count;
    child_count = other.child_count;

    for (int i = 0; i < data_count; i++)
        data[i] = other.data[i];

    for (int i = 0; i < other.child_count; i++){
        subset[i] = new BTree<T>(other.dups_ok);
        subset[i]->copy_tree(*other.subset[i]);
    }
}


template <typename T>
int BTree<T>::size() const
{

    int total = data_count, children = child_count;

    for(int i = 0; i < MAXIMUM + 1 && children; i++){
        if(subset[i]){
            total += subset[i]->size();
            children--;
        }
    }


    return total;
}

template <typename T>
T* BTree<T>::find(const T& entry)
{

    int i = first_ge(data, data_count, entry);
    bool found = (data[i] == entry);
    if (found)
        return &data[i];
    if (is_leaf())
        return NULL;
    return subset[i]->find(entry);
}

template <typename T>
bool BTree<T>::contains(const T& entry)
{
    return find(entry);
}


template<typename T>
void BTree<T>::clear_tree()
{
    if(!child_count)
        return;
    for(int i = 0; i < MAXIMUM + 1 && child_count; i++){
        if(subset[i]){
            subset[i]->clear_tree();
            child_count--;
            delete subset[i];
            subset[i] = NULL;
        }
    }
}

template<typename T>
BTree<T>::BTree(bool dups){
    dups_ok = dups;
    data_count = 0;
    child_count = 0;
    for(int i = 0; i < 1 + MAXIMUM; i++){
        subset[i] = NULL;
    }
}



template <typename T>
void BTree<T>::print_tree(int level, ostream& outs) const
{
    int half = child_count/2;
    if (child_count)
    {
        for (int i = child_count - 1; i >= half; i--)
        {
            subset[i]->print_tree(level+4, outs);
        }
        outs << setw(level) << " ";
        print_array(data, data_count);
        outs << " Data= " << data_count << " Children= " << child_count;

        outs << endl;

        for (int i = half - 1; i >= 0; i--)
        {
            subset[i]->print_tree(level + 5, outs);
        }

    }
    else
    {
        outs << setw(level) << " ";
        print_array(data, data_count);
        outs << " Data= " << data_count << " Children= " << child_count;
        outs << endl;
    }


}




#endif // BTREE_H



