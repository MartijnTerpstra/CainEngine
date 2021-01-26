#pragma once

template<typename T>
inline Common::RefPtr<T>::RefPtr() noexcept
	: m_ptr(nullptr)
{
}

template<typename T>
inline Common::RefPtr<T>::RefPtr(T* ptr) noexcept
	: m_ptr(ptr)
{
	AddRef();
}

template<typename T>
inline Common::RefPtr<T>::RefPtr(const RefPtr& other) noexcept
	: m_ptr(other.m_ptr)
{
	AddRef();
}

template<typename T>
inline Common::RefPtr<T>::RefPtr(RefPtr&& other) noexcept
	: m_ptr(other.m_ptr)
{
	other.m_ptr = nullptr;
}

template<typename T>
inline Common::RefPtr<T>::~RefPtr() noexcept
{
	Release();
}

template<typename T>
template<class _Ty2,
	std::enable_if_t<std::is_convertible_v<_Ty2*, T*>, int>>
	Common::RefPtr<T>::RefPtr(const RefPtr<_Ty2>& other) noexcept
	: m_ptr(other.m_ptr)
{
	AddRef();
}

template<typename T>
template<class _Ty2,
	std::enable_if_t<std::is_convertible_v<_Ty2*, T*>, int>>
	Common::RefPtr<T>::RefPtr(RefPtr<_Ty2>&& other) noexcept
	: m_ptr(other.m_ptr)
{
	other.m_ptr = nullptr;
}

template<typename T>
inline Common::RefPtr<T>& Common::RefPtr<T>::operator = (const RefPtr& other) noexcept
{
	Release();

	m_ptr = other.m_ptr;

	AddRef();

	return *this;
}

template<typename T>
template<typename T2>
inline Common::RefPtr<T>& Common::RefPtr<T>::operator = (const RefPtr<T2>& other) noexcept
{
	RefPtr(other).swap(*this);
	return (*this);
}

template<typename T>
inline Common::RefPtr<T>& Common::RefPtr<T>::operator = (RefPtr&& other) noexcept
{
	std::swap(m_ptr, other.m_ptr);

	return *this;
}

template<typename T>
template<typename T2>
inline Common::RefPtr<T>& Common::RefPtr<T>::operator = (RefPtr<T2>&& other) noexcept
{
	RefPtr(move(_Right)).swap(*this);
	return (*this);
}

template<typename T>
template<typename ...Args>
inline Common::RefPtr<T> Common::RefPtr<T>::Create(Args&& ...args)
{
	return RefPtr(InternalInit(), new T(std::forward<Args>(args)...));
}

template<typename T>
inline void Common::RefPtr<T>::Swap(RefPtr& other)
{
	std::swap(m_ptr, other.m_ptr);
}

template<typename T>
inline T* Common::RefPtr<T>::Get() const noexcept
{
	return m_ptr;
}

template<typename T>
inline T* Common::RefPtr<T>::operator -> () const noexcept
{
	return m_ptr;
}

template<typename T>
inline T& Common::RefPtr<T>::operator * () const noexcept
{
	return *m_ptr;
}

template<typename T>
inline Common::RefPtr<T>::operator bool () const noexcept
{
	return m_ptr != nullptr;
}

template<typename T>
inline Common::RefPtr<T>::RefPtr(InternalInit, T* ptr) noexcept
	: m_ptr(ptr)
{
}

template<typename T>
inline void Common::RefPtr<T>::AddRef() noexcept
{
	if (m_ptr)
	{
		m_ptr->AddRef();
	}
}

template<typename T>
inline void Common::RefPtr<T>::Release() noexcept
{
	if (m_ptr)
	{
		m_ptr->Release();
	}
}

// operators

template<typename T, typename T2>
inline bool operator == (const Common::RefPtr<T>& l, const Common::RefPtr<T2>& r)
{
	return l.Get() == r.Get();
}

template<typename T, typename T2>
inline bool operator != (const Common::RefPtr<T>& l, const Common::RefPtr<T2>& r)
{
	return !(l == r);
}