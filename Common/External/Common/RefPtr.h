#pragma once

namespace Common {

template<typename T>
class RefPtr
{
	static_assert(std::is_base_of_v<BaseObject, T>, "T must derive from BaseObject");

	template<typename T2>
	friend class RefPtr;

public:
	// ctor & dtor

	inline RefPtr() noexcept;
	inline RefPtr(T* ptr) noexcept;
	inline RefPtr(const RefPtr& other) noexcept;
	inline RefPtr(RefPtr&& other) noexcept;
	inline ~RefPtr() noexcept;

	template<class _Ty2,
		std::enable_if_t<std::is_convertible_v<_Ty2*, T*>, int> = 0>
		RefPtr(const RefPtr<_Ty2>& _Other) noexcept;

	template<class _Ty2,
		std::enable_if_t<std::is_convertible_v<_Ty2*, T*>, int> = 0>
		RefPtr(RefPtr<_Ty2>&& _Right) noexcept;

	RefPtr& operator = (const RefPtr& other) noexcept;
	RefPtr& operator = (RefPtr&& other) noexcept;

	template<typename T2>
	RefPtr& operator = (const RefPtr<T2>& other) noexcept;
	template<typename T2>
	RefPtr& operator = (RefPtr<T2>&& other) noexcept;

public:
	// Main functionality

	template<typename... Args>
	inline static RefPtr Create(Args&&... args);

	void Swap(RefPtr& other);

	inline T* Get() const noexcept;

	inline T* operator -> () const noexcept;

	inline T& operator * () const noexcept;

	inline explicit operator bool() const noexcept;

private:
	// Internal functionality

	struct InternalInit {};
	inline RefPtr(InternalInit, T* ptr) noexcept;

	inline void AddRef() noexcept;
	inline void Release() noexcept;

private:
	// Member variable

	T* m_ptr;

}; // class RefPtr<T>

}; // namespace Common