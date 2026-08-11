#pragma once

namespace CainEngine::EntitySystem {

template<typename Func>
void Scene::ForEach(Func func) noexcept
{
	if (m_entityCount == 0) [[unlikely]]
		return;

	auto index = m_entitySkips[0];
	while (index != m_entityCount)
	{
		auto& e = m_entities[index];

		EntityID id{ index, e.version };

		if constexpr (std::is_invocable_v<Func, EntityID, Transform>)
		{
			std::invoke(func, id, Transform{ this, id });
		}
		else if constexpr (std::is_invocable_v<Func, EntityID, std::string_view>)
		{
			std::invoke(func, id, e.name);
		}
		else if constexpr (std::is_invocable_v<Func, EntityID, Transform, std::string_view>)
		{
			std::invoke(func, id, Transform{ this, id }, e.name);
		}
		else if constexpr (std::is_invocable_v<Func, EntityID, std::string_view, Transform>)
		{
			std::invoke(func, id, e.name, Transform{ this, id });
		}
		else // if constexpr (std::is_invocable_v<func, EntityID>)
		{
			std::invoke(func, id);
		}

		index = index + m_entitySkips[index + 1] + 1;
	}
}

template<typename Func, typename Arg>
void Scene::ForEach(Func func, Arg&& arg) noexcept
{
	if (m_entityCount == 0) [[unlikely]]
		return;

	auto index = m_entitySkips[0];
	while (index != m_entityCount)
	{
		auto& e = m_entities[index];

		EntityID id{ index, e.version };

		if constexpr (std::is_invocable_v<Func, Arg, EntityID, Transform>)
		{
			std::invoke(func, std::forward<Arg>(arg), id, Transform{ this, id });
		}
		else if constexpr (std::is_invocable_v<Func, Arg, EntityID, std::string_view>)
		{
			std::invoke(func, std::forward<Arg>(arg), id, e.name);
		}
		else if constexpr (std::is_invocable_v<Func, Arg, EntityID, Transform, std::string_view>)
		{
			std::invoke(func, std::forward<Arg>(arg), id, Transform{ this, id }, e.name);
		}
		else if constexpr (std::is_invocable_v<Func, Arg, EntityID, std::string_view, Transform>)
		{
			std::invoke(func, std::forward<Arg>(arg), id, e.name, Transform{ this, id });
		}
		else // if constexpr (std::is_invocable_v<Func, EntityID>)
		{
			std::invoke(func, std::forward<Arg>(arg), id);
		}

		index = index + m_entitySkips[index + 1] + 1;
	}
}

template<typename Func>
void Scene::ForEach(Func func) const noexcept
{
	if (m_entityCount == 0) [[unlikely]]
		return;

	auto index = m_entitySkips[0];
	while (index != m_entityCount)
	{
		auto& e = m_entities[index];

		EntityID id{ index, e.version };

		if constexpr (std::is_invocable_v<Func, EntityID, CTransform>)
		{
			std::invoke(func, id, CTransform{ this, id });
		}
		else if constexpr (std::is_invocable_v<Func, EntityID, std::string_view>)
		{
			std::invoke(func, id, e.name);
		}
		else if constexpr (std::is_invocable_v<Func, EntityID, CTransform, std::string_view>)
		{
			std::invoke(func, id, CTransform{ this, id }, e.name);
		}
		else if constexpr (std::is_invocable_v<Func, EntityID, std::string_view, CTransform>)
		{
			std::invoke(func, id, e.name, CTransform{ this, id });
		}
		else // if constexpr (std::is_invocable_v<func, EntityID>)
		{
			std::invoke(func, id);
		}

		index = index + m_entitySkips[index + 1] + 1;
	}
}

template<typename Func, typename Arg>
void Scene::ForEach(Func func, Arg&& arg) const noexcept
{
	if (m_entityCount == 0) [[unlikely]]
		return;

	auto index = m_entitySkips[0];
	while (index != m_entityCount)
	{
		auto& e = m_entities[index];

		EntityID id{ index, e.version };

		if constexpr (std::is_invocable_v<Func, Arg, EntityID, CTransform>)
		{
			std::invoke(func, std::forward<Arg>(arg), id, CTransform{ this, id });
		}
		else if constexpr (std::is_invocable_v<Func, Arg, EntityID, std::string_view>)
		{
			std::invoke(func, std::forward<Arg>(arg), id, e.name);
		}
		else if constexpr (std::is_invocable_v<Func, Arg, EntityID, CTransform, std::string_view>)
		{
			std::invoke(func, std::forward<Arg>(arg), id, CTransform{ this, id }, e.name);
		}
		else if constexpr (std::is_invocable_v<Func, Arg, EntityID, std::string_view, CTransform>)
		{
			std::invoke(func, std::forward<Arg>(arg), id, e.name, CTransform{ this, id });
		}
		else // if constexpr (std::is_invocable_v<Func, Arg, EntityID>)
		{
			std::invoke(func, std::forward<Arg>(arg), id);
		}

		index = index + m_entitySkips[index + 1] + 1;
	}
}

};