#include "QuickProfiler.h"
#include<fstream>
#include <iomanip>
#include<time.h>

using namespace std;

QuickProfiler* QuickProfiler::pInst = NULL;

// Singleton [static method]
QuickProfiler* QuickProfiler::getInst() {
  if (pInst == NULL) {
    pInst = new QuickProfiler();
  }
  return pInst;

}

void QuickProfiler::_begin(string memo) {
#ifdef PROFILER
	clock_t now = clock();
  timestack.push((double)now);
  memostack.push(memo);

  if (callcount.find(memo) == callcount.end()) {  // never before
    nameOrders.push_back(memo);
  }
#endif
}

void QuickProfiler::_end() {
#ifdef PROFILER
	clock_t now = clock();
  double lasttime = timestack.top();
  timestack.pop();
  string lastmemo = memostack.top();
  memostack.pop();

  double diff = ((double)now - lasttime) / 1000;

  callcount[lastmemo]++;
  timesum[lastmemo] += diff;
#endif
}

QuickProfiler::QuickProfiler() {
#ifdef PROFILER
  nameOrders.empty();
  timestack.empty();
  memostack.empty();
  callcount.empty();
  timesum.empty();
#endif
}

void QuickProfiler::_writeToFile(string filepath) {
#ifdef PROFILER
	ofstream fout(filepath.c_str(), ios::app);

  // SANITY CHECK
  // If the stack is not empty, i.e. someone begin without ending.
  if (timestack.size() != 0) {
    fout<<"SANITY ERROR: missing block ending"<<endl;
    // flush all the content out
    while (memostack.size() != 0) {
      fout<< " - " << memostack.top() << endl;
      memostack.pop();
    }
    fout.close();
    return;
  }

  // write to file
  fout << setw(7)<<left<<"Calls" << setw(7)<<left<<"Time" << setw(10)<<left<<"Per call" << "Name" << endl;

	for (auto it = nameOrders.cbegin(); it != nameOrders.cend(); ++it) {
    string name = *it;
    fout<< setw(7) << left << callcount[name];
    fout<< setw(7) << left << setprecision(2) << fixed << timesum[name];
    double percall = timesum[name] / callcount[name];
    fout<< setw(10) << left << setprecision(5) << fixed << percall;
    fout<< name << endl;
  }
	fout<<endl;

  fout.close();
#endif
}

QuickProfiler::~QuickProfiler() {
}
