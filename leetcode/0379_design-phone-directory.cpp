#include "common.h"

/*
设计一个电话目录管理系统，让它支持以下功能：
get: 分配给用户一个未被使用的电话号码，获取失败请返回 -1
check: 检查指定的电话号码是否被使用
release: 释放掉一个电话号码，使其能够重新被分配

示例：

// 初始化电话目录，包括 3 个电话号码：0，1 和 2。
PhoneDirectory directory = new PhoneDirectory(3);

// 可以返回任意未分配的号码，这里我们假设它返回 0。
directory.get();

// 假设，函数返回 1。
directory.get();

// 号码 2 未分配，所以返回为 true。
directory.check(2);

// 返回 2，分配后，只剩一个号码未被分配。
directory.get();

// 此时，号码 2 已经被分配，所以返回 false。
directory.check(2);

// 释放号码 2，将该号码变回未分配状态。
directory.release(2);

// 号码 2 现在是未分配状态，所以返回 true。
directory.check(2);
 

提示：

1 <= maxNumbers <= 10^4
0 <= number < maxNumbers
调用方法的总数处于区间 [0 - 20000] 之内

来源：力扣（LeetCode）
链接：https://leetcode.cn/problems/design-phone-directory
著作权归领扣网络所有。商业转载请联系官方授权，非商业转载请注明出处。
*/

class PhoneDirectory {
public:
    queue<int> free_num;
    unordered_set<int> occupy_num;

    PhoneDirectory(int maxNumbers) {
        for(int i = 0; i < maxNumbers; i++) {
            free_num.push(i);
        }
    }
    
    int get() {
        if (free_num.empty()) return -1;
        int num = free_num.front();
        free_num.pop();
        occupy_num.emplace(num);
        return num;
    }
    
    bool check(int number) {
        return !occupy_num.count(number);
    }
    
    void release(int number) {
        if (check(number)) return;
        occupy_num.erase(number);
        free_num.push(number);
    }
};

/**
 * Your PhoneDirectory object will be instantiated and called as such:
 * PhoneDirectory* obj = new PhoneDirectory(maxNumbers);
 * int param_1 = obj->get();
 * bool param_2 = obj->check(number);
 * obj->release(number);
 */

class PhoneDirectory {
public:
    /** Initialize your data structure here
        @param maxNumbers - The maximum numbers that can be stored in the phone directory. */
    vector<int> nums;
    vector<int> used;
    int N;
    int k;
    PhoneDirectory(int maxNumbers) {
        N = maxNumbers;
        k = 0;
        used = vector<int>(N, 0);
        for (int i = 0; i < N; ++i) {
            nums.push_back(i);
        }
    }
    
    /** Provide a number which is not assigned to anyone.
        @return - Return an available number. Return -1 if none is available. */
    int get() {
        if (k == N) return -1;
        int n = nums[k++];
        used[n] = 1;
        return n;
    }
    
    /** Check if a number is available or not. */
    bool check(int number) {
        if (number >= N) return false;
        return !used[number];
    }
    
    /** Recycle or release a number. */
    void release(int number) {
        if (number < N && used[number]) {
            used[number] = 0;
            nums[--k] = number;
        }
    }
};

