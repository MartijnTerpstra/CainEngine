#pragma once

namespace CainEngine::Common {

template<typename T>
inline CastHandle<T> BaseObject::as()
{
	if(this == nullptr)
		return CastHandle<T>(nullptr);

	return as<T>(typename std::is_same<std::remove_cv_t<T>, BaseObject>::type());
}

template<typename T>
inline CastHandle<const T> BaseObject::as() const
{
	if(this == nullptr)
		return CastHandle<T>(nullptr);

	return as<const T>(typename std::is_same<std::remove_cv_t<T>, BaseObject>::type());
}

template<typename T>
inline CastHandle<T> BaseObject::as(std::true_type)
{
	return CastHandle<T>(this);
}

template<typename T>
inline CastHandle<T> BaseObject::as(std::false_type)
{
	return CastHandle<T>(reinterpret_cast<T*>(asImpl(::mst::hash_of64<std::remove_cv_t<T>>())));
}

template<typename T>
inline CastHandle<const T> BaseObject::as(std::true_type) const
{
	return CastHandle<const T>(this);
}

template<typename T>
inline CastHandle<const T> BaseObject::as(std::false_type) const
{
	return CastHandle<T>(reinterpret_cast<T*>(asImpl(::mst::hash_of64<std::remove_cv_t<T>>())));
}

template<typename T>
inline bool BaseObject::is() const
{
	return this != nullptr && asImpl(::mst::hash_of64<std::remove_cv_t<T>>()) != nullptr;
}

inline void Common::BaseObject::addRef()
{
	++m_refCount;
}

inline void Common::BaseObject::release()
{
	if(--m_refCount == 0)
	{
		delete this;
	}
}

template<typename T>
inline CastHandle<T>::CastHandle(T* ptr)
	: m_ptr(ptr)
{ }

template<typename T>
inline T* CastHandle<T>::operator->() const
{
	return m_ptr;
}

template<typename T>
inline T* CastHandle<T>::get() const
{
	return m_ptr;
}

template<typename T>
inline CastHandle<T>::operator bool() const
{
	return m_ptr != nullptr;
}

template<typename T>
inline CastHandle<T>::operator Common::RefPtr<T>() const
{
	return { m_ptr };
}

template<typename T>
inline bool operator==(const Common::RefPtr<T>& l, const CastHandle<T>& r)
{
	return l.get() == r.get();
}

template<typename T>
inline bool operator==(const CastHandle<T>& l, const Common::RefPtr<T>& r)
{
	return l.get() == r.get();
}

template<typename T>
inline bool operator==(const CastHandle<T>& l, const CastHandle<T>& r)
{
	return l.get() == r.get();
}

template<typename T>
inline bool operator==(const CastHandle<T>& l, ::std::nullptr_t)
{
	return l.get() == nullptr;
}

template<typename T>
inline bool operator==(::std::nullptr_t, const CastHandle<T>& r)
{
	return r.get() == nullptr;
}

template<typename T>
inline bool operator!=(const Common::RefPtr<T>& l, const CastHandle<T>& r)
{
	return l.get() != r.get();
}

template<typename T>
inline bool operator!=(const CastHandle<T>& l, const Common::RefPtr<T>& r)
{
	return l.get() != r.get();
}

template<typename T>
inline bool operator!=(const CastHandle<T>& l, const CastHandle<T>& r)
{
	return l.get() != r.get();
}

template<typename T>
inline bool operator!=(const CastHandle<T>& l, ::std::nullptr_t)
{
	return l.get() != nullptr;
}

template<typename T>
inline bool operator!=(::std::nullptr_t, const CastHandle<T>& r)
{
	return r.get() != nullptr;
}

} // namespace CainEngine::Common