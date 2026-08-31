#pragma once

namespace CainEngine {
namespace Platform {

struct Rect
{
	Rect() = default;
	Rect(int left, int top, int right, int bottom)
		: Left(left)
		, Top(top)
		, Right(right)
		, Bottom(bottom)
	{ }
	Rect(const int2& position, const int2& size)
		: Left(position.x)
		, Top(position.y)
		, Right(position.x + size.x)
		, Bottom(position.y + size.y)
	{ }

	int getWidth() const
	{
		return Right - Left;
	}

	int getHeight() const
	{
		return Bottom - Top;
	}

	int2 getPosition() const
	{
		return int2(Left, Top);
	}

	int2 getSize() const
	{
		return int2(getWidth(), getHeight());
	}

	int Left, Top, Right, Bottom;

}; // struct Rect

}; // namespace Platform
}; // namespace CainEngine