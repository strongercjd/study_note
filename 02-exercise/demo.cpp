#include <iostream>
#include <memory>
#include <vector>

using namespace std;

class Solution {
public:
    std::vector<int> runningSum(vector<int>& nums) {
        int n = nums.size();
        for (int i = 1; i < n; i++) {
            nums[i] += nums[i - 1];
        }
        return nums;
    }
};

int main()
{
    Solution solution_exe;
    vector<int> data = { 1,2,3,4,5 };
    vector<int> data_out;
    data_out = solution_exe.runningSum(data);
    for(int i=0; i<data_out.size(); i++)
        cout<<data_out[i]<<" " ;
    cout<<"\r\n" ;
    return 0;
}

