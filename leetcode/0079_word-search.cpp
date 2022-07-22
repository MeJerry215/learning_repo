#include "common.h"


/*
给定一个 m x n 二维字符网格 board 和一个字符串单词 word 。如果 word 存在于网格中，返回 true ；否则，返回 false 。
单词必须按照字母顺序，通过相邻的单元格内的字母构成，其中“相邻”单元格是那些水平相邻或垂直相邻的单元格。同一个单元格内的字母不允许被重复使用。

示例 1：
输入：board = [["A","B","C","E"],["S","F","C","S"],["A","D","E","E"]], word = "ABCCED"
输出：true
示例 2：
输入：board = [["A","B","C","E"],["S","F","C","S"],["A","D","E","E"]], word = "SEE"
输出：true
示例 3：
输入：board = [["A","B","C","E"],["S","F","C","S"],["A","D","E","E"]], word = "ABCB"
输出：false
提示：
m == board.length
n = board[i].length
1 <= m, n <= 6
1 <= word.length <= 15
board 和 word 仅由大小写英文字母组成
来源：力扣（LeetCode）
链接：https://leetcode.cn/problems/word-search
著作权归领扣网络所有。商业转载请联系官方授权，非商业转载请注明出处。
*/

class Solution {
public:
    bool dfs_exist(vector<vector<char>>& board, string& word, int i, int j, int k, vector<vector<bool>>& visited) {
        if (k == word.size()) return true;
        if (k > word.size() || i < 0 || i >= board.size() || j < 0 || j >= board[0].size() || visited[i][j] || board[i][j] != word[k]) {
            // visited[i][j] = false;
            return false;
        }
        visited[i][j] = true;
        if (dfs_exist(board, word, i - 1, j, k + 1, visited) || dfs_exist(board, word, i, j - 1, k + 1, visited) || 
            dfs_exist(board, word, i + 1, j, k + 1, visited) || dfs_exist(board, word, i, j + 1, k + 1, visited)) return true;
        visited[i][j] = false;
        return false;
    }

    bool exist(vector<vector<char>>& board, string word) {
        vector<vector<bool>> visited(board.size(), vector<bool>(board[0].size()));
        for(int i = 0; i < board.size(); i++) {
            for(int j = 0; j < board[0].size(); j++) {
                bool is_exist = dfs_exist(board, word, i, j, 0, visited);
                if (is_exist) return true;
            }
        }
        return false;
    }
};

void TestSolution() {
    Solution s;
    vector<vector<char>> boards = {
        {'A','B','C','E'},
        {'S','F','C','S'},
        {'A','D','E','E'}
    };
    string word = "ABCCED";
    cout << s.exist(boards, word) << endl;
    word = "ABCB";
    cout << s.exist(boards, word) << endl;
    boards = {
        {'A','B','C','E'},
        {'S','F','C','S'},
        {'A','D','E','E'}
    };
    word = "SEE";
    cout << s.exist(boards, word) << endl;
}