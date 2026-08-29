#pragma once

#include <cstdint>
#include <limits>
#include <vector>
#include <cstring>

#ifdef _MSC_VER
#define BSWAP16 _byteswap_ushort
#define BSWAP32 _byteswap_ulong
#define BSWAP64 _byteswap_uint64
#else
#define BSWAP16 __builtin_bswap16
#define BSWAP32 __builtin_bswap32
#define BSWAP64 __builtin_bswap64
#endif

namespace esfa::bit {

enum class Endianness
{
	Little = 0,
	Big = 1,

#if (defined(__BYTE_ORDER__) && (__BYTE_ORDER__ == __ORDER_BIG_ENDIAN__)) || defined(__BIG_ENDIAN__)
    Native = Big,
#else
    Native = Little,
#endif
};

class BitConverter
{
public:
	static inline int8_t ToInt8BE(const uint8_t* data, size_t offset)
	{
		return static_cast<int8_t>(data[offset]);
	}

	static inline int8_t ToInt8BE(const std::vector<uint8_t>& data, size_t offset)
	{
		return static_cast<int8_t>(data[offset]);
	}

	static inline uint8_t ToUInt8BE(const uint8_t* data, size_t offset)
	{
		return data[offset];
	}

	static inline uint8_t ToUInt8BE(const std::vector<uint8_t>& data, size_t offset)
	{
		return data[offset];
	}

	static inline int16_t ToInt16BE(const uint8_t* data, size_t offset)
	{
		return static_cast<int16_t>((static_cast<uint16_t>(data[offset]) << 8) |
		                            static_cast<uint16_t>(data[offset + 1]));
	}

	static inline int16_t ToInt16BE(const std::vector<uint8_t>& data, size_t offset)
	{
		return static_cast<int16_t>((static_cast<uint16_t>(data[offset]) << 8) |
		                            static_cast<uint16_t>(data[offset + 1]));
	}

	static inline uint16_t ToUInt16BE(const uint8_t* data, size_t offset)
	{
		return static_cast<uint16_t>((static_cast<uint16_t>(data[offset]) << 8) |
		                             static_cast<uint16_t>(data[offset + 1]));
	}

	static inline uint16_t ToUInt16BE(const std::vector<uint8_t>& data, size_t offset)
	{
		return static_cast<uint16_t>((static_cast<uint16_t>(data[offset]) << 8) |
		                             static_cast<uint16_t>(data[offset + 1]));
	}

	static inline int32_t ToInt32BE(const uint8_t* data, size_t offset)
	{
		return static_cast<int32_t>((static_cast<uint32_t>(data[offset]) << 24) |
		                            (static_cast<uint32_t>(data[offset + 1]) << 16) |
		                            (static_cast<uint32_t>(data[offset + 2]) << 8) |
		                            static_cast<uint32_t>(data[offset + 3]));
	}

	static inline int32_t ToInt32BE(const std::vector<uint8_t>& data, size_t offset)
	{
		return static_cast<int32_t>((static_cast<uint32_t>(data[offset]) << 24) |
		                            (static_cast<uint32_t>(data[offset + 1]) << 16) |
		                            (static_cast<uint32_t>(data[offset + 2]) << 8) |
		                            static_cast<uint32_t>(data[offset + 3]));
	}

	static inline uint32_t ToUInt32BE(const uint8_t* data, size_t offset)
	{
		return (static_cast<uint32_t>(data[offset]) << 24) |
		       (static_cast<uint32_t>(data[offset + 1]) << 16) |
		       (static_cast<uint32_t>(data[offset + 2]) << 8) |
		       static_cast<uint32_t>(data[offset + 3]);
	}

	static inline uint32_t ToUInt32BE(const std::vector<uint8_t>& data, size_t offset)
	{
		return (static_cast<uint32_t>(data[offset]) << 24) |
		       (static_cast<uint32_t>(data[offset + 1]) << 16) |
		       (static_cast<uint32_t>(data[offset + 2]) << 8) |
		       static_cast<uint32_t>(data[offset + 3]);
	}

	static inline int64_t ToInt64BE(const uint8_t* data, size_t offset)
	{
		return static_cast<int64_t>((static_cast<uint64_t>(data[offset]) << 56) |
		                            (static_cast<uint64_t>(data[offset + 1]) << 48) |
		                            (static_cast<uint64_t>(data[offset + 2]) << 40) |
		                            (static_cast<uint64_t>(data[offset + 3]) << 32) |
		                            (static_cast<uint64_t>(data[offset + 4]) << 24) |
		                            (static_cast<uint64_t>(data[offset + 5]) << 16) |
		                            (static_cast<uint64_t>(data[offset + 6]) << 8) |
		                            static_cast<uint64_t>(data[offset + 7]));
	}

	static inline int64_t ToInt64BE(const std::vector<uint8_t>& data, size_t offset)
	{
		return static_cast<int64_t>((static_cast<uint64_t>(data[offset]) << 56) |
		                            (static_cast<uint64_t>(data[offset + 1]) << 48) |
		                            (static_cast<uint64_t>(data[offset + 2]) << 40) |
		                            (static_cast<uint64_t>(data[offset + 3]) << 32) |
		                            (static_cast<uint64_t>(data[offset + 4]) << 24) |
		                            (static_cast<uint64_t>(data[offset + 5]) << 16) |
		                            (static_cast<uint64_t>(data[offset + 6]) << 8) |
		                            static_cast<uint64_t>(data[offset + 7]));
	}

	static inline uint64_t ToUInt64BE(const uint8_t* data, size_t offset)
	{
		return (static_cast<uint64_t>(data[offset]) << 56) |
		       (static_cast<uint64_t>(data[offset + 1]) << 48) |
		       (static_cast<uint64_t>(data[offset + 2]) << 40) |
		       (static_cast<uint64_t>(data[offset + 3]) << 32) |
		       (static_cast<uint64_t>(data[offset + 4]) << 24) |
		       (static_cast<uint64_t>(data[offset + 5]) << 16) |
		       (static_cast<uint64_t>(data[offset + 6]) << 8) |
		       static_cast<uint64_t>(data[offset + 7]);
	}

	static inline uint64_t ToUInt64BE(const std::vector<uint8_t>& data, size_t offset)
	{
		return (static_cast<uint64_t>(data[offset]) << 56) |
		       (static_cast<uint64_t>(data[offset + 1]) << 48) |
		       (static_cast<uint64_t>(data[offset + 2]) << 40) |
		       (static_cast<uint64_t>(data[offset + 3]) << 32) |
		       (static_cast<uint64_t>(data[offset + 4]) << 24) |
		       (static_cast<uint64_t>(data[offset + 5]) << 16) |
		       (static_cast<uint64_t>(data[offset + 6]) << 8) |
		       static_cast<uint64_t>(data[offset + 7]);
	}

	static inline float ToFloatBE(const uint8_t* data, size_t offset)
	{
		float value = 0.0f;
		uint32_t floatData = (static_cast<uint32_t>(data[offset]) << 24) |
		                     (static_cast<uint32_t>(data[offset + 1]) << 16) |
		                     (static_cast<uint32_t>(data[offset + 2]) << 8) |
		                     static_cast<uint32_t>(data[offset + 3]);
		static_assert(sizeof(uint32_t) == sizeof(float), "expected 32-bit float");
		std::memcpy(&value, &floatData, sizeof(value));
		return value;
	}

	static inline float ToFloatBE(const std::vector<uint8_t>& data, size_t offset)
	{
		float value = 0.0f;
		uint32_t floatData = (static_cast<uint32_t>(data[offset]) << 24) |
		                     (static_cast<uint32_t>(data[offset + 1]) << 16) |
		                     (static_cast<uint32_t>(data[offset + 2]) << 8) |
		                     static_cast<uint32_t>(data[offset + 3]);
		static_assert(sizeof(uint32_t) == sizeof(float), "expected 32-bit float");
		std::memcpy(&value, &floatData, sizeof(value));
		return value;
	}

	static inline double ToDoubleBE(const uint8_t* data, size_t offset)
	{
		double value = 0.0;
		uint64_t floatData =
			(static_cast<uint64_t>(data[offset]) << 56) |
			(static_cast<uint64_t>(data[offset + 1]) << 48) |
			(static_cast<uint64_t>(data[offset + 2]) << 40) |
			(static_cast<uint64_t>(data[offset + 3]) << 32) |
			(static_cast<uint64_t>(data[offset + 4]) << 24) |
			(static_cast<uint64_t>(data[offset + 5]) << 16) |
			(static_cast<uint64_t>(data[offset + 6]) << 8) |
			static_cast<uint64_t>(data[offset + 7]);
		static_assert(sizeof(uint64_t) == sizeof(double), "expected 64-bit double");
		static_assert(std::numeric_limits<double>::is_iec559,
		              "expected IEC559 doubles on host machine");
		std::memcpy(&value, &floatData, sizeof(value));
		return value;
	}

	static inline double ToDoubleBE(const std::vector<uint8_t>& data, size_t offset)
	{
		double value = 0.0;
		uint64_t floatData =
			(static_cast<uint64_t>(data[offset]) << 56) |
			(static_cast<uint64_t>(data[offset + 1]) << 48) |
			(static_cast<uint64_t>(data[offset + 2]) << 40) |
			(static_cast<uint64_t>(data[offset + 3]) << 32) |
			(static_cast<uint64_t>(data[offset + 4]) << 24) |
			(static_cast<uint64_t>(data[offset + 5]) << 16) |
			(static_cast<uint64_t>(data[offset + 6]) << 8) |
			static_cast<uint64_t>(data[offset + 7]);
		static_assert(sizeof(uint64_t) == sizeof(double), "expected 64-bit double");
		static_assert(std::numeric_limits<double>::is_iec559,
		              "expected IEC559 doubles on host machine");
		std::memcpy(&value, &floatData, sizeof(value));
		return value;
	}
};

}
