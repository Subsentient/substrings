/*SubStrings C-String manipulation library. This software is public domain.
 * Please read the file UNLICENSE.TXT for more information.*/
#ifndef __SUBSTRINGS_H__
#define __SUBSTRINGS_H__

#ifdef __cplusplus
extern "C" {
#endif

#define SUBSTRINGS_VERSION "0.1"

typedef signed char SSBool;

enum { SPLIT_NOKEEP, SPLIT_HALFONE, SPLIT_HALFTWO };

extern const struct _SubStrings
{
	SSBool (*Compare)(const char *Match, const char *Source);
	SSBool (*NCompare)(const char *Match, const unsigned long Length, const char *Source);
	SSBool (*StartsWith)(const char *Match, const char *Source);
	SSBool (*EndsWith)(const char *Match, const char *Source);
	unsigned long (*Length)(const char *String);
	unsigned long (*Copy)(char *Dest, const char *Source, unsigned long Max);
	unsigned long (*Cat)(char *Dest, const char *Source, unsigned long DestTotalSize);
	char *(*Find)(const char *Match, const int ResultNumber, const char *InStream);
	char *(*CFind)(const char Match, const int ResultNumber, const char *InStream);
	SSBool (*Replace)(register char *Stream, unsigned long StreamSize, const char *Match, const char *Replacement);
	SSBool (*Split)(char *HalfOneOut, char *HalfTwoOut, const char *Match, const char *InStream, int Mode);
	char *(*Between)(char *OutBuf, const char *First, const char *Second, const char *InStream);
	char *(*Reverse)(char *OutStream, const char *InStream);
	
	struct
	{
		char *(*NextLine)(const char *InStream);
		char *(*WhitespaceJump)(const char *InStream);
	} Line;
} SubStrings;

#ifdef __cplusplus
}
#endif

#endif /*__SUBSTRINGS_H__*/
