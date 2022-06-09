#include "Employee.h"
#include "AVLTree.h"
#include "UF.h"

#define INSERT(EI) \
tree = insertNode(EI->EmployeeId,EI,tree,EI->grade);

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

int main(){
    Employee* e1 = new Employee(1,1, 1000, 10);
    Employee* e2 = new Employee(2,1, 1000, 4);
    Employee* e3 = new Employee(3,1, 1000, 7);
    Employee* e4 = new Employee(4,1, 1000, 8);
    Employee* e5 = new Employee(5,1, 1000, 25);

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
