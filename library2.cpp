#include <iostream>
#include "library2.h"
#include "Workplace.h"


void* Init(int k){
    if(k <= 0){
        return nullptr;
    }
    Workplace* DS = new Workplace(k);
    return (void*)DS;
}

StatusType AddEmployee(void *DS, int employeeID, int companyID, int grade){
    if(DS == nullptr){
        return INVALID_INPUT;
    }
    StatusType result;
    try{
        result = ((Workplace*)DS)->addEmployee(employeeID,companyID,grade);
    }
    catch(std::bad_alloc&){
        return ALLOCATION_ERROR;
    }
    return result;
}

StatusType RemoveEmployee(void *DS, int employeeID){
    if(DS == nullptr){
        return INVALID_INPUT;
    }
    StatusType result;
    try{
        result = ((Workplace*)DS)->removeEmployee(employeeID);
    }
    catch(std::bad_alloc&){
        return ALLOCATION_ERROR;
    }
    return result;
}

StatusType AcquireCompany(void *DS, int companyID1, int companyID2, double factor){
    if(DS == nullptr){
        return INVALID_INPUT;
    }
    StatusType result;
    try{
        result = ((Workplace*)DS)->acquireCompany(companyID1,companyID2,factor);
    }
    catch(std::bad_alloc&){
        return ALLOCATION_ERROR;
    }
    return result;
}

StatusType EmployeeSalaryIncrease(void *DS, int employeeID, int salaryIncrease){
    if(DS == nullptr){
        return INVALID_INPUT;
    }
    StatusType result;
    try{
        result = ((Workplace*)DS)->employeeSalIncrease(employeeID,salaryIncrease);
    }
    catch(std::bad_alloc&){
        return ALLOCATION_ERROR;
    }
    return result;
}

StatusType PromoteEmployee(void *DS, int employeeID, int bumpGrade){
    if(DS == nullptr){
        return INVALID_INPUT;
    }
    StatusType result;
    try{
        result = ((Workplace*)DS)->promoteEmp(employeeID,bumpGrade);
    }
    catch(std::bad_alloc&){
        return ALLOCATION_ERROR;
    }
    return result;
}

StatusType SumOfBumpGradeBetweenTopWorkersByGroup(void *DS, int companyID, int m){
    if(DS == nullptr){
        return INVALID_INPUT;
    }
    StatusType result;
    try{
        result = ((Workplace*)DS)->sumGradesBetweenTop(companyID,m);
    }
    catch(std::bad_alloc&){
        return ALLOCATION_ERROR;
    }
    return result;
}

StatusType AverageBumpGradeBetweenSalaryByGroup(void *DS, int companyID, int lowerSalary, int higherSalary){
    if(DS == nullptr){
        return INVALID_INPUT;
    }
    StatusType result;
    try{
        result = ((Workplace*)DS)->averageGradeInRange(companyID,lowerSalary,higherSalary);
    }
    catch(std::bad_alloc&){
        return ALLOCATION_ERROR;
    }
    return result;
}

StatusType CompanyValue(void *DS, int companyID){
    if(DS == nullptr){
        return INVALID_INPUT;
    }
    StatusType result;
    try{
        result = ((Workplace*)DS)->compValue(companyID);
    }
    catch(std::bad_alloc&){
        return ALLOCATION_ERROR;
    }
    return result;
}

void Quit(void** DS){
    if(DS == nullptr){
        return;
    }
    (*((Workplace**)DS))->Quit();
    delete( (*((Workplace**)DS)));
    (*((Workplace**)DS)) = nullptr;
}
