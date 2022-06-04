#include <iostream>
#include "Employee.h"
#include "exceptions.h"


void Employee::EmployeeInfo(int* employerId, int* sal,int* grade)const{
    *employerId=this->EmployerId;
    *sal=this->salary;
    *grade=this->grade;
}

void:: Employee::Promote(int salary, int bump)
{
    this->salary+=salary;
    if(bump>0)
        this->grade++;
}