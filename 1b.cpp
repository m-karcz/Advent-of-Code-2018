#include <fstream>
#include <iostream>
#include <vector>
#include <boost/range/istream_range.hpp>
#include <set>

using namespace ::boost;

int main()
{
    auto file = std::fstream{"1.txt"};
    auto differences = copy_range<std::vector<int>>(istream_range<int>(file));
    std::set<int> frequencies{0};
    int lastFrequency = 0;
    while(true)
    {
        for(int diff : differences)
        {
            int newFrequency = lastFrequency + diff;
            if(frequencies.find(newFrequency) != frequencies.end())
            {
                std::cout << newFrequency << '\n';
                return 0;
            }
            frequencies.insert(newFrequency);
            lastFrequency = newFrequency;
        }
    }
}
