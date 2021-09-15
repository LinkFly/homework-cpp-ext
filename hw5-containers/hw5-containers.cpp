// hw4-containers.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <map>
#include <set>
#include <algorithm>
#include <vector>
#include <list>
#include <iterator>
#include <string>
#include <sstream>

using namespace std;

template <class IT>
void words_unique(IT cur, IT end)
{
	using ITOwnerType = typename remove_reference<decltype(*cur)>::type;
	set<ITOwnerType> words;
	for (; cur != end; ++cur) {
		words.insert(*cur);
	}
	copy(words.begin(), words.end(), ostream_iterator<ITOwnerType>(cout, " "));
}

void split_sort_sentences(istream& in) {
	multimap<size_t, string> sentences;
	//copy(istream_iterator<char>(cin), {}, ostream_iterator<char>(cout, ""));
	auto add_sent{ [&sentences](const string& sent) {
		sentences.insert({sent.size(), sent});
	} };
	string cur_sent;
	
	for_each(istream_iterator<char>(in), {}, [&cur_sent, &add_sent](char ch) {
		if (ch == '.') {
			add_sent(cur_sent);
			cur_sent.clear();
		}
		else {
			cur_sent += ch;
		}
		});
	if (!cur_sent.empty())
		add_sent(cur_sent);
	transform(sentences.begin(), sentences.end(), ostream_iterator<string>(cout, "\n"), [](const auto& pair) {
		return pair.second;
		});
}

int main()
{
	cout << "--- Task1 ---\n";
	cout << "using vector iterators:\n";
	vector<string> words = { "one", "two", "xxx", "one", "xxx" };
	words_unique(words.begin(), words.end());
	cout << endl;
	cout << "using list iterators:\n";
	list<string> words_list{ words.begin(), words.end() };
	words_unique(words_list.begin(), words_list.end());
	cout << endl;

	cout << "--- Task2 ---\n";
	//istringstream istr{ "Some sentence.Second sentence.Some xxxxence" };
	//split_sort_sentences(istr);
	split_sort_sentences(cin);
}
