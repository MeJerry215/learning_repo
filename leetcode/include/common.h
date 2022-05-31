#include <vector>
using std::vector;

#include <algorithm>
using std::all_of;
using std::for_each;
using std::lower_bound;
using std::max;
using std::min;
using std::reverse;
using std::sort;

#include <iostream>
using std::cout;
using std::endl;
using std::sort;

#include <numeric>
using std::abs;
using std::accumulate;
#include <memory>
using std::move;

#include <utility>
using std::make_pair;
using std::pair;

#include <string>
using std::string;

#include <climits>

#include <queue>
using std::queue;

#include <map>
using std::map;

#include <unordered_map>
using std::unordered_map;

#include <unordered_set>
using std::unordered_set;

extern void TestSolution();
int main(int argc, char const *argv[])
{
    TestSolution();
    return 0;
}

template <typename T>
void print_vec(vector<T> &vec)
{
    typedef typename vector<T>::size_type size_type;
    for (size_type i = 0; i < vec.size(); i++)
    {
        cout << vec[i] << "\t";
    }
    cout << endl;
}

template <typename T>
void print_mat(vector<vector<T>> &mat)
{
    typedef typename vector<T>::size_type size_type;
    size_type M = mat.size();
    size_type N = mat[0].size();
    for (size_type i = 0; i < M; i++)
    {
        for (size_type j = 0; j < N; j++)
        {
            cout << mat[i][j] << "\t";
        }
        cout << endl;
    }
}