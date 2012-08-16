#pragma once


#include "config.h"
#include<map>
#include<stack>
#include<list>
#include<fstream>
#include<string>

using namespace std;

class QuickProfiler {
public:

  static QuickProfiler* getInst();
  static void begin(string memo) { getInst()->_begin(memo); }
  static void end() { getInst()->_end(); }
  static void writeToFile(string filepath) { getInst()->_writeToFile(filepath); };

  QuickProfiler::~QuickProfiler();

private:
  QuickProfiler();
  QuickProfiler(QuickProfiler const&) {}; // copy constructor is private

  stack<double, list<double>> timestack;
  stack<string, list<string>> memostack;

  list<string> nameOrders; // to keep track of the order

  void _begin(string memo);
  void _end();
  void _writeToFile(string filepath);

  map<string, double> timesum;
  map<string, int> callcount;

  static QuickProfiler* pInst;
};

#define PROFILE_BEG(s) QuickProfiler::begin(s);
#define PROFILE_END() QuickProfiler::end();
#define PB(s) PROFILE_BEG(s);
#define PE() PROFILE_END();
