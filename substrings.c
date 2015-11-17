/*SubStrings C-String manipulation library. This software is public domain.
 * Please read the file UNLICENSE.TXT for more information.*/
 
#define NULL ((void*)0)

#include "substrings.h"

enum { false, true };

/*Function prototypes.*/
static unsigned __SubStrings__Length(const char *String);
static SSBool __SubStrings__StartsWith(const char *Match, const char *Source);
static SSBool __SubStrings__EndsWith(const char *Match, const char *Source);
static unsigned __SubStrings__Copy(char *Dest, const char *Source, unsigned Max);
static SSBool __SubStrings__Compare(const char *Match, const char *Source);
static SSBool __SubStrings__NCompare(const char *Match, const unsigned Length, const char *Source);
static SSBool __SubStrings__CaseNCompare(const char *Match, const unsigned Length, const char *Source);
static SSBool __SubStrings__CaseCompare(register const char *Match, register const char *Source);
static unsigned __SubStrings__Cat(char *Dest, const char *Snip, unsigned DestTotalSize);
static char *__SubStrings__Find(const char *const Match, const int ResultNumber, const char *const InStream);
static char *__SubStrings__CFind(const char Match, const int ResultNumber, const char *InStream);
static unsigned __SubStrings__Replace(register char *Stream, void *TempBuf, unsigned StreamTotalSize, const char *Match,
									const char *Replacement);
static SSBool __SubStrings__Split(register char *HalfOneOut, register char *HalfTwoOut,
								const char *const Match, const char *const InStream, int Mode);
static char *__SubStrings__Between(char *OutBuf, const char *After, const char *Before, const char *InStream);
static char *__SubStrings__Extract(char *OutBuf, register unsigned OutBufSize, const char *After, const char *Before, const char *InStream);
static char *__SubStrings__Reverse(char *OutStream, const char *InStream);
static SSBool __SubStrings__CopyUntil(char *Dest, register unsigned DestTotalSize,
							const char **Ptr, const char *const Trigger, const SSBool SkipPastAdjacentTriggers);
static SSBool __SubStrings__CopyUntilC(register char *Dest, register unsigned DestTotalSize, const char **Ptr,
										const char *Triggers, const SSBool SkipPastAdjacentTriggers);
static char *__SubStrings__FindAnyOf(const char *CharList, const char *Source);
static unsigned __SubStrings__Strip(const char *const Match, char *const Source);
static unsigned __SubStrings__StripC(const char *const Match, char *const Source);
static char *__SubStrings__LP__NextLine(const char *InStream);
static char *__SubStrings__LP__WhitespaceJump(const char *InStream);
static SSBool __SubStrings__LP__GetLine(char *OutStream, unsigned OutStreamTotalSize, const char **Ptr);
static char __SubStrings__ASCII__LowerC(const char C);
static char __SubStrings__ASCII__UpperC(const char C);
static char *__SubStrings__ASCII__LowerS(char *const S);
static char *__SubStrings__ASCII__UpperS(char *const S);
static unsigned __SubStrings__StripLeadingChars(register char *Stream, const char *Match);
static unsigned __SubStrings__StripTrailingChars(register char *Stream, const char *Match);
static SSBool __SubStrings__ASCII__IsLowerC(const char Char);
static SSBool __SubStrings__ASCII__IsUpperC(const char Char);
static SSBool __SubStrings__ASCII__IsDigitC(const char Char);
static SSBool __SubStrings__ASCII__IsLowerS(const char *String);
static SSBool __SubStrings__ASCII__IsUpperS(const char *String);
static SSBool __SubStrings__ASCII__IsDigitS(const char *String);

const struct _SubStrings SubStrings =
	{ /*Add all functions to a proper place in this struct, and in substrings.h's definition of its type.*/
		__SubStrings__Compare, __SubStrings__NCompare, __SubStrings__CaseCompare, __SubStrings__CaseNCompare,
		__SubStrings__StartsWith, __SubStrings__EndsWith,
		__SubStrings__Length, __SubStrings__Copy, __SubStrings__Cat,
		__SubStrings__Find, __SubStrings__CFind, __SubStrings__Replace,
		__SubStrings__Split, __SubStrings__Between, __SubStrings__Extract, __SubStrings__Reverse,
		__SubStrings__CopyUntil, __SubStrings__CopyUntilC, __SubStrings__FindAnyOf,
		__SubStrings__Strip, __SubStrings__StripC, __SubStrings__StripTrailingChars,
		__SubStrings__StripLeadingChars,
		{ __SubStrings__LP__NextLine, __SubStrings__LP__WhitespaceJump, __SubStrings__LP__GetLine },
		{ __SubStrings__ASCII__UpperC, __SubStrings__ASCII__LowerC,
			__SubStrings__ASCII__UpperS, __SubStrings__ASCII__LowerS,
			__SubStrings__ASCII__IsUpperC, __SubStrings__ASCII__IsLowerC, __SubStrings__ASCII__IsDigitC,
			__SubStrings__ASCII__IsUpperS, __SubStrings__ASCII__IsLowerS, __SubStrings__ASCII__IsDigitS}
	};
	
/*Actual functions.*/
static unsigned __SubStrings__Length(register const char *String)
{
	register unsigned Inc = 0;
	
	for (; *String++ != '\0'; ++Inc);
	
	return Inc;
}

static SSBool __SubStrings__Compare(register const char *Match, register const char *Source)
{	
	for (; *Match && *Source; ++Match, ++Source)
	{
		if (*Match != *Source) return false;
	}
	
	if (*Match != *Source) return false; /*If one string is shorter than the other*/
	
	return true;
}

static SSBool __SubStrings__NCompare(register const char *Match, const unsigned Length, register const char *Source)
{
	register unsigned Inc = 0;
	
	for (; Inc < Length && *Match && *Source; ++Inc, ++Match, ++Source)
	{
		if (*Match != *Source) return false;
	}
	
	if (*Match != *Source) return false;
	
	return true;
}

static SSBool __SubStrings__CaseCompare(register const char *Match, register const char *Source)
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

static SSBool __SubStrings__CaseNCompare(register const char *Match, const unsigned Length, register const char *Source)
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

static SSBool __SubStrings__StartsWith(const char *Match, const char *Source)
{
	const unsigned Len = SubStrings.Length(Match);
	register unsigned Inc = 0;
	
	if (!Len) return false;
	
	for (; *Match == *Source && *Match && *Source; ++Inc, ++Match, ++Source);
	
	if (Len == Inc) return true;
	
	return false;
}

static SSBool __SubStrings__EndsWith(const char *Match, const char *Source)
{
	const unsigned SourceLen = SubStrings.Length(Source);
	const unsigned MatchLen = SubStrings.Length(Match);
	register unsigned Inc = 0;
	
	if (SourceLen < MatchLen || !MatchLen) return false;
	
	Source += SourceLen;
	Source -= MatchLen;

	for (; *Source == *Match && *Source && *Match; ++Source, ++Match, ++Inc);
	
	if (Inc == MatchLen) return true;
	
	return false;
}

static unsigned __SubStrings__Strip(const char *const Match, char *const Source)
{ /*Removes all matching patterns from Source and returns the number of detections.*/
	register char *Worker = Source;
	const unsigned Len = SubStrings.Length(Source);
	const unsigned MatchLen = SubStrings.Length(Match);
	register unsigned Inc = 0;
	
	for (; (Worker = SubStrings.Find(Match, 1, Worker)); ++Inc)
	{
		SubStrings.Copy(Worker, Worker + MatchLen, Len);
	}
	
	return Inc;
}


static unsigned __SubStrings__StripC(const char *const Match, char *const Source)
{
	register char *Worker = Source;
	const unsigned Len = SubStrings.Length(Source);
	register unsigned Inc = 0;
	
	for (; (Worker = SubStrings.FindAnyOf(Match, Worker)); ++Inc)
	{
		SubStrings.Copy(Worker, Worker + 1, Len);
	}
	
	return Inc;
}

static char *__SubStrings__LP__NextLine(const char *InStream)
{
	register const char *Worker = InStream;
	
	while (*Worker != '\r' && *Worker != '\n' && *Worker != '\0') ++Worker;
	
	if (*Worker == '\0') return NULL;
	
	while (*Worker == '\r' || *Worker == '\n') ++Worker;
	
	if (*Worker == '\0') return NULL;
	
	return (char*)Worker;
}


static char *__SubStrings__LP__WhitespaceJump(const char *InStream)
{
	register const char *Worker = InStream;
	
	while (*Worker != ' ' && *Worker != '\t' && *Worker != '\r' && *Worker != '\n' && *Worker != '\0') ++Worker;
	
	if (*Worker == '\0' || *Worker == '\r' || *Worker == '\n') return NULL;
	
	while (*Worker == ' ' || *Worker == '\t') ++Worker;
	
	if (*Worker == '\0' || *Worker == '\r' || *Worker == '\n') return NULL;
	
	return (char*)Worker;
}

static char __SubStrings__ASCII__LowerC(const char C)
{
	if (C >= 'A' && C <= 'Z') return C + ('a' - 'A');
	else return C;
}

static char __SubStrings__ASCII__UpperC(const char C)
{
	if (C >= 'a' && C <= 'z') return C - ('a' - 'A');
	else return C;
}

static char *__SubStrings__ASCII__UpperS(char *const S)
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

static char *__SubStrings__ASCII__LowerS(char *const S)
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

static SSBool __SubStrings__ASCII__IsUpperC(const char Char)
{
	if (Char >= 'A' && Char <= 'Z') return true;
	return false;
}

static SSBool __SubStrings__ASCII__IsLowerC(const char Char)
{
	if (Char >= 'a' && Char <= 'z') return true;
	return false;
}

static SSBool __SubStrings__ASCII__IsDigitC(const char Char)
{
	if (Char >= '0' && Char <= '9') return true;
	return false;
}

static SSBool __SubStrings__ASCII__IsUpperS(register const char *String)
{
	if (!*String) return false;
	
	for (; *String != '\0'; ++String)
	{
		if (*String < 'A' || *String > 'Z') return false;
	}
	
	return true;
}
	
static SSBool __SubStrings__ASCII__IsLowerS(register const char *String)
{
	if (!*String) return false;
	
	for (; *String != '\0'; ++String)
	{
		if (*String < 'a' || *String > 'z') return false;
	}
	
	return true;
}
	
static SSBool __SubStrings__ASCII__IsDigitS(register const char *String)
{
	if (!*String) return false;
	
	for (; *String != '\0'; ++String)
	{
		if (*String < '0' || *String > '9') return false;
	}
	
	return true;
}

static char *__SubStrings__Between(register char *Dest, const char *After, const char *Before, const char *InStream)
{ /*Pull something out of the middle of a string.*/
	return SubStrings.Extract(Dest, SubStrings.Length(InStream) + 1, After, Before, InStream); /*Making an assumption for DestSize*/
}

static char *__SubStrings__Extract(register char *Dest, register unsigned DestSize, const char *After, const char *Before, const char *InStream)
{ /*Pull something out of the middle of a string.*/
	const char *Begin;
	register const char *Worker;
	register const char *End;
	const char *RetVal;
	
	if ((After && !SubStrings.Find(After, 1, InStream)) || (Before && !SubStrings.Find(Before, 1, InStream)))
	{ /*Check if we're being given bad things to search for.*/
		return NULL;
	}
	
	Begin = After ? SubStrings.Find(After, 1, InStream) : InStream;
	Worker = Begin + (After ? SubStrings.Length(After) : 0);
	End = Before ? SubStrings.Find(Before, 1, Worker) : InStream + SubStrings.Length(InStream);
	RetVal = Worker;
	
	for (; DestSize - 1 > 0 && Worker != End; ++Worker, ++Dest, --DestSize)
	{
		*Dest = *Worker;
	}
	*Dest = '\0';
	
	return (char*)RetVal;
}

static unsigned __SubStrings__Copy(register char *Dest, register const char *Source, unsigned Max)
{
	register unsigned Inc = 0;
	
	for (; *Source != '\0' && Inc < Max - 1; ++Inc) *Dest++ = *Source++;
	*Dest = '\0';
	
	return Inc;
}

static SSBool __SubStrings__CopyUntil(char *Dest, register unsigned DestTotalSize,
							const char **Ptr, const char *const Trigger, const SSBool SkipPastAdjacentTriggers)
{ /*Copy Source to Dest until the string Until, copying a maximum of DestTotalSize - 1 characters.*/
	register const char *Worker = NULL;
	register const char *Stopper = NULL;
	
	if (!*Ptr || **Ptr == '\0') return false;
	
	Worker = *Ptr;
	Stopper = SubStrings.Find(Trigger, 1, *Ptr);
	
	for (; DestTotalSize - 1 > 0 && Worker != Stopper && *Worker != '\0'; ++Dest, ++Worker, --DestTotalSize)
	{
		*Dest = *Worker;
	}
	*Dest = '\0';
	
	
	if (!Stopper) *Ptr = NULL;
	else
	{
		const unsigned TLen = SubStrings.Length(Trigger);
		
		*Ptr = Stopper + TLen;
		
		if (SkipPastAdjacentTriggers)
		{ /*So if we have wibblederpderpderpderpwibble, we get wibblewibble.*/
			while (SubStrings.Compare(Trigger, *Ptr)) *Ptr += TLen;
		}
	}
	
	return true;
}

static char *__SubStrings__FindAnyOf(const char *CharList, const char *Source)
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
	
static SSBool __SubStrings__CopyUntilC(register char *Dest, register unsigned DestTotalSize, const char **Ptr,
										const char *Triggers, const SSBool SkipPastAdjacentTriggers)
{ /*Same as CopyUntil(), except it does strpbrk() style matching instead of strstr() style.*/
	register const char *Worker = NULL;
	register const char *Stopper = NULL;
	
	if (!*Ptr || **Ptr == '\0') return false;
	
	Worker = *Ptr;
	Stopper = SubStrings.FindAnyOf(Triggers, *Ptr);
	
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

static unsigned __SubStrings__Cat(char *Dest, const char *Snip, const unsigned DestTotalSize)
{ /*Safe concatenation function.*/
	register unsigned Inc = 0;
	register unsigned DestTotalLength = SubStrings.Length(Dest);
	
	if (!DestTotalSize || !*Snip) return 0;
	
	Dest += DestTotalLength;
	
	for (; *Snip != '\0' && DestTotalLength < DestTotalSize - 1; ++Inc, ++Snip, ++Dest, ++DestTotalLength)
	{	
		*Dest = *Snip;
	}
	*Dest = '\0';
	
	return Inc;
}

static char *__SubStrings__Find(const char *const Match, const int ResultNumber, const char *const InStream)
{ /*Like strstr(), but can get a certain result number.*/
	register const char *Worker = InStream;
	const unsigned MatchLen = SubStrings.Length(Match);
	const unsigned SLen = SubStrings.Length(InStream);
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

static char *__SubStrings__CFind(const char Match, const int ResultNumber, const char *const InStream)
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

static unsigned __SubStrings__Replace(register char *Stream, void *TempBuf, unsigned StreamTotalSize, const char *Match, const char *Replacement)
{ /*I decided to use some existing functions so we don't have to reinvent the wheel here.
	The optimizer should do ok by inlining these.*/
	char *HalfOne = TempBuf, *HalfTwo = (char*)TempBuf + StreamTotalSize;
	register unsigned ReplaceCount = 0;
	char *Last;
	
	for (; SubStrings.Split(HalfOne, HalfTwo, Match, Stream, SPLIT_NOKEEP); ++ReplaceCount)
	{
		*Stream = '\0';
		SubStrings.Cat(Stream, HalfOne, StreamTotalSize);
		
		Last = Stream + SubStrings.Length(Stream);
		
		SubStrings.Cat(Stream, Replacement, StreamTotalSize);
		SubStrings.Cat(Stream, HalfTwo, StreamTotalSize);
		
		Stream = Last + SubStrings.Length(Replacement);
	}
	
	return ReplaceCount;
}
		
static SSBool __SubStrings__Split(register char *HalfOneOut, register char *HalfTwoOut,
								const char *const Match, const char *const InStream, int Mode)
{
	register const char *Worker = InStream;
	const char *const Delimiter = SubStrings.Find(Match, 1, InStream);
	const char *StopH1 = NULL;
	const char *StartH2 = NULL;
	
	if (!Delimiter) return false;
	
	switch (Mode)
	{ /*What to do with the delimiter.*/
		case SPLIT_NOKEEP:
			StopH1 = Delimiter;
			StartH2 = Delimiter + SubStrings.Length(Match);
			break;
		case SPLIT_HALFONE:
			StartH2 = StopH1 = Delimiter + SubStrings.Length(Match);
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

static char *__SubStrings__Reverse(register char *OutStream, register const char *InStream)
{ /*Reverse a string of text*/
	const char *End = NULL, *RetVal = OutStream;
	const unsigned Len = SubStrings.Length(InStream);
	
	InStream += Len - 1;
	End = OutStream + Len;
	
	for (; OutStream != End; --InStream, ++OutStream)
	{
		*OutStream = *InStream;
	}
	*OutStream = '\0';
	
	return (char*)RetVal;
}

static SSBool __SubStrings__LP__GetLine(char *OutStream, const unsigned OutStreamTotalSize, const char **Ptr)
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

static unsigned __SubStrings__StripTrailingChars(char *Stream, const char *Match)
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

static unsigned __SubStrings__StripLeadingChars(register char *Stream, const char *Match)
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
	
	SubStrings.Copy(Orig, Worker, SubStrings.Length(Worker) + 1);
	
	return StripCount;
}
