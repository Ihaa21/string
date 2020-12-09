
//
// NOTE: Char functions
//

inline b32 IsLetter(char C)
{
    b32 Result = (C >= 'a' && C <= 'z') || (C >= 'A' && C <= 'Z');
    return Result;
}

inline b32 IsUInt(char A)
{
    b32 Result = (A >= '0' && A <= '9');
    return Result;
}

inline b32 IsNumeric(char A)
{
    b32 Result = (A == '.' || A == '-' || (A >= '0' && A <= '9'));
    return Result;
}

inline b32 IsSpace(char C)
{
    b32 Result = C == ' ';
    return Result;
}

inline b32 IsNewLine(char C)
{
    b32 Result = C == '\n';

    return Result;
}

inline b32 IsDeadSpace(char A)
{
    b32 Result = A < 33;
    return Result;
}

inline u32 CharToUInt(char C)
{
    u32 Result = u32(C) - u32('0');
    Assert(Result <= 9);
    Assert(Result >= 0);

    return Result;
}

//
// NOTE: String functions
//

inline string String(char* Chars, u32 NumChars)
{
    string Result = {};
    Result.Chars = Chars;
    Result.NumChars = NumChars;

    return Result;
}

inline string String(char* Chars)
{
    // IMPORTANT: We assume the string has a 0 at the end
    // TODO: Custom strlen?
    string Result = String(Chars, (u32)strlen(Chars));
    return Result;
}

inline void AdvanceString(string* Str, u32 NumAdvanced)
{
    Assert(Str->NumChars >= NumAdvanced);
    Str->Chars += NumAdvanced;
    Str->NumChars -= NumAdvanced;
}

inline b32 StringContained(string Small, char* Big)
{
    // IMPORTANT: We assume the big char is larger than the small char
    Assert(Small.NumChars != 0);
    Assert(Small.Chars);
    Assert(Big);
    
    b32 Result = true;
    
    for (u32 CharId = 0; CharId < Small.NumChars; ++CharId)
    {
        if (Big[CharId] != Small.Chars[CharId])
        {
            Result = false;
            break;
        }
    }

    return Result;
}

inline b32 StringContained(string Small, string Big)
{
    b32 Result = false;
    Assert(Big.NumChars != 0);

    if (Big.NumChars >= Small.NumChars)
    {
        Result = StringContained(Small, Big.Chars);
    }
    
    return Result;
}

inline b32 StringsEqual(string A, string B)
{
    b32 Result = false;

    if (A.NumChars == B.NumChars)
    {
        Result = StringContained(A, B.Chars);
    }

    return Result;
}

//
// NOTE: Advance string functions
//

inline u32 NumCharsToNewLine(string Body)
{
    u32 Result = 0;
    while(!IsNewLine(Body.Chars[Result]) && Result < Body.NumChars)
    {
        Result += 1;
    }

    return Result;
}

inline void AdvanceCharsToNewline(string* Body)
{
    u32 NumChars = NumCharsToNewLine(*Body);
    AdvanceString(Body, NumChars);
}

inline u32 NumDeadSpaceChars(string Body)
{
    u32 Result = 0;
    while (IsDeadSpace(Body.Chars[Result]) && Result < Body.NumChars)
    {
        Result += 1;
    }

    return Result;
}

inline void AdvancePastDeadSpaces(string* Body)
{
    u32 NumChars = NumDeadSpaceChars(*Body);
    AdvanceString(Body, NumChars);
}

inline u32 NumLetterChars(string Body)
{
    u32 Result = 0;
    while (IsLetter(Body.Chars[Result]) && Result < Body.NumChars)
    {
        Result += 1;
    }

    return Result;
}

inline void AdvancePastLetters(string* Body)
{
    u32 NumChars = NumLetterChars(*Body);
    AdvanceString(Body, NumChars);
}

inline u32 NumSpaceChars(string Body)
{
    u32 Result = 0;
    while (IsSpace(Body.Chars[Result]) && Result < Body.NumChars)
    {
        Result += 1;
    }

    return Result;
}

inline void AdvancePastSpaces(string* Body)
{
    u32 NumChars = NumSpaceChars(*Body);
    AdvanceString(Body, NumChars);
}

inline u32 ReadUInt(string Body, u32* OutUInt)
{
    u32 NumChars = 0;
   
    // NOTE: Find number of digits
    u32 NumDigits = 0;
    {
        char* CurrChar = Body.Chars;
        while (IsUInt(CurrChar[NumDigits]) && NumDigits < Body.NumChars)
        {
            NumDigits += 1;
        }
    }
    NumChars = NumDigits;

    // NOTE: Calculate the uint 
    u32 OutDigit = 0;
    u32 CurrBase = 1;
    {
        for (u32 DigitId = 0; DigitId < NumDigits; ++DigitId)
        {
            u32 CurrDigit = CharToUInt(Body.Chars[NumDigits - DigitId - 1]);
            OutDigit += CurrBase*CurrDigit;
            CurrBase *= 10;
        }
    }
    *OutUInt = OutDigit;

    return NumChars;
}

inline u32 ReadUIntAndAdvance(string* Body, u32* OutUInt)
{
    u32 NumUIntChars = ReadUInt(*Body, OutUInt);
    AdvanceString(Body, NumUIntChars);

    return NumUIntChars;
}

inline u32 ReadFloat(string Body, f32* OutFloat)
{
    u32 NumChars = 0;
    
    // NOTE: Find number of digits
    b32 HasPoint = false;
    u32 NumDigitSet1 = 0;
    u32 NumDigitSet2 = 0;
    {
        string CurrChar = Body;
        while (IsUInt(CurrChar.Chars[0]) && CurrChar.NumChars > 0)
        {
            NumDigitSet1 += 1;
            AdvanceString(&CurrChar, 1);
        }

        // NOTE: Get past point
        if (CurrChar.Chars[0] == '.')
        {
            HasPoint = true;
            AdvanceString(&CurrChar, 1);
        
            while (IsUInt(CurrChar.Chars[0]) && CurrChar.NumChars > 0)
            {
                NumDigitSet2 += 1;
                AdvanceString(&CurrChar, 1);
            }
        }
    }
    NumChars = NumDigitSet1 + NumDigitSet2 + HasPoint;

    // NOTE: Calculate the float
    float DigitSet1 = 0.0f;
    {
        char* CurrChar = Body.Chars + NumDigitSet1 - 1;
        float CurrBase = 1.0f;
        for (u32 DigitId = 0; DigitId < NumDigitSet1; ++DigitId)
        {
            f32 CurrDigit = (f32)CharToUInt(CurrChar[0]);

            DigitSet1 += CurrBase*CurrDigit;
            CurrBase *= 10.0f;
            CurrChar -= 1;
        }
    }

    float DigitSet2 = 0.0f;
    if (HasPoint)
    {
        // NOTE: Get past point
        char* CurrChar = Body.Chars + NumDigitSet1 + 1;
        float CurrBase = 0.1f;
        for (u32 DigitId = 0; DigitId < NumDigitSet2; ++DigitId)
        {
            f32 CurrDigit = (f32)CharToUInt(CurrChar[0]);

            DigitSet2 += CurrBase*CurrDigit;
            CurrBase /= 10.0f;
            CurrChar += 1;
        }
    }

    *OutFloat = DigitSet1 + DigitSet2;
    return NumChars;
}

inline u32 ReadFloatAndAdvance(string* Body, f32* OutFloat)
{
    u32 NumFloatChars = ReadFloat(*Body, OutFloat);
    AdvanceString(Body, NumFloatChars);
    return NumFloatChars;
}
