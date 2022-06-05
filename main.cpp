#include "Employee.h"
#include "AVLTree.h"

#define INSERT(EI) \
tree = insertNode(EI->EmployeeId,EI,tree,EI->grade);

int main(){
    Employee* e1 = new Employee(1,1, 1000, 10);
    Employee* e2 = new Employee(2,1, 1000, 4);
    Employee* e3 = new Employee(3,1, 1000, 7);
    Employee* e4 = new Employee(4,1, 1000, 8);
    Employee* e5 = new Employee(5,1, 1000, 25);

    AVLTree<int, Employee*>* tree = insertNode(e1->EmployeeId,e1, nullptr, e1->grade);
    INSERT(e2)
    INSERT(e3)
    INSERT(e4)
    INSERT(e5)




}
