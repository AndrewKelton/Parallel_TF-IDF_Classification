#include <string>
#include <iostream>
#include <vector>
#include <map>
#include <tuple>
#include <mutex>
#include <sstream>

using namespace std;

mutex mtx;

void count_words(const string& text, map<string, int>& global_count);