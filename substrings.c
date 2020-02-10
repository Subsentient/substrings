/*SubStrings C-String manipulation library. This software is public domain.
 * Please read the file UNLICENSE.TXT for more information.*/
 
#define NULL ((void*)0)

#include "substrings.h"

enum { false, true };

/*Function prototypes.*/
const struct _SubStrings SubStrings =
	{ /*Add all functions to a proper place in this struct, and in substrings.h's definition of its type.*/
		SubStrings_Compare, SubStrings_NCompare, SubStrings_CaseCompare, SubStrings_CaseNCompare,
		SubStrings_StartsWith, SubStrings_EndsWith,
		SubStrings_Length, SubStrings_Copy, SubStrings_Cat,
		SubStrings_Find, SubStrings_CFind, SubStrings_Replace,
		SubStrings_Split, SubStrings_Between, SubStrings_Extract, SubStrings_Reverse,
		SubStrings_CopyUntil, SubStrings_CopyUntilC, SubStrings_FindAnyOf,
		SubStrings_Strip, SubStrings_StripC, SubStrings_StripTrailingChars,
		SubStrings_StripLeadingChars,
		{ SubStrings_LP_NextLine, SubStrings_LP_WhitespaceJump, SubStrings_LP_GetLine },
		{ SubStrings_ASCII_UpperC, SubStrings_ASCII_LowerC,
			SubStrings_ASCII_UpperS, SubStrings_ASCII_LowerS,
			SubStrings_ASCII_IsUpperC, SubStrings_ASCII_IsLowerC, SubStrings_ASCII_IsDigitC,
			SubStrings_ASCII_IsUpperS, SubStrings_ASCII_IsLowerS, SubStrings_ASCII_IsDigitS}
	};
	
/*Actual functions.*/
unsigned SubStrings_Length(const char *const String)
{
	register const char *Worker = String;
	
	while (*Worker) ++Worker;
	
	return Worker - String;
}

SSBool SubStrings_Compare(register const char *Match, register const char *Source)
{	
	for (; *Match && *Source; ++Match, ++Source)
	{
		if (*Match != *Source) return false;
	}
	
	if (*Match != *Source) return false; /*If one string is shorter than the other*/
	
	return true;
}

SSBool SubStrings_NCompare(register const char *Match, const unsigned Length, register const char *Source)
{
	register unsigned Inc = 0;
	
	for (; Inc < Length && *Match && *Source; ++Inc, ++Match, ++Source)
	{
		if (*Match != *Source) return false;
	}
	
	if (*Match != *Source) return false;
	
	return true;
}

SSBool SubStrings_CaseCompare(register const char *Match, register const char *Source)
{
	for (; *Match && *Source; ++Match, ++Source)
	{
		register char Cmp1 = *Match;
		register char Cmp2 = *Source;
		
		if (Cmp1 >= 'A' && Cmp1 <= 'Z') Cmp1 = Cmp1 + ('a' - 'A');
		if (Cmp2 >= 'A' && Cmp2 <= 'Z') Cmp2 = Cmp2 + ('a' - 'A');
		
		if (Cmp1 != Cmp2) return false;
	}
	
	if (*Match != *Source) return false;
	
	return true;
}

SSBool SubStrings_CaseNCompare(register const char *Match, const unsigned Length, register const char *Source)
{
	unsigned Inc = 0;
	
	for (; Inc < Length && *Match && *Source; ++Inc, ++Match, ++Source)
	{
		register char Cmp1 = *Match;
		register char Cmp2 = *Source;
		
		if (Cmp1 >= 'A' && Cmp1 <= 'Z') Cmp1 = Cmp1 + ('a' - 'A');
		if (Cmp2 >= 'A' && Cmp2 <= 'Z') Cmp2 = Cmp2 + ('a' - 'A');
		
		if (Cmp1 != Cmp2) return false;
	}
	
	if (*Match != *Source) return false;
	
	return true;
}

SSBool SubStrings_StartsWith(const char *Match, const char *Source)
{
	const unsigned Len = SubStrings_Length(Match);
	register unsigned Inc = 0;
	
	if (!Len) return false;
	
	for (; *Match == *Source && *Match && *Source; ++Inc, ++Match, ++Source);
	
	if (Len == Inc) return true;
	
	return false;
}

SSBool SubStrings_EndsWith(const char *Match, const char *Source)
{
	const unsigned SourceLen = SubStrings_Length(Source);
	const unsigned MatchLen = SubStrings_Length(Match);
	register unsigned Inc = 0;
	
	if (SourceLen < MatchLen || !MatchLen) return false;
	
	Source += SourceLen;
	Source -= MatchLen;

	for (; *Source == *Match && *Source && *Match; ++Source, ++Match, ++Inc);
	
	if (Inc == MatchLen) return true;
	
	return false;
}

unsigned SubStrings_Strip(const char *const Match, char *const Source)
{ /*Removes all matching patterns from Source and returns the number of detections.*/
	register char *Worker = Source;
	const unsigned Len = SubStrings_Length(Source);
	const unsigned MatchLen = SubStrings_Length(Match);
	register unsigned Inc = 0;
	
	for (; (Worker = SubStrings_Find(Match, 1, Worker)); ++Inc)
	{
		SubStrings_Copy(Worker, Worker + MatchLen, Len);
	}
	
	return Inc;
}


unsigned SubStrings_StripC(const char *const Match, char *const Source)
{
	register char *Worker = Source;
	const unsigned Len = SubStrings_Length(Source);
	register unsigned Inc = 0;
	
	for (; (Worker = SubStrings_FindAnyOf(Match, Worker)); ++Inc)
	{
		SubStrings_Copy(Worker, Worker + 1, Len);
	}
	
	return Inc;
}

char *SubStrings_LP_NextLine(const char *InStream)
{
	register const char *Worker = InStream;
	
	while (*Worker != '\r' && *Worker != '\n' && *Worker != '\0') ++Worker;
	
	if (*Worker == '\0') return NULL;
	
	while (*Worker == '\r' || *Worker == '\n') ++Worker;
	
	if (*Worker == '\0') return NULL;
	
	return (char*)Worker;
}


char *SubStrings_LP_WhitespaceJump(const char *InStream)
{
	register const char *Worker = InStream;
	
	while (*Worker != ' ' && *Worker != '\t' && *Worker != '\r' && *Worker != '\n' && *Worker != '\0') ++Worker;
	
	if (*Worker == '\0' || *Worker == '\r' || *Worker == '\n') return NULL;
	
	while (*Worker == ' ' || *Worker == '\t') ++Worker;
	
	if (*Worker == '\0' || *Worker == '\r' || *Worker == '\n') return NULL;
	
	return (char*)Worker;
}

char SubStrings_ASCII_LowerC(const char C)
{
	if (C >= 'A' && C <= 'Z') return C + ('a' - 'A');
	else return C;
}

char SubStrings_ASCII_UpperC(const char C)
{
	if (C >= 'a' && C <= 'z') return C - ('a' - 'A');
	else return C;
}

char *SubStrings_ASCII_UpperS(char *const S)
{
	register char *Worker = S;
	
	for (; *Worker != '\0'; ++Worker)
	{
		if (*Worker >= 'a' && *Worker <= 'z')
		{
			*Worker -= ('a' - 'A');
		}
	}

	return S;
}

char *SubStrings_ASCII_LowerS(char *const S)
{
	register char *Worker = S;
	
	for (; *Worker != '\0'; ++Worker)
	{
		if (*Worker >= 'A' && *Worker <= 'Z')
		{
			*Worker += ('a' - 'A');
		}
	}
	
	return S;
}

SSBool SubStrings_ASCII_IsUpperC(const char Char)
{
	if (Char >= 'A' && Char <= 'Z') return true;
	return false;
}

SSBool SubStrings_ASCII_IsLowerC(const char Char)
{
	if (Char >= 'a' && Char <= 'z') return true;
	return false;
}

SSBool SubStrings_ASCII_IsDigitC(const char Char)
{
	if (Char >= '0' && Char <= '9') return true;
	return false;
}

SSBool SubStrings_ASCII_IsUpperS(register const char *String)
{
	if (!*String) return false;
	
	for (; *String != '\0'; ++String)
	{
		if (*String < 'A' || *String > 'Z') return false;
	}
	
	return true;
}
	
SSBool SubStrings_ASCII_IsLowerS(register const char *String)
{
	if (!*String) return false;
	
	for (; *String != '\0'; ++String)
	{
		if (*String < 'a' || *String > 'z') return false;
	}
	
	return true;
}
	
SSBool SubStrings_ASCII_IsDigitS(register const char *String)
{
	if (!*String) return false;
	
	for (; *String != '\0'; ++String)
	{
		if (*String < '0' || *String > '9') return false;
	}
	
	return true;
}

char *SubStrings_Between(register char *Dest, const char *After, const char *Before, const char *InStream)
{ /*Pull something out of the middle of a string.*/
	return SubStrings_Extract(Dest, SubStrings_Length(InStream) + 1, After, Before, InStream); /*Making an assumption for DestSize*/
}

char *SubStrings_Extract(register char *Dest, register unsigned DestSize, const char *After, const char *Before, const char *InStream)
{ /*Pull something out of the middle of a string.*/
	const char *Begin;
	register const char *Worker;
	register const char *End;
	const char *RetVal;
	
	if ((After && !SubStrings_Find(After, 1, InStream)) || (Before && !SubStrings_Find(Before, 1, InStream)))
	{ /*Check if we're being given bad things to search for.*/
		return NULL;
	}
	
	Begin = After ? SubStrings_Find(After, 1, InStream) : InStream;
	Worker = Begin + (After ? SubStrings_Length(After) : 0);
	End = Before ? SubStrings_Find(Before, 1, Worker) : InStream + SubStrings_Length(InStream);
	RetVal = Worker;
	
	for (; DestSize - 1 > 0 && Worker != End; ++Worker, ++Dest, --DestSize)
	{
		*Dest = *Worker;
	}
	*Dest = '\0';
	
	return (char*)RetVal;
}

unsigned SubStrings_Copy(register char *Dest, register const char *Source, unsigned Max)
{
	register unsigned Inc = 0;
	
	for (; *Source != '\0' && Inc < Max - 1; ++Inc) *Dest++ = *Source++;
	*Dest = '\0';
	
	return Inc;
}

SSBool SubStrings_CopyUntil(char *Dest, register unsigned DestTotalSize,
							const char **Ptr, const char *const Trigger, const SSBool SkipPastAdjacentTriggers)
{ /*Copy Source to Dest until the string Until, copying a maximum of DestTotalSize - 1 characters.*/
	register const char *Worker = NULL;
	register const char *Stopper = NULL;
	
	if (!*Ptr || **Ptr == '\0') return false;
	
	Worker = *Ptr;
	Stopper = SubStrings_Find(Trigger, 1, *Ptr);
	
	for (; DestTotalSize - 1 > 0 && Worker != Stopper && *Worker != '\0'; ++Dest, ++Worker, --DestTotalSize)
	{
		*Dest = *Worker;
	}
	*Dest = '\0';
	
	
	if (!Stopper) *Ptr = NULL;
	else
	{
		const unsigned TLen = SubStrings_Length(Trigger);
		
		*Ptr = Stopper + TLen;
		
		if (SkipPastAdjacentTriggers)
		{ /*So if we have wibblederpderpderpderpwibble, we get wibblewibble.*/
			while (SubStrings_Compare(Trigger, *Ptr)) *Ptr += TLen;
		}
	}
	
	return true;
}

char *SubStrings_FindAnyOf(const char *CharList, const char *Source)
{ /*Like strpbrk().*/
	register const char *Worker = Source;
	register const char *CL;
	
	for (; *Worker != '\0'; ++Worker)
	{
		for (CL = CharList; *CL != '\0'; ++CL)
		{
			if (*Worker == *CL) return (char*)Worker;
		}
	}
	
	return NULL;
}
	
SSBool SubStrings_CopyUntilC(register char *Dest, register unsigned DestTotalSize, const char **Ptr,
										const char *Triggers, const SSBool SkipPastAdjacentTriggers)
{ /*Same as CopyUntil(), except it does strpbrk() style matching instead of strstr() style.*/
	register const char *Worker = NULL;
	register const char *Stopper = NULL;
	
	if (!*Ptr || **Ptr == '\0') return false;
	
	Worker = *Ptr;
	Stopper = SubStrings_FindAnyOf(Triggers, *Ptr);
	
	for (; DestTotalSize - 1 > 0 && Worker != Stopper && *Worker != '\0'; ++Dest, ++Worker, --DestTotalSize)
	{
		*Dest = *Worker;
	}
	*Dest = '\0';
	
	
	if (!Stopper) *Ptr = NULL;
	else
	{
		const char *CL;

	CheckRestart:
		for (CL = Triggers; *CL != '\0'; ++CL)
		{
			if (*Worker == *CL)
			{
				++Worker;
				
				if (SkipPastAdjacentTriggers) goto CheckRestart;
				else break;
			}
		}
		
		*Ptr = Worker;
	}	
	
	return true;
}

unsigned SubStrings_Cat(char *Dest, const char *Snip, const unsigned DestTotalSize)
{ /*Safe concatenation function.*/
	register unsigned Inc = 0;
	register unsigned DestTotalLength = SubStrings_Length(Dest);
	
	if (!DestTotalSize || !*Snip) return 0;
	
	Dest += DestTotalLength;
	
	for (; *Snip != '\0' && DestTotalLength < DestTotalSize - 1; ++Inc, ++Snip, ++Dest, ++DestTotalLength)
	{	
		*Dest = *Snip;
	}
	*Dest = '\0';
	
	return Inc;
}

char *SubStrings_Find(const char *const Match, const int ResultNumber, const char *const InStream)
{ /*Like strstr(), but can get a certain result number.*/
	register const char *Worker = InStream;
	const unsigned MatchLen = SubStrings_Length(Match);
	const unsigned SLen = SubStrings_Length(InStream);
	register const char *SubWorker = Worker, *SubMatch = NULL;
	register unsigned Inc = 0;
	register int CountInc = 0;
	
	if (MatchLen > SLen || !MatchLen || !SLen || !ResultNumber) return NULL;
	
	for (; *Worker != '\0'; SubWorker = ++Worker)
	{
		for (Inc = 0, SubMatch = Match; (*SubWorker == *SubMatch) && (*SubWorker && *SubMatch);
			++SubWorker, ++SubMatch, ++Inc);
		
		if (Inc == MatchLen)
		{
			if (CountInc + 1 == ResultNumber) return (char*)Worker;
			else ++CountInc;
		}
	}
	
	return NULL;
}

char *SubStrings_CFind(const char Match, const int ResultNumber, const char *const InStream)
{ /*Like strchr(), but can get a certain result number.*/
	register const char *Worker = InStream;
	register int CountInc = 0;
	
	for (; *Worker != '\0'; ++Worker)
	{
		if (*Worker == Match)
		{
			if (CountInc + 1 == ResultNumber) return (char*)Worker;
			else ++CountInc;
		}
	}
	
	return NULL;
}

unsigned SubStrings_Replace(register char *Stream, void *TempBuf, unsigned StreamTotalSize, const char *Match, const char *Replacement)
{ /*I decided to use some existing functions so we don't have to reinvent the wheel here.
	The optimizer should do ok by inlining these.*/
	char *HalfOne = TempBuf, *HalfTwo = (char*)TempBuf + StreamTotalSize;
	register unsigned ReplaceCount = 0;
	char *Last;
	
	for (; SubStrings_Split(HalfOne, HalfTwo, Match, Stream, SPLIT_NOKEEP); ++ReplaceCount)
	{
		*Stream = '\0';
		SubStrings_Cat(Stream, HalfOne, StreamTotalSize);
		
		Last = Stream + SubStrings_Length(Stream);
		
		SubStrings_Cat(Stream, Replacement, StreamTotalSize);
		SubStrings_Cat(Stream, HalfTwo, StreamTotalSize);
		
		Stream = Last + SubStrings_Length(Replacement);
	}
	
	return ReplaceCount;
}
		
SSBool SubStrings_Split(register char *HalfOneOut, register char *HalfTwoOut,
								const char *const Match, const char *const InStream, int Mode)
{
	register const char *Worker = InStream;
	const char *const Delimiter = SubStrings_Find(Match, 1, InStream);
	const char *StopH1 = NULL;
	const char *StartH2 = NULL;
	
	if (!Delimiter) return false;
	
	switch (Mode)
	{ /*What to do with the delimiter.*/
		case SPLIT_NOKEEP:
			StopH1 = Delimiter;
			StartH2 = Delimiter + SubStrings_Length(Match);
			break;
		case SPLIT_HALFONE:
			StartH2 = StopH1 = Delimiter + SubStrings_Length(Match);
			break;
		case SPLIT_HALFTWO:
			StopH1 = StartH2 = Delimiter;
			break;
		default:
			return false;
	}

	if (HalfOneOut)
	{
		for (; Worker != StopH1; ++HalfOneOut, ++Worker)
		{
			*HalfOneOut = *Worker;
		}
		*HalfOneOut = '\0';
	}
	
	if (HalfTwoOut)
	{
		for (Worker = StartH2; *Worker != '\0'; ++HalfTwoOut, ++Worker)
		{
			*HalfTwoOut = *Worker;
		}
		*HalfTwoOut = '\0';
	}
	
	return true;
}	

char *SubStrings_Reverse(register char *OutStream, register const char *InStream)
{ /*Reverse a string of text*/
	const char *End = NULL, *RetVal = OutStream;
	const unsigned Len = SubStrings_Length(InStream);
	
	InStream += Len - 1;
	End = OutStream + Len;
	
	for (; OutStream != End; --InStream, ++OutStream)
	{
		*OutStream = *InStream;
	}
	*OutStream = '\0';
	
	return (char*)RetVal;
}

SSBool SubStrings_LP_GetLine(char *OutStream, const unsigned OutStreamTotalSize, const char **Ptr)
{ /*Takes the value of *Ptr, gets a line from it, and modifies *Ptr to point to the next line after, or returns false if bad.*/
	register const char *Worker = *Ptr;
	register unsigned Inc = 0;
	
	/*No line.*/
	if (*Worker == '\0') return false;
	
	/*Do the line copy.*/
	for (; Inc < OutStreamTotalSize - 1 && *Worker != '\r' && *Worker != '\n' && *Worker != '\0'; ++Worker, ++OutStream, ++Inc)
	{
		*OutStream = *Worker;
	}
	*OutStream = '\0';
	
	if (*Worker == '\r' || *Worker == '\n') ++Worker;
	
	if (Worker > *Ptr && Worker[-1] == '\r' && *Worker == '\n') ++Worker;
	
	*Ptr = Worker;
	
	return true;
}

unsigned SubStrings_StripTrailingChars(char *Stream, const char *Match)
{
	register const char *M;
	register unsigned StripCount = 0;
	register int Inc = 0;
	
	if (!*Stream) return 0;
	
	while (Stream[Inc]) ++Inc;
	
	for (--Inc; Inc >= 0; --Inc)
	{
		SSBool Found = false;
		
		for (M = Match; *M; ++M)
		{
			if (Stream[Inc] == *M)
			{
				Stream[Inc] = '\0';
				++StripCount;
				Found = true;
				break;
			}
		}
		
		if (!Found) break;
	}
	
	return StripCount;
}

unsigned SubStrings_StripLeadingChars(register char *Stream, const char *Match)
{
	register const char *M;
	char *Orig = Stream, *Worker = Stream;
	unsigned StripCount = 0;
	
	for (; *Stream; ++Stream)
	{
		for (M = Match; *M; ++M)
		{
			if (*Stream == *M)
			{
				Worker = Stream + 1;
				++StripCount;
				break;
			}
		}
	}

	if (Stream == Orig) return 0;
	
	SubStrings_Copy(Orig, Worker, SubStrings_Length(Worker) + 1);
	
	return StripCount;
}
