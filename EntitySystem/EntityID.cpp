#include "Precomp.h"

using namespace ::CainEngine;
using namespace ::CainEngine::EntitySystem;

const EntityID EntityID::Null = EntityID{ UINT32_MAX };

EntityID::EntityID() noexcept
	: m_value(UINT32_MAX)
{
}

EntityID::EntityID(uint32_t value) noexcept
	: m_value(value)
{
}

EntityID::EntityID(int32_t index, uint16_t version) noexcept
	: m_value(index | ((uint32_t)version << 20))
{
	COMMON_ASSERT(index < 0xFFFFFU);
	COMMON_ASSERT(version < 0xFFFU);
}

int32_t EntityID::index() const noexcept
{
	return m_value & 0xFFFFFU;
}

uint16_t EntityID::version() const noexcept
{
	return uint16_t((m_value >> 20) & 0xFFFU);
}

uint32_t EntityID::value() const noexcept
{
	return m_value;
}

bool EntityID::isNull() const noexcept
{
	return m_value == UINT32_MAX;
}

EntityID::operator bool() noexcept
{
	return !isNull();
}

bool CainEngine::EntitySystem::operator == (EntityID left, EntityID right) noexcept
{
	return left.m_value == right.m_value;
}

bool CainEngine::EntitySystem::operator != (EntityID left, EntityID right) noexcept
{
	return !(left == right);
}