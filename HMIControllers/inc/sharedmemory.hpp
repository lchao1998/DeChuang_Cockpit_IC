/**
* @file         sharedmemory.hpp
* @brief        Atom with number/string/struct
* @details      
* @author       xu.qiang@zlingsmart.com
* @date         2022-3-2
* @version      v0.0.1
* @copyright    Copyright (c) 2022, zlingsmart Corporation.
**********************************************************************************
* @attention 
* 
* @par modity log:
* <table>
* <tr><th>Date        <th>Version  <th>Author    <th>Description
* <tr><td>2022/03/02  <td>v0.0.1   <td>          <td>Create
* </table>
*
**********************************************************************************
*/
#pragma once

#define BOOST_DATE_TIME_NO_LIB
#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/allocators/allocator.hpp>
#include <boost/interprocess/containers/string.hpp>
#include <list>
#include <sys/file.h>

namespace SharedMemory
{
	using namespace std;

	class AtomicLocker
	{
	public:
        	AtomicLocker(const string& name): m_fd( open(name.c_str(), O_CREAT | O_RDWR, 0777) )
    		{
        		assert(m_fd > 0);
    		}

    		void atomic(function<void(void)> func){
        		flock(m_fd , LOCK_EX);
        		func();
        		flock(m_fd , LOCK_UN);
    		}

    	private:
        	int m_fd;
    	};

	class Pool:public AtomicLocker, public boost::interprocess::managed_shared_memory
	{
	public:
		Pool(const char* name, const size_t& maxSize):
			AtomicLocker(string("/tmp/")+ name+".locker"),
			boost::interprocess::managed_shared_memory(boost::interprocess::open_or_create, name, maxSize)
		{}
	};

	template <typename NumberType>
	class AtomicNumber
	{
		static_assert(
			is_same<NumberType, bool>::value ||
			is_same<NumberType, float>::value ||
			is_same<NumberType, signed char>::value ||
			is_same<NumberType, unsigned char>::value ||
			is_same<NumberType, signed short>::value ||
			is_same<NumberType, unsigned short>::value ||
			is_same<NumberType, signed int>::value ||
			is_same<NumberType, unsigned int>::value ||
			"class AtomicNumber's template type isn't number type.");

	public:
		AtomicNumber(Pool& pool, const char* name, const NumberType& initValue):
			m_pool(pool)
		{
			m_pool.atomic([&]() {
				auto result = pool.find_no_lock<NumberType>(name);
				if (result.first)
					m_ptr = result.first;
				else
					m_ptr = pool.construct<NumberType>(name)(initValue);
			});
			assert(m_ptr != nullptr);
			m_local = initValue;
		}

		bool isModified() const
		{
			bool ret;
			m_pool.atomic([&]() {  ret = (m_local != *m_ptr); });
			return ret;
		}

		NumberType getValue() 
		{
			m_pool.atomic([&]() {  m_local = *m_ptr; });
			return m_local;
		}

		bool syncValue(NumberType& dst)
		{
			NumberType cur = getValue();
			if(dst != cur)
			{
				dst = cur;
				return true;
			}
			return false;
		}

		void setValue(const NumberType& value)
		{
			m_pool.atomic([&]() { *m_ptr = value; });
		}

		void operator=(const NumberType& value) { setValue(value); }

		bool operator==(const NumberType& value) { return (value == getValue()); }

		bool operator!=(const NumberType& value){ return (value != getValue()); }

	private:
		Pool& m_pool;
		NumberType* m_ptr;
		NumberType m_local;
	};

	class AtomicString
	{
		typedef boost::interprocess::allocator<char, boost::interprocess::managed_shared_memory::segment_manager> string_allocator;
		typedef boost::interprocess::basic_string<char, std::char_traits<char>, string_allocator > shm_string;

	public:
		AtomicString(Pool& pool, const char* name, const string& initValue) :
			m_pool(pool)
		{
			m_pool.atomic([&]() {
				auto result = pool.find_no_lock<shm_string>(name);
				if (result.first)
					m_ptr = result.first;
				else
					m_ptr = pool.construct<shm_string>(name)(initValue.c_str(), pool.get_segment_manager());
			});

			assert(m_ptr != nullptr);
			m_local = initValue;
		}

		bool isModified() const
		{
			bool ret;
			m_pool.atomic([&]() { ret = (m_local != m_ptr->c_str()); });
			return ret;
		}

		string getValue() {
			m_pool.atomic([&]() { m_local = m_ptr->c_str(); });
			return m_local;
		}

		bool syncValue(string& dst)
		{
			string cur = getValue();
			if(dst != cur)
			{
				dst = cur;
				return true;
			}
			return false;
		}

		void setValue(const string& value)
		{
			m_pool.atomic([&](){*m_ptr = value.c_str();});
		}

		void operator=(const string& value) { setValue(value); }

		bool operator==(const string& value) { return (value == getValue()); }

		bool operator!=(const string& value) { return (value != getValue()); }

	private:
		Pool& m_pool;
		shm_string* m_ptr;
		string m_local;
	};

	template <typename LiteralType>
	class AtomicStruct
	{
		static_assert(std::is_literal_type<LiteralType>::value, "class AtomicStruct's template isn't literal type");

		static bool equal(const LiteralType& src, const LiteralType& dst)
		{
			return !memcmp(&src, &dst, sizeof(LiteralType));
		}

		static void asign(LiteralType& src, const LiteralType& dst)
		{
			memcpy(&src, &dst, sizeof(LiteralType));
		}

	public:
		AtomicStruct(Pool& pool, const char* name, const LiteralType& initValue):
			m_pool(pool)
		{
			m_pool.atomic([&](){
				auto result = m_pool.find_no_lock<LiteralType>(name);
				if (result.first)
					m_ptr = result.first;
				else
				{
					m_ptr = m_pool.construct<LiteralType>(name)(initValue);
				}
			});

			assert(m_ptr != nullptr);

			asign(m_local,initValue);
		}

		bool isModified() const
		{
			bool ret;
			m_pool.atomic([&]() { ret = !equal(m_local, *m_ptr); });
			return ret;
		}

		LiteralType getValue()
		{
			m_pool.atomic([&]() { asign(m_local, *m_ptr); });
			return m_local;
		}

		bool syncValue(LiteralType& dst)
		{
			LiteralType cur = getValue();
			if(!equal(dst,cur))
			{
				dst = cur;
				return true;
			}
			return false;
		}

		void setValue(const LiteralType& value)
		{
			m_pool.atomic([&]() { asign(*m_ptr,value); });
		}

		void operator=(const LiteralType& value) { setValue(value); }

		bool operator==(const LiteralType& value) { return equal(value, getValue()); }

		bool operator!=(const LiteralType& value) { return !(*this == value); }

	private:
		Pool& m_pool;
		LiteralType* m_ptr;
		LiteralType m_local;
	};
}

