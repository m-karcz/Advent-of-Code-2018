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
    auto fileRange = readlines_range{file};
    auto entries = std::vector<std::string>{fileRange.begin(), fileRange.end()};
    std::vector<std::array<int, FABRIC_SIZE>> fabric(FABRIC_SIZE);
    struct Entry
    {
        int index, left, top, width, height;
    };
    const auto traverseFabric = [&](const auto& line, auto callback){
        Entry e;
        sscanf(line.c_str(), "#%i @ %i,%i: %ix%i", &e.index, &e.left, &e.top, &e.width, &e.height);
        for(auto& row : fabric | sliced(e.top, e.top+e.height))
        {
            for(auto& piece : row | sliced(e.left, e.left+e.width))
            {
                callback(piece, e);
            }
        }
    };
    for(const auto& entry : entries)
    {
        traverseFabric(entry, [](auto& piece, const auto& e){
            piece = e.index;
        });
    }
    for(const auto& entry : entries)
    {
        bool ok = true;
        traverseFabric(entry, [&](auto& piece, const auto& e){
            if(piece != e.index)
            {
                ok = false;
                piece = e.index;
            }
        });
        if(ok)
        {
            std::cout << entry << std::endl;
            return 0;
        }
    }
}
