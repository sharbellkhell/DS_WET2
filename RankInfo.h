#ifndef HW_RANKINFO_H
#define HW_RANKINFO_H

struct RankInfo {
    int NumEmployees;
    long long SumGrades;

    explicit RankInfo(int NumEmployees = 0,
                      long long SumGrades = 0 ) :
            NumEmployees(NumEmployees),
            SumGrades(SumGrades) {}


    RankInfo& operator=(const RankInfo& other)
    {
        if (this == &other){
            return *this;

        }
        this->NumEmployees = other.NumEmployees;
        this->SumGrades = other.SumGrades;
        return *this;
    }



};


#endif //HW_RANKINFO_H
