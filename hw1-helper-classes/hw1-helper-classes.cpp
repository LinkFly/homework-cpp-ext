// hw1-helper-classes.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
#include <tuple>
#include <optional>
#include <fstream>
#include <vector>
#include <regex>
#include <algorithm>
#include <iomanip>
#include <sstream>

using namespace std;

struct Person {
    string name;
    string surname;
    string patronymic;
    bool operator<(const Person& person);
	bool operator==(const Person& person) const;
    friend ostream& operator<<(ostream& out, const Person& person);
};

struct PhoneNumber {
    int countryCode;
    int cityCode;
    string number;
    optional<int> extNumber;
    friend ostream& operator<<(ostream& out, const PhoneNumber& phoneNumber);
};

struct PhoneBook {
	using PersonNumber = pair<Person, PhoneNumber>;
	vector<PersonNumber> personsNumbers;
	PhoneBook(ifstream& fin);
	void SortByName();
	void SortByPhone();
	pair<string, PhoneNumber*> GetPhoneNumber(const string& surname);
	void ChangePhoneNumber(const Person& person, const PhoneNumber& phone);
	friend ostream& operator<<(ostream& out, const PhoneBook& person);
};

bool Person::operator<(const Person& person) {
	return tie(surname, name, patronymic)
		<
		tie(person.surname, person.name, person.patronymic);
}

bool Person::operator==(const Person& person) const {
	return
		tie(name, surname, patronymic)
		==
		tie(person.name, person.surname, person.patronymic);
}

//////// Overloaded output operators ///////
ostream& operator<<(ostream& out, const Person& person) {
	//out.
	out.setf(ios::left); out.unsetf(ios::right);
	//out.width(15);
    out 
		<< setw(15)
		<< person.surname
	;
	//out.setf(ios::right);
	//out.width(10);
    out << setw(15) << person.name;
	//out.width(15);
	out << setw(15) << person.patronymic;
    return out;
}

ostream& operator<<(ostream& out, const PhoneNumber& phoneNumber) {
	ostringstream sstr;
	sstr << "+" << phoneNumber.countryCode << string{ "(" } << phoneNumber.cityCode << ")" << phoneNumber.number;
	if (phoneNumber.extNumber)
		sstr << " " << *phoneNumber.extNumber;
	out.setf(ios::left); out.unsetf(ios::right);
	out.width(15);
	out << sstr.str();
	return out;
}

ostream& operator<<(ostream& out, const PhoneBook& person) {
	for (auto [person, phone] : person.personsNumbers) {
		out << person << phone << endl;
	}
	return out;
}
//////// end Overloaded output operators ///////

PhoneBook::PhoneBook(ifstream& fin) {
	static const regex regx(R"(\+(\d+)\((\d+)\)(\d+))");
	/* fin will be close */
	if (!fin.is_open()) {
		cerr << "ifstream isn't open\n";
		return;
	}
	while (!fin.eof()) {
		string str, sPhone = "", surnameOrExtNumber;
		vector<string> strs{ 3 };
		PhoneNumber phone;
		bool bUseLastValue = false, bCollectPhone = false;
		for (int i = 0;; ++i) {
			if (bUseLastValue) {
				strs[i] = surnameOrExtNumber;
			}
			else {
				fin >> str;
			}
			if (str[0] == '+' || bCollectPhone) {
				sPhone += str;
				smatch m;
				if (regex_search(sPhone, m, regx)) {
					phone.countryCode = stoi(m[1]);
					phone.cityCode = stoi(m[2]);
					phone.number = m[3];
					break;
				}
				else {
					bCollectPhone = true;
				}
			}
			else {
				strs[i] = str;
			}
		}
		Person person;
		person.surname = strs[0];
		person.name = strs[1];
		person.patronymic = strs[2];
		fin >> surnameOrExtNumber;
		
		try {
			phone.extNumber = stoi(surnameOrExtNumber);
			bUseLastValue = false;
		}
		catch (std::invalid_argument e) {
			bUseLastValue = true;
		}

		auto pair = make_pair(person, phone);
		personsNumbers.push_back(pair);
	}
	fin.close();
}

void PhoneBook::SortByName()
{
	sort(personsNumbers.begin(), personsNumbers.end(), [](PersonNumber& perNumLeft, PersonNumber& perNumRight) {
		return perNumLeft.first < perNumRight.first;

	});
}

void PhoneBook::SortByPhone()
{
	sort(personsNumbers.begin(), personsNumbers.end(), [](PersonNumber& perNumLeft, PersonNumber& perNumRight) {
		PhoneNumber& phoneLeft = perNumLeft.second;
		PhoneNumber& phoneRight = perNumRight.second;
		auto [country1, city1, number1, ext1] = phoneLeft;
		auto [country2, city2, number2, ext2] = phoneRight;
		if (!ext1) ext1 = 0;
		if (!ext2) ext2 = 0;
		return tie(country1, city1, number1, *ext1) < tie(country2, city2, number2, *ext2);
	});
}

pair<string, PhoneNumber*> PhoneBook::GetPhoneNumber(const string& surname)
{
	int count = 0;
	PhoneNumber* pPhoneNum = nullptr;
	for_each(personsNumbers.begin(), personsNumbers.end(), [&pPhoneNum, &surname, &count](PersonNumber& personNum) {
		if (surname == personNum.first.surname) {
			++count;
			pPhoneNum = &personNum.second;
		}
		});
	if (count == 0) {
		return make_pair("<<not found>>", nullptr);
	}
	else if (count > 1) {
		return make_pair("<<found more than 1>>", pPhoneNum);
	}
	else { // count == 1
		return make_pair("", pPhoneNum);
	}
}

void PhoneBook::ChangePhoneNumber(const Person& person, const PhoneNumber& phone)
{
	auto it = find_if(personsNumbers.begin(), personsNumbers.end(), [&person](PersonNumber& personNum) {
		return person == personNum.first;
	});
	if (it != personsNumbers.end()) {
		it->second = phone;
	}
}

int main()
{
	/* 
	// My debug code, please ignore it 
    Person person1{"Alexey", "Ivanov", "Sergeevich"};
    Person person2{ "Alexey", "Ivanov", "Sergeevich" };
    Person person3{ "Mike", "Sergeev", "Ivanovich" };
    cout 
        << "person1: " << person1 << endl
        << "person2: " << person2 << endl
        << "person3: " << person3 << endl
        << "person1 == person2: " << (person1 == person2) << endl
        << "person1 < person3: " << (person1 < person3) << endl
        ;

    PhoneNumber phone1{7, 985, "1112233"};
    cout << "phone1: " << phone1 << endl;
	PhoneNumber phone2{ 7, 985, "1112233", 77 };
	cout << "phone2: " << phone2 << endl;

    cout << "---------------------------\n";
	*/

	ifstream fin("phone-book.txt");
	PhoneBook book{ fin };
	cout << book;

	cout << "------SortByPhone-------" << endl;
	book.SortByPhone();
	cout << book;

	cout << "------SortByName--------" << endl;
	book.SortByName();
	cout << book;

	cout << "-----GetPhoneNumber-----" << endl;
	// лямбда функция, которая принимает фамилию и выводит номер телефона этого    	человека, либо строку с ошибкой
	auto print_phone_number = [&book](const string& surname) {
		cout << surname << "\t";
		auto answer = book.GetPhoneNumber(surname);
		if (get<0>(answer).empty())
			cout << get<1>(answer);
		else
			cout << get<0>(answer);
		cout << endl;
	};

	// вызовы лямбды
	print_phone_number("Ivanov");
	print_phone_number("Petrov");

	cout << "----ChangePhoneNumber----" << endl;
	book.ChangePhoneNumber(Person{ "Kotov", "Vasilii", "Eliseevich" }, PhoneNumber{ 7, 123, "15344458", nullopt });
	book.ChangePhoneNumber(Person{ "Mironova", "Margarita", "Vladimirovna" }, PhoneNumber{ 16, 465, "9155448", 13 });
	cout << book;
}



