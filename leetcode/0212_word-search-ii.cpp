#include "common.h"

class Solution {
public:
    struct TrieNode {
        string word;
        unordered_map<char, TrieNode*> children;
        TrieNode(): word("") {};
    };

    vector<string> findWords(vector<vector<char>>& board, vector<string>& words) {
        TrieNode* root = new TrieNode();
        int directions[5] = {0, 1, 0, -1, 0};
        function<void(string&)> build = [&](string& word) {
            TrieNode* node = root;
            for(auto c: word) {
                if (!node->children.count(c)) {
                    node->children[c] = new TrieNode();
                }
                node = node->children[c];
            }
            node->word = word;
        };
        for(auto& word: words) {
            build(word);
        }
        unordered_set<string> res;
        function<void(int, int, TrieNode*)> dfs = [&](int x, int y, TrieNode* node) {
            char c = board[x][y];
            if (!node->children.count(board[x][y])) return;
            node = node->children[c];
            if (!node->word.empty()) res.insert(node->word);
            board[x][y] = '#';
            for(int i = 0; i < 4; i++) {
                int p = x + directions[i];
                int q = y + directions[i + 1];
                if (p < 0 || q < 0 || p >= board.size() || q>= board[0].size() || board[p][q] == '#') continue;
                dfs(p, q, node);
            }
            board[x][y] = c;
        };
        for(int i = 0; i < board.size(); i++) {
            for(int j = 0; j < board[0].size(); j++) {
                dfs(i, j, root);
            }
        }
        return vector<string>(res.begin(), res.end());

    }
};

void TestSolution() {
    Solution s;
    vector<vector<char>> board = {
        {'o','a','a','n'},
        {'e','t','a','e'},
        {'i','h','k','r'},
        {'i','f','l','v'}
    };
    vector<string> words = {"oath","pea","eat","rain","hklf", "hf"};
    auto res = s.findWords(board, words);
    print_vec(res);
}