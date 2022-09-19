#include "common.h"

class WordDictionary {
public:
    struct String
    {
        String(string& s): str(s) {}

        string str;

        bool operator==(const String& rhs) {
            if (str.size() != rhs.str.size()) return false;
            for(int i = 0; i < str.size(); i++) {
                if (rhs.str[i] == '.') continue;
                if (rhs.str[i] != str[i]) return false;
            }
            return true;
        }
    };
    
    WordDictionary() {
    }
    
    void addWord(string word) {
        strs.push_back(word);
    }
    
    bool search(string word) {
        return find(strs.begin(), strs.end(), String(word)) != strs.end();
    }

private:
    vector<String> strs;
};

/**
 * Your WordDictionary object will be instantiated and called as such:
 * WordDictionary* obj = new WordDictionary();
 * obj->addWord(word);
 * bool param_2 = obj->search(word);
 */