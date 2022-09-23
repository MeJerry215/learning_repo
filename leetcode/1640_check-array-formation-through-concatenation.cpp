#include "common.h"

class Solution {
public:
    bool canFormArray(vector<int>& arr, vector<vector<int>>& pieces) {
        for(int i = 0; i < arr.size(); i++) {
            for(const vector<int>& piece: pieces) {
                if (piece[0] == arr[i]) {
                    for(int j = 1; j < piece.size(); j++) {
                        if (piece[j] == arr[++i]) continue;
                        return false;
                    }
                }
            }
            return false;
        }
        return true;
    }
};