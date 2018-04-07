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
    cerr << "adsasdsadasdaasd\n";

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


    auto a1 = a.begin(), a2 = ++a.begin(), a3 = ++++a.begin(), a4 = ++++++a.begin(), a5 = ++++++++a.begin();
    auto b1 = b.begin(), b2 = ++b.begin(), b3 = ++++b.begin(), b4 = ++++++b.begin(), b5 = ++++++++b.begin();
    a.splice(a3, a, a1, a3);

    //a.clear();
    cerr << "\n1111111\n";
    for(auto it = a.begin(); it!= a.end(); ++it) {
        cerr << *it << " ";
    }
    cerr << "\n";
    for(auto it = b.begin(); it!= b.end(); ++it) {
        cerr << *it << " ";
    }
    cerr << "\n2222222\n";

    cerr << "\n";
    for(auto it = a.begin(); it!= a.end(); ++it) {
        cerr << *it << " ";
    }
    cerr << "\n";
    for(auto it = b.begin(); it!= b.end(); ++it) {
        cerr << *it << " ";
    }
    cerr << "\n";
    //a.clear();

    b.clear();

    list<int>A, B;
    A.clear();
    B.clear();
    A.push_back(1);
    A.push_back(2);
    A.push_back(3);
    A.push_back(4);
    B.push_back(11);
    B.push_back(22);
    B.push_back(33);
    B.push_back(44);
    B.push_back(55);
    //swap(A, b);
    cerr << "\n eeee "  << " \n";
    cerr << "\n";
    for(auto it = A.begin(); it!= A.end(); ++it) {
        cerr << *it << " ";
    }
    B = A;
    A.push_back(6);
    A.splice(A.end(), B);
    cerr << "\n";
    for(auto it = B.begin(); it!= B.end(); ++it) {
        cerr << *it << " ";
    }
    cerr << "\n";
    for(auto it = A.begin(); it!= A.end(); ++it) {
        cerr << *it << " ";
    }
    A.clear();
    B.clear();

    A.clear();
    B.clear();
    A.push_back(1);
    A.push_back(2);
    A.push_back(3);
    A.push_back(4);

    B.push_back(11);
    B.push_back(22);
    B.push_back(33);
    B.push_back(44);

    swap(B, A);
    swap(A, B);
    cerr << "\n";
    for(auto it = A.begin(); it!= A.end(); ++it) {
        cerr << *it << " ";
    }
    cerr << "\n";
    for(auto it = B.begin(); it!= B.end(); ++it) {
        cerr << *it << " ";
    }


    list<double> aa;
    aa.push_back(1.0);
    aa.push_back(2.0);
    aa.push_back(3.0);
    aa.push_back(4.0);
    aa.push_back(5.0);
    aa.push_back(6.0);

    list<double> bb;
    bb.push_back(1.1);
    bb.push_back(2.1);
    bb.push_back(3.1);
    bb.push_back(4.1);
    bb.push_back(5.1);
    bb.push_back(6.1);

    cerr << "\n";
    cerr << "\n";


    for (auto &it : aa) {
        cerr << it << " ";
    }
    cerr << "\n";
    for (auto &it : bb) {
        cerr << it << " ";
    }
    cerr << "\n";

    swap(aa, bb);

    cerr << "1:\n";

    for (auto &it : aa) {
        cerr << it << " ";
    }
    cerr << "\n";
    for (auto &it : bb) {
        cerr << it << " ";
    }
    cerr << "\n";

 bb = aa;

    cerr << "1:\n";



    for (auto &it : aa) {
        cerr << it << " ";
    }
    cerr << "\n";
    for (auto &it : bb) {
        cerr << it << " ";
    }
    cerr << "\n";




    for (auto &it : aa) {
        cerr << it << " ";
    }
    cerr << "\n";
    for (auto &it : bb) {
        cerr << it << " ";
    }
    cerr << "\n";




    for (auto &it : aa) {
        cerr << it << " ";
    }
    cerr << "\n";
    for (auto &it : bb) {
        cerr << it << " ";
    }
    cerr << "\n";





    for (auto &it : aa) {
        cerr << it << " ";
    }
    cerr << "\n";
    for (auto &it : bb) {
        cerr << it << " ";
    }
    cerr << "\n";




    for (auto &it : aa) {
        cerr << it << " ";
    }
    cerr << "\n";
    for (auto &it : bb) {
        cerr << it << " ";
    }
    cerr << "\n";

    return 0;
}

