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

	[[nodiscard]] const float3& Position() const noexcept;

	void SetPosition(const float3& position)
		requires(!IsConst);
	void Translate(const float3& translation)
		requires(!IsConst);

	[[nodiscard]] bool UsingEulerAngles() const noexcept;
	[[nodiscard]] bool UsingQuaternion() const noexcept;

	[[nodiscard]] const quaternion& Quaternion() const noexcept;

	[[nodiscard]] degrees EulerX() const noexcept;
	[[nodiscard]] degrees EulerY() const noexcept;
	[[nodiscard]] degrees EulerZ() const noexcept;
	[[nodiscard]] euler_rotation_order RotationOrder() const noexcept;

	void SetQuaternion(const quaternion& quat)
		requires(!IsConst);
	void SetEulerAngles(
		degrees x, degrees y, degrees z, euler_rotation_order order = euler_rotation_order::zxy)
		requires(!IsConst);

	void Reset()
		requires(!IsConst);

	[[nodiscard]] EntityID Entity() const noexcept;

	void CopyTo(TransformImpl<false>& other) const;

private:
	// Internal functionality

	TransformImpl(SceneT* scene, EntityID entity) noexcept;

	const Scene::EntityData& GetData() const noexcept;
	Scene::EntityData& GetData() noexcept
		requires(!IsConst);

private:
	// Member variables

	SceneT* m_scene;
	EntityID m_entity;
};

extern template class TransformImpl<false>;
extern template class TransformImpl<true>;

} // namespace CainEngine::EntitySystem
