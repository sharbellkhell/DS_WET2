#include "Workplace.h"

Workplace::Workplace(int k): non_zero_sal(0){
    companies = new UF(k);
    employees=new HashTable<Employee*>();
    emp_sals = nullptr;
}

Workplace Workplace::init(int k)
{
    return Workplace(k);
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

StatusType Workplace::addEmployee(int emp_id, int comp_id, int grade)
{
    if(emp_id<=0 || comp_id<=0 || grade<0 || comp_id>this->companies->size)
        return INVALID_INPUT;
    Employee* temp = new Employee(emp_id,comp_id,0,grade);
    if(this->employees->insertToTable(emp_id,temp)==false)
    {
        delete temp;
        return FAILURE;
    }
    return SUCCESS;
}

StatusType Workplace::removeEmployee(int emp_id)
{
    if(emp_id <= 0)
        return INVALID_INPUT;
    AVLTree<int,Employee*>* temp_emp=this->employees->find(emp_id);
    if(temp_emp==nullptr)
        return FAILURE;
    if(temp_emp->value->salary>0)
    {
        this->emp_sals=removeDuplicate(temp_emp->value->salary,emp_id,this->emp_sals);
        this->non_zero_sal--;
    }
    this->employees->remove(emp_id);
    delete(temp_emp);
    return SUCCESS;
}

static void mergeSals(AVLTree<int,AVLTree<int,Employee*>*>* acq,AVLTree<int,AVLTree<int,Employee*>*>* target, int id)
{
    //maybe useless
}
StatusType Workplace::acquireCompany(int acq_id, int target_id,double factor)
{
    int k = this->companies->size;
    if(acq_id<=0 || acq_id>k || target_id<=0 || target_id>k || factor <=0 || acq_id==target_id)
        return INVALID_INPUT;
    int acq=this->companies->Find(acq_id);
    int target=this->companies->Find(target_id);
    this->companies->Elements[acq]->value+=(factor*this->companies->Elements[target]->value);
    this->companies->Last_Values[target]=this->companies->Elements[acq]->value;
    AVLTree<int,AVLTree<int,Employee*>*>* temp = this->companies->Elements[target]->workersSal;
    while(temp->value!=nullptr)
    {
        this->companies->Elements[acq]->workersSal=insertDuplicate(temp->value->value->salary,temp->value->value,this->companies->Elements[acq]->workersSal);
        temp=removeDuplicate(temp->value->value->salary,temp->value->value->EmployeeId,temp);
    }
    return SUCCESS;
}

StatusType Workplace::employeeSalIncrease(int emp_id,int sal_increase)
{
    if(emp_id<=0||sal_increase<=0)
        return INVALID_INPUT;
    AVLTree<int, Employee*>* target=this->employees->find(emp_id);
    if(target==nullptr)
        return FAILURE;
    target->value->salary+=sal_increase;
    int sal=target->value->salary;
    if(sal-sal_increase!=0){
        this->emp_sals=removeDuplicate(sal,emp_id,this->emp_sals);
        this->companies->Elements[target->value->EmployerId]->workersSal=removeDuplicate(sal,emp_id,this->companies->Elements[target->value->EmployerId]->workersSal);
        this->non_zero_sal--;
    }
    if(sal!=0)
    {
        this->emp_sals=insertDuplicate(sal,target->value,this->emp_sals);
        this->companies->Elements[target->value->EmployerId]->workersSal=insertDuplicate(sal,target->value,this->companies->Elements[target->value->EmployerId]->workersSal);
        this->non_zero_sal++;
    }
    return SUCCESS;
}

StatusType Workplace::promoteEmp(int emp_id, int bump_grade)
{
    if(emp_id<=0||bump_grade<=0)//maybe negative is ok
        return INVALID_INPUT;
    AVLTree<int, Employee*>* target=this->employees->find(emp_id);
    if(target==nullptr)
        return FAILURE;
    target->value->grade+=bump_grade;
    target=target->parent;
    while(target->parent!=nullptr)
    {
        target->rank.SumGrades+=bump_grade;
        target=target->parent;
    }
    return SUCCESS;
}

StatusType Workplace::compValue(int comp_id)
{
    if(comp_id <= 0 || comp_id>this->companies->size)
        return INVALID_INPUT;
    this->companies->Find(comp_id);
    std::cout<<"CompanyValue: "<<this->companies->Elements[comp_id]->value<<"\n";
    return SUCCESS;
}
