#pragma once

namespace CainEngine {
namespace Platform {
namespace Internal {

class EnumConverter
{
public:
	// Main functionality

	static KeyCodes toKeyCodes(WPARAM wParam);

}; // class EnumConverter

}; // namespace Internal
}; // namespace Platform
}; // namespace CainEngine