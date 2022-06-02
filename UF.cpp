#include "UF.h"
#include <climits>
#include <cassert>

template<class T>
UF<T>::UF(unsigned int size):
    size(size),
    Elements(new T*[size]),
    Parents(new key[size]),
    Sizes(new unsigned int[size]),
    current(0)
{
        for(int i = 0; i < size; i++){
            Sizes[i] = INT_MAX;
        }
}

template<class T>
bool UF<T>::isRoot(key Key){
    return (Parents[current] == current);

}

template<class T>
key UF<T>::MakeSet(T* data){
    Elements[current] = data;
    Parents[current] = current; // Root is parent of itself
    Sizes[current] = 1;
    current++;
}

template<class T>
void UF<T>::InitAllElements(T** data_array){
    assert(current == 0);
    for(int i = 0; i < size; i++ ){
        MakeSet(data_array[i]);
    }
}

template<class T>
key UF<T>::Find(key Key){
    key iterator_key = Key;
    while(Parents[iterator_key] != iterator_key){
        iterator_key = Parents[iterator_key];
    }
    key root = iterator_key; // Save root index
    iterator_key = Key;
    // Cycle once again to set parent values in find Path to Root
    while (iterator_key != root) {
        iterator_key = Parents[iterator_key];
        Parents[iterator_key] = root; // Optimize next Find operations
    }
    return root;
}

template<class T>
T* UF<T>::Union(key Key1, key Key2){
    key Root1 = Key1, Root2 = Key2;
    if(!isRoot(Root1)){
        Root1 =  Find(Root1);
    }
    if(!isRoot(Root2)){
        Root2 =  Find(Root2);
    }
    assert(Sizes[Root1] != INT_MAX && Sizes[Root2] != INT_MAX);

    if(Sizes[Root1] < Sizes[Root2]) {
        Parents[Root1] = Root2;
        Sizes[Root2] += Sizes[Root1];
        Sizes[Root1] = INT_MAX;
    } else {
        Parents[Root2] = Root1;
        Sizes[Root1] += Sizes[Root2];
        Sizes[Root2] = INT_MAX;
    }
}




