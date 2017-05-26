
#pragma once

namespace Turbo
{
	namespace Graphics
	{
		typedef unsigned char BYTE;
		typedef char CHAR;
		typedef unsigned short USHORT;
		typedef short SHORT;
		typedef unsigned char UINT24[3];
		typedef unsigned long ULONG;
		typedef long LONG;
		typedef SHORT Fixed[2];
		typedef SHORT FWORD;
		typedef USHORT UFWORD;
		typedef short F2DOT14;
		typedef unsigned char LONGDATETIME[16];
		typedef char Tag[4];
		typedef USHORT GlyphID;
		typedef USHORT Offset;

		struct FontHeader
		{
			Fixed	sfntVersion;				//	0x00010000 for version 1.0.
			USHORT	numTables;					//	Number of tables.
			USHORT	searchRange;				//	(Maximum power of 2 <= numTables) x 16.
			USHORT	entrySelector;				//	Log2(maximum power of 2 <= numTables).
			USHORT	rangeShift;					//	NumTables x 16-searchRange.
		};

		struct OffsetTable
		{
			ULONG	tag;						//	4 -byte identifier.
			ULONG	checkSum;					//	CheckSum for this table.
			ULONG	offset;						//	Offset from beginning of TrueType font file.
			ULONG	length;						//	Length of this table.
		};

		struct FontFileTable
		{
			Tag		tag;
			char	*table;
		};

		struct FontFile
		{
			FontHeader		*fontHeader;
			OffsetTable		*offsetTables;
			FontFileTable	*fontFileTables;
		};

		struct cmapTable
		{

		};

		class IFont
		{
		};
	}
}