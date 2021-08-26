// lesson-memory-runtime.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <iostream>
#include <chrono>
#include <thread>
#include <vector>
#include <algorithm>
#include <string>
#include <string_view>
#include <fstream>

#include "Timer.h"

void Sort(vector<int> numbers) {
	Timer timer("STL sort");
	sort(numbers.begin(), numbers.end());
	timer.print();
}

void BubbleSort(vector<int> numbers) {
	size_t size = numbers.size();
	int temp;
	Timer timer("Bubble sort");
	for (int i = 0; i < size - 1; i++) {
		for (int j = 0; j < size - i - 1; j++) {
			if (numbers[j] > numbers[j + 1]) {
				temp = numbers[j];
				numbers[j] = numbers[j + 1];
				numbers[j + 1] = temp;
			}
		}
	}
	timer.print();
}

void InsertionSort(vector<int> numbers) {
	size_t size = numbers.size();
	int temp, item;
	Timer timer("Insertion sort");
	for (int counter = 1; counter < size; counter++)
	{
		temp = numbers[counter];
		item = counter - 1;
		while (item >= 0 && numbers[item] > temp)
		{
			numbers[item + 1] = numbers[item];
			numbers[item] = temp;
			item--;
		}
	}
	timer.print();
}

string CreateBigString() {
	return string(1'000'000'00, 'a');
}

vector<string> SplitString(const string& s) {
	Timer timer("string");
	vector<string> result;
	size_t space_pos = 0;
	while (true) {
		size_t pos = s.find(' ', space_pos);
		if (pos == s.npos) {
			result.push_back(s.substr(space_pos, s.size() - space_pos));
			break;
		}
		result.push_back(s.substr(space_pos, pos - space_pos));
		space_pos = pos + 1;
	}
	timer.print();
	return result;
}

vector<string_view> SplitStringWithSW(const string& s) {
	Timer timer("string_view");
	string_view sw = s;
	vector<string_view> result;
	size_t space_pos = 0;
	while (true) {
		size_t pos = sw.find(' ', space_pos);
		if (pos == sw.npos) {
			result.push_back(sw.substr(space_pos, sw.size() - space_pos));
			break;
		}
		result.push_back(sw.substr(space_pos, pos - space_pos));
		space_pos = pos + 1;
	}
	timer.print();
	return result;
}


int main()
{
	//Timer t1{ "test-timer" };
	//std::cout << "Hello World!\n";
	//this_thread::sleep_for(1.5s);
	//t1.print();

	//int size = 100000;
	//vector<int> numbers;
	//for (size_t i = 0; i < size; i++)
	//{
	//	numbers.push_back(rand());
	//}

	//Sort(numbers);
	//InsertionSort(numbers);
	//BubbleSort(numbers);

	//vector<string> strings;
	//Timer timer("with variable");
	//string big_string = CreateBigString();
	//strings.push_back(big_string);
	//timer.print();

	//timer.start("without variable");
	//strings.push_back(CreateBigString());
	//timer.print();

	//vector<string> strings;
	//string s;
	//getline(cin, s);
	//strings.push_back(move(s)); // перемещаем данные строки s
	//cout << "strings[0] = " << strings[0] << '\n'
	//	<< "s = " << s << endl;

	//vector<int> ints;
	//int x;
	//cin >> x;
	//ints.push_back(move(x));
	//cout << "ints[0] = " << ints[0] << '\n'
	//	<< "x = " << x << endl;

	ifstream file("C:\\Users\\LinkFly\\Dropbox\\courses\\gb-ue4\\Tolstoy Leo. War and Peace.txt");
	if (!file.is_open()) {
		cerr << "Failed open file\n";
		return -1;
	}
	file.seekg(0, std::ios::end);
	size_t size = file.tellg();
	file.seekg(0);
	string s(size, ' ');
	file.read(&s[0], size);

	SplitString(s);
	SplitStringWithSW(s);


	return 0;
}
