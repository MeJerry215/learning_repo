#include "common.h"


class SegmentTree {
public:
    SegmentTree(vector<int>& nums): nums(nums) {
        int n = nums.size();
        tree.resize(4 * n);
        build(1, 1, n);
    }

    int left(int x) {
        return x << 1;
    }

    int right(int x) {
        return x << 1 | 1;
    }

    void build(int k, int l, int r) {
        if (l == r) {
            tree[k] = nums[l - 1];
        } else {
            int m = l + (r - l) / 2;
            build(left(k), l, m);
            build(right(k), m + 1, r);
            push_up(k);
        }
    }

    void push_up(int k) {
        // tree[k] = max(tree[left(k)], tree[right(k)]);
        tree[k] = tree[left(k)] + tree[right(k)];
    }

    void update(int p, int v, int l, int r, int k) {
        if (l == r) {
            nums[k - 1] = v, tree[k] += v;
        } else {
            int m = l + (r - l) / 2;
            if (p <= m) {

            } else {
                
            }
        }
    }

// private:
    vector<int> tree;
    vector<int>& nums;
};


void TestSolution() {
    vector<int> nums = {1, 2, 3, 4, 5, 6};
    SegmentTree seg(nums);
    print_vec(seg.tree);
    print_vec(seg.nums);
}