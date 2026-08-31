#pragma once

namespace CainEngine::EntitySystem {

class Scene;

// TransformImpl is the single implementation behind both Transform (IsConst = false, a mutable
// view into an entity's transform data) and CTransform (IsConst = true, a read-only view that
// can also be obtained from a `const Scene&`). Mutating members are removed from CTransform via
// `requires (!IsConst)`, so the two views can never drift apart the way a hand-duplicated pair
// would.
template<bool IsConst>
class TransformImpl final
{
	template<bool>
	friend class TransformImpl;
	friend class Scene;

	using SceneT = std::conditional_t<IsConst, const Scene, Scene>;

public:
	// ctor & dtor
	TransformImpl() noexcept;
	~TransformImpl();

	// Implicit conversion/assignment from the mutable view to the const view.
	TransformImpl(const TransformImpl<false>& other) noexcept
		requires(IsConst);
	TransformImpl& operator=(const TransformImpl<false>& other) noexcept
		requires(IsConst);

public:
	// Main functionality

	[[nodiscard]] const float3& position() const noexcept;

	void setPosition(const float3& position)
		requires(!IsConst);
	void translate(const float3& translation)
		requires(!IsConst);

	[[nodiscard]] bool usingEulerAngles() const noexcept;
	[[nodiscard]] bool usingQuaternion() const noexcept;

	// Named rotation() rather than quaternion() because a member function
	// with the exact same spelling as its own return type self-shadows that
	// type within class scope on some compilers.
	[[nodiscard]] const quaternion& rotation() const noexcept;

	[[nodiscard]] degrees eulerX() const noexcept;
	[[nodiscard]] degrees eulerY() const noexcept;
	[[nodiscard]] degrees eulerZ() const noexcept;
	[[nodiscard]] euler_rotation_order rotationOrder() const noexcept;

	void setQuaternion(const quaternion& quat)
		requires(!IsConst);
	void setEulerAngles(
		degrees x, degrees y, degrees z, euler_rotation_order order = euler_rotation_order::zxy)
		requires(!IsConst);

	void reset()
		requires(!IsConst);

	[[nodiscard]] EntityID entity() const noexcept;

	void copyTo(TransformImpl<false>& other) const;

private:
	// Internal functionality

	TransformImpl(SceneT* scene, EntityID entity) noexcept;

	const Scene::EntityData& getData() const noexcept;
	Scene::EntityData& getData() noexcept
		requires(!IsConst);

private:
	// Member variables

	SceneT* m_scene;
	EntityID m_entity;
};

extern template class TransformImpl<false>;
extern template class TransformImpl<true>;

} // namespace CainEngine::EntitySystem
