#include <iostream>
#include "library2.h"
#include "hashTable.h"
#include "Employee.h"
#include "AVLTree.h"
#include "Company.h"

Company::Company(int company_id,int value) : companyId(company_id),value(value),add_value(0),purchased(0) {
    workersId = new HashTable<Employee*>();
    workersSal = nullptr;
}
Company::~Company()
{
    Quit(workersSal);
    delete workersId;
}
static AVLTree<int,AVLTree<int,Employee*>*>* insertDuplicateNode(int sal,Employee* emp,AVLTree<int,AVLTree<int,Employee*>*>* root)
{
    AVLTree<int,AVLTree<int,Employee*>*>* temp=findNode(root,sal);
    if(temp==nullptr)
    {
        AVLTree<int, Employee*>* sal_Range=init(emp->EmployeeId,emp);
        root=insertNode(sal,sal_Range,root);
        return root;
    }
    else{
        temp->value=insertNode(emp->EmployeeId,emp,temp->value);
        return root;
    }

}

StatusType Company::AddEmployee(Employee* emp){
    try{
    this->workersId->insertToTable(emp->EmployeeId,emp);
    if(emp->salary!=0)
        this->workersSal = insertDuplicateNode(emp->salary, emp, this->workersSal);
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
static AVLTree<int,AVLTree<int,Employee*>*>* removeDuplicateNode(int sal,int emp_id,AVLTree<int,AVLTree<int,Employee*>*>* root)
{
    AVLTree<int,AVLTree<int,Employee*>*>* temp=findNode(root,sal);
    if(temp!=nullptr)
        temp->value=removeNode(temp->value,emp_id);
    if(temp!=nullptr && temp->value==nullptr) root=removeNode(root,sal);
    return root;
}
StatusType Company::RemoveEmployeeByID(const int employee_id)
{
    AVLTree<int,Employee*>* employee = this->workersId->find(employee_id);
    if(employee == nullptr)
        return FAILURE;
    int sal_to_remove=employee->value->salary;
    this->workersId->remove(employee_id);
    this->workersSal=removeDuplicateNode(sal_to_remove,employee_id,this->workersSal);
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



