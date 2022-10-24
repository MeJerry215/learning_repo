#include "common.h"


class Solution {
public:
    int f(string& str) {
        int cnt = 1;
        char min_char = str[0];
        for(int i = 1; i < str.size(); i++) {
            if (str[i] < min_char) {
                min_char = str[i];
                cnt = 1;
            } else if (str[i] == min_char) {
                cnt ++;
            }
        }
        return cnt;
    }

    vector<int> numSmallerByFrequency(vector<string>& queries, vector<string>& words) {
        unordered_map<int, int> word_cnts;
        for(string& word: words) {
            word_cnts[f(word)]++;
        }
        vector<pair<int,int>> cnts; 
        for(auto iter: word_cnts) {
            cnts.push_back({iter.first, iter.second});
        }
        print_vec(cnts);
        sort(cnts.begin(), cnts.end(), [](pair<int, int>& lhs, pair<int, int>& rhs){
            return lhs.first < rhs.first;
        });
        print_vec(cnts);
        for(int i = cnts.size() - 2; i >= 0; i--) {
            cnts[i].second += cnts[i + 1].second;
        }
        vector<int> res;
        for(string& query: queries) {
            int cnt = f(query);
            auto iter = upper_bound(cnts.begin(), cnts.end(), cnt, [](int lhs, pair<int, int> rhs) {
                return lhs < rhs.first;
            });
            if (iter == cnts.end()) res.push_back(0);
            else {
                res.push_back(iter->second);
            }
        }
        return res;
    }
}; 


void TestSolution() {
    Solution s;
    vector<string> queries = {
        "cbbbbbd"
    };
    vector<string> words = {
        "zaaaz"
    };
    auto res = s.numSmallerByFrequency(queries, words);

    print_vec(res);
}