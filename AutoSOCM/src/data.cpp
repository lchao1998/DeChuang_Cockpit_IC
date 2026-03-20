#include "data.hpp"
#include <iostream>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/serialization.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/unordered_map.hpp>
#include <boost/iostreams/device/array.hpp>
#include <boost/iostreams/device/back_inserter.hpp>
#include <boost/iostreams/stream.hpp>
#include <boost/lexical_cast.hpp>

namespace AutoSOCM
{

class DataSerializer
{
public:
    static Data::Buffer& getDataBuffer( Data& data)
    {
        return data.m_buffer;
    }

    template<typename T>
    static void serializeData(const T& data, Data::Buffer& buffer)
    {
        boost::iostreams::back_insert_device< Data::Buffer > sink{buffer};
        boost::iostreams::stream< boost::iostreams::back_insert_device<Data::Buffer> > os{sink};
        os << data;
    }

    template<typename T>
    static void deserializeData(T& data, const Data::Buffer& buffer)
    {
        boost::iostreams::array_source source{buffer.data(), buffer.size()};
        boost::iostreams::stream<boost::iostreams::array_source> is{source};
        is >> data;
    }

    template<typename T>
    static void serializeComplexData(const T& data, Data::Buffer& buffer)
    {
        boost::iostreams::back_insert_device< Data::Buffer > sink{buffer};
        boost::iostreams::stream< boost::iostreams::back_insert_device<Data::Buffer> > os{sink};
        boost::archive::text_oarchive oa(os, boost::archive::no_header|boost::archive::no_codecvt);
        oa << data;
    }

    template<typename T>
    static void deserializeComplexData(T& data,const Data::Buffer& buffer)
    {
        try
        {
            boost::iostreams::array_source source{buffer.data(), buffer.size()};
            boost::iostreams::stream<boost::iostreams::array_source> is{source};
            boost::archive::text_iarchive ia(is, boost::archive::no_header|boost::archive::no_codecvt);
            ia >> data;
        }
        catch(boost::archive::archive_exception& e)
        {
            fprintf(stderr, "deserializeComplexData failed:%s, dumpinfo: typeid(%s), data(%s)\n",
                    e.what(),
                    typeid(T).name(),
                    std::string(buffer.data(),buffer.size()).c_str());
        }
    }
};
}

namespace boost { namespace serialization {

template <typename Archive>
void serialize(Archive &ar, AutoSOCM::Data& d, const unsigned int version)
{
    ar & AutoSOCM::DataSerializer::getDataBuffer(d);
}

} }

namespace AutoSOCM {

template <typename T>
static T convertHelper(const char* data, size_t size, bool* ok)
{
    T ret = T();
    try
    {
        ret = boost::lexical_cast<T>(data, size);
        if(ok)
            *ok=true;
    }
    catch(boost::bad_lexical_cast)
    {
        if(ok)
            *ok = false;
    }
    return ret;
}

Data::Data()
{
}

Data::Data(const int& value)
{
    DataSerializer::serializeData(value, m_buffer);
}

Data::Data(const unsigned& value)
{
    DataSerializer::serializeData(value, m_buffer);
}

Data::Data(const long long& value)
{
    DataSerializer::serializeData(value, m_buffer);
}

Data::Data(const unsigned long long& value)
{
    DataSerializer::serializeData(value, m_buffer);
}

Data::Data(const bool& value)
{
    DataSerializer::serializeData(value, m_buffer);
}

Data::Data(const std::string& value)
{
    m_buffer.assign(value.begin(), value.end());
}

Data::Data(const char* value, size_t size)
{
    if(size == -1)
        size = std::strlen(value);
    m_buffer.assign(value, value + size);
}

Data::Data(const float& value)
{
    DataSerializer::serializeData(value, m_buffer);
}

Data::Data(const double& value)
{
    DataSerializer::serializeData(value, m_buffer);
}

Data::Data(const long double& value)
{
    DataSerializer::serializeData(value, m_buffer);
}

Data::Data(const DataArray& array)
{
    DataSerializer::serializeComplexData(array, m_buffer);
}

Data::Data(const DataMap& map)
{
    DataSerializer::serializeComplexData(map, m_buffer);
    DataMap m;
    DataSerializer::deserializeComplexData(m, m_buffer);
}

Data::Data(Data &&d)
{
    m_buffer = std::move(d.m_buffer);
}

int Data::toInt(bool* ok) const
{
    return convertHelper<int>(m_buffer.data(), m_buffer.size(), ok);
}

unsigned int Data::toUInt(bool* ok)  const
{
    return convertHelper<unsigned int>(m_buffer.data(), m_buffer.size(), ok);
}

long long Data::toLongLong(bool* ok) const
{
    return convertHelper<long long>(m_buffer.data(), m_buffer.size(), ok);
}

unsigned long long Data::toULongLong(bool* ok) const
{
    return convertHelper<unsigned long long>(m_buffer.data(), m_buffer.size(), ok);
}

float Data::toFloat(bool* ok) const
{
    return convertHelper<float>(m_buffer.data(), m_buffer.size(), ok);
}

double Data::toDouble(bool* ok) const
{
    return convertHelper<double>(m_buffer.data(), m_buffer.size(), ok);
}

bool Data::toBool() const
{
    return (toInt() != 0);
}

std::string Data::toString() const
{
    return std::string(m_buffer.data(), m_buffer.size());
}

DataArray Data::toArray() const
{
    DataArray ret;
    DataSerializer::deserializeComplexData(ret, m_buffer);
    return ret;
}

DataMap Data::toMap() const
{
    DataMap ret;
    DataSerializer::deserializeComplexData(ret, m_buffer);
    return ret;
}

const char* Data::getData() const
{
    return m_buffer.data();
}

size_t Data::getSize() const
{
    return m_buffer.size();
}

bool Data::empty() const
{
    return m_buffer.empty();
}

bool Data::operator==(const Data& other) const
{
    return (m_buffer == other.m_buffer);
}

bool Data::operator!=(const Data& other) const
{
    return (m_buffer != other.m_buffer);
}

Data &Data::operator=(Data && other)
{
    m_buffer = std::move(other.m_buffer);
    return *this;
}

}

