#pragma once

namespace CainEngine {
namespace Graphics {
namespace DX12 {

class Event final
{
public:
	// ctor & dtor

	Event();
	~Event();

public:
	// Main functionality

	void init(ID3D12Device* device);

	void waitTillCompletion() const;
	bool isCompleted() const;
	void signalFence(ID3D12CommandQueue* queue);

private:
	// Internal functionality

private:
	// Member variables

	com_ptr<ID3D12Fence> m_fence;
	HANDLE m_event;
	UINT64 m_fenceValue;

}; // class Event

}; // namespace DX12
}; // namespace Graphics
}; // namespace CainEngine