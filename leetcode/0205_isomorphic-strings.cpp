#include "common.h"

class Solution
{
public:
    bool isIsomorphic(string s, string t) {
        unordered_map<char, char> charMappings;
        unordered_set<char> mappingValues;
        if (s.size() != t.size())
            return false;
        for (int i = 0; i < s.size(); i++) {
            if (charMappings.find(s[i]) != charMappings.end()) {
                if (charMappings[s[i]] != t[i])
                    return false;
            } else {
                if (mappingValues.find(t[i]) != mappingValues.end())
                    return false;
                charMappings[s[i]] = t[i];
                mappingValues.insert(t[i]);
            }
        }
        return true;
    }
};