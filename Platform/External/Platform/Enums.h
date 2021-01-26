#pragma once

namespace CainEngine {
namespace Platform {

enum class WindowType : byte
{
	Sizable = 1,
	Borderless,
	FullscreenBorderless,

	Default = Sizable

}; // enum class WindowType

enum class WindowFlags : byte
{
	NoMinimizeButton = 1,
	NoMaximizeButton = 2,
	NoCloseButton = 4,

	Default = 0

}; // enum class WindowFlags

enum class KeyCodes : uint32_t
{
	A = 'A',
	B = 'B',
	C = 'C',
	D = 'D',
	E = 'E',
	F = 'F',
	G = 'G',
	H = 'H',
	I = 'I',
	J = 'J',
	K = 'K',
	L = 'L',
	M = 'M',
	N = 'N',
	O = 'O',
	P = 'P',
	Q = 'Q',
	R = 'R',
	S = 'S',
	T = 'T',
	U = 'U',
	V = 'V',
	W = 'W',
	X = 'X',
	Y = 'Y',
	Z = 'Z',
	Zero = '0',
	One = '1',
	Two = '2',
	Three = '3',
	Four = '4',
	Five = '5',
	Six = '6',
	Seven = '7',
	Eight = '8',
	Nine = '9',

	UpArrow = 0xFFFF,
	DownArrow,
	LeftArrow,
	RightArrow,
	Space,
	LeftShift,
	RightShift,
	Shift,
	LeftCtrl,
	RightCtrl,
	LeftAlt,
	RightAlt,
	Escape

}; // enum class KeyCodes

enum class KeyModifiers : uint32_t
{
	Ctrl = 1,
	Shift,
	Alt,

	CapsLock,
	NumLock,

}; // enum class KeyModifiers

}; // namespace Platform
}; // namespace CainEngine