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
static unsigned __SubStrings__Cat(char *Dest, const char *Snip, unsigned DestTotalSize);
static char *__SubStrings__Find(const char *const Match, const int ResultNumber, const char *const InStream);
static char *__SubStrings__CFind(const char Match, const int ResultNumber, const char *InStream);
static unsigned __SubStrings__Replace(register char *Stream, char **TempBuf, unsigned StreamTotalSize, const char *Match, const char *Replacement);
static SSBool __SubStrings__Split(register char *HalfOneOut, register char *HalfTwoOut,
								const char *const Match, const char *const InStream, int Mode);
static char *__SubStrings__Between(char *OutBuf, const char *First, const char *Second, const char *InStream);
static char *__SubStrings__Reverse(char *OutStream, const char *InStream);
static char *__SubStrings__CopyUntil(char *Dest, const char *Source, register unsigned DestTotalSize, const char *const Until, const SSBool RetValSkipsPastUntil);
static char *__SubStrings__CopyUntilC(register char *Dest, const char *Source, register unsigned DestTotalSize, const char *UntilC, const SSBool RetValSkipPastMatching);
static char *__SubStrings__FindAnyOf(const char *CharList, const char *Source);
static char *__SubStrings__LP__NextLine(const char *InStream);
static char *__SubStrings__LP__WhitespaceJump(const char *InStream);

/*Actual functions.*/
const struct _SubStrings SubStrings =
	{
		__SubStrings__Compare, __SubStrings__NCompare, 
		__SubStrings__StartsWith, __SubStrings__EndsWith,
		__SubStrings__Length, __SubStrings__Copy, __SubStrings__Cat,
		__SubStrings__Find, __SubStrings__CFind, __SubStrings__Replace,
		__SubStrings__Split, __SubStrings__Between, __SubStrings__Reverse,
		__SubStrings__CopyUntil, __SubStrings__CopyUntilC, __SubStrings__FindAnyOf,
		{ __SubStrings__LP__NextLine, __SubStrings__LP__WhitespaceJump }
	};

static unsigned __SubStrings__Length(const char *String)
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
	
	for (; *Match && *Source && Inc < Length - 1; ++Inc, ++Match, ++Source)
	{
		if (*Match != *Source) return false;
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

static char *__SubStrings__LP__NextLine(const char *InStream)
{
	register const char *Worker = InStream;
	
	while (*Worker != '\n' && *Worker != '\0') ++Worker;
	
	if (*Worker == '\0') return NULL;
	
	while (*Worker == '\n') ++Worker;
	
	if (*Worker == '\0') return NULL;
	
	return (char*)Worker;
}


static char *__SubStrings__LP__WhitespaceJump(const char *InStream)
{
	register const char *Worker = InStream;
	
	while (*Worker != ' ' && *Worker != '\t' && *Worker != '\n' && *Worker != '\0') ++Worker;
	
	if (*Worker == '\0' || *Worker == '\n') return NULL;
	
	while (*Worker == ' ' || *Worker == '\t') ++Worker;
	
	if (*Worker == '\0' || *Worker == '\n') return NULL;
	
	return (char*)Worker;
}

static unsigned __SubStrings__Copy(register char *Dest, register const char *Source, unsigned Max)
{
	register unsigned Inc = 0;
	
	for (; *Source != '\0' && Inc < Max - 1; ++Inc) *Dest++ = *Source++;
	*Dest = '\0';
	
	return Inc;
}

static char *__SubStrings__CopyUntil(char *Dest, const char *Source, register unsigned DestTotalSize,
									const char *const Until, const SSBool RetValSkipsPastUntil)
{ /*Copy Source to Dest until Until, copying a maximum of DestTotalSize - 1 characters.*/
	register const char *Worker = Source;
	const char *Stopper = SubStrings.Find(Until, 1, Source); /*Look for the stopper.*/
	
	if (!Stopper) Stopper = (void*)-1;
	
	for (; Worker != Stopper && *Worker != '\0' && DestTotalSize > 0; ++Dest, ++Worker, --DestTotalSize)
	{
		*Dest = *Worker;
	}
	*Dest = '\0';
	
	if (Stopper == (void*)-1) return NULL;
	
	
	return (char*)Stopper + (RetValSkipsPastUntil ? SubStrings.Length(Until) : 0);
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
	
static char *__SubStrings__CopyUntilC(register char *Dest, const char *Source, register unsigned DestTotalSize,
									const char *UntilC, const SSBool RetValSkipPastMatching)
{ /*Same as CopyUntil(), except it does strpbrk() style matching instead of strstr() style.*/
	register const char *Worker = Source;
	const char *Stopper = SubStrings.FindAnyOf(UntilC, Source);
	
	if (!Stopper) Stopper = (void*)-1;
	
	for (; Worker != Stopper && *Worker != '\0' && DestTotalSize > 0; ++Dest, ++Worker, --DestTotalSize)
	{
		*Dest = *Worker;
	}
	*Dest = '\0';
	
	if (Stopper == (void*)-1) return NULL;
	
	if (RetValSkipPastMatching)
	{
		const char *CL;
		
		Worker = Stopper;
	CheckRestart:
		for (CL = UntilC; *CL != '\0'; ++CL)
		{
			if (*Worker == *CL)
			{
				++Worker;
				goto CheckRestart;
			}
		}
	}
		
	return (char*)Worker;
	
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

static unsigned __SubStrings__Replace(register char *Stream, char **TempBuf, unsigned StreamTotalSize, const char *Match, const char *Replacement)
{ /*I decided to use some existing functions so we don't have to reinvent the wheel here.
	The optimizer should do ok by inlining these.*/
	char *HalfOne = TempBuf[0], *HalfTwo = TempBuf[1];
	register unsigned ReplaceCount = 0;
	
	for (; SubStrings.Split(HalfOne, HalfTwo, Match, Stream, SPLIT_NOKEEP); ++ReplaceCount)
	{
		*Stream = '\0';
		SubStrings.Cat(Stream, HalfOne, StreamTotalSize);
		SubStrings.Cat(Stream, Replacement, StreamTotalSize);
		SubStrings.Cat(Stream, HalfTwo, StreamTotalSize);
		
		Stream = SubStrings.Find(Replacement, 1, Stream) + SubStrings.Length(Replacement);
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

static char *__SubStrings__Between(register char *OutBuf, const char *First, const char *Second, const char *InStream)
{ /*Get the data between First and Second, and return the location in InStream we found it.*/
	const char *RetVal = NULL;
	register const char *Worker = NULL;
	const char *FirstStart = SubStrings.Find(First, 1, InStream);
	const char *SecondStart = SubStrings.Find(Second, 1, InStream);
	
	if (!FirstStart || !SecondStart || FirstStart >= SecondStart) return NULL;
	
	RetVal = Worker = FirstStart + SubStrings.Length(First);
	
	while (Worker != SecondStart)
	{
		*OutBuf++ = *Worker++;
	}
	*OutBuf = '\0';
	
	return (char*)RetVal;
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
