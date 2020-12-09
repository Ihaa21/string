#pragma once

/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Ihor Szlachtycz $
   $Notice: (C) Copyright 2014 by Dream.Inc, Inc. All Rights Reserved. $
   ======================================================================== */

// IMPORTANT: The below stuff causes bugs when modified because of \n\r stuff in windows and else where
// Keep that in mind so try to add extra cases instead of modifying current cases

struct string
{
    u32 NumChars;
    char* Chars;
};

//
// NOTE: Char Functions
//

inline b32 IsLetter(char C);
inline b32 IsUInt(char A);
inline b32 IsNumeric(char A);
inline b32 IsSpace(char C);
inline b32 IsNewLine(char C);
inline b32 IsDeadSpace(char A);
inline u32 CharToUInt(char C);

//
// NOTE: String functions
//

inline string String(char* Chars, u32 NumChars);
inline string String(char* Chars);
inline void AdvanceString(string* Str, u32 NumAdvanced);
inline b32 StringContained(string Small, char* Big);
inline b32 StringContained(string Small, string Big);
inline b32 StringsEqual(string A, string B);

//
// NOTE: Advance string functions
//

inline u32 NumCharsToNewLine(string Body);
inline void AdvanceCharsToNewline(string* Body);
inline u32 NumDeadSpaceChars(string Body);
inline void AdvancePastDeadSpaces(string* Body);
inline u32 NumLetterChars(string Body);
inline void AdvancePastLetters(string* Body);
inline u32 NumSpaceChars(string Body);
inline void AdvancePastSpaces(string* Body);
inline u32 ReadUInt(string Body, u32* OutUInt);
inline u32 ReadUIntAndAdvance(string* Body, u32* OutUInt);
inline u32 ReadFloat(string Body, f32* OutFloat);
inline u32 ReadFloatAndAdvance(string* Body, f32* OutFloat);

#include "string.cpp"
