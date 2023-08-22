#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <iterator>
#include <fstream>

class CSVRow
{
public:
    std::string_view operator[](std::size_t index) const;
    std::size_t size() const;
    void readNextRow(std::istream& str);
    
private:
    std::string m_line;
    std::vector<int> m_data;
};


class CSVIterator
{
public:
    typedef std::input_iterator_tag iterator_category;
    typedef CSVRow value_type;
    typedef std::size_t difference_type;
    typedef CSVRow* pointer;
    typedef CSVRow& reference;

    CSVIterator(std::istream& str);
    CSVIterator();
    
    CSVIterator& operator++();
    CSVIterator operator++(int);
    CSVRow const& operator*() const;
    CSVRow const* operator->() const;
    
    bool operator==(CSVIterator const& rhs);
    bool operator!=(CSVIterator const& rhs);

private:
    std::istream* m_str;
    CSVRow m_row;
};

class CSVRange {
    std::istream& stream;

public:
    CSVRange(std::istream& str);
    CSVIterator begin() const;
    CSVIterator end() const;
    int getRowCount() const; 
};


std::istream& operator>>(std::istream& str, CSVRow& data);

