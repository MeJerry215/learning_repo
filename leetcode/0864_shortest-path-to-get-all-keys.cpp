#include "common.h"


class Solution {
public:
  int shortestPathAllKeys(vector<string> &grid) {
    int m = grid.size(), n = grid[0].size();
    vector<vector<int>> mat(m, vector<int>(n, 0));
    int step_shift = 16, key_shift = 10, x_shift = 5, y_shift = 0;
    int step_mask = 0x7fff0000, key_mask = 0xfc00, x_mask = 0x3e0,
        y_mask = 0x1f;
    int state = 0, key_cnt = 0;
    int directions[5] = {0, 1, 0, -1, 0};
    auto is_valid = [&](int x, int y) -> bool {
      return x >= 0 && y >= 0 && x < m && y < n;
    };
    unordered_map<int, int> keys, locks;
    for (int i = 0; i < m; i++) {
      for (int j = 0; j < n; j++) {
        if (grid[i][j] == '#') {
          mat[i][j] = -1;
        } else if (grid[i][j] == '@') {
          state = (i << x_shift) + j;
        } else if (islower(grid[i][j])) {
          keys[i * n + j] = grid[i][j] - 'a';
          mat[i][j] = 0;
          key_cnt++;
        } else if (isupper(grid[i][j])) {
          locks[i * n + j] = grid[i][j] - 'A';
          mat[i][j] = 0;
        }
      }
    }
    vector<int> starts;
    int iter_cnt = 0;
    starts.push_back(state);
    for (int i = 0; i < key_cnt; i++) {
      vector<int> next_starts;
      if (starts.size() == 0)
        return -1;
      for (int j = 0; j < starts.size(); j++) {
        iter_cnt++;
        state = starts[j];
        queue<int> states;
        states.push(state);
        cout << "push " << state << endl;
        while (!states.empty()) {
          state = states.front();
          states.pop();
          int x = (state & x_mask) >> x_shift, y = state & y_mask;
          cout << "visit x " << x << " y " << y << endl;
          if (mat[x][y] == iter_cnt)
            continue;
          // 每一次迭代都以拿到钥匙为目的， 拿到就退
          if (keys.count(x * n + y) &&
              (state & key_mask) >> key_shift != 1 << keys[x * n + y]) {
            next_starts.push_back(state | ((1 << keys[x * n + y]) << key_shift));
            cout << "got the key" << endl;
            continue;
          }
          mat[x][y] = iter_cnt;
          for (int k = 0; k < 4; k++) {
            int p = x + directions[k];
            int q = y + directions[k + 1];
            // 墙或者已经访问
            if (!is_valid(p, q) || mat[p][q] < 0 || mat[p][q] == iter_cnt)
              continue;
            cout << "p " << p << " q " << q << endl;
            // 是锁，没有钥匙
            if (locks.count(p * n + q)){
                cout << "meet lock " << endl;
            }
            if (locks.count(p * n + q) &&
                ((state & key_mask) >> key_shift) != (1 << locks[x * n + y])) {
               cout << "is lock, no key" << endl;
              continue;
            }
            int next_state = ((((state & step_mask) >> step_shift) + 1)
                                 << step_shift) + q + (p << x_shift) + (state & key_mask);
            cout << "== p " << p << " == q " << q << " step " << ((((state & step_mask) >> step_shift) + 1)) << endl;
            states.push(next_state);
          }
        }
      }
      swap(starts, next_starts);
    }
    int res = INT_MAX;
    for (int i = 0; i < starts.size(); i++) {
      res = min(res, int((starts[i] & step_mask) >> step_shift));
    }
    return res == INT_MAX ? -1 : res;
  }
};
