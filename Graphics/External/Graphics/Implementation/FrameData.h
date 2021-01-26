#pragma once

namespace CainEngine {
namespace Graphics {
namespace Implementation {

struct CameraData
{
	matrix projection, inverseProjection;
	matrix view, inverseView;
	matrix viewProjection;
};

struct MeshData
{
	matrix globalMatrix;
	shared_ptr<Model> data;
};

struct FrameData
{
	std::vector<CameraData> cameras;
	std::vector<MeshData> meshes;

}; // class FrameData

}; // namespace Implementation
}; // namespace Graphics
}; // namespace CainEngine