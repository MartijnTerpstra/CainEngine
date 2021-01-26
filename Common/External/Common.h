#pragma once

// Remove integral constant overflow
#pragma warning(disable : 4307)

// Likely/Unlikely attribute requires higher standard version
#pragma warning(disable : 5051)

#define _ENABLE_EXTENDED_ALIGNED_STORAGE 1

#define _HAS_EXCEPTIONS 0

#if _DEBUG
#define DEBUG_CHECKS 1
#endif

// stl includes
#include <future>
#include <memory>
#include <vector>
#include <chrono>
#include <tuple>
#include <fstream>
#include <optional>
#include <functional>
#include <map>
#include <variant>

using ::std::chrono::seconds;
using ::std::chrono::milliseconds;
using ::std::future;
using ::std::unique_ptr;
using ::std::make_unique;
using ::std::pair;
using ::std::make_pair;
using ::std::shared_ptr;
using ::std::make_shared;
using ::std::weak_ptr;
using ::std::move;
using ::std::tuple;
using ::std::lock_guard;
using ::std::vector;
using ::std::string;
using ::std::optional;
using ::std::string_view;
using ::std::variant;

template<typename KeyType, typename ValueType>
using hash_map = ::std::unordered_map<KeyType, ValueType>;

inline constexpr ::std::nullopt_t none = ::std::nullopt;

// mst includes
#include <mcommon.h>
#include <mmath2.h>
#include <mplatform.h>
#include <mflag.h>
#include <malgorithm.h>
#include <mthreading_slim.h>
#include <mstack_string.h>
#include <mcompiletime.h>
#include <moptional.h>
#include <mplatform.h>
#include <mstack_string.h>
#include <marray_view.h>
#include <marraymap.h>
#include <maligned_malloc.h>
#include <muuid.h>
#include <msparse_set.h>
#include <mcolony.h>
#include <mranges.h>

namespace swizzle {
using namespace ::mst::math::swizzle;
}

namespace platform {
using namespace ::mst::platform;
}

using ::mst::flag;
using ::mst::make_flag;
using ::mst::memory_view;
using ::mst::array_view;
using ::mst::arraymap;
using ::mst::uuid;
using ::mst::sparse_set;
using ::mst::colony;
using ::mst::range;
using ::mst::crange;

template<size_t MaxElements>
using stack_string = ::mst::stack_string<char, MaxElements>;

typedef ::mst::math::vector<float, 2> float2;
typedef ::mst::math::vector<float, 3> float3;
typedef ::mst::math::vector<float, 4> float4;

typedef ::mst::math::matrix<float, 4, 4> matrix, matrix4x4;
typedef ::mst::math::matrix<float, 4, 3> matrix4x3;

typedef ::mst::math::quaternion<float> quaternion;

typedef ::mst::math::vector<int, 2> int2;
typedef ::mst::math::vector<uint32_t, 2> uint2;
typedef ::mst::math::vector<uint32_t, 3> uint3;

typedef ::mst::math::aabb<float, 3> aabb;

typedef ::mst::math::radians<float> radians;
typedef ::mst::math::degrees<float> degrees;

using ::mst::math::euler_rotation_order;

// Abseil includes
#include <absl/container/inlined_vector.h>
#include <absl/container/flat_hash_map.h>
#include <absl/container/node_hash_map.h>

template<typename T, size_t N>
using inlined_vector = ::absl::InlinedVector<T, N>;

using absl::flat_hash_map;
using absl::node_hash_map;

namespace mst {

template <typename H>
inline H AbslHashValue(H state, const uuid& v)
{
	uint8_t bytes[sizeof(v)];
	memcpy(bytes, &v, sizeof(v));

	return H::combine_contiguous(std::move(state), bytes, sizeof(v));
}

}

#define COMMON_DECLARE_NON_COPY(TypeName) \
TypeName(TypeName&&) = delete; \
TypeName(const TypeName&) = delete; \
TypeName& operator = (TypeName&&) = delete; \
TypeName& operator = (const TypeName&) = delete

#define COMMON_DECLARE_INTERFACE(TypeName) \
protected: \
TypeName() = default; \
virtual ~TypeName() = 0; \
COMMON_DECLARE_NON_COPY(TypeName)

#if DEBUG_CHECKS
#define COMMON_ASSERT(x) [&]() { if(!(x)) [[unlikely]] { Common::FatalError("Assertion failed: " #x); } }()
#else // !DEBUG_CHECKS
#define COMMON_ASSERT(x)
#endif

namespace Common {

template<typename T>
class RefPtr;

template<typename T>
class CastHandle;

class BaseObject;

constexpr uint32_t PackChars(char a, char b, char c, char d)
{
	return (uint32_t)d +
		((uint32_t)c << 8) +
		((uint32_t)b << 16) +
		((uint32_t)a << 24);
}

string CombinePath(const string& left, const string& right);

}; // namespace Common

#include "Common/Enums.h"
#include "Common/Logging.h"
#include "Common/Callstack.h"
#include "Common/BaseObject.h"
#include "Common/RefPtr.h"
#include "Common/JsonParser.h"
#include "Common/Source.h"
#include "Common/Details/Thread.h"
#include "Common/Tasks.h"

#include "Common/BaseObject.inl"
#include "Common/RefPtr.inl"
#include "Common/JsonParser.inl"
#include "Common/Source.inl"
