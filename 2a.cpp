#include <fstream>
#include <iostream>
#include <boost/range/istream_range.hpp>
#include <map>
#include <boost/algorithm/cxx11/any_of.hpp>

using namespace ::boost;
using namespace ::boost::algorithm;

int main()
{
    auto file = std::fstream{"2.txt"};
    size_t twos = 0;
    size_t threes = 0;
    const auto hasN = [](size_t N){return [=](const auto& pair){return pair.second == N;};};
    const auto hasTwo = hasN(2);
    const auto hasThree = hasN(3);
    for(const auto& line : istream_range<std::string>(file))
    {
        std::map<char,size_t> letterCounts;
        for(char c : line)
        {
            letterCounts[c]++;
        }
        twos += any_of(letterCounts, hasTwo);
        threes += any_of(letterCounts, hasThree);
    }
    std::cout << twos * threes << std::endl;
}
