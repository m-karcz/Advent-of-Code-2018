#include <fstream>
#include <iostream>
#include <boost/range/istream_range.hpp>
#include <boost/range/numeric.hpp>

using namespace ::boost;

int main()
{
    auto file = std::fstream{"1.txt"};
    std::cout << accumulate(istream_range<int>(file), 0) << "\n";
}
