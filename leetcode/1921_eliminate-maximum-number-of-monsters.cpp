#include "common.h"

class Solution
{
public:
    int eliminateMaximum(vector<int>& dist, vector<int>& speed) {
    // priority_queue<int, vector<int>, greater<int>> arrive_times;
    vector<int> arrive_times(dist.size(), 0);
    /*
    怪兽距离 4/5/6, speed = 5, 则产生的 结果为 0/1/1
    由于如果某个怪物 恰 在某一分钟开始时到达城市，这会被视为 输掉 游戏，则按照这个计算 实际应该为 0分钟就应该砍了 距离5的
    */
    for(int i = 0; i < dist.size(); i++) {
      arrive_times[i] = (dist[i] - 1) / speed[i];
    }
    sort(arrive_times.begin(), arrive_times.end());
    int time = 0;
    for(int i = 0; i < arrive_times.size(); i++) {
      if (time > arrive_times[i]) return time;
      time++;
    }
    return time;
  }
};