#include "UF.h"
#include <climits>
#include <cassert>

UF::UF(int size):
    size(size),
    Elements(new T*[size]),
    Parents(new key[size]),
    Sizes(new int[size]),
    current(0)
{
        for(int i = 0; i < size; i++){
            Sizes[i] = INT_MAX;
        }
}

bool UF::isRoot(key Key){
    return (Parents[current] == current);

}

key UF::MakeSet(T* data){
    Elements[current] = data;
    Parents[current] = current; // Root is parent of itself
    Sizes[current] = 1;
    current++;
}

void UF::InitAllElements(T** data_array){
    assert(current == 0);
    for(int i = 0; i < size; i++ ){
        MakeSet(data_array[i]);
    }
}

//key UF::FindCheckAndUpdate(key Key){
//
//}

key UF::Find(key Key){
    key iterator_key = Key;
    key last_son_key; // Stores the key of the node before the root
    while(Parents[iterator_key] != iterator_key){
        last_son_key = iterator_key;
        iterator_key = Parents[iterator_key];

    }
    key root = iterator_key; // Save root index
    bool value_was_updated = false;
    // Check if there has been a change in value
    if(Last_Values[last_son_key] != Last_Values[root]){
        value_was_updated = true;
    }
    iterator_key = Key;
    // Cycle once again to set parent values in find Path to Root
    while (iterator_key != root) {
        iterator_key = Parents[iterator_key];
        Parents[iterator_key] = root; // Optimize next Find operations
        if(value_was_updated){
            Last_Values[iterator_key] = Last_Values[root];
        }
    }
    return root;
}

T* UF::Union(key Key1, key Key2){
    key Root1 = Key1, Root2 = Key2;
    if(!isRoot(Root1)){
        Root1 =  Find(Root1);
    }
    if(!isRoot(Root2)){
        Root2 =  Find(Root2);
    }
    assert(Sizes[Root1] != INT_MAX && Sizes[Root2] != INT_MAX);
    key absolute_root;

    if(Sizes[Root1] < Sizes[Root2]) {
        absolute_root = Root2;
        Parents[Root1] = Root2;
        Sizes[Root2] += Sizes[Root1];
        Sizes[Root1] = INT_MAX;
    } else {
        absolute_root = Root1;
        Parents[Root2] = Root1;
        Sizes[Root1] += Sizes[Root2];
        Sizes[Root2] = INT_MAX;
    }
    return Elements[absolute_root];
}

bool UF::UpdateLastValue(int index, double new_value){
    if(index > size || index < 0){
        return false;
    }
    double last_value = Last_Values[index];
    Last_Values[index] = new_value;
    return (last_value == new_value);
}





