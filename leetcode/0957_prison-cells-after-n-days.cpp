#include "common.h"

/*
监狱中 8 间牢房排成一排，每间牢房可能被占用或空置。
每天，无论牢房是被占用或空置，都会根据以下规则进行变更：
如果一间牢房的两个相邻的房间都被占用或都是空的，那么该牢房就会被占用。
否则，它就会被空置。
注意：由于监狱中的牢房排成一行，所以行中的第一个和最后一个牢房不存在两个相邻的房间。
给你一个整数数组 cells ，用于表示牢房的初始状态：如果第 i 间牢房被占用，则 cell[i]==1，否则 cell[i]==0 。另给你一个整数 n 。
请你返回 n 天后监狱的状况（即，按上文描述进行 n 次变更）。

示例 1：
输入：cells = [0,1,0,1,1,0,0,1], n = 7
输出：[0,0,1,1,0,0,0,0]
解释：下表总结了监狱每天的状况：
Day 0: [0, 1, 0, 1, 1, 0, 0, 1]
Day 1: [0, 1, 1, 0, 0, 0, 0, 0]
Day 2: [0, 0, 0, 0, 1, 1, 1, 0]
Day 3: [0, 1, 1, 0, 0, 1, 0, 0]
Day 4: [0, 0, 0, 0, 0, 1, 0, 0]
Day 5: [0, 1, 1, 1, 0, 1, 0, 0]
Day 6: [0, 0, 1, 0, 1, 1, 0, 0]
Day 7: [0, 0, 1, 1, 0, 0, 0, 0]
示例 2：
输入：cells = [1,0,0,1,0,0,1,0], n = 1000000000
输出：[0,0,1,1,1,1,1,0]
提示：
cells.length == 8
cells[i] 为 0 或 1
1 <= n <= 109
来源：力扣（LeetCode）
链接：https://leetcode.cn/problems/prison-cells-after-n-days
著作权归领扣网络所有。商业转载请联系官方授权，非商业转载请注明出处。
*/

class Solution {
public:
    enum { CELL_NUM = 8 };

    int encode(vector<int>& cells) {
        int sum = 0;
        for(const int cell: cells) {
            sum *= 2;
            sum += cell;
        }
        return sum;
    }

    void decode(vector<int>& cells, int encode_val) {
        int i = CELL_NUM - 1;
        while(i >= 0) {
            cells[i] = encode_val % 2;
            encode_val /= 2;
            i --;
        }
    }

    vector<int> prisonAfterNDays(vector<int>& cells, int n) {
        int interval = 0;
        int day = 0;
        unordered_map<int, int> day_prision_state;
        int cells_state = 0;
        while(day < n) {
            day++;
            int pre_state = cells[0];
            for(int i = 1; i < CELL_NUM - 1; i++) {
                int cur_state = cells[i];
                cells[i] = !(pre_state ^ cells[i + 1]);
                pre_state = cur_state;
            }
            cells[0] = cells[CELL_NUM - 1] = 0;

            cells_state = encode(cells);
            if (day_prision_state.find(cells_state) != day_prision_state.end()) {
                interval = day - day_prision_state[cells_state];
                break;
            }
            day_prision_state[cells_state] = day;
        }
        if (day == n) return cells;
        day = (n - day) % interval + day_prision_state[cells_state];
        auto iter = day_prision_state.begin();
        while(iter != day_prision_state.end()) {
            if (iter->second == day) break;
            iter++;
        }
        int encode_val = iter->first;
        decode(cells, encode_val);
        return cells;
        
    }
};

void TestSolution() {
    Solution s;
    vector<int> cells = {1, 0, 0, 1, 0, 0, 1, 0};
    // int encode_val = s.encode(cells);
    // cout << encode_val << endl;
    // s.decode(cells, encode_val);
    // print_vec(cells);
    // encode_val = 8;
    // s.decode(cells, encode_val);
    // print_vec(cells);
    cout << "day 0 : ";
    print_vec(cells);
    auto res = s.prisonAfterNDays(cells, 1000000000);
    print_vec(res);
}

// [1,0,0,1,0,0,1,0]
// 1000000000