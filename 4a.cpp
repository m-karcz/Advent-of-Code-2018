#include <fstream>
#include <iostream>
#include <sstream>
#include <boost/range/algorithm/max_element.hpp>
#include <boost/range/algorithm/sort.hpp>
#include <vector>
#include <array>
#include <map>
#include <cstdio>
#include <ctime>
#include <iomanip>
#include "readlines_range.hpp"

using namespace ::boost;

constexpr int SECONDS_IN_MINUTE = 60;
constexpr int MINUTES_IN_HOUR = 60;

struct Entry
{
    tm timestamp;
    std::string text;
    bool operator<(const Entry& other)
    {
        return mktime(&timestamp) < mktime(const_cast<tm*>(&other.timestamp));
    }
};

int calcSleepingTime(tm* start, tm* end)
{
    return (mktime(end) - mktime(start)) / SECONDS_IN_MINUTE;
}



int main()
{
    auto file = std::fstream{"4.txt"};
    auto fileRange = readlines_range{file};

    std::vector<Entry> entries;

    for(const auto& rawEntry : fileRange)
    {
        std::stringstream ss("20" + rawEntry.substr(3,16));
        tm timestamp{};
        entries.emplace_back(Entry{timestamp, rawEntry.substr(19)});
        ss >> std::get_time(&entries.back().timestamp, "%Y-%m-%d %H:%M");

    }

    sort(entries);

    struct
    {
        int guardId{0};
        tm* startSleeping = nullptr;
    } state;

    std::map<int,int> guardId2sleepTime;

    const auto iterateOverEntries = [&](auto wakeCallback)
    {
        for(auto& entry : entries)
        {
            switch(entry.text[0])
            {
                case 'G':
                {
                    sscanf(entry.text.c_str(), "Guard #%i ", &state.guardId);
                    break;
                }
                case 'w':
                {

                    if(state.startSleeping)
                    {
                        int sleepingTime = calcSleepingTime(state.startSleeping, &entry.timestamp);

                        wakeCallback(sleepingTime);

                        state.startSleeping = nullptr;
                    }
                    break;
                }
                case 'f':
                {
                    state.startSleeping = &entry.timestamp;
                    break;
                }
            }
        }
    };

    iterateOverEntries([&](int sleepingTime){
            guardId2sleepTime[state.guardId] += sleepingTime;
    });

    int longestSleepGuardId = max_element(guardId2sleepTime, [](const auto& a, const auto& b){return a.second < b.second;})->first;

    std::array<int, MINUTES_IN_HOUR> minutes{};

    iterateOverEntries([&](int sleepingTime){
        if(state.guardId == longestSleepGuardId)
        {
            int initialMinute = state.startSleeping->tm_min;

            for(int n = 0; n < sleepingTime; n++)
            {
                minutes[(initialMinute + n + 60 - 1) % MINUTES_IN_HOUR]++;
            }
        }
    });

    int mostFrequentMinute = std::distance(minutes.begin(), max_element(minutes)) + 1;

    std::cout << longestSleepGuardId << std::endl;
    std::cout << mostFrequentMinute << std::endl;
    std::cout << longestSleepGuardId * mostFrequentMinute << std::endl;
}
