#pragma once

#include <boost/iterator/iterator_facade.hpp>
#include <boost/optional.hpp>
#include <istream>

struct readlines_range
{
    readlines_range(std::istream& p_stream) : m_stream{p_stream}
    {}
    struct iterator : boost::iterator_facade<iterator,
                                             std::string,
                                             boost::single_pass_traversal_tag>
    {
        iterator(std::istream& p_stream,
                 std::string& p_line) :
            m_stream{p_stream},
            m_line{p_line}
        {
            increment();
        }
        iterator() = default;
    private:
        friend class boost::iterator_core_access;

        void increment()
        {
            if(not std::getline(*m_stream, *m_line))
            {
                *this = iterator{};
            }
        }
        std::string& dereference() const
        {
            return *m_line;
        }
        bool equal(const iterator& p_other) const
        {
            return m_line == p_other.m_line;
        }
        boost::optional<std::istream&> m_stream;
        boost::optional<std::string&> m_line;
    };
    iterator begin() const
    {
        return iterator{m_stream, m_line};
    }
    iterator end() const
    {
        return iterator{};
    }
private:
    std::istream& m_stream;
    mutable std::string m_line;
};
