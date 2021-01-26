#pragma once

namespace CainEngine::EntitySystem {

class Scene;
class CTransform;

class Transform final
{
	friend class Scene;
	friend class CTransform;
public:
	// ctor & dtor
	Transform() noexcept;
	~Transform();

public:
	// Main functionality

	[[nodiscard]] const float3& Position() const noexcept;

	void SetPosition(const float3& position);
	void Translate(const float3& translation);

	[[nodiscard]] bool UsingEulerAngles() const noexcept;
	[[nodiscard]] bool UsingQuaternion() const noexcept;

	[[nodiscard]] const quaternion& Quaternion() const noexcept;

	[[nodiscard]] degrees EulerX() const noexcept;
	[[nodiscard]] degrees EulerY() const noexcept;
	[[nodiscard]] degrees EulerZ() const noexcept;
	[[nodiscard]] euler_rotation_order RotationOrder() const noexcept;

	void SetQuaternion(const quaternion& quat);
	void SetEulerAngles(degrees x, degrees y, degrees z, euler_rotation_order order = euler_rotation_order::zxy);

	void Reset();

	[[nodiscard]] EntityID Entity() const noexcept;

	void CopyTo(Transform& other) const;

private:
	// Internal functionality

	Transform(Scene* scene, EntityID entity) noexcept;

private:
	// Internal functionality

	const Scene::EntityData& GetData() const noexcept;
	Scene::EntityData& GetData() noexcept;

private:
	// Member variables

	Scene* m_scene;
	EntityID m_entity;
};

class CTransform final
{
	friend class Scene;
public:
	// ctor & dtor
	CTransform() noexcept;
	~CTransform();

	CTransform(const Transform& other) noexcept;
	CTransform& operator = (const Transform& other) noexcept;

public:
	// Main functionality

	[[nodiscard]] const float3& Position() const noexcept;

	[[nodiscard]] bool UsingEulerAngles() const noexcept;
	[[nodiscard]] bool UsingQuaternion() const noexcept;

	[[nodiscard]] const quaternion& Quaternion() const noexcept;

	[[nodiscard]] degrees EulerX() const noexcept;
	[[nodiscard]] degrees EulerY() const noexcept;
	[[nodiscard]] degrees EulerZ() const noexcept;
	[[nodiscard]] euler_rotation_order RotationOrder() const noexcept;

	[[nodiscard]] EntityID Entity() const noexcept;

	void CopyTo(Transform& other) const;

private:
	// Internal functionality

	CTransform(const Scene* scene, EntityID entity) noexcept;

private:
	// Internal functionality

	const Scene::EntityData& GetData() const noexcept;

private:
	// Member variables

	const Scene* m_scene;
	EntityID m_entity;
};

}