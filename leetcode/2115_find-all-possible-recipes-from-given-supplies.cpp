#include "common.h"

class Solution
{
public:
    vector<string> findAllRecipes(vector<string> &recipes, vector<vector<string>> &ingredients, vector<string> &supplies)
    {
        vector<string> res;
        unordered_set<string> resources(supplies.begin(), supplies.end());
        unordered_map<string, int> indexs;
        for (int i = 0; i < recipes.size(); i++)
        {
            indexs.emplace(recipes[i], i);
        }
        vector<bool> visited(recipes.size(), false);
        function<bool(string &)> cook = [&](string &recipe)
        {
            if (resources.count(recipe))
                return true;
            if (!indexs.count(recipe))
                return false;
            int idx = indexs[recipe];
            if (visited[idx])
                return false;
            visited[idx] = true;
            for (auto &ingredient : ingredients[idx])
            {
                if (!resources.count(ingredient) && !cook(ingredient))
                {
                    visited[idx] = false;
                    return false;
                }
            }
            visited[idx] = false;
            resources.insert(recipe);
            return true;
        };
        for (auto &recipe : recipes)
        {
            if (cook(recipe))
                res.push_back(recipe);
        }
        return res;
    }
};