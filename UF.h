#ifndef DS_WET2_UF_H
#define DS_WET2_UF_H
#include "Company.h"

typedef unsigned int key;
typedef Company T;

//template<class T>
class UF {
public:
    unsigned int size;
    unsigned int current;
    T** Elements ; // Array of the Elements
    key* Parents; // Array of the indices of the Parents of the Elements
    unsigned int* Sizes; // Array of the Sizes of the Elements
    double* Last_Values; // Holds the company values the last time they were updated


    UF(unsigned int size);
    void InitAllElements(T** data_array);
    bool isRoot(key Key);
    key MakeSet(T* data);
    key Find(key Key);
    T* Union(key Key1, key Key2);
    bool UpdateLastValue(int index, double new_value);
};


#endif //DS_WET2_UF_H
