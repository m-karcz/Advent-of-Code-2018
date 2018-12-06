#include <fstream>
#include <iostream>
#include <boost/range/istream_range.hpp>
#include <boost/range/algorithm.hpp>
#include <boost/range/adaptors.hpp>
#include <boost/range/numeric.hpp>
#include <boost/range/size.hpp>
#include <vector>
#include <array>
#include <cstdio>
#include "readlines_range.hpp"

using namespace ::boost;
using namespace boost::adaptors;

constexpr size_t FABRIC_SIZE = 1000;

int main()
{
    auto file = std::fstream{"3.txt"};
    std::vector<std::array<int, FABRIC_SIZE>> fabric(FABRIC_SIZE);
    for(const auto& line : readlines_range{file})
    {
        int left, top, width, height;
        sscanf(line.c_str(), "#%*i @ %i,%i: %ix%i", &left, &top, &width, &height);
        for(auto& row : fabric | sliced(top, top+height))
        {
            for(auto& piece : row | sliced(left, left+width))
            {
                ++piece;
            }
        }
    }
    std::cout << accumulate(fabric | transformed([](const auto& row){return count_if(row, [](int n){return n > 1;});}),0) << std::endl;
}
