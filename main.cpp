#include "Employee.h"
#include "AVLTree.h"
#include "UF.h"

#define INSERT(EI) \
tree = insertNode(EI->EmployeeId,EI,tree,EI->grade);

#define WHOIS(EI) \
std::cout << "id:" << EI->EmployeeId << " ,grade:" << EI->grade << " ,sal:" << EI->salary << std::endl;

void printTreeInternal(AVLTree<int,Employee*>* root, std::string indent, bool last) {
    if (root != nullptr) {
        std::cout << indent;
        if (last) {
            std::cout << "R----";
            indent += "   ";
        } else {
            std::cout << "L----";
            indent += "|  ";
        }
        std::cout << "key:" << root->key << " ,rank:" << root->rank.NumEmployees <<
                " ,sum:" << root->rank.SumGrades <<  std::endl;
        printTreeInternal(root->left, indent, false);
        printTreeInternal(root->right, indent, true);
    }
}

void PrintTree(AVLTree<int,Employee*>* root){
    printTreeInternal(root, "", true);
    std::cout << std::endl;
    std::cout << std::endl;

}

template<class T>
void PrintArr(T* arr, int size){
    for(int i = 0; i < size+1; i++){
        std::cout << arr[i] << ", ";
    }
    std::cout << std::endl;
}

void PrintCompArr(Company** arr, int size){
    for(int i = 0; i < size+1; i++){
        std::cout << "(" << arr[i]->companyId << "," << arr[i]->value << ")" << ", ";
    }
    std::cout << std::endl;
}

void PrintCurrUF(UF* uf){

    std::cout << "Elements: ";
    PrintCompArr(uf->Elements,uf->size);
    std::cout << "Parents:     ";
    PrintArr<key>(uf->Parents, uf->size);
    std::cout << "Sizes:       ";
    PrintArr<int>(uf->Sizes,uf->size);
    std::cout << "Last_Values: ";
    PrintArr<double>(uf->Last_Values, uf->size);
    std::cout << "end of UF" << std::endl;

}

int main(){
    Employee* e1 = new Employee(1,1, 1000, 10);
    Employee* e2 = new Employee(2,1, 1000, 4);
    Employee* e3 = new Employee(3,1, 1000, 7);
    Employee* e4 = new Employee(4,1, 1000, 8);
    Employee* e5 = new Employee(5,1, 1000, 25);


    WHOIS(e1)
    WHOIS(e2)
    WHOIS(e3)
    WHOIS(e4)
    WHOIS(e5)

    AVLTree<int, Employee*>* tree = insertNode<int,Employee*>(e1->EmployeeId,e1, nullptr, e1->grade);
    PrintTree(tree);
    INSERT(e2)
    PrintTree(tree);
    INSERT(e3)
    PrintTree(tree);
    INSERT(e4)
    PrintTree(tree);
    INSERT(e5)
    PrintTree(tree);






}
