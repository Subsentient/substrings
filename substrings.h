/*SubStrings C-String manipulation library. This software is public domain.
 * Please read the file UNLICENSE.TXT for more information.*/
#ifndef __SUBSTRINGS_H__
#define __SUBSTRINGS_H__

#ifdef __cplusplus
extern "C" {
#endif

#define SUBSTRINGS_VERSION "0.2"

typedef signed char SSBool;

enum { SPLIT_NOKEEP, SPLIT_HALFONE, SPLIT_HALFTWO };

extern const struct _SubStrings
{
	SSBool (*Compare)(const char *Match, const char *Source);
	SSBool (*NCompare)(const char *Match, const unsigned Length, const char *Source);
	SSBool (*StartsWith)(const char *Match, const char *Source);
	SSBool (*EndsWith)(const char *Match, const char *Source);
	unsigned (*Length)(const char *String);
	unsigned (*Copy)(char *Dest, const char *Source, unsigned Max);
	unsigned (*Cat)(char *Dest, const char *Source, unsigned DestTotalSize);
	char *(*Find)(const char *Match, const int ResultNumber, const char *InStream);
	char *(*CFind)(const char Match, const int ResultNumber, const char *InStream);
	unsigned (*Replace)(register char *Stream, void *TempBuf, unsigned StreamSize, const char *Match, const char *Replacement);
	SSBool (*Split)(char *HalfOneOut, char *HalfTwoOut, const char *Match, const char *InStream, int Mode);
	char *(*Between)(char *OutBuf, const char *First, const char *Second, const char *InStream);
	char *(*Reverse)(char *OutStream, const char *InStream);
	SSBool (*CopyUntil)(char *Dest, register unsigned DestTotalSize,
									const char **Ptr, const char *const Trigger);
	SSBool (*CopyUntilC)(register char *Dest, register unsigned DestTotalSize, const char **Ptr,
										const char *Triggers);
	char *(*FindAnyOf)(const char *CharList, const char *Source);
	unsigned (*Strip)(const char *Match, char *Source);
	unsigned (*StripC)(const char *Match, char *Source);
	
	struct
	{ /*Line-processing, for iterating through files in memory with newlines and/or carriage returns separating lines.*/
		char *(*NextLine)(const char *InStream);
		char *(*WhitespaceJump)(const char *InStream);
		SSBool (*GetLine)(const char **Ptr, char *OutStream, unsigned OutStreamTotalSize);
	} Line;
	
	struct
	{ /*ASCII coversion tools.*/
		char (*UpperC)(char Character);
		char (*LowerC)(char Character);
		char *(*UpperS)(char *String);
		char *(*LowerS)(char *String);
	} ASCII;
	
} SubStrings;

#ifdef __cplusplus
}
#endif

#endif /*__SUBSTRINGS_H__*/
