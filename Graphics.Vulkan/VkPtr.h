#pragma once

namespace CainEngine::Graphics::Vulkan {

template<typename T>
class VkPtr;

template<typename T, typename MyDerived>
class VkWeakPtr;

template<typename T>
class VkRefBlock
{
public:
	// ctor & dtor

	VkRefBlock(T ptr, shared_ptr<const APIFunctions> api, void(*onDestroyFunc)(T, const shared_ptr<const APIFunctions>&))
		: m_pointer(ptr), API(move(api)), m_onDestroyFunc(onDestroyFunc)
	{
		COMMON_CALLSTACK_CALL;
	}

	~VkRefBlock()
	{
		COMMON_CALLSTACK_CALL;

		if (m_pointer)
			m_onDestroyFunc(m_pointer, API);
	}

public:
	// Main functionality

	T Get() const
	{
		return m_pointer;
	}

	const T& GetRef() const
	{
		return m_pointer;
	}

	T& GetCreateRef()
	{
		return m_pointer;
	}

	const shared_ptr<const APIFunctions>& GetAPI() const
	{
		return API;
	}

	T Release()
	{
		return m_pointer->Release();
	}

protected:
	// Member variables

	T m_pointer;
	const shared_ptr<const APIFunctions> API;
	void(*const m_onDestroyFunc)(T, const shared_ptr<const APIFunctions>&);

}; // class VkRefBlock<T>

template<typename T>
class VkPtr
{
	template<typename T, typename Derived>
	friend class VkWeakPtr;
protected:
	// ctor & dtor

	VkPtr()
		: m_block(null)
	{
	}

	VkPtr(const shared_ptr<VkRefBlock<T>>& block) noexcept
		: m_block(block)
	{
	}

	VkPtr(shared_ptr<VkRefBlock<T>>&& block) noexcept
		: m_block(move(block))
	{
	}

public:
	// Main functionality

	T Get() const
	{
		if (m_block)
			return m_block->Get();

		return (T)VK_NULL_HANDLE;
	}

	const T* operator & () const
	{
		return &m_block->GetRef();
	}

	explicit operator bool() const
	{
		return Get() != (T)VK_NULL_HANDLE;
	}

	const shared_ptr<const APIFunctions>& GetAPI() const
	{
		return m_block->GetAPI();
	}

	T Release()
	{
		if (m_block)
		{
			T released = m_block->Get();

			m_block->GetCreateRef() = (T)VK_NULL_HANDLE;
			m_block = null;
			return released;
		}

		return (T)VK_NULL_HANDLE;
	}

protected:
	// Member variables

	shared_ptr<VkRefBlock<T>> m_block;

}; // class VkPtr<T>

template<typename T, typename MyDerived>
class VkWeakPtr
{
protected:
	// ctor & dtor

	VkWeakPtr()
		: m_block()
	{
	}

	VkWeakPtr(const shared_ptr<VkRefBlock<T>>& block) noexcept
		: m_block(block)
	{
	}

	VkWeakPtr(shared_ptr<VkRefBlock<T>>&& block) noexcept
		: m_block(move(block))
	{
	}

	VkWeakPtr(const weak_ptr<VkRefBlock<T>>& block)
		: m_block(block)
	{
	}

	VkWeakPtr(weak_ptr<VkRefBlock<T>>&& block)
		: m_block(move(block))
	{
	}

	VkWeakPtr& operator = (const MyDerived& other)
	{
		m_block = other.m_block;

		return *this;
	}

public:
	// Main functionality

	MyDerived Lock() const
	{
		return MyDerived(m_block.lock());
	}

	bool Expired() const
	{
		return m_block.expired();
	}

	const shared_ptr<const APIFunctions>& GetAPI() const
	{
		return m_block->GetAPI();
	}

protected:
	// Member variables

	weak_ptr<VkRefBlock<T>> m_block;

}; // class VkPtr<T>

#define GRAPHICS_VULKAN_BEGIN_VKPTR(vkType) \
class vkType##WeakPtr; \
class vkType##Ptr : public VkPtr<vkType> \
{ \
	friend class VkPtr<vkType>; \
	friend class VkWeakPtr<vkType, vkType##Ptr>; \
	friend class vkType##WeakPtr; \
	 \
	typedef VkPtr<vkType> Base; \
	 \
public:\
	\
	vkType##Ptr() \
	{ \
	} \
	 \
	vkType##Ptr(const vkType##Ptr& other) noexcept \
		: Base(other.m_block) \
	{ \
	} \
	 \
	vkType##Ptr(vkType##Ptr&& other) noexcept \
		: Base(move(other.m_block)) \
	{ \
	} \
	 \
	vkType##Ptr& operator = (const vkType##Ptr& other) \
	{ \
		m_block = other.m_block; \
		return *this; \
	} \
	 \
	vkType##Ptr& operator = (vkType##Ptr&& other) \
	{ \
		m_block = move(other.m_block); \
		return *this; \
	} \
	 \
	vkType* Create(shared_ptr<const APIFunctions> api) \
	{ \
		m_block = make_shared<VkRefBlock<vkType>>((vkType)VK_NULL_HANDLE, move(api), &DestroyVkObject); \
		 \
		return &m_block->GetCreateRef(); \
	} \
	 \
	void Adopt(vkType ptr, shared_ptr<const APIFunctions> api) \
	{ \
		m_block = make_shared<VkRefBlock<vkType>>(ptr, move(api), &DestroyVkObject); \
	} \
	 \
private: \
	 \
	vkType##Ptr(shared_ptr<VkRefBlock<vkType>>&& refBlock) \
		:  Base(move(refBlock))	\
	{ \
	} \
	 \
	 \
private: \
	 \
	static void DestroyVkObject(vkType ptr, const shared_ptr<const APIFunctions>& api) {

#define GRAPHICS_VULKAN_END_VKPTR(vkType) \
 } }; \
 \
class vkType##WeakPtr : public VkWeakPtr<vkType, vkType##Ptr> \
{ \
	friend class VkPtr<vkType>; \
	friend class VkWeakPtr<vkType, vkType##Ptr>; \
	 \
	typedef VkWeakPtr<vkType, vkType##Ptr> Base; \
	 \
public:\
	\
	vkType##WeakPtr() \
	{ \
	} \
	 \
	vkType##WeakPtr(const vkType##Ptr& other) noexcept \
		: Base(other.m_block) \
	{ \
	} \
	 \
	vkType##WeakPtr(vkType##Ptr&& other) noexcept \
		: Base(move(other.m_block)) \
	{ \
	} \
	 \
	vkType##WeakPtr(const vkType##WeakPtr& other) \
		: Base(other.m_block) \
	{ \
	} \
	 \
	vkType##WeakPtr(vkType##WeakPtr&& other) \
		: Base(move(other.m_block)) \
	{ \
	} \
	 \
	vkType##WeakPtr& operator = (const vkType##Ptr& other) \
	{ \
		m_block = other.m_block; \
		return *this; \
	} \
	 \
	vkType##WeakPtr& operator = (vkType##Ptr&& other) \
	{ \
		m_block = move(other.m_block); \
		return *this; \
	} \
	 \
	 \
};

GRAPHICS_VULKAN_BEGIN_VKPTR(VkRenderPass)
vkDestroyRenderPass(api->Device, ptr, null);
GRAPHICS_VULKAN_END_VKPTR(VkRenderPass)

GRAPHICS_VULKAN_BEGIN_VKPTR(VkSwapchainKHR)
vkDestroySwapchainKHR(api->Device, ptr, null);
GRAPHICS_VULKAN_END_VKPTR(VkSwapchainKHR)

GRAPHICS_VULKAN_BEGIN_VKPTR(VkSurfaceKHR)
vkDestroySurfaceKHR(api->Instance, ptr, null);
GRAPHICS_VULKAN_END_VKPTR(VkSurfaceKHR)

GRAPHICS_VULKAN_BEGIN_VKPTR(VkImage)
vkDestroyImage(api->Device, ptr, null);
GRAPHICS_VULKAN_END_VKPTR(VkImage)

GRAPHICS_VULKAN_BEGIN_VKPTR(VkImageView)
vkDestroyImageView(api->Device, ptr, null);
GRAPHICS_VULKAN_END_VKPTR(VkImageView)

GRAPHICS_VULKAN_BEGIN_VKPTR(VkBuffer)
vkDestroyBuffer(api->Device, ptr, null);
GRAPHICS_VULKAN_END_VKPTR(VkBuffer)

GRAPHICS_VULKAN_BEGIN_VKPTR(VkBufferView)
vkDestroyBufferView(api->Device, ptr, null);
GRAPHICS_VULKAN_END_VKPTR(VkBufferView)

GRAPHICS_VULKAN_BEGIN_VKPTR(VkPipelineLayout)
vkDestroyPipelineLayout(api->Device, ptr, null);
GRAPHICS_VULKAN_END_VKPTR(VkPipelineLayout)

GRAPHICS_VULKAN_BEGIN_VKPTR(VkPipeline)
vkDestroyPipeline(api->Device, ptr, null);
GRAPHICS_VULKAN_END_VKPTR(VkPipeline)

GRAPHICS_VULKAN_BEGIN_VKPTR(VkPipelineCache)
vkDestroyPipelineCache(api->Device, ptr, null);
GRAPHICS_VULKAN_END_VKPTR(VkPipelineCache)

GRAPHICS_VULKAN_BEGIN_VKPTR(VkDescriptorSetLayout)
vkDestroyDescriptorSetLayout(api->Device, ptr, null);
GRAPHICS_VULKAN_END_VKPTR(VkDescriptorSetLayout)

GRAPHICS_VULKAN_BEGIN_VKPTR(VkShaderModule)
vkDestroyShaderModule(api->Device, ptr, null);
GRAPHICS_VULKAN_END_VKPTR(VkShaderModule)

GRAPHICS_VULKAN_BEGIN_VKPTR(VkDescriptorPool)
vkDestroyDescriptorPool(api->Device, ptr, null);
GRAPHICS_VULKAN_END_VKPTR(VkDescriptorPool)

GRAPHICS_VULKAN_BEGIN_VKPTR(VkFramebuffer)
vkDestroyFramebuffer(api->Device, ptr, null);
GRAPHICS_VULKAN_END_VKPTR(VkFramebuffer)

GRAPHICS_VULKAN_BEGIN_VKPTR(VkCommandPool)
vkDestroyCommandPool(api->Device, ptr, null);
GRAPHICS_VULKAN_END_VKPTR(VkCommandPool)

GRAPHICS_VULKAN_BEGIN_VKPTR(VkFence)
vkDestroyFence(api->Device, ptr, null);
GRAPHICS_VULKAN_END_VKPTR(VkFence)

GRAPHICS_VULKAN_BEGIN_VKPTR(VkSemaphore)
vkDestroySemaphore(api->Device, ptr, null);
GRAPHICS_VULKAN_END_VKPTR(VkSemaphore)

GRAPHICS_VULKAN_BEGIN_VKPTR(VkDeviceMemory)
vkFreeMemory(api->Device, ptr, null);
GRAPHICS_VULKAN_END_VKPTR(VkDeviceMemory)

GRAPHICS_VULKAN_BEGIN_VKPTR(VkSampler)
vkDestroySampler(api->Device, ptr, null);
GRAPHICS_VULKAN_END_VKPTR(VkSampler)

};