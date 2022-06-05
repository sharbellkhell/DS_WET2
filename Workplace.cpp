#include "Workplace.h"

Workplace::Workplace(int k){
    companies = new UF<Company*>(k);
    employees=new HashTable<Employee*>();
    emp_sals = nullptr;
}

Workplace Workplace::init(int k)
{
    return Workplace(k);
}

