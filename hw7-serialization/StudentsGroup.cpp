#include "StudentsGroup.h"

#include <fstream>
#include <tuple>
#include <cmath>
#include <sstream>

StudentsGroup::StudentsGroup(const string& file): StudentsGroup::StudentsGroup{file, make_shared<Group>() } {}

StudentsGroup::StudentsGroup(const string& file, shared_ptr<Group> pGroup): file{file}, pGroup{pGroup}, group{*pGroup}
{
	group = ref(*pGroup);
}

void StudentsGroup::Open()
{
	std::ifstream fin(file, std::ios::binary);
	group.ParseFromIstream(&fin);
	fin.close();
}

void StudentsGroup::Save()
{
	std::ofstream fout(file, std::ios::binary);
	group.SerializeToOstream(&fout);
	fout.close();
}

//std::optional<const StudentsGroup::Student&> StudentsGroup::FindStudentTest(const FullName& fullName)
//{
//	return group.students().Get(0);
//
//}
const StudentsGroup::Student* StudentsGroup::FindStudent(const FullName& fullName)
{
	static auto cmpFullName{ [](const FullName& left, const FullName& right) {
		return
			std::tie(left.name(), left.surname(), left.patronymic())
			==
			std::tie(right.name(), right.surname(), right.patronymic());
	} };
	auto& students = group.students();
	const Student* res = nullptr;
	for (size_t i = 0; i < students.size(); ++i) {
		const Student& curStudent = const_cast<Student&>(students.Get(i));
		if (cmpFullName(curStudent.fullname(), fullName)) {
			//Student* stud = new Student();
			res = &curStudent;
			break;
			//return std::optional<const Student&>{curStudent};
		}
	}
	//auto opt = FindStudentTest(fullName);
	return res;
}

std::string StudentsGroup::GetStudentInfo(const Student& student)
{
	ostringstream sout;
	const FullName& fname = student.fullname();
	const auto& scores = student.scores();
	const auto& middleScore = student.middle_score();
	sout
		<< "FullName: name = " << fname.name()
		<< ", surname = " << fname.surname()
		<< ", patronymic = " << fname.patronymic()
		<< "\n";
	sout << "Scores: ";
	for (size_t i = 0; i < scores.size(); ++i) {
		sout << scores.Get(i) << " ";
	}
	sout << "\n";
	sout << "Middle score: " << middleScore << endl;
	return sout.str();
}



double StudentsGroup::GetAverageScore(const FullName& name)
{
	const Student* student = FindStudent(name);
	double res = 0.0;
	if (!student) return res;
	auto& scores = student->scores();
	for (size_t i = 0; i < scores.size(); ++i) {
		res += scores.Get(i);
	}
	res /= scores.size();
	return res;
}

string StudentsGroup::GetAllInfo(const FullName& name)
{
	const Student* student = FindStudent(name);
	if (!student) return {};
	return GetStudentInfo(*student);
}

string StudentsGroup::GetAllInfo()
{
	ostringstream sout;
	const auto& students = group.students();
	for (size_t i = 0; i < students.size(); ++i) {
		const auto& student = students.Get(i);
		sout << GetAllInfo(student.fullname()) << "\n";
	}
	return sout.str();
}
