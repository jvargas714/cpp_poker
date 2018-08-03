#include <iostream>
#include <set>
#include <ctime>
#include <sstream>
using namespace std;

int main() {
    std::stringstream ss;
    ss << "hello this is cool" << endl;
    cout << "testing --> " << ss.str();
    ss.clear();
    cout << "after clear: " << ss.str();
    return 0;
}

