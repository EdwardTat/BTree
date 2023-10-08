#ifndef TEMPFUNCTIONS_H
#define TEMPFUNCTIONS_H

#include <iostream>
#include <assert.h>
#include <vector>
#include <iomanip>
using namespace std;



template <class T>
T maximal(const T& a, const T& b)                      //return the larger of the two items
{
    if(a > b)
        return a;
    return b;
}


template <class T>
void mySwap(T& a, T& b)  //swap the two items
{
    T temp = a;
    a = b;
    b = temp;
}


template <class T>
int index_of_maximal(T data[ ], int n)                  //return index of the largest item in data
{
    int max = 0;
    for(int i = 1; i < n; i++){
        if(data[i] >= data[max])
            max = i;
    }
    return max;
}

template <class T>
void insert_item(T data[ ], int i, int& n, T entry)     //insert entry at index i in data
{
    for (int j = n; j > i; j--)
        mySwap(data[j], data[j - 1]);
    data[i] = entry;
    n++;
}

template <class T>
void ordered_insert(T data[ ], int& n, T entry)         //insert entry into the sorted array
                                                        //data with length n
{
    int i = 0;
    while(i < n && data[i] < entry)
        i++;
    insert_item(data, i, n , entry);
}


template <class T>
int first_ge(const T data[ ], int n, const T& entry)    //return the first element in data that is
                                                        //not less than entry
{
    for(int i = 0; i < n; i++){
//        cout << "data[" << i << "] = " << data[i] << endl;
////        cout << "entry = " << entry << endl;
//        cout << "(data[i] >= entry) = " << (data[i] >= entry) << endl;
        if(data[i] >= entry){
            return i;
        }
    }
    return n;
}




template <class T>
void detach_item(T data[ ], int& n, T& entry)           //remove the last element in data and place
//it in entry
{
    entry = data[n - 1];
    n--;
}

template <class T>
void delete_item(T data[ ], int i, int& n, const T& entry)    //delete item at index i and place it in entry
{
    while(i < n - 1){
        mySwap(data[i], data[i + 1]);
        i++;
    }
    n--;
}

template <class T>
void delete_item(T data[ ], int i, int& n, T& entry)    //delete item at index i and place it in entry
{
    entry = data[i];

    while(i < n - 1)
    {
        mySwap(data[i], data[i + 1]);
        i++;
    }
    n--;
}


//template <class T>
//void merge(T data1[ ], int& n1, T data2[ ], int& n2)    //append data2 to the right of data1
//{
//    for(int i = 0, j = n1; i < n2; i++, j++){
//        data1[j] = data2[i];
//    }
//    n1 += n2;
//}

template <typename T>
void merge(T data1[], int& n1, T data2[], int& n2)
{
    int j;  // Starting index of data1

    j = n1; // Set index to size of data1

    // Traverse data2
    for (int i = 0; i < n2; i++)
    {
        // Each time a value is set, increase
        // the index of j for data1
        data1[j] = data2[i];
        j++;
    }

    n1 = j;  // The new size of data1
    n2 = 0;  // *NOTE* Unsure, data2 new size is left 0
}

template <typename T>
void split(T data1[], int& n1, T data2[], int& n2)
{
    int temp = n1/2;
    if (n1 % 2)
        temp++;


    n2 = 0;
    for (int i = temp; i < n1; i++, n2++)
    {
        data2[n2] = data1[i];
    }

    n1 = temp;
}


template <class T>
void copy_array(T dest[], const T src[],
                int& dest_size, int src_size)               //copy src[] into dest[]
{
    dest_size = src_size;
    for(int i = 0; i < src_size; i++){
        dest[i] = src[i];
    }
}


template <class T>
void print_array(const T data[], int n, int pos = 0)   //print array data
{
    for (int i = pos; i < n; i++)
        cout << setw(4) << data[i];
}


template <class T>
bool is_gt(const T data[], int n, const T& item)        //item > all data[i]
{
    for(int i = 0; i < n; i++)
        if(data[i] <= item)
            return false;
    return true;
}



template <class T>
bool is_le(const T data[], int n, const T& item)        //item <= all data[i]
{
    for(int i = 0; i < n; i++)
        if(data[i] > item)
            return false;
    return true;
}


//template <typename T>
//ostream& operator <<(ostream& outs, const vector<T>& list)  //print vector list
//{
//    for(int i = 0; i < list.size(); i++){
//        outs << i << ". " << list[i] << " ";
//    }
//    return outs;
//}




template <typename T>
void attach_item(T data[ ], int& n, const T& entry)
{
    data[n] = entry;
    n++;
}

#endif // BTREEFUNCTIONS_H
