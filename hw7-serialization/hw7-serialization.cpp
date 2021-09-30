// hw7-serialization.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <tuple>

#include "persons.pb.h"
#include "StudentsGroup.h"



int main()
{
    using namespace std;
    using protobuf_ns::persons::FullName;
    using protobuf_ns::persons::Student;

 /*   FullName fullName;
    fullName.set_name("Mike");
    cout << "before serialize: " << fullName.name() << endl;
    ofstream fout("test.bin", ios::binary);
    fullName.SerializeToOstream(&fout);
    fout.close();

    ifstream fin("test.bin");
    FullName fullName2;
    fullName2.ParseFromIstream(&fin);
    fin.close();
    cout << "after unserialize: " << fullName2.name() << endl;*/

    using FullNameData = vector<string>;
    using ScoresData = vector<int32_t>;
    using StudentData = tuple<FullNameData, ScoresData, int32_t>;
    using Group_t = vector<StudentData>;
    tuple<int, int> tpl = {3, 4};
    FullNameData someFullNameData = { "Lena", "FamilyLena", "PatroLena" };
    Group_t groupData = {
        {
            {"Kate", "FamilyKate", "PatroKate"},
            {3, 4, 5},
            6
        },
        {
            someFullNameData,
            {3, 4, 11},
            6
        },
        {
            {"Mike", "FamilyMike", "PatroMike"},
            {3, 4, 5},
            6
        }
    };
    auto fillFullName = [](FullName& fullName, FullNameData& fullNameData) {
        fullName.set_name(fullNameData[0]);
        fullName.set_surname(fullNameData[1]);
        fullName.set_patronymic(fullNameData[2]);
    };
    auto pProtobufStudentsGroup = make_shared<protobuf_ns::persons::StudentsGroup>();
    for (auto& studentData : groupData) {
        Student* pStudent = new Student{};
        auto students = pProtobufStudentsGroup->mutable_students();
        students->AddAllocated(pStudent);
        Student& student = *pStudent;
  
        auto& fullNameData = get<0>(studentData);
        FullName* fullName = new FullName{};//*(student.mutable_fullname());
        fillFullName(*fullName, fullNameData);

        student.set_allocated_fullname(fullName);

        auto scores = student.mutable_scores();
        auto& scoresData = get<1>(studentData);
        for(auto& num: scoresData)
            scores->Add(num);

        student.set_middle_score(get<2>(studentData));
    }

    const std::string file = "data.bin";
    StudentsGroup group(file, pProtobufStudentsGroup);
    group.Save();
    
    StudentsGroup group2(file);
    group2.Open();
    FullName fullName; 
    fillFullName(fullName, someFullNameData);
    cout << "AverageScore: " << group2.GetAverageScore(fullName) << endl;
    cout << "\nGetAllInfo for fullName: \n----------\n" << group2.GetAllInfo(fullName) << "\n---------\n";
    cout << "\nGetAllInfo for all: \n----------\n" << group2.GetAllInfo() << "---------";

}
