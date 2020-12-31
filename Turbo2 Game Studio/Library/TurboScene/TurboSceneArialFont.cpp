
#include <pch.h>

#include <TurboSceneArialFont.h>

using namespace Turbo::Scene;

//  Constructors -------------------------------------------------------------------------------------------------------

TurboSceneArialFont::TurboSceneArialFont(std::shared_ptr<ITurboDebug> debug) :
	TurboSceneFont(debug, "Arial")
{
	SetTextureExtent(640, 360);

	SetCharacterExtent(' ',   3,   2,  14,  58);
	SetCharacterExtent('!',  15,   2,  30,  58);
	SetCharacterExtent('"',  43,   2,  65,  58);
	SetCharacterExtent('#',  78,   2, 103,  58);
	SetCharacterExtent('$', 116,   2, 144,  58);
	SetCharacterExtent('%', 157,   2, 197,  58);
	SetCharacterExtent('&', 210,   2, 245,  58);
	SetCharacterExtent('\'',258,   2, 269,  58);
	SetCharacterExtent('(', 282,   2, 298,  58);
	SetCharacterExtent(')', 311,   2, 327,  58);
	SetCharacterExtent('*', 340,   2, 360,  58);
	SetCharacterExtent('+', 373,   2, 400,  58);
	SetCharacterExtent(',', 413,   2, 427,  58);
	SetCharacterExtent('-', 440,   2, 455,  58);
	SetCharacterExtent('.', 468,   2, 482,  58);
	SetCharacterExtent('/', 495,   2, 507,  58);

	SetCharacterExtent('0',   3,  59,  31, 115);
	SetCharacterExtent('1',  44,  59,  72, 115);
	SetCharacterExtent('2',  85,  59, 113, 115);
	SetCharacterExtent('3', 126,  59, 154, 115);
	SetCharacterExtent('4', 167,  59, 195, 115);
	SetCharacterExtent('5', 208,  59, 236, 115);
	SetCharacterExtent('6', 249,  59, 277, 115);
	SetCharacterExtent('7', 290,  59, 318, 115);
	SetCharacterExtent('8', 331,  59, 359, 115);
	SetCharacterExtent('9', 372,  59, 400, 115);
	SetCharacterExtent(':', 413,  59, 427, 115);
	SetCharacterExtent(';', 440,  59, 454, 115);
	SetCharacterExtent('<', 467,  59, 494, 115);
	SetCharacterExtent('=', 507,  59, 534, 115);
	SetCharacterExtent('>', 547,  59, 574, 115);
	SetCharacterExtent('?', 587,  59, 614, 115);

	SetCharacterExtent('@',   3, 116,  49, 172);
	SetCharacterExtent('A',  62, 116,  96, 172);
	SetCharacterExtent('B',  97, 116, 131, 172);
	SetCharacterExtent('C', 132, 116, 167, 172);
	SetCharacterExtent('D', 168, 116, 203, 172);
	SetCharacterExtent('E', 204, 116, 235, 172);
	SetCharacterExtent('F', 235, 116, 264, 172);
	SetCharacterExtent('G', 265, 116, 302, 172);
	SetCharacterExtent('H', 303, 116, 338, 172);
	SetCharacterExtent('I', 339, 116, 353, 172);
	SetCharacterExtent('J', 354, 116, 381, 172);
	SetCharacterExtent('K', 382, 116, 417, 172);
	SetCharacterExtent('L', 418, 116, 446, 172);
	SetCharacterExtent('M', 447, 116, 486, 172);
	SetCharacterExtent('N', 487, 116, 522, 172);
	SetCharacterExtent('O', 523, 116, 560, 172);

	SetCharacterExtent('P',   3, 173,  34, 229);
	SetCharacterExtent('Q',  35, 173,  72, 229);
	SetCharacterExtent('R',  73, 173, 107, 229);
	SetCharacterExtent('S', 108, 173, 139, 229);
	SetCharacterExtent('T', 140, 173, 169, 229);
	SetCharacterExtent('U', 170, 173, 205, 229);
	SetCharacterExtent('V', 206, 173, 238, 229);
	SetCharacterExtent('W', 239, 173, 283, 229);
	SetCharacterExtent('X', 284, 173, 312, 229);
	SetCharacterExtent('Y', 313, 173, 342, 229);
	SetCharacterExtent('Z', 243, 173, 373, 229);
	SetCharacterExtent('[', 386, 173, 402, 229);
	SetCharacterExtent('\\',415, 173, 427, 229);
	SetCharacterExtent(']', 440, 173, 456, 229);
	SetCharacterExtent('^', 469, 173, 496, 229);
	SetCharacterExtent('_', 509, 173, 532, 229);

	SetCharacterExtent('`',   3, 230,  18, 286);
	SetCharacterExtent('a',  31, 230,  59, 286);
	SetCharacterExtent('b',  72, 230, 101, 286);
	SetCharacterExtent('c', 114, 230, 142, 286);
	SetCharacterExtent('d', 155, 230, 184, 286);
	SetCharacterExtent('e', 197, 230, 225, 286);
	SetCharacterExtent('f', 238, 230, 256, 286);
	SetCharacterExtent('g', 269, 230, 301, 286);
	SetCharacterExtent('h', 314, 230, 342, 286);
	SetCharacterExtent('i', 355, 230, 367, 286);
	SetCharacterExtent('j', 380, 230, 392, 286);
	SetCharacterExtent('k', 405, 230, 432, 286);
	SetCharacterExtent('l', 445, 230, 457, 286);
	SetCharacterExtent('m', 470, 230, 511, 286);
	SetCharacterExtent('n', 524, 230, 552, 286);
	SetCharacterExtent('o', 565, 230, 593, 286);

	SetCharacterExtent('p',   3, 287,  32, 343);
	SetCharacterExtent('q',  45, 287,  74, 343);
	SetCharacterExtent('r',  87, 287, 107, 343);
	SetCharacterExtent('s', 120, 287, 145, 343);
	SetCharacterExtent('t', 158, 287, 174, 343);
	SetCharacterExtent('u', 187, 287, 215, 343);
	SetCharacterExtent('v', 228, 287, 253, 343);
	SetCharacterExtent('w', 266, 287, 304, 343);
	SetCharacterExtent('x', 317, 287, 341, 343);
	SetCharacterExtent('y', 354, 287, 379, 343);
	SetCharacterExtent('z', 392, 287, 416, 343);
	SetCharacterExtent('{', 429, 287, 446, 343);
	SetCharacterExtent('|', 459, 287, 471, 343);
	SetCharacterExtent('}', 484, 287, 501, 343);
	SetCharacterExtent('~', 514, 287, 541, 343);
	SetCharacterExtent(' ', 554, 287, 581, 343);

}
