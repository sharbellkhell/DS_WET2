#ifndef DS_WET2_UF_H
#define DS_WET2_UF_H
#include "Company.h"

typedef int key;
typedef Company T;

//template<class T>
class UF {
public:
    int size;
    int current;
    T** Elements ; // Array of the Elements
    key* Parents; // Array of the indices of the Parents of the Elements
    int* Sizes; // Array of the Sizes of the Elements
    double* Last_Values; // Holds the company values the last time they were updated


    explicit UF(int size);
    ~UF();
    void InitAllElements();
    key Find(key Key) const;
    T* Union(key Key1, key Key2) const;
    void MakeSet(int index);

    /*bool UpdateLastValue(int index, double new_value);
    //bool isRoot(key Key);
    private:
        double aux_find(key Key);
        */
};


#endif //DS_WET2_UF_H
