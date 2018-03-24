#include <iostream>
#include "list.h"
#include <vector>

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

    a = b;
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

    list<int> one;
    cerr << (one.begin() == one.cend());
    cerr << (one.cend() == one.begin());
    cerr << (one.end() == one.begin());
    cerr << (one.end() == one.begin());
    auto itt = one.begin();
    one.begin() = one.cbegin();
    std::vector<int> av, bv;
    one.push_back(1);
    one.push_back(2);
    one.push_back(3);
    one.push_back(4);
    cerr << "\n";
    for(auto it = one.begin(); it!= one.end(); ++it) {
        cerr << *it << " ";
    }
    one = one;
    cerr << "\n";
    for(auto it = one.begin(); it!= one.end(); ++it) {
        cerr << *it << " ";
    }
    a.clear();
    b.clear();
    a.push_back(1);
    a.push_back(2);
    a.push_back(3);
    a.push_back(4);
    b.push_back(11);
    b.push_back(22);
    b.push_back(33);
    b.push_back(44);
    cerr << "\n";
    for(auto it = a.begin(); it!= a.end(); ++it) {
        cerr << *it << " ";
    }
    cerr << "\n";
    for(auto it = b.begin(); it!= b.end(); ++it) {
        cerr << *it << " ";
    }
    //av.begin() = bv.cbegin();
    auto a1 = a.begin(), a2 = ++a.begin(), a3 = ++++a.begin(), a4 = ++++++a.begin(), a5 = ++++++++a.begin();
    auto b1 = b.begin(), b2 = ++b.begin(), b3 = ++++b.begin(), b4 = ++++++b.begin(), b5 = ++++++++b.begin();
    a.splice(a5, a, a1, a5);
    cerr << "\n";
    for(auto it = a.begin(); it!= a.end(); ++it) {
        cerr << *it << " ";
    }
    cerr << "\n";
    for(auto it = b.begin(); it!= b.end(); ++it) {
        cerr << *it << " ";
    }

    return 0;
}
