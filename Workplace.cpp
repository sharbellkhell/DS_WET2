    #include "Employee.h"
    #include "AVLTree.h"
    #include "UF.h"
    #include "Workplace.h"
    #include "Company.h"

    Workplace::Workplace(int k): non_zero_sal(0), zero_sal_count(0), zero_sal_grades(0){
        companies = new UF(k);
        companies->InitAllElements();
        employees=new HashTable();
        emp_sals = nullptr;
    }

    Workplace* Workplace::init(int k)
    {
        Workplace* workplace = new Workplace(k);
        return workplace;
    }


    static AVLTree<int,AVLTree<int,Employee*>*>* insertDuplicateNode(int sal,Employee* emp,
                                                                    AVLTree<int,AVLTree<int,Employee*>*>* root)
    {

        AVLTree<int,AVLTree<int,Employee*>*>* temp=findNode(root,sal);
        if(temp==nullptr)
        {
            AVLTree<int, Employee*>* sal_Range=init(emp->EmployeeId,emp);
            sal_Range->rank.NumEmployees=1;
            sal_Range->rank.SumGrades=emp->grade;
            root=insertNode(sal,sal_Range,root,emp->grade);
        }
        else{
            temp->value=insertNode(emp->EmployeeId,emp,temp->value,emp->grade);
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
        bool a=false;
        AVLTree<int,AVLTree<int,Employee*>*>* temp=findNode(root,sal);
       
        AVLTree<int,AVLTree<int,Employee*>*>* rank_fixer=temp;
        AVLTree<int,Employee*>* emp=findNode(temp->value,emp_id);
        int g=emp->value->grade;
        if(temp!=nullptr){
            AVLTree<int,Employee*>* temp_emp=findNode(temp->value,emp_id);
            while(temp_emp!=nullptr && rank_fixer!=nullptr){
                rank_fixer->rank.SumGrades-=temp_emp->value->grade;
                rank_fixer=rank_fixer->parent;
            }

        }
        temp->value = removeNode(temp->value,emp_id,g); 
        if(temp->value==nullptr){
            root=removeNode(root,sal);
        }
        return root;
    }

    StatusType Workplace::addEmployee(int emp_id, int comp_id, int grade)
    {
        if(emp_id<=0 || comp_id<=0 || grade<0 || comp_id>this->companies->size)
            return INVALID_INPUT;
        comp_id=this->companies->Find(comp_id);
        Employee* temp = new Employee(emp_id,comp_id,0,grade);
        if(this->employees->insertToTable(emp_id,temp)==false)
        {
            delete temp;
            return FAILURE;
        }
        this->zero_sal_count++;
        this->zero_sal_grades+=grade;
        this->companies->Elements[comp_id]->nonZeroGrades+=grade;
        this->companies->Elements[comp_id]->nonZeroWorkers++;
        this->companies->Elements[comp_id]->workersId->insertToTable(emp_id,temp);
        delete temp;
        return SUCCESS;
    }

    StatusType Workplace::removeEmployee(int emp_id)
    {
        if(emp_id <= 0)
            return INVALID_INPUT;
        AVLTree<int,Employee*>* temp_emp=this->employees->find(emp_id);
        if(temp_emp==nullptr)
            return FAILURE;
        int was=this->emp_sals->rank.SumGrades;
        int g=temp_emp->value->grade;
        if(temp_emp->value->salary>0)
        { 
            //std::cout<<"removing "<<g<<" should be "<<this->emp_sals->rank.SumGrades-g<<"\n";
            this->companies->Elements[temp_emp->value->EmployerId]->workersSal=removeDuplicateNode(temp_emp->value->salary,emp_id,this->companies->Elements[temp_emp->value->EmployerId]->workersSal);
            this->emp_sals=removeDuplicateNode(temp_emp->value->salary,emp_id,this->emp_sals);
            this->non_zero_sal--;
            this->companies->Elements[temp_emp->value->EmployerId]->nonZeroCompEmps--;
        }
        if(temp_emp->value->salary==0)
        {
            this->companies->Elements[temp_emp->value->EmployerId]->nonZeroGrades-=temp_emp->value->grade;
            this->companies->Elements[temp_emp->value->EmployerId]->nonZeroWorkers--;
            this->zero_sal_count--;
            this->zero_sal_grades-=temp_emp->value->grade;
        }
        this->companies->Elements[temp_emp->value->EmployerId]->workersId->remove(emp_id);
        this->employees->remove(emp_id);
        temp_emp=this->employees->find(emp_id);
        return SUCCESS;
    }

    static void mergeHash(int acq_id ,HashTable* acq,HashTable* target)
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
    static Employee* getNextWorkerInHash(HashTable* hash)
    {
        if(hash->num_elements==0)
            return nullptr;
        for(int i=0;i<hash->array_size;i++)
            if(hash->elements[i]!=nullptr)
                return hash->elements[i]->value;
        return nullptr;
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
            
        int show=factor*this->companies->Elements[target]->value+this->companies->Elements[acq]->value;
        this->companies->Elements[acq]->value+=(factor*this->companies->Elements[target]->value);
            /*
            
        AVLTree<int,AVLTree<int,Employee*>*>* target_sals = this->companies->Elements[target]->workersSal;
        AVLTree<int,AVLTree<int,Employee*>*>* acq_sals=this->companies->Elements[acq]->workersSal;*/
        HashTable* target_hash=this->companies->Elements[target]->workersId;
        HashTable* acq_hash=this->companies->Elements[acq]->workersId;
        Employee* next=getNextWorkerInHash(target_hash);
        int size=target_hash->array_size;
        while(next!=nullptr)
        {
            
            Employee* save= new Employee(next->EmployeeId,acq,next->salary,next->grade);
            this->removeEmployee(next->EmployeeId);
            target_hash->remove(save->EmployeeId);
            this->addEmployee(save->EmployeeId,save->EmployerId,save->grade);
            if(save->salary!=0)
                this->employeeSalIncrease(save->EmployeeId,save->salary);
            delete save;
            next=getNextWorkerInHash(target_hash);
        }
        this->companies->Union(acq,target);
        /*for(int i=0;i<target_hash->array_size;i++)
        {
            while(target_hash->elements[i]!=nullptr)
            {   
                target_hash->elements[i]->value->EmployerId=acq_id;
                Employee* temp_emp=target_hash->elements[i]->value;
                acq_hash->insertToTable(temp_emp->EmployeeId,temp_emp);
                AVLTree<int, Employee*>* copy=acq_hash->find(temp_emp->EmployeeId);
                if(copy->value->salary>0){
                    acq_sals=insertDuplicateNode(copy->value->salary,copy->value,acq_sals);
                    target_sals=removeDuplicateNode(target_sals->value->value->salary,target_sals->value->value->EmployeeId,target_sals);
                }
                target_hash->remove(temp_emp->EmployeeId,1);
            }
        }
        while(temp!=nullptr)
        {
            temp->value->value->EmployerId=acq;
            this->companies->Elements[acq]->workersSal=insertDuplicateNode(temp->value->value->salary,temp->value->value,this->companies->Elements[acq]->workersSal);
            temp=removeDuplicateNode(temp->value->value->salary,temp->value->value->EmployeeId,temp);
        }
        mergeHash(acq, this->companies->Elements[acq]->workersId,this->companies->Elements[target]->workersId);
        delete[] target_hash->elements;
        target_hash->elements=new AVLTree<int,Employee*>*[1];
        target_hash->elements[0]=nullptr;
        target_hash->array_size=1;*/
        return SUCCESS;
    }

    StatusType Workplace::employeeSalIncrease(int emp_id,int sal_increase)
    {
        Company* comp=this->companies->Elements[43];
        if(emp_id<=0||sal_increase<=0)
            return INVALID_INPUT;
        AVLTree<int, Employee*>* target=this->employees->find(emp_id);
        if(target==nullptr)
            return FAILURE;
        int comp_id=target->value->EmployerId;
        AVLTree<int, Employee*>* copy=this->companies->Elements[comp_id]->workersId->find(emp_id);
        target->value->salary+=sal_increase;
        copy->value->salary+=sal_increase;
        int sal=target->value->salary;
        if(sal-sal_increase!=0){//if salary wasnt 0
            this->emp_sals=removeDuplicateNode(sal-sal_increase,emp_id,this->emp_sals);
            this->companies->Elements[comp_id]->workersSal=removeDuplicateNode(sal-sal_increase,emp_id,this->companies->Elements[comp_id]->workersSal);
            this->non_zero_sal--;
        }
        else{//salary was 0 it was
            //if(this->emp_sals!=nullptr)
              //  std::cout<<"adding "<<copy->value->grade<<" should be "<<this->emp_sals->rank.SumGrades+copy->value->grade<<"\n";
            this->zero_sal_count--;
            this->zero_sal_grades-=target->value->grade;
            this->companies->Elements[comp_id]->nonZeroWorkers--;
            this->companies->Elements[comp_id]->nonZeroGrades-=target->value->grade;
            this->non_zero_sal++;
            this->companies->Elements[comp_id]->nonZeroCompEmps++;
        }

        this->emp_sals=insertDuplicateNode(sal,target->value,this->emp_sals);
        //Employee* temp= new Employee(copy->value->EmployeeId,copy->value->EmployerId,copy->value->salary,copy->value->grade);
        this->companies->Elements[comp_id]->workersSal=insertDuplicateNode(sal,copy->value,this->companies->Elements[comp_id]->workersSal);
        return SUCCESS;
    }

    static AVLTree<int,AVLTree<int, Employee*>*>* findSalaryRange(AVLTree<int,AVLTree<int, Employee*>*>* root, int sal)
    {
        if(root==nullptr || root->key==sal)
            return root;
        if(root->key<sal)
            return findSalaryRange(root->right,sal);
        return findSalaryRange(root->left,sal);
    }
    StatusType Workplace::promoteEmp(int emp_id, int bump_grade)
    {
        
        Company* comp=this->companies->Elements[26];
        if(emp_id <= 0)
            return INVALID_INPUT;
        AVLTree<int, Employee*>* target=this->employees->find(emp_id);
        if(target == nullptr || target->value == nullptr) {
            return FAILURE;
        }
        if(bump_grade<=0)
            return SUCCESS;
        int was=this->emp_sals->rank.SumGrades;
        int comp_id = target->value->EmployerId;
        int old_grade = target->value->grade;
        int salary = target->value->salary;
        AVLTree<int, Employee*>* targett=this->companies->Elements[comp_id]->workersId->find(emp_id);
        target->value->grade+=bump_grade;
        targett->value->grade+=bump_grade;
        /*
        this->removeEmployee(emp_id);
        this->addEmployee(emp_id,comp_id,old_grade+bump_grade);*/
        if(salary==0)
        {
            this->zero_sal_grades+=bump_grade;
            this->companies->Elements[comp_id]->nonZeroGrades+=bump_grade;

        }
        else{ // in such case also update employee inside trees
            //std::cout<<"adding "<<bump_grade<<" should be "<<this->emp_sals->rank.SumGrades+bump_grade<<"\n";
            AVLTree<int,AVLTree<int, Employee*>*>* total=findSalaryRange(this->emp_sals,salary);
            AVLTree<int,AVLTree<int, Employee*>*>* inside_comp=findSalaryRange(this->companies->Elements[comp_id]->workersSal,salary);
            AVLTree<int, Employee*>* emp_total=findNode(total->value,emp_id);
            AVLTree<int, Employee*>* emp_comp=findNode(inside_comp->value,emp_id);
            while(total!=nullptr){
                total->rank.SumGrades+=bump_grade;
                total=total->parent;
                }
            while(inside_comp!=nullptr){
                inside_comp->rank.SumGrades+=bump_grade;
                inside_comp=inside_comp->parent;
                }
            while(emp_comp!=nullptr)
            {
                emp_comp->rank.SumGrades+=bump_grade;
                emp_comp=emp_comp->parent;
            }
            while(emp_total)
            {
                emp_total->rank.SumGrades+=bump_grade;
                emp_total=emp_total->parent;
            }
        }
        /*
        else    
            this->companies->Elements[target->value->EmployerId]->nonZeroGrades+=bump_grade;
        target=this->employees->find(emp_id);
        target->value->grade+=bump_grade;
        target->rank.SumGrades+=bump_grade;
        if(target->value->salary==0)
            this->zero_sal_grades+=bump_grade;
        target=target->parent;
        if(target==nullptr)
            return SUCCESS;
        while(target->parent!=nullptr)
        {
            target->rank.SumGrades+=bump_grade;
            target=target->parent;
        }*/
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
    static double aux_averageGrade(AVLTree<int,AVLTree<int,Employee*>*>* sal_tree, int l_sal, int h_sal, long long zero_sum_grade=0, long long zero_worker_count=0)
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
        if(sal_tree==nullptr && zero_worker_count==0)
            return -1;
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
        return (double)sum_grades/worker_count;
    }
    StatusType Workplace::averageGradeInRange(int comp_id, int l_sal, int h_sal)
    {
        if(l_sal<0 || h_sal<0|| h_sal<l_sal || comp_id<0 || comp_id>this->companies->size)
            return INVALID_INPUT;
        AVLTree<int,AVLTree<int,Employee*>*>* target=this->emp_sals;
        if(comp_id!=0)
            target=this->companies->Elements[comp_id]->workersSal;
        double result;
        if(h_sal==0 || l_sal==0)
            if(comp_id!=0)
                result = aux_averageGrade(target,l_sal,h_sal,this->companies->Elements[comp_id]->nonZeroGrades,this->companies->Elements[comp_id]->nonZeroWorkers);
            else  
                result = aux_averageGrade(target,l_sal,h_sal,this->zero_sal_grades,this->zero_sal_count);  
        else
            result = aux_averageGrade(target,l_sal,h_sal);
        if(result==-1)
            return FAILURE;
        //printf("AverageBumpGradeBetweenSalaryByGroup: %.1f\n", result); TODO FIX
        printf("AverageBumpGradeBetweenSalaryByGroup: %.1f\n", floor(10 * result + 0.5f) / 10);
        return SUCCESS;
    }

//    typedef AVLTree<int,AVLTree<int,Employee*>*> AVLAVL;

    RankInfo fixRanksAVLAVL(AVLAVL* root){
        if(root == nullptr){
            return RankInfo();
        }
        AVLAVL* original_root = root;
        if(root->left != nullptr){
            fixRanksAVLAVL(root->left);
        }
        if(root->right != nullptr){
            fixRanksAVLAVL(root->right);
        }
        root = original_root;
        root->rank.NumEmployees = root->value->rank.NumEmployees
                                  + fixRanksAVLAVL(root->left).NumEmployees
                                  + fixRanksAVLAVL(root->right).NumEmployees;

        root->rank.SumGrades = root->value->rank.SumGrades
                               + fixRanksAVLAVL(root->left).SumGrades
                               + fixRanksAVLAVL(root->right).SumGrades;

        return RankInfo(root->rank.NumEmployees,root->rank.SumGrades);

    }

    enum PossibleCases {AllBiggerThanM, FoundEqualToM, FoundLessThanM };

    AVLAVL* findRightMostLessOrEqualThanM(AVLAVL* tree, PossibleCases* found, int m){
        AVLAVL* iterator = tree;
        if(iterator->right == nullptr){ // one node
            if(iterator->rank.NumEmployees == m){
                (*found) = FoundEqualToM;
                return iterator;
            } else if(iterator->rank.NumEmployees > m){
                (*found) = AllBiggerThanM;
                return iterator;
            } else{
                (*found) = FoundLessThanM;
                return iterator;
            }
        }
        while(iterator->right != nullptr){
            iterator = iterator->right; //Iterator is Right Most
        }
        if(iterator->rank.NumEmployees == m){
            (*found) = FoundEqualToM;
            return iterator;
        } else if (iterator->rank.NumEmployees > m) {
            (*found) = AllBiggerThanM;
            return iterator;
        } else if (iterator->rank.NumEmployees < m) {
            (*found) = FoundLessThanM;
            return iterator;
        }

        iterator = tree;
        while(iterator->right != nullptr){
            if(iterator->rank.NumEmployees < m){
                (*found) = FoundLessThanM;
                return iterator;
            }
            iterator = iterator->right;
        }
        assert(false == true); //TODO DELETE to check never reaches
        return nullptr;
    }


    AVLTree<int,Employee*>* selectKthElement(AVLTree<int,Employee*>* tree, int k){
        if(k == 0 || tree==nullptr){
            return  tree;
        }
        int left_employees = (tree->left) ? tree->left->rank.NumEmployees : 0;
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
        if(num_of_all_employees == k){
            return tree->rank.SumGrades;
        }
        AVLTree<int,Employee*>* iterator = selectKthElement(tree, num_of_all_employees - k);
        // points to employee with rank m-k (smaller than Top Employees By one)
        long long sum = 0;
        SonType son_type;
        while (iterator != nullptr){
            son_type = whichSonIsNode<int,Employee*>(iterator);
            switch (son_type){
                case root: {break;}
                case isRight: {break;}
                case isLeft: {
                    if(iterator->parent){
                        sum += getSumGradesOnlyInNode<int,Employee*>(iterator->parent);
                        if(iterator->parent->right){
                            sum += getSumGradesOnlyInNode<int,Employee*>(iterator->parent->right);
                        }
                    }
                    break;
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
        PossibleCases* found = new PossibleCases();
        AVLAVL* v = findRightMostLessOrEqualThanM(tree, found, m - (*count_added));
        switch (*found) {
            case FoundEqualToM:{
                *sum_grades += v->rank.SumGrades;
                *count_added += v->rank.NumEmployees;
                delete(found);
                return;
            }
            case FoundLessThanM:{
                *sum_grades += v->rank.SumGrades;
                *count_added += v->rank.NumEmployees;
                if(getNumEmployeesOnlyInNode<int,AVLTree<int,Employee*>*>(v->parent) > m - (*count_added)){
                    *sum_grades += getKTopEmployeesSumGradesInternalTree(v->parent->value, m - (*count_added));
                    *count_added = m;
                    delete(found);
                    return;
                }
                else{ // parent doesn't have enough employees, take all of them and go left
                    *sum_grades += getSumGradesOnlyInNode<int,AVLTree<int,Employee*>*>(v->parent);
                    *count_added += getNumEmployeesOnlyInNode<int,AVLTree<int,Employee*>*>(v->parent);
                    iterateAndSum(v->parent->left,m,sum_grades,count_added);
                }
                break;
            }
            case AllBiggerThanM:
                *sum_grades += getKTopEmployeesSumGradesInternalTree(v->value, m - (*count_added));
                *count_added = m;
                delete(found);
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
                delete sum_grades;
                delete count_added;
                return FAILURE;
            }
            fixRanksAVLAVL(this->emp_sals); //TODO
            if(this->non_zero_sal == m){
                (*sum_grades) = this->emp_sals->rank.SumGrades;
            } else {
                iterateAndSum(this->emp_sals, m,sum_grades,count_added);
            }
        }
        else{
            if(companies->Elements[comp_id]->nonZeroCompEmps < m){
                delete sum_grades;
                delete count_added;
                return FAILURE;
            }
            fixRanksAVLAVL(companies->Elements[comp_id]->workersSal);
            if(companies->Elements[comp_id]->nonZeroCompEmps == m){
                (*sum_grades) = companies->Elements[comp_id]->workersSal->rank.SumGrades;
            }
            else{
                iterateAndSum(companies->Elements[comp_id]->workersSal, m, sum_grades,count_added);
            }
        }
        printf("SumOfBumpGradeBetweenTopWorkersByGroup: %.1lld\n", (*sum_grades) );
        delete sum_grades;
        delete count_added;
        return SUCCESS;

    }

    void Workplace::Quit()
    {
        
        while(this->emp_sals!=nullptr)
        {
            this->emp_sals=removeDuplicateNode(this->emp_sals->key,this->emp_sals->value->value->EmployeeId,this->emp_sals);
        }
        for(int j=1;j<this->companies->size+1;j++)
        {
            while(this->companies->Elements[j]->workersSal!=nullptr)
            {
                this->companies->Elements[j]->workersSal=removeDuplicateNode(this->companies->Elements[j]->workersSal->key,this->companies->Elements[j]->workersSal->value->value->EmployeeId,this->companies->Elements[j]->workersSal);
            }
        }
        for(int i=0;i<this->employees->array_size;i++)
            while(this->employees->elements[i]!=nullptr)
            {
                this->employees->remove(this->employees->elements[i]->key,1);
            }
        delete this->companies;
        delete this->employees;
    }