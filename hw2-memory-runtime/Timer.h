#pragma once

#include <chrono>
#include <iostream>
#include <string>
#include <stdint.h>

using namespace std;

class Timer {
	using clock_t = chrono::high_resolution_clock;
	using milli_sec_t = chrono::duration<int64_t, milli>;

	string m_name;
	chrono::time_point<clock_t> m_beg;
	int64_t elapsed() const {
		return chrono::duration_cast<milli_sec_t>(clock_t::now() - m_beg).count();
	}
public:
	Timer(): m_beg(clock_t::now()) {}
	Timer(const string& name): m_name{name}, m_beg(clock_t::now()) {}
	void start(const string& name) {
		m_name = name;
		m_beg = clock_t::now();
	}
	void print() const {
		cout << m_name << ":\t" << (elapsed() / 1000.0) << " sec" << endl;
	}
};