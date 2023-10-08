#ifndef BPTREE_H
#define BPTREE_H

#include <iostream>
#include <iomanip>
#include "tempfunctions.h"
using namespace std;

template <class T>
class BPTree
{
public:
    
    class Iterator{
    public:
        friend class BPlusTree;
        Iterator(BPTree<T>* _it=NULL, int _key_ptr = 0):it(_it), key_ptr(_key_ptr){}
        
        T operator *(){
            //            assert(key_ptr<it->data_count);
            return it->data[key_ptr];
        }
        
        Iterator& operator=(const Iterator& other)
        {
            it = other.it;
            key_ptr = other.key_ptr;
            return *this;
        }

        Iterator operator++(int)
        {
            if(it == NULL)
                return *this;
            //            cout << "incriment called\n";
            //            cout << "data count = " << it->data_count << endl;
            if (key_ptr + 1 >= it->data_count)
            {
                key_ptr = 0;
                //                cout << "it->next called\n";
                if(it->next){
                    //                    cout << "next exists\n";
                    it = it->next;
                }
                else
                {
                    it = NULL;
                    //                    cout << "null returned from ++ iterator\n";
                }

            }
            else
            {
                key_ptr++;
            }

            return *this;
        }

        // TEST THIS
        Iterator operator++()
        {
            if(it == NULL)
                return *this;
            //            cout << "incriment called\n";
            //            cout << "data count = " << it->data_count << endl;

            if (key_ptr + 1 >= it->data_count)
            {
                key_ptr = 0;
                //                cout << "it->next called\n";
                if(it->next){
                    //                    cout << "next exists\n";
                    it = it->next;
                }
                else
                {
                    it = NULL;
                    //                    cout << "null returned from ++ iterator\n";
                }

            }
            else
            {
                key_ptr++;
            }

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
        bool is_null(){return !it;}
    private:
        BPTree<T>* it;
        int key_ptr;
    };
    
    BPTree(bool dups = false);
    //big three:
    BPTree(const BPTree<T>& other);
    ~BPTree();
    BPTree<T>& operator =(const BPTree<T>& RHS);
    
    void insert(const T& entry);                //insert entry into the tree
    void remove(const T& entry);                //remove entry from the tree
    
    void clear_tree();                          //clear this object (delete all nodes etc.)
    //    void copy_tree(const BPTree<T>& other, BPTree<T> *&ptr);      //copy other into this object
    void copy_tree(const BPTree<T>& other, BPTree<T>*& last_leaf);
    
    bool contains(const T& entry);              //true if entry can be found in the array
    T& get(const T& entry);                     //return a reference to entry in the tree
    T* find(const T& entry);                    //return a pointer to this key. NULL if not there.
    
    int size() const;                           //count the number of elements in the tree
    bool empty() const{return (child_count + data_count == 0);}  //true if the tree is empty
    
    void print_tree(int level = 0, ostream &outs=cout) const; //print a readable version of the tree
    friend ostream& operator<<(ostream& outs, const BPTree<T>& print_me){
        print_me.print_tree(0, outs);
        return outs;
    }
    bool is_valid();

    Iterator begin(){
        BPTree<T> *temp = this;
        while(!temp->is_leaf())
            temp = temp->subset[0];
        Iterator out(temp);
        return out;
    }

    Iterator end(){
        BPTree<T> *temp = this;
        while(!temp->is_leaf())
            temp = temp->subset[temp->child_count - 1];
        Iterator out(temp, temp->data_count - 1);
        return out;
    }

    Iterator lower_bound(const T& key){
        int i = first_ge(data, data_count, key);
        bool found = ((data[i] == key) && i < data_count);
        if(is_leaf()){
            Iterator it(this, i);
            return it;
        }
        if(found)
            return subset[i + 1]->lower_bound(key);
        //        cout << "subset i = " << *subset[i] << endl;
        return subset[i]->lower_bound(key);
    }

    BPTree upper_bound_bpt(const T& key){
        //        cout << "starting tree = " << endl << *this << endl;
        int i = first_ge(data, data_count, key);

        bool found = ((data[i] == key) && i < data_count);
        //        if(found)
        //            cout << "value has been found\n";
        //        cout << "i = " << i << endl;
        if(is_leaf()){
            //            cout << "i = " << i << endl;
            //            cout << "data count = " << data_count << endl;
            //            if(this->next)
            //            cout << "subet next = " << *this->next << endl;

            Iterator it(this, i);
            if(data[i] <= key)
                it++;
            return it.it;
        }
        if(found){
            //            cout << "subset i + 1 = " << *subset[i + 1] << endl;
            return subset[i + 1]->upper_bound(key);
        }
        //        cout << "subset i = " << *subset[i] << endl;
        return subset[i]->upper_bound(key);
    }

    Iterator upper_bound(const T& key){
        //        cout << "starting tree = " << endl << *this << endl;
        int i = first_ge(data, data_count, key);

        bool found = ((data[i] == key) && i < data_count);
        //        if(found)
        //            cout << "value has been found\n";
        //        cout << "i = " << i << endl;
        if(is_leaf()){
            //            cout << "i = " << i << endl;
            //            cout << "data count = " << data_count << endl;
            //            if(this->next)
            //            cout << "subet next = " << *this->next << endl;

            Iterator it(this, i);
            if(data[i] <= key)
                it++;
            return it;
        }
        if(found){
            //            cout << "subset i + 1 = " << *subset[i + 1] << endl;
            return subset[i + 1]->upper_bound(key);
        }
        //        cout << "subset i = " << *subset[i] << endl;
        return subset[i]->upper_bound(key);
    }

    T &get_largest(){
        if(is_leaf()){
            return data[data_count - 1];
        }
        else
        {
            return subset[child_count - 1]->get_largest();
        }
    }


private:

    BPTree* next;
    static const int MINIMUM = 1;
    static const int MAXIMUM = 2 * MINIMUM;
    BPTree* subset[MAXIMUM + 2];
    
    bool dups_ok;                                   //true if duplicate keys may be inserted
    int data_count;                                 //number of data elements
    T data[MAXIMUM + 1];                            //holds the keys
    int child_count;                                //number of children
    //suBPTrees

    
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
    T &get_smallest();

    void linkLeaves(BPTree *ptr);
    void nullEdge();

};



template<typename T>
void BPTree<T>::linkLeaves(BPTree *ptr)
{
    for(int i = 0; i < child_count - 1; i++){
        subset[i]->next = subset[i + 1];
        subset[i]->linkLeaves(subset[i + 1]);
    }
    for(int i = 0; i < ptr->child_count - 1; i++){
        ptr->subset[i]->next = ptr->subset[i + 1];
        ptr->subset[i]->linkLeaves(ptr->subset[i + 1]);
    }
    next = ptr;
    if(!is_leaf()){
        subset[this->child_count - 1]->next = ptr->subset[0];
    }
}

template<typename T>
void BPTree<T>::nullEdge()
{
    next = NULL;
    if(child_count)
        subset[child_count - 1]->nullEdge();
}




template<typename T>
BPTree<T>::BPTree(const BPTree<T>& other)
{

    BPTree<T>* temp = nullptr;
    this->copy_tree(other, temp);
}


template<typename T>
bool BPTree<T>::is_valid()
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
void BPTree<T>::merge_with_next_subset(int i)
{
    //    cout << "merge = " << i << endl;
    //    cout << "this is my tree:\n\n";
    //    print_tree();
    T temp;
    BPTree<T> *btemp;

    delete_item(data, i, data_count, temp);
    ordered_insert(subset[i]->data, subset[i]->data_count, subset[i + 1]->data[0]);
        if (subset[i]->child_count)
                attach_item(subset[i]->data, subset[i]->data_count, temp);
        merge(subset[i]->data, subset[i]->data_count,
                  subset[i + 1]->data, subset[i + 1]->data_count);


            merge(subset[i]->subset, subset[i]->child_count,
                  subset[i + 1]->subset, subset[i + 1]->child_count);

            // 5. delete subset[i+1] (store in a temp ptr)
            delete_item(subset, i + 1, child_count, btemp);
            delete btemp;
//    delete subset[i + 1];
//    child_count--;
//    subset[i + 1] = NULL;
//    data_count--;

//    delete_item(data, i, data_count, temp);
    
//    attach_item(subset[i]->data, subset[i]->data_count, temp);
//    merge(subset[i]->subset, subset[i]->child_count, subset[i + 1]->subset,
//            subset[i + 1]->child_count);
//    merge(subset[i]->data, subset[i]->data_count, subset[i + 1]->data,
//            subset[i + 1]->data_count);
//    delete_item(subset, i + 1, child_count, btemp);
//    delete_item(data, i, data_count, temp);
}


template <typename T>
void BPTree<T>::rotate_right(int i)
{
    insert_item(subset[i + 1]->data, 0, subset[i + 1]->data_count, data[i]);
    detach_item(subset[i]->data, subset[i]->data_count, data[i]);
    if (subset[i]->child_count)
    {
        BPTree<T>* temp;
        detach_item(subset[i]->subset, subset[i]->child_count, temp);
        insert_item(subset[i + 1]->subset, 0, subset[i + 1]->child_count, temp);
    }
    else
    {
        subset[i + 1]->data[0] = data[i];
    }
}

template <typename T>
void BPTree<T>::rotate_left(int i)
{
    attach_item(subset[i - 1]->data, subset[i - 1]->data_count, data[i - 1]);
    delete_item(subset[i]->data, 0, subset[i]->data_count, data[i - 1]);

    if (subset[i]->child_count)
    {
        BPTree<T>* temp;
        delete_item(subset[i]->subset, 0, subset[i]->child_count, temp);
        attach_item(subset[i - 1]->subset, subset[i - 1]->child_count, temp);
    }
    else
    {
        data[i - 1] = subset[i]->data[0];
    }
}


//template<typename T>
//void BPTree<T>::rotate_right(int i)
//{
//    //    cout << "rotate right\n";
//    if (!subset[i]->is_leaf())
//    {

//        T temp;
//        ordered_insert(subset[i]->data, subset[i]->data_count, data[0]);
//        delete_item(data, 0, data_count, temp);
//        ordered_insert(data, data_count, subset[i - 1]->data[data_count - 1]);
//        subset[i - 1]->data_count--;
//        BPTree<T>* temp2;
//        detach_item(subset[i]->subset, subset[i]->child_count, temp2);
//        insert_item(subset[i + 1]->subset, 0, subset[i + 1]->child_count, temp2);
//    }
//    else
//    {
//        T temp;
//        delete_item(subset[i - 1]->data, 0, subset[i - 1]->data_count, temp);
//        ordered_insert(subset[i]->data, subset[i]->data_count, temp);
//        data[i - 1] = subset[i]->data[0];
//    }


//}

//template<typename T>
//void BPTree<T>::rotate_left(int i)
//{
//    //    cout << "rotate left\n";
//    if (!subset[i]->is_leaf())
//    {
//        T temp;
//        ordered_insert(subset[i]->data, subset[i]->data_count, data[data_count - 1]);
//        data_count--;
//        ordered_insert(data, data_count, subset[i + 1]->data[0]);
//        delete_item(subset[i + 1]->data, 0, subset[i + 1]->data_count, temp);
//        BPTree<T>* temp2;
//        delete_item(subset[i]->subset, 0, subset[i]->child_count, temp2);
//        attach_item(subset[i - 1]->subset, subset[i - 1]->child_count, temp2);
//    }
//    else
//    {
//        T temp;
//        delete_item(subset[i + 1]->data, 0, subset[i + 1]->data_count, temp);
//        ordered_insert(subset[i]->data, subset[i]->data_count, temp);
//        data[i] = subset[i + 1]->data[0];
//    }

//}

template<typename T>
void BPTree<T>::fix_shortage(int i)
{
    if(subset[i]->data_count >= MINIMUM)
        return;
    if (i - 1 >= 0 && subset[i - 1]->data_count > MINIMUM)
    {
        rotate_right(i - 1);
        subset[i];
    }
    else if (i + 1 < child_count && subset[i + 1]->data_count > MINIMUM)
    {
        rotate_left(i + 1);
        subset[i];
    }
    else if (i - 1 >= 0)
    {
        merge_with_next_subset(i - 1);
    }
    else if (i + 1 < child_count)
    {
        merge_with_next_subset(i);
    }

//    if(subset[i]->data_count < MINIMUM){
//        if(i - 1 > 0 && subset[i - 1]->data_count > MINIMUM){
//            rotate_right(i);
//        } else if(i + 1 < child_count && subset[i + 1]->data_count > MINIMUM){
//            rotate_left(i);
//        } else if(i > 0){
//            merge_with_next_subset(i - 1);
//        } else if (i < child_count - 1){
//            merge_with_next_subset(i);
//        }
//        if(subset[i]->is_leaf()){
//            for(int i = 0; i < child_count; i++)
//                subset[i]->next = subset[i + 1];
//        }
//    }
    
}


template<typename T>
void BPTree<T>::remove_biggest(T& entry)
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
T& BPTree<T>::get_smallest()
{
//    cout << "child count = " << child_count << endl;
    if(is_leaf()){
        //        cout << "found value again " << data[0] << endl;
        //        cout << "get, get smallest, contains, and find work\n";
        return data[0];
    }
    else
    {
        return subset[0]->get_smallest();
    }
}

template <typename T>
void BPTree<T>::remove(const T& entry)
{
    loose_remove(entry);
    //    cout << "loose remove resolved\n";
    //    print_tree();
    //    cout << endl << endl;
    if(data_count == 0 && child_count == 1)
    {
        //        cout << "advanced called\n";
        BPTree<T> *temp = subset[0];
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
void BPTree<T>::fix_excess(int i)
{
    BPTree<T>* bpt;
    T n;

    bpt = new BPTree<T>(dups_ok);
    //cout << "inserted\n";
    insert_item(subset, i + 1, child_count, bpt);

    split(subset[i]->data, subset[i]->data_count, bpt->data,
          bpt->data_count);

    split(subset[i]->subset, subset[i]->child_count, bpt->subset,
          bpt->child_count);

    detach_item(subset[i]->data, subset[i]->data_count, n);
    ordered_insert(data, data_count, n);

    //            cout << "i = " << i << endl;
    //            cout << "data count = " << data_count << endl;
    //            if(this->next)
    //            cout << "subet next = " << *this->next << endl;
    if (bpt->is_leaf())
    {
        insert_item(bpt->data, 0, bpt->data_count, n);
        subset[i]->next = bpt;
    }

    for(int i = 0; i < child_count - 1; i++){
        subset[i]->next = subset[i + 1];
    }
}

template <typename T>
void BPTree<T>::loose_remove(const T& entry)
{
    
    int i = first_ge(data, data_count, entry);
    //    cout << "i = " << i << endl;
    bool found = ((data[i] == entry) && i < data_count);
    
    if(found && is_leaf()){
        // void delete_item(T data[ ], int i, int& n, const T& entry)
        delete_item(data, i, data_count, entry);
    }
    else if(found && (child_count))
    {
        //        T temp;
        //        delete_item(data, i, data_count, entry);
        //        subset[i]->remove_biggest(temp);
        //        ordered_insert(data, data_count, temp);
        cout << "entry = " << entry << endl;
        subset[i + 1]->loose_remove(entry);
        cout << "loose remove tree display\n";
        cout << *this << endl;
        fix_shortage(i + 1);
        if(child_count > i)
        data[i] = subset[i+1]->get_smallest();
    }
    else if(!found && is_leaf())
    {
        return;
    }
    else
    {
        subset[i]->loose_remove(entry);
        cout << "loose remove tree display\n";
        cout << *this << endl;
        fix_shortage(i);
    }
    
}

template <typename T>
T& BPTree<T>::get(const T& entry)
{
    //    assert(contains(entry));
    
    int i = first_ge(data, data_count, entry);
    bool found = ((data[i] == entry) && i < data_count);
    if (found && is_leaf())
        return data[i];
    else if(found){
        return subset[i + 1]->get_smallest();
    }
    return subset[i]->get(entry);
}


template <typename T>
BPTree<T>::~BPTree()
{
    this->clear_tree();
}

template <typename T>
BPTree<T>& BPTree<T>::operator =(const BPTree<T>& RHS)
{
    this->clear_tree();
    BPTree<T>* temp = nullptr;
    this->copy_tree(RHS, temp);
    return *this;
}






template <typename T>
void BPTree<T>::insert(const T& entry)
{
    loose_insert(entry);

    if (data_count > MAXIMUM)
    {
        BPTree<T>* temp = new BPTree<T>(dups_ok);

        copy_array(temp->data, data, temp->data_count, data_count);
        copy_array(temp->subset, subset, temp->child_count, child_count);


        child_count = 1;
        data_count = 0;
        subset[0] = temp;

        fix_excess(0);
        //        cout << "fix excess resolved\n";
    }


}




template <typename T>
void BPTree<T>::loose_insert(const T& entry){

    int i = first_ge(data, data_count, entry);
    bool found = ((data[i] == entry) && i < data_count);
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

    //    cout << "loose insert for " << entry << "worked\n\n";
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
void BPTree<T>::copy_tree(const BPTree<T>& other, BPTree<T>* &ptr)
{

    data_count = other.data_count;
    child_count = other.child_count;

    for (int i = 0; i < data_count; i++)
        data[i] = other.data[i];

    for (int i = 0; i < other.child_count; i++){
        subset[i] = new BPTree<T>(other.dups_ok);
        subset[i]->copy_tree(*other.subset[i], ptr);
    }
    if (is_leaf())
    {
        if (ptr)
            ptr->next = this;
        ptr = this;
    }
}


template <typename T>
int BPTree<T>::size() const
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
T* BPTree<T>::find(const T& entry)
{
    
    int i = first_ge(data, data_count, entry);
    bool found = ((data[i] == entry) && i < data_count);
    if (found)
        return &data[i];
    if (is_leaf())
        return NULL;
    return subset[i]->find(entry);
}

template <typename T>
bool BPTree<T>::contains(const T& entry)
{
    return find(entry);
}


template<typename T>
void BPTree<T>::clear_tree()
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
BPTree<T>::BPTree(bool dups){
    dups_ok = dups;
    data_count = 0;
    child_count = 0;
    for(int i = 0; i < 1 + MAXIMUM; i++){
        subset[i] = NULL;
    }
    next = NULL;
}



template <typename T>
void BPTree<T>::print_tree(int level, ostream& outs) const
{
    int half = child_count/2;
    if (child_count)
    {
        for (int i = child_count - 1; i >= half; i--)
        {
            subset[i]->print_tree(level+5, outs);
        }
        outs << setw(level) << " ";
        print_array(data, data_count);
        //        for(int i = 0; i < data_count; i++){
        //            outs << data[i] << endl << setw(level) << " ";
        //        }
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
        //        for(int i = 0; i < data_count; i++){
        //            outs << data[i] << endl << setw(level) << " ";
        //        }
        outs << " Data= " << data_count << " Children= " << child_count;
        outs << endl;
    }
    
    
}

#endif // BPTREE_H
