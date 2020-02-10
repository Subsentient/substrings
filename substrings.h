/*SubStrings C-String manipulation library. This software is public domain.
 * Please read the file UNLICENSE.TXT for more information.*/
#ifndef __SUBSTRINGS_H__
#define __SUBSTRINGS_H__

#ifdef __cplusplus
extern "C" {
#endif

#define SUBSTRINGS_VERSION "0.50"

typedef signed char SSBool;

enum { SPLIT_NOKEEP, SPLIT_HALFONE, SPLIT_HALFTWO };

unsigned SubStrings_Length(const char *String);
SSBool SubStrings_StartsWith(const char *Match, const char *Source);
SSBool SubStrings_EndsWith(const char *Match, const char *Source);
unsigned SubStrings_Copy(char *Dest, const char *Source, unsigned Max);
SSBool SubStrings_Compare(const char *Match, const char *Source);
SSBool SubStrings_NCompare(const char *Match, const unsigned Length, const char *Source);
SSBool SubStrings_CaseNCompare(const char *Match, const unsigned Length, const char *Source);
SSBool SubStrings_CaseCompare(register const char *Match, register const char *Source);
unsigned SubStrings_Cat(char *Dest, const char *Snip, unsigned DestTotalSize);
char *SubStrings_Find(const char *const Match, const int ResultNumber, const char *const InStream);
char *SubStrings_CFind(const char Match, const int ResultNumber, const char *InStream);
unsigned SubStrings_Replace(register char *Stream, void *TempBuf, unsigned StreamTotalSize, const char *Match,
							const char *Replacement);
SSBool SubStrings_Split(register char *HalfOneOut, register char *HalfTwoOut,
								const char *const Match, const char *const InStream, int Mode);
char *SubStrings_Between(char *OutBuf, const char *After, const char *Before, const char *InStream);
char *SubStrings_Extract(char *OutBuf, register unsigned OutBufSize, const char *After, const char *Before, const char *InStream);
char *SubStrings_Reverse(char *OutStream, const char *InStream);
SSBool SubStrings_CopyUntil(char *Dest, register unsigned DestTotalSize,
							const char **Ptr, const char *const Trigger, const SSBool SkipPastAdjacentTriggers);
SSBool SubStrings_CopyUntilC(register char *Dest, register unsigned DestTotalSize, const char **Ptr,
							const char *Triggers, const SSBool SkipPastAdjacentTriggers);
char *SubStrings_FindAnyOf(const char *CharList, const char *Source);
unsigned SubStrings_Strip(const char *const Match, char *const Source);
unsigned SubStrings_StripC(const char *const Match, char *const Source);
char *SubStrings_LP_NextLine(const char *InStream);
char *SubStrings_LP_WhitespaceJump(const char *InStream);
SSBool SubStrings_LP_GetLine(char *OutStream, unsigned OutStreamTotalSize, const char **Ptr);
char SubStrings_ASCII_LowerC(const char C);
char SubStrings_ASCII_UpperC(const char C);
char *SubStrings_ASCII_LowerS(char *const S);
char *SubStrings_ASCII_UpperS(char *const S);
unsigned SubStrings_StripLeadingChars(register char *Stream, const char *Match);
unsigned SubStrings_StripTrailingChars(register char *Stream, const char *Match);
SSBool SubStrings_ASCII_IsLowerC(const char Char);
SSBool SubStrings_ASCII_IsUpperC(const char Char);
SSBool SubStrings_ASCII_IsDigitC(const char Char);
SSBool SubStrings_ASCII_IsLowerS(const char *String);
SSBool SubStrings_ASCII_IsUpperS(const char *String);
SSBool SubStrings_ASCII_IsDigitS(const char *String);

extern const struct _SubStrings
{
	SSBool (*Compare)(const char *Match, const char *Source);
	SSBool (*NCompare)(const char *Match, const unsigned Length, const char *Source);
	SSBool (*CaseCompare)(const char *Match, const char *Source);
	SSBool (*CaseNCompare)(const char *Match, const unsigned Length, const char *Source);
	SSBool (*StartsWith)(const char *Match, const char *Source);
	SSBool (*EndsWith)(const char *Match, const char *Source);
	unsigned (*Length)(const char *String);
	unsigned (*Copy)(char *Dest, const char *Source, unsigned Max);
	unsigned (*Cat)(char *Dest, const char *Source, unsigned DestTotalSize);
	char *(*Find)(const char *Match, const int ResultNumber, const char *InStream);
	char *(*CFind)(const char Match, const int ResultNumber, const char *InStream);
	unsigned (*Replace)(register char *Stream, void *TempBuf, unsigned StreamSize, const char *Match, const char *Replacement);
	SSBool (*Split)(char *HalfOneOut, char *HalfTwoOut, const char *Match, const char *InStream, int Mode);
	
	/*Between and Extract only differ in that Extract requires OutBufSize, and one does not. Between is deprecated.*/
	char *(*Between)(char *OutBuf, const char *First, const char *Second, const char *InStream);
	char *(*Extract)(char *OutBuf, unsigned OutBufSize, const char *First, const char *Second, const char *InStream);
	
	char *(*Reverse)(char *OutStream, const char *InStream);
	SSBool (*CopyUntil)(char *Dest, register unsigned DestTotalSize,
									const char **Ptr, const char *const Trigger, const SSBool SkipPastAdjacentTriggers);
	SSBool (*CopyUntilC)(register char *Dest, register unsigned DestTotalSize, const char **Ptr,
										const char *Triggers, const SSBool SkipPastAdjacentTriggers);
	char *(*FindAnyOf)(const char *CharList, const char *Source);
	unsigned (*Strip)(const char *Match, char *Source);
	unsigned (*StripC)(const char *Match, char *Source);
	unsigned (*StripTrailingChars)(char *Stream, const char *Match);
	unsigned (*StripLeadingChars)(register char *Stream, const char *Match);
	
	struct
	{ /*Line-processing, for iterating through files in memory with newlines and/or carriage returns separating lines.*/
		char *(*NextLine)(const char *InStream);
		char *(*WhitespaceJump)(const char *InStream);
		SSBool (*GetLine)(char *OutStream, unsigned OutStreamTotalSize, const char **Ptr);
	} Line;
	
	struct
	{ /*ASCII coversion tools.*/
		char (*UpperC)(char Character);
		char (*LowerC)(char Character);
		char *(*UpperS)(char *String);
		char *(*LowerS)(char *String);
		SSBool (*IsUpperC)(const char Character);
		SSBool (*IsLowerC)(const char Character);
		SSBool (*IsDigitC)(const char Character);
		SSBool (*IsUpperS)(const char *String);
		SSBool (*IsLowerS)(const char *String);
		SSBool (*IsDigitS)(const char *String);
	} ASCII;
	
} SubStrings;

#ifdef __cplusplus
}
#endif

#endif /*__SUBSTRINGS_H__*/
