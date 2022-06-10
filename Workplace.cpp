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
    this->zero_sal_count++;
    this->zero_sal_grades+=grade;
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
    if(temp_emp->value->salary==0)
    {
        this->zero_sal_count--;
        this->zero_sal_grades-=temp_emp->value->grade;
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
    this->zero_sal_count--;
    this->zero_sal_grades-=target->value->grade;
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
    if(emp_id<=0)//maybe negative is ok
        return INVALID_INPUT;
    AVLTree<int, Employee*>* target=this->employees->find(emp_id);
    if(target==nullptr)
        return FAILURE;
    if(bump_grade<=0)
        return SUCCESS;
    target->value->grade+=bump_grade;
    target=target->parent;
    this->zero_sal_grades+=bump_grade;
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
    printf("CompanyValue: %.1f\n", this->companies->Elements[comp_id]->value);
    return SUCCESS;
}

static long long aux_averageGrade(AVLTree<int,AVLTree<int,Employee*>*>* sal_tree, int l_sal, int h_sal, long long zero_sum_grade=0, long long zero_worker_count=0)
{
    //find first node in range
    bool flag = false;
    while(sal_tree!=nullptr && flag==false)
    {
        if(sal_tree->key<l_sal)
            sal_tree=sal_tree->right;
        else if(sal_tree->key>h_sal)
            sal_tree=sal_tree->left;
        else flag=true;
    }
    if(sal_tree==nullptr)
        return 0;
    //lets start, add and subtract the nessecary values around the right bound
    long long sum_grades=zero_sum_grade;
    long long worker_count=zero_worker_count;
    AVLTree<int,AVLTree<int,Employee*>*>* right_bound=sal_tree;
    while(right_bound!=nullptr)
    {
        if(flag)
            {
                sum_grades+=right_bound->rank.SumGrades;
                worker_count+=right_bound->rank.NumEmployees;
                right_bound=out_side_right(right_bound,h_sal);
                flag=false;
            }
        else{
            sum_grades-=right_bound->rank.SumGrades;
            worker_count-=right_bound->rank.NumEmployees;
            right_bound=in_side_right(right_bound,h_sal);
            flag=true;
        }
    }
    flag=false;
    //do the same for the ledt bound
    AVLTree<int,AVLTree<int,Employee*>*>* left_bound=out_side_left(sal_tree,l_sal);
    while(left_bound!=nullptr)
    {   
        if(flag)
        {
            sum_grades+=left_bound->rank.SumGrades;
            worker_count+=left_bound->rank.NumEmployees;
            left_bound=out_side_left(left_bound,l_sal);
            flag=false;
        }
        else{
            sum_grades-=left_bound->rank.SumGrades;
            worker_count-=left_bound->rank.NumEmployees;
            left_bound=in_side_left(left_bound,l_sal);
            flag=true;
        }
    }
    //return results
    if(worker_count==0)
        return -1;
    return sum_grades/worker_count;
    
}
static AVLTree<int,AVLTree<int,Employee*>*>* out_side_right(AVLTree<int,AVLTree<int,Employee*>*>* sal_tree, int h_sal)
{
    while(sal_tree!=nullptr)
    {
        if(sal_tree->key>h_sal)
                return sal_tree;
        sal_tree=sal_tree->right;
    }
    return nullptr;
}

static AVLTree<int,AVLTree<int,Employee*>*>* out_side_left(AVLTree<int,AVLTree<int,Employee*>*>* sal_tree, int l_sal)
{
    while(sal_tree!=nullptr)
    {
        if(sal_tree->key<l_sal)
                return sal_tree;
        sal_tree=sal_tree->left;
    }
    return nullptr;
}

static AVLTree<int,AVLTree<int,Employee*>*>* in_side_right(AVLTree<int,AVLTree<int,Employee*>*>* sal_tree, int h_sal)
{
    while(sal_tree!=nullptr)
    {
        if(sal_tree->key<=h_sal)
                return sal_tree;
        sal_tree=sal_tree->left;
    }
    return nullptr;
}

static AVLTree<int,AVLTree<int,Employee*>*>* in_side_left(AVLTree<int,AVLTree<int,Employee*>*>* sal_tree, int l_sal)
{
    while(sal_tree!=nullptr)
    {
        if(sal_tree->key>=l_sal)
                return sal_tree;
        sal_tree=sal_tree->right;
    }
    return nullptr;
}
StatusType Workplace::averageGradeInRange(int comp_id, int l_sal, int h_sal)
{
    if(l_sal<0 || h_sal<0|| h_sal<l_sal || comp_id<0 || comp_id>this->companies->size)
        return INVALID_INPUT;
    AVLTree<int,AVLTree<int,Employee*>*>* target=this->emp_sals;
    if(comp_id!=0)
        target=this->companies->Elements[comp_id]->workersSal;
    long long result;
    if(h_sal==0 || l_sal==0)
        result = aux_averageGrade(target,l_sal,h_sal,this->zero_sal_grades,this->zero_sal_count);
    else
        result = aux_averageGrade(target,l_sal,h_sal);
    if(result==-1)
        return FAILURE;
    printf("AverageBumpGradeBetweenSalaryByGroup: %.1f\n", result);
    return SUCCESS;
}