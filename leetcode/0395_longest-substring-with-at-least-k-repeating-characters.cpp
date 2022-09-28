#include "common.h"


class Solution
{
public:
    int longestSubstring(string s, int k)
    {
        int ans = 0;
        int freq[26] = {0};
        int n = s.size();
        int unique = 0;
        for (int i = 0; i < n; i++)
        {
            freq[s[i] - 'a']++;
            if (freq[s[i] - 'a'] == 1)
                unique++;
        }

        for (int curr_unique = 1; curr_unique <= unique; curr_unique++) {
            memset(freq, 0, sizeof(freq));
            int start = 0, end = 0;
            int cnt = 0, count_k = 0;

            while (end < n) {
                if (cnt <= curr_unique) {
                    int ind = s[end] - 'a';
                    if (freq[ind] == 0)
                        cnt++;
                    freq[ind]++;
                    if (freq[ind] == k)
                        count_k++;
                    end++;
                } else {
                    int ind = s[start] - 'a';
                    if (freq[ind] == k)
                        count_k--;
                    freq[ind]--;
                    if (freq[ind] == 0)
                        cnt--;
                    start++;
                }
                if (cnt == curr_unique && count_k == curr_unique)
                    ans = max(ans, end - start);
            }
        }

        return ans;
    }
};