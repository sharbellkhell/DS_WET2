#ifndef HASHTABLE_H_
#define HASHTABLE_H_
#include "AVLTree.h"
#include <cmath>

#define START_SIZE 4
#define MAX_RATIO 1
#define MIN_RATIO 0.25
#define INCREASE_SIZE 2
#define DECREASE_SIZE 0.5

template<class T>
struct HashTable{
    AVLTree<int,T>* *elements;
    int array_size;
    int num_elements;
    double alpha;
    public:
        HashTable() : array_size(START_SIZE), num_elements(0){
            alpha= ((sqrt(5) - 1) / 2.0);//golden ratio, as seen in lecture 7
            elements = new AVLTree<int,T>*[START_SIZE];
            for(int i=0;i<START_SIZE;i++)
                elements[i]=nullptr;
        }
        ~HashTable(){
            for(int i=0;i<array_size;i++)
                if(elements[i]!=nullptr)
                    delete elements[i];
            delete[] elements;
        }

        int hashIndex(int id)
        {
            double x=alpha*id-floor(alpha*id);
            return floor(x*array_size);
        }
        
        bool insertToTable(const int id,const T& data)
        {
            int index=hashIndex(id);
            try{
            elements[index]=insertNode(id,data,elements[index]);
            }catch(...)
            {
                return false;
            }
            num_elements++;
            checkIfRehashNeeded();
            return true;
        }

        void checkIfRehashNeeded()
        {
            if(num_elements>=array_size*MAX_RATIO)
                rehash(array_size*INCREASE_SIZE);
            else if(num_elements<array_size*MIN_RATIO)
                rehash(array_size*DECREASE_SIZE);
        }

        void rehash(int size)
        {
            AVLTree<int,T>* *new_elements = new AVLTree<int,T>*[size];
            for(int i=0;i<size;i++)
                new_elements[i]=nullptr;
            int old_array_size=array_size;
            array_size=size;
            for(int i=0;i<old_array_size;i++)
            {
                while(elements[i]!=nullptr)
                    {
                        int temp_id=elements[i]->key;
                        T temp_data=elements[i]->value;
                        int index=hashIndex(temp_id);
                        new_elements[index]=insertNode(temp_id,temp_data,new_elements[index]);
                        elements[i]=removeNode(elements[i],temp_id);
                    }
            }
            delete[] elements;
            elements=new_elements;
        }

        void remove(int id)
        {
            int index=hashIndex(id);
            try{
                elements[index]=removeNode(elements[index],id);
            }catch(...){}
            num_elements--;
            checkIfRehashNeeded();
        }

        AVLTree<int,T>* find(int id)
        {
            int index=hashIndex(id);
            if(elements[index]==nullptr)
                return nullptr;
            return findNode(elements[index],id);
        }
};




#endif