// hw2-memory-runtime.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <array>
//#include <chrono>
//#include <thread>
//
#include "Timer.h"

using namespace std;

template<typename T>
void Swap(T* t1, T* t2) {
	T tmp(move(*t2));
	*t2 = move(*t1);
	*t1 = move(tmp);
}

template<typename T>
void SortPointers(vector<T*>& vec_ptrs) {
	sort(vec_ptrs.begin(), vec_ptrs.end(), [](T* left, T* right) {
		return *left < *right;
		});
}

template<typename T>
void print_vec(const vector<T>& vec) {
	for (const auto& v : vec) {
		cout << v << " ";
	}
	cout << endl;
}

template<typename T>
void print_vec_ptrs(const vector<T*>& vec) {
	for (const auto& pv : vec) {
		cout << *pv << " ";
	}
	cout << endl;
}

// Helper
void read_book_to_vector(string& dest, const string& book_path) {
	ifstream file(book_path);
	if (!file.is_open()) {
		cerr << "Failed open file\n";
		exit(-1);
	}
	file.seekg(0, std::ios::end);
	size_t size = static_cast<size_t>(file.tellg());
	file.seekg(0);
	//string s(size, ' ');
	dest.resize(size);
	file.read(&dest[0], size);
	file.close();
}


array<bool, 256> create_letters_check_buf() {
	static auto fnToByte = [](char ch) { return static_cast<uint8_t>(ch); };
	array<bool, 256> letters;
	letters.fill(false);
	letters[fnToByte('a')] = true;
	letters[fnToByte('e')] = true;
	letters[fnToByte('i')] = true;
	letters[fnToByte('o')] = true;
	letters[fnToByte('u')] = true;
	letters[fnToByte('y')] = true;

	letters[fnToByte('A')] = true;
	letters[fnToByte('E')] = true;
	letters[fnToByte('I')] = true;
	letters[fnToByte('O')] = true;
	letters[fnToByte('U')] = true;
	letters[fnToByte('Y')] = true;
	return letters;
}

/****
NOTE-1! Использование метода find класса string, мне казалось не подходящим, так 
как с помощью этого метода производится поиск конкретного подходящего символа, тогда как у нас
этих символов несколько (все гласные). Поэтому я дополнительно реализовал поиск глассных
через ф-ию find_if.

NOTE-2! Проверку, является ли буква глассной, я реализовал через разряженный массив значений,
типа bool, где для глассных букв по индексу который представляется значением конкретной буквы,
 лежит значение true, для остальных false. Сделал это из соображений эффективности (проходить
 каждый раз по циклу, сравнивая все глассные буквы с проверяемой буквой - кажется очень 
 неэффективным по скорости). Но оставил закомментированную. реализацию на цикле 
 (чтобы соотв. заданию).

NOTE-3! Результат: Два цикла отрабатывают быстрее - by_twice_for (подозреваю это из-за удобного
для использования кэша алгоритма - проверяемая часть строки, кладётся в кэш и как правило 
следующие данные для проверки читаются уже из кэша)
*/

vector<char> getAllVowels() {
	return vector<char>{
		'a', 'e', 'i', 'o', 'u', 'y',
			'A', 'E', 'I', 'O', 'U', 'Y'
	};
}

bool is_vowel(char ch) {
	static array<bool, 256> letters = create_letters_check_buf();
	return letters[static_cast<uint8_t>(ch)];
}

// Trivial implementation check char on vowel:
//bool is_vowel(char ch) {
//	static vector<char> vowels = getAllVowels();
//	for (char vowel : vowels) {
//		if (vowel == ch)
//			return true;
//	}
//	return false;
//}

int by_count_if(const string& book) {
	Timer timer("count_if");
	auto res = count_if(book.begin(), book.end(), is_vowel);
	cout << endl;
	timer.print();
	cout << "Result (by_count_if): " << res << " (size = " << book.size() << ")\n";
	return 0;
}

int by_find_if(const string& book) {
	Timer timer("find_if");
	auto curPos = book.begin();
	int res = 0;
	while (true) {
		curPos = find_if(curPos, book.end(), is_vowel);
		if (curPos == book.end())
			break;
		else {
			++curPos;
			++res;
		}
	}
	cout << endl;
	timer.print();
	cout << "Result (by_find_if): " << res << " (size = " << book.size() << ")\n";
	return 0;
}

int by_find_from_string(const string& book) {
	static vector<char> vowels = getAllVowels();
	Timer timer("string.find");
	int res = 0;
	for (char ch : vowels) {
		size_t curPos = 0;
		while (true) {
			curPos = book.find(ch, curPos);
			if (curPos == string::npos)
				break;
			else {
				++curPos;
				++res;
			}
		}
	}
	cout << endl;
	timer.print();
	cout << "Result (by_find_from_string): " << res << " (size = " << book.size() << ")\n";
	return 0;
}

int by_for(const string& book) {
	Timer timer("for");
	int res = 0;
	for(char ch : book)
	{
		if (is_vowel(ch)) {
			++res;
		}
	}
	cout << endl;
	timer.print();
	cout << "Result (by_for): " << res << " (size = " << book.size() << ")\n";
	return 0;
}

int by_twice_for(const string& book) {
	static vector<char> vowels = getAllVowels();
	Timer timer("by_twice_for");
	int res = 0;
	for (char chVowel : vowels) {
		size_t curPos = 0;
		for (char ch : book)
		{
			if (ch == chVowel) {
				++res;
			}
		}
	}
	cout << endl;
	timer.print();
	cout << "Result (by_twice_for): " << res << " (size = " << book.size() << ")\n";
	return 0;
}

int main()
{
	////// Checking task1
	vector<int> v1 = { 1, 2, 3, 4 };
	vector<int> v2 = { 5, 6, 7, 8 };
	cout << "vectors before:\n";
	print_vec(v1);
	print_vec(v2);
	Swap(&v1, &v2);
	cout << "vectors after swap:\n";
	print_vec(v1);
	print_vec(v2);

	///////// Checking task2
	cout << "----------------------\n";
	int n1 = 1, n2 = 2, n3 = 3, n4 = 4;
	vector<int*> vecIntPtrs = {&n2, &n4, &n1, &n3};
	cout << "Before sorting vector with pointers:\n";
	print_vec_ptrs(vecIntPtrs);
	SortPointers(vecIntPtrs);
	cout << "After sorting vector with pointers:\n";
	print_vec_ptrs(vecIntPtrs);
	
	const string bookPath = "..\\Tolstoy Leo. War and Peace.txt";
	//vector<char> letters;
	string letters;
	read_book_to_vector(letters, bookPath);

	by_count_if(letters);
	by_find_if(letters);
	by_for(letters);
	by_find_from_string(letters);
	by_twice_for(letters);
	return 0;
}

