#include "common.h"


class Solution {
public:

    // the size this problem will be O(n^3) if using normal method to solve.
    // the core to this method to caculate the distance of each two nodes, so at least the size will O(n^2)
    // change the thought, if one nodes has k nodes of same distance, it will generate k * (k - 1) boomerangs
    // so just iter on the n nodes, to caculate the count of same distance to this node.
    // the finaly time complexity will be O(n^2)
    int numberOfBoomerangs(vector<vector<int>>& points) {
        int res=0;
        int dis=0;
        for(int i=0;i<points.size();++i){
            unordered_map<int,int> record;
            for(int j=0;j<points.size();j++)
                if(j!=i){
                    // 计算距离时不进行开根运算, 以保证精度
                    dis=pow(points[i][0]-points[j][0],2)+pow(points[i][1]-points[j][1],2);
                    record[dis]++;
                }
            for(unordered_map<int, int>::iterator iter = record.begin() ; iter != record.end() ; iter ++)
            {
                if((*iter).second>=2)
                    res+=((*iter).second)*((*iter).second-1);
            }
        }
        return res;
    }

};