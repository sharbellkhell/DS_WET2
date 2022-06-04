#ifndef EMPLOYEE_H_
#define EMPLOYEE_H_
#include <iostream>

class Employee{
    public:
        int EmployeeId;
        int EmployerId;
        int salary;
        int grade;
        Employee(int EmployeeId,int EmployerId,int salary,int grade) : EmployeeId(EmployeeId),EmployerId(EmployerId),
        salary(salary),grade(grade){};
        Employee(const Employee& emp) = default;
        ~Employee() = default;
        void EmployeeInfo(int* employerId, int* sal,int* grade) const;
        void Promote(int salary,int bump);
};


#endif