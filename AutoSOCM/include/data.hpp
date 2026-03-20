#ifndef AUTOSOCM_DATA_HPP
#define AUTOSOCM_DATA_HPP

#include <vector>
#include <unordered_map>
#include <string>

namespace AutoSOCM {

class Data;
typedef std::vector<Data> DataArray;
typedef std::unordered_map<std::string, Data> DataMap;

class Data
{
    friend class DataSerializer;
public:
    typedef std::vector<char> Buffer;

    Data();

    Data(const int& value);

    Data(const unsigned& value);

    Data(const long long& value);

    Data(const unsigned long long& value);

    Data(const bool& value);

    Data(const std::string& value);

    Data(const char* value, size_t size = -1);

    Data(const float& value);

    Data(const double& value);

    Data(const long double& value);

    Data(const DataArray& array);

    Data(const DataMap& map);

    int toInt(bool* ok = nullptr) const;

    unsigned int toUInt(bool* ok = nullptr)  const;

    long long toLongLong(bool* ok = nullptr) const;

    unsigned long long toULongLong(bool* ok = nullptr) const;

    float toFloat(bool* ok = nullptr) const;

    double toDouble(bool* ok = nullptr) const;

    bool toBool() const;

    std::string toString() const;

    DataArray toArray() const;

    DataMap toMap() const;

    const char* getData() const;

    size_t getSize() const;

    bool empty() const;

    bool operator == (const Data& other) const;

    bool operator != (const Data& other) const;

    Data& operator = (Data&& other);

    //Move Constructor
    Data(Data&& d);

private:
    Data(const Data& cpy) = delete;

private:
    Buffer m_buffer;
};
}
#endif
