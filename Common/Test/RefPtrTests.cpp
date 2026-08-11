#include <Common.h>

#include <gtest/gtest.h>

using namespace ::CainEngine;
using namespace ::CainEngine::Common;

namespace {

// Tracks construction/destruction so tests can assert RefPtr's refcounting
// destroys the object at exactly the right time - not early, not never.
struct Counters
{
	static inline int constructed = 0;
	static inline int destroyed = 0;

	static void Reset()
	{
		constructed = 0;
		destroyed = 0;
	}
};

class Base : public BaseObject
{
public:
	Base() { ++Counters::constructed; }
	~Base() override { ++Counters::destroyed; }

	virtual int Tag() const { return 1; }

private:
	// Deliberately returns nullptr for unrecognized types, matching
	// BaseObject::As()'s documented "null on failure" contract. Some other
	// _As() overrides elsewhere in the codebase instead call
	// Common::FatalError() in their default case, which contradicts that
	// contract and would abort the process on a failed cast rather than
	// letting the caller check the result - not something to imitate here.
	void* _As(uint64_t typeHash) const override
	{
		switch(typeHash)
		{
			CHECK_TYPE_AND_RETURN(BaseObject);
			CHECK_TYPE_AND_RETURN(Base);
		default:
			return nullptr;
		}
	}
};

class Derived final : public Base
{
public:
	int Tag() const override { return 2; }

private:
	void* _As(uint64_t typeHash) const override
	{
		switch(typeHash)
		{
			CHECK_TYPE_AND_RETURN(BaseObject);
			CHECK_TYPE_AND_RETURN(Base);
			CHECK_TYPE_AND_RETURN(Derived);
		default:
			return nullptr;
		}
	}
};

class Unrelated final : public BaseObject
{
private:
	void* _As(uint64_t typeHash) const override
	{
		switch(typeHash)
		{
			CHECK_TYPE_AND_RETURN(BaseObject);
			CHECK_TYPE_AND_RETURN(Unrelated);
		default:
			return nullptr;
		}
	}
};

class RefPtrTest : public ::testing::Test
{
protected:
	void SetUp() override
	{
		Counters::Reset();
	}
};

} // namespace

// -- Construction & basic lifetime --------------------------------------------

TEST_F(RefPtrTest, DefaultConstructedIsNull)
{
	RefPtr<Base> ptr;

	EXPECT_FALSE(bool(ptr));
	EXPECT_EQ(nullptr, ptr.Get());
}

TEST_F(RefPtrTest, CreateConstructsExactlyOneObject)
{
	auto ptr = RefPtr<Base>::Create();

	EXPECT_TRUE(bool(ptr));
	EXPECT_NE(nullptr, ptr.Get());
	EXPECT_EQ(1, Counters::constructed);
	EXPECT_EQ(0, Counters::destroyed);
}

TEST_F(RefPtrTest, GoingOutOfScopeDestroysTheObject)
{
	{
		auto ptr = RefPtr<Base>::Create();
		EXPECT_EQ(0, Counters::destroyed);
	}

	EXPECT_EQ(1, Counters::destroyed);
}

// -- Reference counting --------------------------------------------------------

TEST_F(RefPtrTest, CopyKeepsObjectAliveUntilLastReferenceIsGone)
{
	auto first = RefPtr<Base>::Create();
	{
		RefPtr<Base> second = first;
		EXPECT_EQ(0, Counters::destroyed);
	}

	// second went out of scope, but first is still holding a reference.
	EXPECT_EQ(0, Counters::destroyed);
}

TEST_F(RefPtrTest, LastReferenceGoingOutOfScopeDestroysTheObject)
{
	auto first = RefPtr<Base>::Create();
	RefPtr<Base> second = first;

	second = RefPtr<Base>();
	EXPECT_EQ(0, Counters::destroyed);

	first = RefPtr<Base>();
	EXPECT_EQ(1, Counters::destroyed);
}

TEST_F(RefPtrTest, MoveDoesNotIncrementRefCount)
{
	auto first = RefPtr<Base>::Create();
	Base* rawPtr = first.Get();

	RefPtr<Base> second = std::move(first);

	EXPECT_EQ(nullptr, first.Get());
	EXPECT_EQ(rawPtr, second.Get());
	EXPECT_EQ(0, Counters::destroyed);
}

TEST_F(RefPtrTest, CopyAssignmentReplacesHeldObject)
{
	auto a = RefPtr<Base>::Create();
	auto b = RefPtr<Base>::Create();
	ASSERT_EQ(2, Counters::constructed);

	a = b;

	EXPECT_EQ(1, Counters::destroyed); // a's original object was released
	EXPECT_EQ(a.Get(), b.Get());
}

TEST_F(RefPtrTest, SelfAssignmentIsSafe)
{
	auto ptr = RefPtr<Base>::Create();
	Base* rawPtr = ptr.Get();

	ptr = ptr;

	EXPECT_EQ(rawPtr, ptr.Get());
	EXPECT_EQ(0, Counters::destroyed);
}

// -- Converting construction/assignment across the class hierarchy -----------
//
// RefPtr<T>::operator=(const RefPtr<T2>&) and operator=(RefPtr<T2>&&) used to
// call a lowercase, nonexistent `.swap()` (the real method is `Swap()`), and
// the move overload called bare `move()` instead of `std::move()` - neither
// had ever been instantiated before, so the bugs were invisible until these
// tests exercised them.

TEST_F(RefPtrTest, ConvertingCopyConstructorSharesOwnership)
{
	auto derived = RefPtr<Derived>::Create();

	RefPtr<Base> base = derived;

	EXPECT_EQ(derived.Get(), base.Get());

	derived = RefPtr<Derived>();
	EXPECT_EQ(0, Counters::destroyed); // base still holds a reference

	base = RefPtr<Base>();
	EXPECT_EQ(1, Counters::destroyed);
}

TEST_F(RefPtrTest, ConvertingMoveConstructorTransfersOwnership)
{
	auto derived = RefPtr<Derived>::Create();
	Base* rawPtr = derived.Get();

	RefPtr<Base> base = std::move(derived);

	EXPECT_EQ(nullptr, derived.Get());
	EXPECT_EQ(rawPtr, base.Get());
}

TEST_F(RefPtrTest, ConvertingCopyAssignmentSharesOwnership)
{
	auto derived = RefPtr<Derived>::Create();
	RefPtr<Base> base;

	base = derived;

	EXPECT_EQ(derived.Get(), base.Get());

	derived = RefPtr<Derived>();
	EXPECT_EQ(0, Counters::destroyed);

	base = RefPtr<Base>();
	EXPECT_EQ(1, Counters::destroyed);
}

TEST_F(RefPtrTest, ConvertingMoveAssignmentTransfersOwnership)
{
	auto derived = RefPtr<Derived>::Create();
	Base* rawPtr = derived.Get();
	RefPtr<Base> base;

	base = std::move(derived);

	EXPECT_EQ(nullptr, derived.Get());
	EXPECT_EQ(rawPtr, base.Get());
}

// -- Comparison ------------------------------------------------------------------

TEST_F(RefPtrTest, EqualityComparesUnderlyingPointer)
{
	auto a = RefPtr<Base>::Create();
	RefPtr<Base> aAlias = a;
	auto b = RefPtr<Base>::Create();

	EXPECT_TRUE(a == aAlias);
	EXPECT_FALSE(a == b);
	EXPECT_TRUE(a != b);
}

// -- As<T>() / Is<T>() casting ----------------------------------------------------

TEST_F(RefPtrTest, AsSucceedsForTheActualDynamicType)
{
	RefPtr<Base> ptr = RefPtr<Derived>::Create();

	auto handle = ptr->As<Derived>();

	ASSERT_TRUE(bool(handle));
	EXPECT_EQ(2, handle->Tag());
}

TEST_F(RefPtrTest, AsFailsForAnUnrelatedType)
{
	RefPtr<Base> ptr = RefPtr<Derived>::Create();

	auto handle = ptr->As<Unrelated>();

	EXPECT_FALSE(bool(handle));
	EXPECT_EQ(nullptr, handle.get());
}

TEST_F(RefPtrTest, AsOnBaseObjectItselfIsAnIdentityCast)
{
	auto ptr = RefPtr<Base>::Create();

	auto handle = ptr->As<BaseObject>();

	EXPECT_EQ(static_cast<BaseObject*>(ptr.Get()), handle.get());
}

TEST_F(RefPtrTest, IsReflectsTheActualDynamicType)
{
	RefPtr<Base> ptr = RefPtr<Derived>::Create();

	EXPECT_TRUE(ptr->Is<Derived>());
	EXPECT_TRUE(ptr->Is<Base>());
	EXPECT_FALSE(ptr->Is<Unrelated>());
}

TEST_F(RefPtrTest, AsOnNullptrIsSafeAndReturnsAFailedHandle)
{
	// BaseObject::As() explicitly checks `this == nullptr` before touching any
	// member data, so calling it through a null RefPtr is documented-safe
	// rather than a crash.
	RefPtr<Base> empty;

	auto handle = empty->As<Derived>();

	EXPECT_FALSE(bool(handle));
}

TEST_F(RefPtrTest, CastHandleConvertsBackToARefPtr)
{
	auto ptr = RefPtr<Base>::Create();

	CastHandle<Base> handle = ptr->As<Base>();
	RefPtr<Base> fromHandle = handle;

	EXPECT_EQ(ptr.Get(), fromHandle.Get());
}

TEST_F(RefPtrTest, CastHandleEqualityOperators)
{
	auto ptr = RefPtr<Base>::Create();
	auto handle = ptr->As<Base>();
	CastHandle<Base> nullHandle(nullptr);

	EXPECT_TRUE(handle == ptr);
	EXPECT_TRUE(ptr == handle);
	EXPECT_FALSE(handle == nullptr);
	EXPECT_TRUE(nullHandle == nullptr);
	EXPECT_TRUE(nullptr == nullHandle);
	EXPECT_TRUE(handle != nullHandle);
}
