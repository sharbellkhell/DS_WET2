#include "Workplace.h"

Workplace::Workplace(int k): non_zero_sal(0), zero_sal_count(0), zero_sal_grades(0){
    companies = new UF(k);
    companies->InitAllElements();
    employees=new HashTable<Employee*>();
    emp_sals = nullptr;
}

Workplace* Workplace::init(int k)
{
    Workplace* workplace = new Workplace(k);
    return workplace;
}

static AVLTree<int,AVLTree<int,Employee*>*>* insertDuplicateNode(int sal,Employee* emp,AVLTree<int,AVLTree<int,Employee*>*>* root)
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

static AVLTree<int,AVLTree<int,Employee*>*>* removeDuplicateNode(int sal,int emp_id,AVLTree<int,AVLTree<int,Employee*>*>* root)
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
        root=nullptr;
    }
    return root;
}

StatusType Workplace::addEmployee(int emp_id, int comp_id, int grade)
{
    if(emp_id<=0 || comp_id<=0 || grade<0 || comp_id>this->companies->size)
        return INVALID_INPUT;
    Employee* temp = new Employee(emp_id,comp_id,0,grade);
    Employee* copy = new Employee(emp_id,comp_id,0,grade);
    if(this->employees->insertToTable(emp_id,temp)==false)
    {
        delete temp;
        return FAILURE;
    }
    this->zero_sal_count++;
    this->zero_sal_grades+=grade;
    this->employees->insertToTable(emp_id,temp);
    this->companies->Elements[comp_id]->workersId->insertToTable(emp_id,copy);
    return SUCCESS;
}

StatusType Workplace::removeEmployee(int emp_id)
{
    if(emp_id <= 0)
        return INVALID_INPUT;
    AVLTree<int,Employee*>* temp_emp=this->employees->find(emp_id);
    AVLTree<int,Employee*>* copy=this->companies->Elements[temp_emp->value->EmployerId]->workersId->find(emp_id);
    if(temp_emp==nullptr)
        return FAILURE;
    if(temp_emp->value->salary>0)
    { 
        this->companies->Elements[temp_emp->value->EmployerId]->workersSal=removeDuplicateNode(temp_emp->value->salary,emp_id,this->emp_sals);
        this->emp_sals=removeDuplicateNode(temp_emp->value->salary,emp_id,this->emp_sals);
        this->non_zero_sal--;
    }
    if(temp_emp->value->salary==0)
    {
        this->zero_sal_count--;
        this->zero_sal_grades-=temp_emp->value->grade;
    }
    this->companies->Elements[temp_emp->value->EmployerId]->workersId->remove(emp_id);
    delete temp_emp->value;
    delete copy;
    this->employees->remove(emp_id);
    return SUCCESS;
}

static void mergeHash(int acq_id ,HashTable<Employee*>* acq,HashTable<Employee*>* target)
{
    for(int i=0;i<target->array_size;i++)
    {
        while(target->elements[i]!=nullptr && target->elements[i]->value!=nullptr)
        {
            target->elements[i]->value->EmployerId=acq_id;
            acq->insertToTable(target->elements[i]->key,target->elements[i]->value);
            target->remove(target->elements[i]->key,1);
        }
    }
    delete[] target->elements;
    target->elements=new AVLTree<int,Employee*>*[1];
    target->elements[0]=nullptr;
    target->array_size=1;
}
StatusType Workplace::acquireCompany(int acq_id, int target_id,double factor)
{
    int k = this->companies->size;
    if(acq_id<=0 || acq_id>k || target_id<=0 || target_id>k || factor <=0 || acq_id==target_id)
        return INVALID_INPUT;
    int acq=this->companies->Find(acq_id);
    int target=this->companies->Find(target_id);
    if(acq==target)
        return INVALID_INPUT;
    this->companies->Union(acq,target);
    this->companies->Elements[acq]->value+=(factor*this->companies->Elements[target]->value);
    this->companies->Last_Values[target]=this->companies->Elements[acq]->value;
    AVLTree<int,AVLTree<int,Employee*>*>* temp = this->companies->Elements[target]->workersSal;
    while(temp!=nullptr && temp->value!=nullptr)
    {
        temp->value->value->EmployerId=acq;
        this->companies->Elements[acq]->workersSal=insertDuplicateNode(temp->value->value->salary,temp->value->value,this->companies->Elements[acq]->workersSal);
        temp=removeDuplicateNode(temp->value->value->salary,temp->value->value->EmployeeId,temp);
    }
    mergeHash(acq, this->companies->Elements[acq]->workersId,this->companies->Elements[target]->workersId);
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
        this->emp_sals=removeDuplicateNode(sal,emp_id,this->emp_sals);
        this->companies->Elements[target->value->EmployerId]->workersSal=removeDuplicateNode(sal,emp_id,this->companies->Elements[target->value->EmployerId]->workersSal);
        this->non_zero_sal--;
    }
    if(sal!=0)
    {
        this->emp_sals=insertDuplicateNode(sal,target->value,this->emp_sals);
        this->companies->Elements[target->value->EmployerId]->workersSal=insertDuplicateNode(sal,target->value,this->companies->Elements[target->value->EmployerId]->workersSal);
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

typedef AVLTree<int,AVLTree<int,Employee*>*> AVLAVL;

AVLAVL* findRightMostLessOrEqualThanM(AVLAVL* tree, bool* found, int m){
    while(tree->right != nullptr && tree->right->rank.NumEmployees > m){
        tree = tree->right;
    }
    *found = (tree->right != nullptr);
    return (*found) ? tree->right : tree;
}

template<class Key,class Value>
long long getSumGradesOnlyInNode(AVLTree<Key,Value>* node){
    if(node == nullptr){
        return 0;
    }
    long long rank_node = node->rank.SumGrades;
    long long rank_left = (node->left) ? node->left->rank.SumGrades : 0;
    long long rank_right = (node->right) ? node->right->rank.SumGrades : 0;
    return (rank_node - rank_right - rank_left);
}


template<class Key,class Value>
int getNumEmployeesOnlyInNode(AVLTree<Key,Value>* node){
    if(node == nullptr){
        return 0;
    }
    int rank_node = node->rank.NumEmployees;
    int rank_left = (node->left) ? node->left->rank.NumEmployees : 0;
    int rank_right = (node->right) ? node->right->rank.NumEmployees : 0;
    return (rank_node - rank_right - rank_left);
}

AVLTree<int,Employee*>* selectKthElement(AVLTree<int,Employee*>* tree, int k){
    int left_employees = tree->left->rank.NumEmployees;
    if(left_employees == k-1){
        return tree;
    }
    else if (left_employees > k-1) {
        return selectKthElement(tree->left, k);
    }
    else if (left_employees < k-1) {
        return selectKthElement(tree->right, k - left_employees - 1);
    }
    return nullptr;///what?
}



long long getKTopEmployeesSumGradesInternalTree(AVLTree<int,Employee*>* tree, int k){
    int num_of_all_employees = tree->rank.NumEmployees;
    AVLTree<int,Employee*>* iterator = selectKthElement(tree, num_of_all_employees - k);
    // points to employee with rank m-k (smaller than Top Employees By one)
    long long sum = 0;
    SonType son_type;
    while (iterator != nullptr){
        son_type = whichSonIsNode<int,Employee*>(iterator);
        switch (son_type){
            case root: {}
            case isRight: {}
            case isLeft: {
                sum += getSumGradesOnlyInNode<int,Employee*>(iterator->parent);
                sum += getSumGradesOnlyInNode<int,Employee*>(iterator->parent->right);
            }
        }
        iterator = iterator->parent;
    }
    return sum;
}

void iterateAndSum (AVLAVL* tree ,int m, long long* sum_grades, int* count_added){
    //TODO DELETE ASSERT
    assert((*count_added) <= m);
    if(tree == nullptr || (*count_added) == m ){
        return;
    }
    bool* found = new bool();
    AVLAVL* v = findRightMostLessOrEqualThanM(tree, found, m);
    // If found, then V is the Right most node with <=m employees
    if(*found){
        if(v->rank.NumEmployees == m - (*count_added)){
            *sum_grades += v->rank.SumGrades;
            *count_added += v->rank.NumEmployees;
            return;
        }
        else if(v->rank.NumEmployees < m - (*count_added)){
            *sum_grades += v->rank.SumGrades;
            *count_added += v->rank.NumEmployees;
            if(getNumEmployeesOnlyInNode<int,AVLTree<int,Employee*>*>(v->parent) > m - (*count_added)){
                *sum_grades += getKTopEmployeesSumGradesInternalTree(v->parent->value, m - (*count_added));
                *count_added = m;
                return;
            }
            else{ // parent doesn't have enough employees, take all of them and go left
                *sum_grades += getSumGradesOnlyInNode<int,AVLTree<int,Employee*>*>(v->parent);
                *count_added += getNumEmployeesOnlyInNode<int,AVLTree<int,Employee*>*>(v->parent);
                iterateAndSum(v->parent->left,m,sum_grades,count_added);
            }
        }
    }
    else{ //if there's >m employees in v
        *sum_grades += getKTopEmployeesSumGradesInternalTree(v->value, m - (*count_added));
        *count_added = m;
        return;
    }
    delete(found);
    return;
}


StatusType Workplace::sumGradesBetweenTop(int comp_id, int m)
{
    if(m<=0 || comp_id <0 || comp_id>this->companies->size){
        return INVALID_INPUT;
    }
    long long* sum_grades = new long long();
    int* count_added = new int();
    (*sum_grades) = 0;
    (*count_added) = 0;
    if(comp_id == 0)
    {
        if(this->non_zero_sal<m){
            return FAILURE;
        }
        iterateAndSum(this->emp_sals, m,sum_grades,count_added);
    }
    else{
        iterateAndSum(companies->Elements[comp_id]->workersSal, m, sum_grades,count_added);
    }
    printf("sumOfBumpGradeBetweenTopWorkersByGroup: %.1f\n", sum_grades);
    return SUCCESS;

}

void Workplace::Quit()
{
    while(this->emp_sals!=nullptr)
    {
        this->emp_sals=removeDuplicateNode(this->emp_sals->key,this->emp_sals->value->value->EmployeeId,this->emp_sals);
    }
    for(int i=0;i<this->employees->array_size;i++)
        while(this->employees->elements[i]!=nullptr)
        {
            delete this->employees->elements[i]->value;
            this->employees->remove(this->employees->elements[i]->key,1);
        }
    delete this->employees;
    delete this->companies;
}