#include "UFontAtlas.h"
#include "Core/Container/String.h"
#include <string>

UFontAtlas::UFontAtlas()
{
	const int textureWidth = 512;
	const int textureHeight = 512;
	const int cellWidth = 14;
	const int cellHeight = 32;
	const int cellsPerRow = 36;
	const int cellsPerColumn = 16;

	const float uvCellWidth = static_cast<float>(cellWidth) / textureWidth;
	const float uvCellHeight = static_cast<float>(cellHeight) / textureHeight;

	const std::wstring glyphString = L" ☺☻♥♦♣♠•◘○◙♂♀♪♫☼►◄↕‼¶§▬↨↑↓→←∟↔▲▼ !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~⌂ÇüéâäàåçêëèïîìÄÅÉæÆôöòûùÿÖÜ¢£¥₧ƒáíóúñÑªº¿⌐¬½¼¡«»░▒▓│┤╡╢╖╕╣║╗╝╜╛┐└┴┬├─┼╞╟╚╔╩╦╠═╬╧╨╤╥╙╘╒╓╫╪┘┌█▄▌▐▀αßΓπΣσµτΦΘΩδ∞φε∩≡±≥≤⌠⌡÷≈°∙·√ⁿ²■□";
	
	for (int row = 0; row < cellsPerColumn; ++row)
	{
		for (int col = 0; col < cellsPerRow; ++col)
		{
			int index = row * cellsPerRow + col;
			if (index >= glyphString.size()) break;

			float u = (static_cast<float>(col) * cellWidth) / textureWidth;
			float v = (static_cast<float>(row) * cellHeight) / textureHeight;

			GlyphInfo glyph = {
				u,               
				v,               
				uvCellWidth,     
				uvCellHeight,    
				0.0f,            
				0.0f             
			};

			wchar_t wc = glyphString[index];
			AddGlyph(wc, glyph);
		}
	}
}
