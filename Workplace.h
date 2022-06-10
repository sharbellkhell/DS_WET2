#ifndef WORKPLACE_H_
#define WORKPLACE_H_

#include <iostream>
#include "Company.h"
#include "UF.h"

class Workplace{
    public:
        UF* companies;
        HashTable<Employee*>* employees;
        AVLTree<int,AVLTree<int,Employee*>*>* emp_sals;
        int non_zero_sal;
        long long zero_sal_grades;
        long long zero_sal_count;
        explicit Workplace(int k);
        ~Workplace() = default;
        static Workplace init(int k);
        StatusType addEmployee(int emp_id, int comp_id, int grade);
        StatusType removeEmployee(int emp_id);
        StatusType acquireCompany(int acq_id, int target_id,double factor);
        StatusType employeeSalIncrease(int emp_id,int sal_increase);
        StatusType promoteEmp(int emp_id, int bump_grade);
        StatusType sumGradesBetweenTop(int comp_id, int m);
        StatusType averageGradeInRange(int comp_id, int l_sal, int h_sal);
        StatusType compValue(int comp_id);
        StatusType promoteAllInRange(int l_sal, int h_sal, int bump_grade);//bonus
};
















#endif