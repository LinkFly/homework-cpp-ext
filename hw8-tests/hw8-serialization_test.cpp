// hw8-serialization_test.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <tuple>
#include <memory>

#include "persons.pb.h"
#include "StudentsGroup.h"
#include "gtest/gtest.h"

class TestLoadSave : public testing::Test {
    using FullNameData = vector<string>;
protected:
    using FullName = protobuf_ns::persons::FullName;
    using Student = protobuf_ns::persons::Student;
	using ScoresData = vector<int32_t>;
	using StudentData = tuple<FullNameData, ScoresData, int32_t>;
	using Group_t = vector<StudentData>;
	FullNameData fullNameData = { "Lena", "FamilyLena", "PatroLena" };
	Group_t groupData = {
		{
			{"Kate", "FamilyKate", "PatroKate"},
			{3, 4, 5},
			6
		},
		{
			fullNameData,
			{3, 4, 11},
			6
		},
		{
			{"Mike", "FamilyMike", "PatroMike"},
			{3, 4, 5},
			6
		}
	};
	void fillFullName(FullName& fullName, FullNameData& fullNameData) {
		fullName.set_name(fullNameData[0]);
		fullName.set_surname(fullNameData[1]);
		fullName.set_patronymic(fullNameData[2]);
	};
    void initGroup() {
        pProtobufStudentsGroup = make_shared<protobuf_ns::persons::StudentsGroup>();
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
            for (auto& num : scoresData)
                scores->Add(num);

            student.set_middle_score(get<2>(studentData));
        }
    }
    shared_ptr<protobuf_ns::persons::StudentsGroup> pProtobufStudentsGroup;
    
protected:
    virtual std::string extractGroupInfo(StudentsGroup& group) {
        return group.GetAllInfo();
    }
public:
    string beforeSave, beforeSaveByName, afterLoad, afterLoadByName;
	void SetUp() override {
        using namespace std;
        initGroup();

        const std::string file = "data.bin";
        StudentsGroup group1(file, pProtobufStudentsGroup);
        beforeSave = extractGroupInfo(group1);
        group1.Save();

        StudentsGroup group2(file);
        group2.Open();
        afterLoad = extractGroupInfo(group2);
    }
};

class TestLoadSaveByName : public TestLoadSave {
    shared_ptr<FullName> fullName;
public:
    TestLoadSaveByName(): TestLoadSave() {
        fullName = make_shared<FullName>();
        fillFullName(*fullName, fullNameData);
    }
protected:
	std::string extractGroupInfo(StudentsGroup& group) override {
		return group.GetAllInfo(*fullName);
	}
};

TEST_F(TestLoadSave, correct_serialize_deserilize) {
    ASSERT_STREQ(beforeSave.c_str(), afterLoad.c_str());
}

TEST_F(TestLoadSaveByName, correct_serialize_deserilize_by_name) {
	ASSERT_STREQ(beforeSave.c_str(), afterLoad.c_str());
}

int main()
{
	testing::InitGoogleTest();
	return RUN_ALL_TESTS();
}

