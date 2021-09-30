#pragma once
#include <string>
#include <optional>
#include <memory>
#include <functional>

#include "persons.pb.h"
#include "interfaces.h"

struct StudentsGroup : public IRepository, public IMethods {
	using FullName = protobuf_ns::persons::FullName;
	using Student = protobuf_ns::persons::Student;
	using Group = protobuf_ns::persons::StudentsGroup;
private:
	std::string file;
	shared_ptr<Group> pGroup;
	//std::reference_wrapper<Group> group{};
	Group& group;
public:
	StudentsGroup(const string& file);
	StudentsGroup(const string& file, shared_ptr<Group> pGroup);

	void Open() override; // binary deserialization from file
	void Save() override; // binary deserialization to file
	double GetAverageScore(const FullName& name) override;
	string GetAllInfo(const FullName& name) override;
	string GetAllInfo() override;
private:
	const Student* FindStudent(const FullName& fullName);
	string GetStudentInfo(const Student& student);
};