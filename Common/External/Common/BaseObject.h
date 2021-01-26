#pragma once

#define CHECK_TYPE_AND_RETURN(Type) case mst::hash_of64<Type>(): return static_cast<Type*>(const_cast<std::remove_cv_t<std::remove_reference_t<decltype(*this)>>*>(this))

namespace Common {

class BaseObject
{
	template<typename T>
	friend class RefPtr;
protected:
	// ctor & dtor

	inline BaseObject() = default;
	inline virtual ~BaseObject();

public:

	/**
		Summary:
			Tries to cast to a different type

		Template params:
			- T: the type you are trying to cast to

		Returns:
			null on a failure, a valid pointer on success
	*/
	template<typename T>
	inline CastHandle<T> As();

	/**
		Summary:
			Tries to cast to a different type

		Template params:
			- T: the type you are trying to cast to

		Returns:
			null on a failure, a valid pointer on success
	*/
	template<typename T>
	inline CastHandle<const T> As() const;

	/**
		Summary:
			Returns whether the object implements the object/interface type
	*/
	template<typename T>
	inline bool Is() const;

private:

	template<typename T>
	inline CastHandle<const T> As(std::true_type) const;

	template<typename T>
	inline CastHandle<const T> As(std::false_type) const;

	template<typename T>
	inline CastHandle<T> As(std::true_type);

	template<typename T>
	inline CastHandle<T> As(std::false_type);

	inline void AddRef();
	inline void Release();

	virtual void* _As(uint64_t) const = 0;

protected:

	std::atomic_uint32_t m_refCount;
};

inline BaseObject::~BaseObject() = default;

/**
	Summary:
		Make casting possible without needing a reference increase every time a cast happens
*/
template<typename T>
class CastHandle
{
	static_assert(std::is_base_of_v<BaseObject, T>, "T must derive from BaseObject");
public:
	inline CastHandle(T* ptr);

	inline T* operator -> () const;

	inline T* get() const;

	inline explicit operator bool() const;

	inline operator RefPtr<T>() const;

private:
	// Member variables
	T* const m_ptr;

}; // class CastHandle<T>

}; // namespace Common