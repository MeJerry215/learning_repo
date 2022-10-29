#include "common.h"


class Solution {
public:
    vector<string> twoEditWords(vector<string>& queries, vector<string>& dictionary) {
        vector<string> res;
        for(auto& query: queries) {
            for(auto& word: dictionary) {
                int edit_cnt = 0;
                for(int i = 0; i < query.size(); i++) {
                    if (query[i] != word[i]) {
                        edit_cnt ++;
                    }
                    if (edit_cnt > 2) {
                        break;
                    }
                }
                if (edit_cnt <= 2) {
                    res.push_back(query);
                    break;
                }
            }
        }
        return res;
    }
};