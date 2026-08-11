#pragma once

namespace CainEngine::Common {

template<typename T>
inline RefPtr<T>::RefPtr() noexcept
	: m_ptr(nullptr)
{ }

template<typename T>
inline RefPtr<T>::RefPtr(T* ptr) noexcept
	: m_ptr(ptr)
{
	AddRef();
}

template<typename T>
inline RefPtr<T>::RefPtr(const RefPtr& other) noexcept
	: m_ptr(other.m_ptr)
{
	AddRef();
}

template<typename T>
inline RefPtr<T>::RefPtr(RefPtr&& other) noexcept
	: m_ptr(other.m_ptr)
{
	other.m_ptr = nullptr;
}

template<typename T>
inline RefPtr<T>::~RefPtr() noexcept
{
	Release();
}

template<typename T>
template<class _Ty2, std::enable_if_t<std::is_convertible_v<_Ty2*, T*>, int>>
RefPtr<T>::RefPtr(const RefPtr<_Ty2>& other) noexcept
	: m_ptr(other.m_ptr)
{
	AddRef();
}

template<typename T>
template<class _Ty2, std::enable_if_t<std::is_convertible_v<_Ty2*, T*>, int>>
RefPtr<T>::RefPtr(RefPtr<_Ty2>&& other) noexcept
	: m_ptr(other.m_ptr)
{
	other.m_ptr = nullptr;
}

template<typename T>
inline RefPtr<T>& RefPtr<T>::operator=(const RefPtr& other) noexcept
{
	// Copy-and-swap: AddRef the new value before releasing the old one, so
	// self-assignment (ptr = ptr) can't drop the refcount to zero and delete
	// the object out from under itself before the reference is re-added.
	RefPtr(other).Swap(*this);

	return *this;
}

template<typename T>
template<typename T2>
inline RefPtr<T>& RefPtr<T>::operator=(const RefPtr<T2>& other) noexcept
{
	RefPtr(other).Swap(*this);
	return (*this);
}

template<typename T>
inline RefPtr<T>& RefPtr<T>::operator=(RefPtr&& other) noexcept
{
	std::swap(m_ptr, other.m_ptr);

	return *this;
}

template<typename T>
template<typename T2>
inline RefPtr<T>& RefPtr<T>::operator=(RefPtr<T2>&& other) noexcept
{
	RefPtr(std::move(other)).Swap(*this);
	return (*this);
}

template<typename T>
template<typename... Args>
inline RefPtr<T> RefPtr<T>::Create(Args&&... args)
{
	return RefPtr(InternalInit(), new T(std::forward<Args>(args)...));
}

template<typename T>
inline void RefPtr<T>::Swap(RefPtr& other)
{
	std::swap(m_ptr, other.m_ptr);
}

template<typename T>
inline T* RefPtr<T>::Get() const noexcept
{
	return m_ptr;
}

template<typename T>
inline T* RefPtr<T>::operator->() const noexcept
{
	return m_ptr;
}

template<typename T>
inline T& RefPtr<T>::operator*() const noexcept
{
	return *m_ptr;
}

template<typename T>
inline RefPtr<T>::operator bool() const noexcept
{
	return m_ptr != nullptr;
}

template<typename T>
inline RefPtr<T>::RefPtr(InternalInit, T* ptr) noexcept
	: m_ptr(ptr)
{
	AddRef();
}

template<typename T>
inline void RefPtr<T>::AddRef() noexcept
{
	if(m_ptr)
	{
		m_ptr->AddRef();
	}
}

template<typename T>
inline void RefPtr<T>::Release() noexcept
{
	if(m_ptr)
	{
		m_ptr->Release();
	}
}

// operators

template<typename T, typename T2>
inline bool operator==(const RefPtr<T>& l, const RefPtr<T2>& r)
{
	return l.Get() == r.Get();
}

template<typename T, typename T2>
inline bool operator!=(const RefPtr<T>& l, const RefPtr<T2>& r)
{
	return !(l == r);
}

} // namespace CainEngine::Common