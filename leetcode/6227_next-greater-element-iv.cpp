#include "common.h"

class Solution {
public:
    vector<int> secondGreaterElement(vector<int>& nums) {
        int n = nums.size();
        stack<int> st;
        priority_queue<pair<int, int>, vector<pair<int,int>>, greater<pair<int,int>>> q;
        vector<int> res(n, -1);
        
        for(int i = 0; i < n; ++i) {
            while(q.size() && q.top().first < nums[i]) {
                res[q.top().second] = nums[i];
                q.pop();
            }
            
            while(!st.empty() && nums[st.top()] < nums[i]) {
                q.push({nums[st.top()], st.top()});
                st.pop();
            }
            st.push(i);
        }
        
        return res;
    }
};

void TestSolution() {
    Solution s;
    vector<int> nums = {2,4,0,9,6};
    s.secondGreaterElement(nums);
}