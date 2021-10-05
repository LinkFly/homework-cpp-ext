// lesson8-gtest.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <gtest/gtest.h>

#include "minmax.h"

using namespace std;

TEST(test_of_test, test_set1) {
    ASSERT_TRUE(true);
}

TEST(minimum, min_el) {
	vector<float> vec{ 1.2, 4.3, .2, 2.5 };
	ASSERT_FLOAT_EQ(min_el(vec), .2);
}

TEST(maximum, max_el) {
	vector<float> vec{ 1.2, 4.003, .2, 2.5 };
	ASSERT_NEAR(max_el(vec), 4.0032, .001);
}

class Date {
private:
	int m_day, m_month, m_year;
public:
	Date() = default;
	Date(int d, int m, int y) : m_day(d), m_month(m), m_year(y) { }
	int day() const { return m_day; }
	int month() const { return m_month; }
	int year() const { return m_year; }
	string print() const {
		vector<int> vec{2, 4};
		cout << "------- print min_el2: " << min_el2(vec) << endl;;
		ostringstream os;
		os << m_day << "." << m_month << "." << m_year;
		return os.str();
	}
	void set_day(int d) { m_day = d; }
	void set_month(int m) { m_month = m; }
	void set_year(int y) { m_year = y; }
};

struct TestDate : public ::testing::Test {
	Date* date;
	void SetUp() override {
		date = new Date{4, 7, 2021};
	}
	void TearDown() override {
		delete date;
	}

};

TEST_F(TestDate, get_methods) {
	ASSERT_EQ(date->day(), 4);
	ASSERT_EQ(date->month(), 7);
	ASSERT_EQ(date->year(), 2021);
}
TEST_F(TestDate, set_methods) {
	date->set_day(10);
	ASSERT_EQ(date->day(), 10);
	date->set_month(9);
	ASSERT_EQ(date->month(), 9);
	date->set_year(2022);
	ASSERT_EQ(date->year(), 2022);
}

TEST_F(TestDate, print) {
	ASSERT_STREQ("4.7.2021", date->print().c_str());
}


int main()
{
    testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}

