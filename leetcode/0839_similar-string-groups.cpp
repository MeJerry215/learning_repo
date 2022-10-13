#include "common.h"

/*
如果交换字符串 X 中的两个不同位置的字母，使得它和字符串 Y 相等，那么称 X 和 Y 两个字符串相似。如果这两个字符串本身是相等的，那它们也是相似的。
例如，"tars" 和 "rats" 是相似的 (交换 0 与 2 的位置)； "rats" 和 "arts" 也是相似的，但是 "star" 不与 "tars"，"rats"，或 "arts" 相似。
总之，它们通过相似性形成了两个关联组：{"tars", "rats", "arts"} 和 {"star"}。注意，"tars" 和 "arts" 是在同一组中，即使它们并不相似。形式上，对每个组而言，要确定一个单词在组中，只需要这个词和该组中至少一个单词相似。
给你一个字符串列表 strs。列表中的每个字符串都是 strs 中其它所有字符串的一个字母异位词。请问 strs 中有多少个相似字符串组？

示例 1：
输入：strs = ["tars","rats","arts","star"]
输出：2
示例 2：
输入：strs = ["omv","ovm"]
输出：1

提示：

1 <= strs.length <= 300
1 <= strs[i].length <= 300
strs[i] 只包含小写字母。
strs 中的所有单词都具有相同的长度，且是彼此的字母异位词。

来源：力扣（LeetCode）
链接：https://leetcode.cn/problems/similar-string-groups
著作权归领扣网络所有。商业转载请联系官方授权，非商业转载请注明出处。
*/

class Solution
{
public:
    bool is_similar(string &str1, string &str2)
    {
        int pre = -1, next = -1;
        for(int i = 0; i < str1.size(); i++) {
            if (str1[i] == str2[i]) continue;
            if (pre == -1) {
                pre = i;
            } else if (next == -1) {
                next = i;
            } else {
                return false;
            }
        }
        cout << pre << " " << next << endl;
        if (pre == -1 && next == -1) return true;
        if (pre == -1 || next == -1) return false;
        return str1[pre] == str2[next] && str1[next] == str2[pre];
    }

    int numSimilarGroups(vector<string> &strs)
    {
        /*
        O(n^2) 次比较str[i] 和str[j] 是否相似, 比较需要占用str size O(k)的扫描复杂度
        bfs 遍历O(n^2) 找连通子图。
        辅助空间O(n^2)
        */
        int n = strs.size();
        int k = strs[0].size();
        vector<vector<bool>> grid(n, vector<bool>(n, false));
        for (int i = 0; i < n; i++) {
            grid[i][i] = true;
            for (int j = i + 1; j < n; j++) {
                bool similar = is_similar(strs[i], strs[j]);
                grid[i][j] = similar;
                grid[j][i] = similar;
            }
        }
        print_mat(grid);
        int res = 0;
        for(int i = 0; i < n; i++) {
            for(int j = i; j < n; j ++) {
                if (!grid[i][j]) continue;
                queue<pair<int, int>> indexes;
                indexes.push({i, j});
                while(indexes.size() > 0) {
                    pair<int, int> index = indexes.front();
                    indexes.pop();
                    // make sure x alway lesss than y
                    if (index.first > index.second) swap(index.first, index.second);
                    grid[index.first][index.second] = false;
                    grid[index.second][index.first] = false;
                    // search index.first connect and index.second.conect;
                    for(int k = 0; k < n; k++) {
                        if (grid[index.first][k]) {
                            grid[k][index.first] = false;
                            indexes.push({index.first, k});
                        }
                        if (grid[index.second][k]) {
                            grid[k][index.second] = false;
                            indexes.push({index.second, k});
                        }
                    }
                }
                res ++;
            }
        }

        return res;
    }
};


void TestSolution() {
    Solution s;
    vector<string> strs = {"tars","rats","arts","star"};
    cout << s.numSimilarGroups(strs) << endl;
    string s1 = "tars";
    string s2 = "star";
    cout << s.is_similar(s1, s2) << endl;
}