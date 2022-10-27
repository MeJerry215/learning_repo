#include "common.h"



class Solution
{
public:
    string bestHand(vector<int> &ranks, vector<char> &suits)
    {
        char suit = suits[0];
        if (all_of(suits.begin(), suits.end(), [suit](const char c)
                   { return c == suit; }))
        {
            return "Flush";
        }
        unordered_map<int, int> rank_cnts;
        int max_cnt = 1;
        for (int rank : ranks)
        {
            rank_cnts[rank]++;
            max_cnt = max(max_cnt, rank_cnts[rank]);
        }
        return max_cnt >= 3 ? "Three of a Kind" : max_cnt == 2 ? "Pair"
                                                               : "High Card";
    }
};