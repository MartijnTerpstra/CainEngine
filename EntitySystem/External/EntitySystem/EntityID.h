#pragma once

namespace CainEngine::EntitySystem {

class EntityID
{
public:

	EntityID() noexcept;
	explicit EntityID(uint32_t value) noexcept;
	explicit EntityID(int32_t index, uint16_t version) noexcept;

	int32_t Index() const noexcept;

	uint16_t Version() const noexcept;

	uint32_t Value() const noexcept;

	bool IsNull() const noexcept;

	friend bool operator == (EntityID left, EntityID right) noexcept;

	friend bool operator != (EntityID left, EntityID right) noexcept;

	explicit operator bool() noexcept;

	static const EntityID Null;

private:
	uint32_t m_value;
};

}