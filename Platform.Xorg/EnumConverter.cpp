#include "Precomp.h"

#include "EnumConverter.h"

#include <X11/keysym.h>

using namespace ::CainEngine::Platform;
using namespace ::CainEngine::Platform::Internal;

KeyCodes EnumConverter::ToKeyCodes(KeySym k)
{
	switch (k)
	{
	case XK_Shift_L:
		return KeyCodes::LeftShift;
	case XK_Shift_R:
		return KeyCodes::LeftShift;
	case XK_0:
		return KeyCodes::Zero;
	case XK_1:
		return KeyCodes::One;
	case XK_2:
		return KeyCodes::Two;
	case XK_3:
		return KeyCodes::Three;
	case XK_4:
		return KeyCodes::Four;
	case XK_5:
		return KeyCodes::Five;
	case XK_6:
		return KeyCodes::Six;
	case XK_7:
		return KeyCodes::Seven;
	case XK_8:
		return KeyCodes::Eight;
	case XK_9:
		return KeyCodes::Nine;
	case XK_A:
		return KeyCodes::A;
	case XK_B:
		return KeyCodes::B;
	case XK_C:
		return KeyCodes::C;
	case XK_D:
		return KeyCodes::D;
	case XK_E:
		return KeyCodes::E;
	case XK_F:
		return KeyCodes::F;
	case XK_G:
		return KeyCodes::G;
	case XK_H:
		return KeyCodes::H;
	case XK_I:
		return KeyCodes::I;
	case XK_J:
		return KeyCodes::J;
	case XK_K:
		return KeyCodes::K;
	case XK_L:
		return KeyCodes::L;
	case XK_M:
		return KeyCodes::M;
	case XK_N:
		return KeyCodes::N;
	case XK_O:
		return KeyCodes::O;
	case XK_P:
		return KeyCodes::P;
	case XK_Q:
		return KeyCodes::Q;
	case XK_R:
		return KeyCodes::R;
	case XK_S:
		return KeyCodes::S;
	case XK_T:
		return KeyCodes::T;
	case XK_U:
		return KeyCodes::U;
	case XK_V:
		return KeyCodes::V;
	case XK_W:
		return KeyCodes::W;
	case XK_X:
		return KeyCodes::X;
	case XK_Y:
		return KeyCodes::Y;
	case XK_Z:
		return KeyCodes::Z;
	case XK_Up:
		return KeyCodes::UpArrow;
	case XK_Down:
		return KeyCodes::DownArrow;
	case XK_Left:
		return KeyCodes::LeftArrow;
	case XK_Right:
		return KeyCodes::RightArrow;
	default:
		Common::FatalError("Unknown key:  %#xlu", k);
	}
}