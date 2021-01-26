#pragma once

template<typename T>
inline ::Common::CastHandle<T>(::Common::BaseObject::As)()
{
	if (this == nullptr)
		return CastHandle<T>(nullptr);

	return As<T>(typename std::is_same<std::remove_cv_t<T>, BaseObject>::type());
}

template<typename T>
inline ::Common::CastHandle<const T>(::Common::BaseObject::As)() const
{
	if (this == nullptr)
		return CastHandle<T>(nullptr);

	return As<const T>(typename std::is_same<std::remove_cv_t<T>, BaseObject>::type());
}

template<typename T>
inline ::Common::CastHandle<T>(::Common::BaseObject::As)(std::true_type)
{
	return CastHandle<T>(this);
}

template<typename T>
inline ::Common::CastHandle<T>(::Common::BaseObject::As)(std::false_type)
{
	return CastHandle<T>(reinterpret_cast<T*>(_As(::mst::hash_of64<std::remove_cv_t<T>>())));
}

template<typename T>
inline ::Common::CastHandle<const T>(::Common::BaseObject::As)(std::true_type) const
{
	return CastHandle<const T>(this);
}

template<typename T>
inline ::Common::CastHandle<const T>(::Common::BaseObject::As)(std::false_type) const
{
	return CastHandle<T>(reinterpret_cast<T*>(_As(::mst::hash_of64<std::remove_cv_t<T>>())));
}

template<typename T>
inline bool ::Common::BaseObject::Is() const
{
	return this != nullptr && _As(::mst::hash_of64<std::remove_cv_t<T>>()) != nullptr;
}

inline void Common::BaseObject::AddRef()
{
	++m_refCount;
}

inline void Common::BaseObject::Release()
{
	if (--m_refCount == 0)
	{
		delete this;
	}
}

template<typename T>
inline ::Common::CastHandle<T>::CastHandle(T* ptr)
	: m_ptr(ptr)
{
}

template<typename T>
inline T* ::Common::CastHandle<T>::operator->() const
{
	return m_ptr;
}

template<typename T>
inline T* ::Common::CastHandle<T>::get() const
{
	return m_ptr;
}

template<typename T>
inline ::Common::CastHandle<T>::operator bool() const
{
	return m_ptr != nullptr;
}

template<typename T>
inline ::Common::CastHandle<T>::operator Common::RefPtr<T>() const
{
	return { m_ptr };
}

template<typename T>
inline bool operator == (const Common::RefPtr<T>& l, const ::Common::CastHandle<T>& r)
{
	return l.get() == r.get();
}

template<typename T>
inline bool operator == (const ::Common::CastHandle<T>& l, const Common::RefPtr<T>& r)
{
	return l.get() == r.get();
}

template<typename T>
inline bool operator == (const ::Common::CastHandle<T>& l, const ::Common::CastHandle<T>& r)
{
	return l.get() == r.get();
}

template<typename T>
inline bool operator == (const ::Common::CastHandle<T>& l, ::std::nullptr_t)
{
	return l.get() == nullptr;
}

template<typename T>
inline bool operator == (::std::nullptr_t, const ::Common::CastHandle<T>& r)
{
	return r.get() == nullptr;
}

template<typename T>
inline bool operator != (const Common::RefPtr<T>& l, const ::Common::CastHandle<T>& r)
{
	return l.get() != r.get();
}

template<typename T>
inline bool operator != (const ::Common::CastHandle<T>& l, const Common::RefPtr<T>& r)
{
	return l.get() != r.get();
}

template<typename T>
inline bool operator != (const ::Common::CastHandle<T>& l, const ::Common::CastHandle<T>& r)
{
	return l.get() != r.get();
}

template<typename T>
inline bool operator != (const ::Common::CastHandle<T>& l, ::std::nullptr_t)
{
	return l.get() != nullptr;
}

template<typename T>
inline bool operator != (::std::nullptr_t, const ::Common::CastHandle<T>& r)
{
	return r.get() != nullptr;
}