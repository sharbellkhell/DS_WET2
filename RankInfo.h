#ifndef HW_RANKINFO_H
#define HW_RANKINFO_H

struct RankInfo {
    int NumEmployees;
    int SumGrades;

    RankInfo(int NumEmployees = 0,
            int SumGrades = 0 ) :
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
