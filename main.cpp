#include "library2.cpp"

template<class T>
void PrintArr(T* arr, int size){
    for(int i = 1; i < size+1; i++){
        std::cout << arr[i] << ", ";
    }
    std::cout << std::endl;
}

void PrintCompArr(Company** arr, int size){
    for(int i = 1; i < size+1; i++){
        std::cout << "(" << arr[i]->companyId << "," << arr[i]->value << ")" << ", ";
    }
    std::cout << std::endl;
}

void PrintCurrUF(UF* uf){

    std::cout << "Elements: ";
    PrintCompArr(uf->Elements,uf->size);
    std::cout << "Parents:     ";
    PrintArr<key>(uf->Parents, uf->size);
    std::cout << "Sizes:       ";
    PrintArr<int>(uf->Sizes,uf->size);
    std::cout << "Last_Values: ";
    PrintArr<double>(uf->Last_Values, uf->size);
    std::cout << "end of UF" << std::endl;

}

int main() {
 void* ds = Init(10);
 PrintCurrUF(((Workplace*)ds)->companies);
 AddEmployee(ds,18,6,31);
 AddEmployee(ds,1,2,4);
 AddEmployee(ds,499104,2,15);
 AddEmployee(ds,-2,-2,33);
 AddEmployee(ds,16,3,13);
 AddEmployee(ds,13,9,13);
 RemoveEmployee(ds,1);
 RemoveEmployee(ds,18);
 AddEmployee(ds,3,10,39);
 AcquireCompany(ds,4,9,1);
// EmployeeSalaryIncrease(ds,16,2);
// PromoteEmployee(ds,13,1);
// SumOfBumpGradeBetweenTopWorkersByGroup(ds,4,1);
// AverageBumpGradeBetweenSalaryByGroup(ds,4,-1,2);
// CompanyValue(ds,8);
// AcquireCompany(ds,10,8,3);
// AcquireCompany(ds,8,7,4);
// EmployeeSalaryIncrease(ds,499104,6);
// EmployeeSalaryIncrease(ds,3,1);
// PromoteEmployee(ds,13,10);
// CompanyValue(ds,8);
// CompanyValue(ds,7);
// PromoteEmployee(ds,16,7);
// AverageBumpGradeBetweenSalaryByGroup(ds,5,2,6);
// SumOfBumpGradeBetweenTopWorkersByGroup(ds,9,1);
// CompanyValue(ds,10);
// Quit(&ds);
}