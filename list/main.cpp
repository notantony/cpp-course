#include <iostream>
#include "list.h"

using namespace std;

int main()
{
    list<int> a;
    a.push_back(1);
    a.push_back(2);
    a.push_back(3);
    a.push_back(4);
    a.push_back(5);
    a.push_back(7);
    for(auto it = a.begin(); it!= a.end(); ++it) {
        cerr << *it << " ";
    }
    cerr << "\n";
    for(auto it = a.end(); it!= a.begin(); ) {
        --it;
        cerr << *it << " ";
    }
    cerr << "\n";
    for(auto it = a.rbegin(); it!= a.rend(); ++it) {
        cerr << *it << " ";
    }
    cerr << "\n";
    for(auto it = a.rend(); it!= a.rbegin(); ) {
        --it;
        cerr << *it << " ";
    }
    cerr << "\n";
    cerr << a.back();
    cerr << "\n";
    cerr << a.front();
    cerr << "\n";
    auto it = a.begin();
    ++++++it;
    cerr << *it;
    cerr << "\n";
    a.erase(it);
    for(auto it = a.begin(); it!= a.end(); ++it) {
        cerr << *it << " ";
    }
    cerr << "\n";
    a.erase(a.begin());
    for(auto it = a.begin(); it!= a.end(); ++it) {
        cerr << *it << " ";
    }
    cerr << "\n";
    a.erase(a.begin(), ----a.end());
    a.push_back(1);
    a.pop_back();
    a.push_front(1);
    a.pop_front();
    for(auto it = a.begin(); it!= a.end(); ++it) {
        cerr << *it << " ";
    }
    cerr << "\n";
    cerr << a.empty();
    cerr << "\n";
    cerr << a.size();
    cerr << "\n";

    list<int> b;
    b.push_back(1);
    b.push_back(2);
    b.push_back(3);
    for(auto it = a.begin(); it!= a.end(); ++it) {
        cerr << *it << " ";
    }
    cerr << "\n";
    for(auto it = b.begin(); it!= b.end(); ++it) {
        cerr << *it << " ";
    }
    cerr << "\n";
    a.splice(a.begin(), b);
    for(auto it = a.begin(); it!= a.end(); ++it) {
        cerr << *it << " ";
    }
    cerr << "\n";
    for(auto it = b.begin(); it!= b.end(); ++it) {
        cerr << *it << " ";
    }
    cerr << "\n";
    b.push_front(1);
    b.push_front(2);
    b.splice(b.begin(), a, a.begin());
    for(auto it = a.begin(); it!= a.end(); ++it) {
        cerr << *it << " ";
    }
    cerr << "s\n";
    for(auto it = b.begin(); it!= b.end(); ++it) {
        cerr << *it << " ";
    }
    cerr << "\n";
    cerr << a.front();
    cerr << "\n";
    cerr << a.back();
    cerr << "\n";
    a.clear();
    for(auto it = a.begin(); it!= a.end(); ++it) {
        cerr << *it << " ";
    }
    cerr << "\n";
    for(auto it = b.begin(); it!= b.end(); ++it) {
        cerr << *it << " ";
    }
    cerr << "\n";
    a = b;
    for(auto it = a.begin(); it!= a.end(); ++it) {
        cerr << *it << " ";
    }
    cerr << "\n";
    for(auto it = b.begin(); it!= b.end(); ++it) {
        cerr << *it << " ";
    }
    cerr << "\n";
    a.pop_back();
    a.pop_front();
    for(auto it = a.begin(); it!= a.end(); ++it) {
        cerr << *it << " ";
    }
    cerr << "\n";

    return 0;
}

