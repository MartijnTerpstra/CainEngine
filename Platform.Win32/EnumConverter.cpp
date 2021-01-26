#include "Precomp.h"

#include "EnumConverter.h"

using namespace ::CainEngine::Platform;
using namespace ::CainEngine::Platform::Internal;

KeyCodes EnumConverter::ToKeyCodes(WPARAM wParam)
{
	COMMON_CALLSTACK_CALL;

	switch (wParam)
	{
	case 'A':
		return KeyCodes::A;
	case 'B':
		return KeyCodes::B;
	case 'C':
		return KeyCodes::C;
	case 'D':
		return KeyCodes::D;
	case 'E':
		return KeyCodes::E;
	case 'F':
		return KeyCodes::F;
	case 'G':
		return KeyCodes::G;
	case 'H':
		return KeyCodes::H;
	case 'I':
		return KeyCodes::I;
	case 'J':
		return KeyCodes::J;
	case 'K':
		return KeyCodes::K;
	case 'L':
		return KeyCodes::L;
	case 'M':
		return KeyCodes::M;
	case 'N':
		return KeyCodes::N;
	case 'O':
		return KeyCodes::O;
	case 'P':
		return KeyCodes::P;
	case 'Q':
		return KeyCodes::Q;
	case 'R':
		return KeyCodes::R;
	case 'S':
		return KeyCodes::S;
	case 'T':
		return KeyCodes::T;
	case 'U':
		return KeyCodes::U;
	case 'V':
		return KeyCodes::V;
	case 'W':
		return KeyCodes::W;
	case 'X':
		return KeyCodes::X;
	case 'Y':
		return KeyCodes::Y;
	case 'Z':
		return KeyCodes::Z;
	case '0':
		return KeyCodes::Zero;
	case '1':
		return KeyCodes::One;
	case '2':
		return KeyCodes::Two;
	case '3':
		return KeyCodes::Three;
	case '4':
		return KeyCodes::Four;
	case '5':
		return KeyCodes::Five;
	case '6':
		return KeyCodes::Six;
	case '7':
		return KeyCodes::Seven;
	case '8':
		return KeyCodes::Eight;
	case '9':
		return KeyCodes::Nine;
	case VK_UP:
		return KeyCodes::UpArrow;
	case VK_DOWN:
		return KeyCodes::DownArrow;
	case VK_LEFT:
		return KeyCodes::LeftArrow;
	case VK_RIGHT:
		return KeyCodes::RightArrow;
	case VK_SPACE:
		return KeyCodes::Space;
	case VK_LSHIFT:
		return KeyCodes::LeftShift;
	case VK_RSHIFT:
		return KeyCodes::RightShift;
	case VK_SHIFT:
		return KeyCodes::Shift;
	case VK_LCONTROL:
		return KeyCodes::LeftCtrl;
	case VK_RCONTROL:
		return KeyCodes::RightCtrl;
	case VK_LMENU:
		return KeyCodes::LeftAlt;
	case VK_RMENU:
		return KeyCodes::RightAlt;
	case VK_ESCAPE:
		return KeyCodes::Escape;
	default:
		Common::Warning(("keyCode not found: " + std::to_string(wParam)).c_str());
		return (KeyCodes)-1;
	}
}