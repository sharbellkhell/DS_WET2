#include "UF.h"
#include <climits>
#include <cassert>

UF::UF(int size):
    size(size),
    Elements(new T*[size+1]),
    Parents(new key[size+1]),
    Sizes(new int[size+1]),
    Last_Values(new double[size+1]),
    current(0)
{
        for(int i = 0; i < size+1; i++){
            Sizes[i] = INT_MAX;
        }
}

bool UF::isRoot(key Key){
    return (Parents[current] == current);

}

void UF::MakeSet(int i){
    Elements[current] = new Company(i,i);
    Parents[current] = current; // Root is parent of itself
    Sizes[current] = 1;
    Last_Values[current]=0;
    current++;
}

void UF::InitAllElements(){
    assert(current == 0);

    for(int i = 1; i < size+1; i++ ){
        MakeSet(i);
    }
}

//key UF::FindCheckAndUpdate(key Key){
//
//}
double UF::aux_find(key Key)
{
    if(Parents[Key]==Key)
        return this->Elements[Key]->value;
    Elements[Key]->value+=aux_find(Parents[Key])-Last_Values[Key];
    Last_Values[Key]=Elements[Parents[Key]]->value;
    return Elements[Key]->value;
}
key UF::Find(key Key){
    if(Parents[Key]==Key)
        return Elements[Key]->value;
    key root=Key;
    while(Parents[root]==root)
        root=Parents[root];
    double result=aux_find(Key);
    key iterator=Key;
    key temp=0;
    while(Parents[iterator]==iterator)
        {
            temp=Parents[iterator];
            Parents[iterator]=root;
            iterator=temp;
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





