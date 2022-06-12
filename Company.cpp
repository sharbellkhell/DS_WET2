#include <iostream>
#include "library2.h"
#include "hashTable.h"
#include "Employee.h"
#include "AVLTree.h"
#include "Company.h"

Company::Company(int company_id,int value) : companyId(company_id),value(value),parent_value(0) {
    workersId = new HashTable<Employee*>();
    workersSal = nullptr;
}
Company::~Company()
{
    if(workersSal!=nullptr && workersSal->value!=nullptr)
        Quit(workersSal);
    for(int i=0;i<workersId->array_size;i++)
        while(workersId->elements!= nullptr && workersId->elements[i]!=nullptr)
            workersId->remove(workersId->elements[i]->key,1);
    delete workersId;
}
static AVLTree<int,AVLTree<int,Employee*>*>* insertDuplicate(int sal,Employee* emp,AVLTree<int,AVLTree<int,Employee*>*>* root)
{
    AVLTree<int,AVLTree<int,Employee*>*>* temp=findNode(root,sal);
    if(temp==nullptr)
    {
        AVLTree<int, Employee*>* sal_Range=init(emp->EmployeeId,emp);
        sal_Range->rank.NumEmployees=1;
        sal_Range->rank.SumGrades=emp->grade;
        root=insertNode(sal,sal_Range,root);
    }
    else{
        temp->value=insertNode(emp->EmployeeId,emp,temp->value);
        temp->rank.NumEmployees++;
        temp->rank.SumGrades+=emp->grade;
        temp=temp->parent;
        while(temp!=nullptr)
        {
            temp->rank.NumEmployees++;
            temp->rank.SumGrades+=emp->grade;
            temp=temp->parent;
        }
    }
    return root;
}

static AVLTree<int,AVLTree<int,Employee*>*>* removeDuplicate(int sal,int emp_id,AVLTree<int,AVLTree<int,Employee*>*>* root)
{
    AVLTree<int,AVLTree<int,Employee*>*>* temp=findNode(root,sal);
    AVLTree<int,AVLTree<int,Employee*>*>* rank_fixer=temp;
    if(temp!=nullptr){
        AVLTree<int,Employee*>* temp_emp=findNode(temp->value,emp_id);
        while(temp_emp!=nullptr && rank_fixer!=nullptr){
            rank_fixer->rank.NumEmployees--;
            rank_fixer->rank.SumGrades-=temp_emp->value->grade;
            rank_fixer=rank_fixer->parent;
        }
    }
    temp->value = removeNode(temp->value,emp_id); 
    if(temp->value==nullptr){
        root=removeNode(root,sal);
    }
    return root;
}

StatusType Company::AddEmployee(Employee* emp){
    try{
    this->workersId->insertToTable(emp->EmployeeId,emp);
    if(emp->salary!=0)
        this->workersSal = insertDuplicate(emp->salary, emp, this->workersSal);
    }catch(InvalidInput const&){
        return INVALID_INPUT;
    }
    catch(NodeAlreadyExists const&){
        return FAILURE;
    }
    catch(std::bad_alloc const&){
        return ALLOCATION_ERROR;
    }
    return SUCCESS;

}
static Employee* find_highest_earner(Company* company)
{
    AVLTree<int,AVLTree<int,Employee*>*>* temp=company->workersSal;
    while(temp->right!=nullptr)
        temp=temp->right;
    AVLTree<int,Employee*>* emp_temp=temp->value;
    if(emp_temp==nullptr)
        return nullptr;
    while(emp_temp->left!=nullptr)
        emp_temp=emp_temp->left;
    return emp_temp->value;
}
StatusType Company::RemoveEmployeeByID(const int employee_id)
{
    AVLTree<int,Employee*>* employee = this->workersId->find(employee_id);
    if(employee == nullptr)
        return FAILURE;
    int sal_to_remove=employee->value->salary;
    this->workersId->remove(employee_id);
    this->workersSal=removeDuplicate(sal_to_remove,employee_id,this->workersSal);
    return SUCCESS;
}

void Company::CompanyInfo(int* v,int* c)
{//send pointer to local ints i.e. int num1=0,int* v=&num1 and then CompanyInfo(v..)
    *v=this->value;
}

void Company::setValue(const int value)
{
    this->value=value;
}



