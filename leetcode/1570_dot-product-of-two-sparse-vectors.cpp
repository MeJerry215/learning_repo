#include "common.h"

/*
给定两个稀疏向量，计算它们的点积（数量积）。
实现类 SparseVector：
SparseVector(nums) 以向量 nums 初始化对象。
dotProduct(vec) 计算此向量与 vec 的点积。
稀疏向量 是指绝大多数分量为 0 的向量。你需要 高效 地存储这个向量，并计算两个稀疏向量的点积。

进阶：当其中只有一个向量是稀疏向量时，你该如何解决此问题？

 

示例 1：

输入：nums1 = [1,0,0,2,3], nums2 = [0,3,0,4,0]
输出：8
解释：v1 = SparseVector(nums1) , v2 = SparseVector(nums2)
v1.dotProduct(v2) = 1*0 + 0*3 + 0*0 + 2*4 + 3*0 = 8
示例 2：

输入：nums1 = [0,1,0,0,0], nums2 = [0,0,0,0,2]
输出：0
解释：v1 = SparseVector(nums1) , v2 = SparseVector(nums2)
v1.dotProduct(v2) = 0*0 + 1*0 + 0*0 + 0*0 + 0*2 = 0
示例 3：

输入：nums1 = [0,1,0,0,2,0,0], nums2 = [1,0,0,0,3,0,4]
输出：6
 

提示：

n == nums1.length == nums2.length
1 <= n <= 10^5
0 <= nums1[i], nums2[i] <= 100

来源：力扣（LeetCode）
链接：https://leetcode.cn/problems/dot-product-of-two-sparse-vectors
著作权归领扣网络所有。商业转载请联系官方授权，非商业转载请注明出处。
*/

class SparseVector {
public:
    
    SparseVector(vector<int> &nums) {
        for(int i = 0; i < nums.size(); i++) {
            if (nums[i] != 0) {
                indexed_num[i] = nums[i];
            }
        }
    }
    
    // Return the dotProduct of two sparse vectors
    int dotProduct(SparseVector& vec) {
        int res = 0;
        for(auto iter: indexed_num) {
            res += iter.second * vec.indexed_num[iter.first];
        }
        return res;
    }
    unordered_map<int, int> indexed_num;
};


class SparseVector {
public:
    vector<pair<int, int>> mRecords{};
    int mSize = 0;

    SparseVector(vector<int> &nums) {
        int n = nums.size();
        for (int i = 0; i < n; ++i) {
            if (nums[i] != 0) {
                mRecords.push_back(pair<int, int>(i, nums[i]));
            }
        }
        mSize = mRecords.size();
    }
    
    // Return the dotProduct of two sparse vectors
    int dotProduct(SparseVector& vec) {
        int p = 0, q = 0;
        int ans = 0;
        while (p < mSize && q < vec.mSize) {
            while (p < mSize && q < vec.mSize && mRecords[p].first < vec.mRecords[q].first) {
                p++;
            }
            while (p < mSize && q < vec.mSize && mRecords[p].first > vec.mRecords[q].first) {
                q++;
            }
            if (p < mSize && q < vec.mSize && mRecords[p].first == vec.mRecords[q].first) {
                ans += mRecords[p].second * vec.mRecords[q].second;
                p++;
                q++;
            }
        }
        return ans;
    }
};

// Your SparseVector object will be instantiated and called as such:
// SparseVector v1(nums1);
// SparseVector v2(nums2);
// int ans = v1.dotProduct(v2);