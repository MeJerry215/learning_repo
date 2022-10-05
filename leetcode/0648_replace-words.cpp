#include "common.h"

/*
在英语中，我们有一个叫做 词根(root) 的概念，可以词根后面添加其他一些词组成另一个较长的单词——我们称这个词为 继承词(successor)。例如，词根an，跟随着单词 other(其他)，可以形成新的单词 another(另一个)。
现在，给定一个由许多词根组成的词典 dictionary 和一个用空格分隔单词形成的句子 sentence。你需要将句子中的所有继承词用词根替换掉。如果继承词有许多可以形成它的词根，则用最短的词根替换它。
你需要输出替换之后的句子。
示例 1：
输入：dictionary = ["cat","bat","rat"], sentence = "the cattle was rattled by the battery"
输出："the cat was rat by the bat"
示例 2：
输入：dictionary = ["a","b","c"], sentence = "aadsfasf absbs bbab cadsfafs"
输出："a a b c"

提示：
1 <= dictionary.length <= 1000
1 <= dictionary[i].length <= 100
dictionary[i] 仅由小写字母组成。
1 <= sentence.length <= 10^6
sentence 仅由小写字母和空格组成。
sentence 中单词的总量在范围 [1, 1000] 内。
sentence 中每个单词的长度在范围 [1, 1000] 内。
sentence 中单词之间由一个空格隔开。
sentence 没有前导或尾随空格。
来源：力扣（LeetCode）
链接：https://leetcode.cn/problems/replace-words
著作权归领扣网络所有。商业转载请联系官方授权，非商业转载请注明出处。
*/
class Solution {
    const static int ALPHA_NUM = 26;
public:
    /* 字典树解法就是比较蛋疼 */
    struct Node {
        string root = "";
        Node* nodes[ALPHA_NUM];
        bool is_root;
        Node() {
            is_root = false;
            memset(nodes, 0, sizeof(Node *) * ALPHA_NUM);
        };
    };

    void make_search_tree(Node* node, string& word) {
        Node* cur = node;
        for(int i = 0; i < word.size(); i++) {
            if (cur->nodes[word[i] - 'a'] == nullptr) {
                Node* tmp = new Node();
                cur->nodes[word[i] - 'a'] = tmp;
            }
            cur = cur->nodes[word[i] - 'a'];
            if (i == word.size() - 1) {
                cur->is_root = true;
                cur->root = word;
            }
        }
    }

    string replaceWords(vector<string>& dictionary, string sentence) {
        Node* root = new Node();
        for(string& word: dictionary) {
            make_search_tree(root, word);
        }
        sentence += " ";
        string ans = "";
        int start = 0;
        for(int i = 0; i < sentence.size(); i++) {
            if (isalpha(sentence[i])) {
                continue;
            }
            string tmp = sentence.substr(start, i - start);
            start = i + 1;
            Node* cur = root;
            for(int j = 0; j < tmp.size(); j++) {
                cur = cur->nodes[tmp[j] - 'a'];
                if (cur != nullptr && cur->is_root) {
                    if (ans.size()) {
                        ans += " ";
                    }
                    ans += cur->root;
                    break;
                }
                if (cur == nullptr || j == tmp.size() - 1) {
                    if (ans.size()) {
                        ans += " ";
                    }
                    ans += tmp;
                    break;
                }
                
            }

        }
        return ans;
    }
};