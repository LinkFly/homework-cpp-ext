#pragma once
#include <string>

#include "persons.pb.h"

using namespace std;

class IRepository {
	virtual void Open() = 0; // �������� �������������� � ����
	virtual void Save() = 0; // �������� ������������ � ����
};

class IMethods {
	using FullName = protobuf_ns::persons::FullName;
	virtual double GetAverageScore(const FullName& name) = 0;
	virtual string GetAllInfo(const FullName& name) = 0;
	virtual string GetAllInfo() = 0;
};

