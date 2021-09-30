// hw6-concurrency.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <optional>
#include <string>
#include <sstream>
#include <unordered_map>
#include <unordered_set>
#include <shared_mutex>
#include <functional>
#include <queue>
#include <random>
#include <chrono>
#include <memory>
#include <set>

using namespace std;

//////// To Task1 /////////
/*
Создайте потокобезопасную оболочку для объекта cout. Назовите ее pcout. 
Необходимо, чтобы несколько потоков могли обращаться к pcout и информация выводилась в консоль. 
Продемонстрируйте работу pcout.
*/
struct PCout {
	
	class End {};
	PCout(ostream& out): out{out} {
	}

	template<typename T>
	struct vals_t {
		vector<T> vals;
		void add(const T& val) {
			vals.push_back(val);
		}
	};
	
	template<typename T>
	PCout& operator<<(const T& value)
	{
		auto tid = this_thread::get_id();
		if (!(cur_thread.has_value() && cur_thread == tid)) {
			out_mtx.lock();
		}
		if (!cur_thread.has_value()) {
			cur_thread = tid;
		}
		out << value;
		return *this;
	}

	PCout& operator<<(End spec)
	{
		auto tid = this_thread::get_id();
		if (cur_thread == tid) {
			cur_thread.reset();
			out_mtx.unlock();
		}
		return *this;
	}
private:
	ostream& out;
	mutex out_mtx;
	optional<thread::id> cur_thread; 
};

PCout pcout{cout};

void print_correct(int i) {
	pcout << "some text text text text" << i << "\n" << PCout::End{};
}

void print(int i) {
	cout << "some text text text text" << i << "\n";
}
//////// end To Task1 /////////

/////// To Task2 /////////
/*
Реализовать функцию, возвращающую i-ое простое число 
(например, миллионное простое число равно 15485863). 
Вычисления реализовать во вторичном потоке. 
В консоли отображать прогресс вычисления.
*/

class FindPrime {
	bool is_prime(int num) {
		for (int i = 2; i < num; ++i) {
			if ((num % i) == 0)
				return false;
		}
		return true;
	}
	int next_prime(int from_num) {
		for (int cur_num = from_num + 1; true; ++cur_num) {
			if (is_prime(cur_num))
				return cur_num;
		}
	}
public:
	void operator()(int primeIdx,
		function<void(int cur_prime)> fnReport,
		function<void(int& res)> fnResult)
	{
		int curnum = 1;
		for (int i = 0; i < primeIdx; ++i) {
			curnum = next_prime(curnum);
			//lock_guard lock{ report_mtx };
			fnReport(curnum);
			//cout << curnum << endl;
		}
		fnResult(curnum);
	}
};

class ReportPrimes {
	using prime_collection_t = queue<int>;
	prime_collection_t primes;
	int primeIdx = 0;
	optional<int> res;
	shared_mutex& report_mtx;
public:
	ReportPrimes(shared_mutex& report_mtx): report_mtx{report_mtx} {}
	bool isEmptyPrimes() {
		shared_lock lock{ report_mtx };
		return primes.empty();
	}
	void operator()(int curPrime) {
		lock_guard lock{report_mtx};
		primes.push(curPrime);
	}
	bool ejectNextPrime(int& curPrime) {
		lock_guard lock{ report_mtx };
		if (primes.empty()) return false;
		curPrime = primes.front();
		primes.pop();
		return true;
	}
	void maybeReportCurPrime() {
		int curPrime{};
		bool bCurPrime = ejectNextPrime(curPrime);
		if (bCurPrime)
			cout << ++primeIdx << ". Found prime: " << curPrime << endl;
	}
	bool getResult(int& result) {
		shared_lock lock{report_mtx};
		if (res.has_value()) {
			result = *res;
			return true;
		}
		return false;
	}	
	void setResult(int result) {
		lock_guard lock{report_mtx};
		res = result;
	}
};
/////// end To Task2 /////////

/////// To Task3 /////////


void search_prime(int primeIdx) {
	shared_mutex report_mtx;
	FindPrime findPrime{};
	ReportPrimes repPrimes{report_mtx};
	auto fnGetResult{ [&repPrimes](int resPrime) {
		repPrimes.setResult(resPrime);

	}};
	thread thFindPrimes{ ref(findPrime), primeIdx, ref(repPrimes), fnGetResult};
	int res;
	while (true) {
		bool bRes = repPrimes.getResult(res);
		bool bEmpty = repPrimes.isEmptyPrimes();
		if (bRes && bEmpty) break;
		repPrimes.maybeReportCurPrime();
	}
	thFindPrimes.join();
	cout << "Result: " << res << endl;
}
/////// end To Task2 /////////

/////// To Task3 /////////
using Thing = int;

class Entity {
public:
	virtual void printThings() = 0;
};

class House: public Entity {
	function<void()> nearHostCallback = nullptr;
public:
	recursive_mutex houseMtx;
	unordered_set<Thing> things;
	House() {}

	void setNearHostCallback(function<void()> callback) {
		nearHostCallback = callback;
	}
	void maybeCallNearHostCallback() {
		if (nearHostCallback) {
			lock_guard lock{ houseMtx };
			nearHostCallback();
		}	
	}
	void printThings() override {
		cout << "House things: ";
		for(Thing thing: things) {
			cout << thing << " ";
		}
		cout << endl;
	}
};

class Random {
	std::random_device dev;
	std::mt19937 gen;
	uniform_int_distribution<int> rndDist;
public:
	Random(int from, int to): gen{dev()}, rndDist{from, to} {}
	int operator()() {
		return rndDist(gen);
	}
};

class Person : public Entity {
	Random randomTime{1, 10};
public:
	Person(House& house) : house{ house } {}
protected:
	int liveTicks = 100;
	House& house;
	enum {IN_STREET, IN_HOUSE} ePlace;
	auto getRandomTime() {
		return chrono::milliseconds(randomTime());
	}
	void busy() {
		this_thread::sleep_for(getRandomTime());
	}
	void goToHouse() {
		lock_guard lock{house.houseMtx};
		ePlace = IN_HOUSE;
	}
	void goToStreet() {
		ePlace = IN_STREET;
	}
public:
	
};

class Host : protected Person {
	chrono::milliseconds timeToNearHome = 10ms;
	Thing thing;
	Random random{0, 9};
public:
	Host(House& house): Person{ house } {
		ePlace = IN_HOUSE;
	}
	void run() {
		for (int i = 0; i < liveTicks; ++i) {
			goToStreet();
			lookForThing();
			addThing(thing);
			startGoHome();
			this_thread::sleep_for(timeToNearHome);
			goToHouse();
			busy();
		}
	}

	void printThings() override {
		cout << "Host things: " << thing << endl;
	}
private:
	Thing getRandomThing() {
		return random();
	}
	void addThing(const Thing& thing) {
		lock_guard lock{ house.houseMtx };
		house.things.insert(thing);
	}

	void lookForThing() {
		busy();
		thing = getRandomThing();
	}
	void startGoHome() {
		busy();
		house.maybeCallNearHostCallback();
	}
};

class Thief : protected Person {
	bool bNearHost = false;
public:
	set<Thing> things;
	Thief(House& house) : Person{ house } {
		ePlace = IN_STREET;
	}
	void run() {
		auto nearHostCallback = [this]() {
			bNearHost = true;
		};
		house.setNearHostCallback(nearHostCallback);
		for (int i = 0; i < liveTicks; ++i) {
			goToHouse();
			Thing bestThing;
			//cout << "near host: " << bNearHost << endl;
			bool bFound = findMostValuableThing(bestThing);
			if (bFound) {
				addThing(bestThing);
			}
			bNearHost = false;
			goToStreet();
			busy();
		}	
	}
	void printThings() override {
		cout << "Thief things: ";
		for (Thing thing : things) {
			cout << thing << " ";
		}
		cout << endl;
	}
private:
	void addThing(const Thing& thing) {
		lock_guard lock{house.houseMtx};
		things.insert(thing);
	}
	bool findMostValuableThing(Thing& thing) {
		lock_guard lock{ house.houseMtx };
		static auto getBestThing = [](const Thing & thingLeft, const Thing & thingRight) {
			return thingLeft > thingRight ? thingLeft : thingRight;
		};
		if (house.things.empty()) return false;
		Thing curThing = *house.things.begin();
		for (auto it = house.things.begin()++; it != house.things.end(); ++it) {
			if (bNearHost) {
				break;
			}
			curThing = getBestThing(curThing, *it);
		}
		thing = curThing;
		return true;
	}
};

class World {
public:
	shared_ptr<Host> host;
	shared_ptr<Thief> thief;
	shared_ptr<House> house;
	World() {
		run();
	}

	void run() {
		house = make_shared<House>();
		host = make_shared<Host>(*house);
		thief = make_shared<Thief>(*house);
		thread thHost{&Host::run, host};
		thread thThief{&Thief::run, thief};
		thHost.join();
		thThief.join();
	}
};
/////// end To Task3 /////////

int main()
{
	//// Task1
	cout << "\n=== Task1 ===\n";
	vector<thread> threads;
	constexpr int tnum = 100; // 1000;
	threads.reserve(tnum);
	for (int i = 0; i < tnum; ++i) {
		//threads.emplace_back(print, i);
		threads.emplace_back(print_correct, i);
	}
	for (auto& th : threads) th.join();
	//// end Task1

	//// Task2
	cout << "\n=== Task2 ===\n";
	//thread th_prime{get_prime};
	//th_prime.join();
	search_prime(100);

	//// Task2
	cout << "\n=== Task3 ===\n";
	World world{};
	world.house->printThings();
	world.host->printThings();
	world.thief->printThings();
	return 0;

}

