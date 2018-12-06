#include <fstream>
#include <iostream>
#include <boost/range/istream_range.hpp>
#include <boost/range/adaptor/transformed.hpp>
#include <boost/range/adaptor/filtered.hpp>
#include <boost/range/combine.hpp>
#include <boost/range/algorithm.hpp>
#include <vector>

using namespace ::boost;
using namespace boost::adaptors;

const auto compareChars = [](const auto& pack)
{
    return get<0>(pack) == get<1>(pack);
};

bool differsByOneLetter(const std::string& a, const std::string& b)
{
    return 1 == count(combine(a,b) | transformed(std::ref(compareChars)), false);
}

int main()
{
    auto file = std::fstream{"2.txt"};
    size_t twos = 0;
    auto ids = copy_range<std::vector<std::string>>(istream_range<std::string>(file));
    for(auto left = ids.begin(); left != ids.end(); ++left)
    {
        for(auto right = left + 1; right != ids.end(); ++right)
        {
            if(differsByOneLetter(*left, *right))
            {
                std::cout << copy_range<std::string>(
                                combine(*left, *right) | filtered(std::ref(compareChars))
                                                       | transformed([](const auto& pack){return get<0>(pack);})) << std::endl;
            }
        }
    }

}
