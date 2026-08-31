#include "Precomp.h"

#include "Event.h"

using namespace ::CainEngine::Graphics;
using namespace ::CainEngine::Graphics::DX12;

Event::Event()
	: m_event(CreateEventW(nullptr, FALSE, FALSE, nullptr))
	, m_fenceValue(0)
{
	COMMON_CALLSTACK_CALL;
}

Event::~Event()
{
	COMMON_CALLSTACK_CALL;

	m_fence.reset();

	if(CloseHandle(m_event) == FALSE)
		Common::fatalError("CloseHandle failed");
}

void Event::init(ID3D12Device* device)
{
	CHECK_HRESULT(device->CreateFence(0, D3D12_FENCE_FLAG_NONE, MST_IID_PPV_ARGS(m_fence)));
}

void Event::waitTillCompletion() const
{
	COMMON_CALLSTACK_CALL;

	if(m_fence->GetCompletedValue() < m_fenceValue)
	{
		CHECK_HRESULT(m_fence->SetEventOnCompletion(m_fenceValue, m_event));

		WaitForSingleObject(m_event, INFINITE);
	}
}

bool Event::isCompleted() const
{
	COMMON_CALLSTACK_CALL;

	return m_fence->GetCompletedValue() >= m_fenceValue;
}

void Event::signalFence(ID3D12CommandQueue* queue)
{
	COMMON_CALLSTACK_CALL;

	CHECK_HRESULT(queue->Signal(m_fence.get(), ++m_fenceValue));
}