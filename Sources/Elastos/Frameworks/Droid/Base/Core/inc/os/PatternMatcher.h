#ifndef __PATTERNMATCHER_H__
#define __PATTERNMATCHER_H__

#ifdef DROID_CORE
#include "Elastos.Droid.Core_server.h"
#else
#include "Elastos.Droid.Core.h"
#endif

namespace Elastos {
namespace Droid {
namespace Os {

class PatternMatcher
{
public:
    PatternMatcher();

    PatternMatcher(
    /* [in] */ const String& pattern,
    /* [in] */ Int32 type);

    CARAPI constructor();

    CARAPI constructor(
    /* [in] */ const String& pattern,
    /* [in] */ Int32 type);

    CARAPI GetPath(
        /* [out] */ String* path);

    CARAPI GetType(
        /* [out] */ Int32* type);

    CARAPI Match(
        /* [in] */ const String& str,
        /* [out] */ Boolean* result);

    CARAPI ReadFromParcel(
        /* [in] */ IParcel* source);

    CARAPI WriteToParcel(
        /* [in] */ IParcel* dest);

    CARAPI ToString(
        /* [out] */ String* str);

protected:
    static CARAPI_(Boolean) MatchPattern(
        /* [in] */ const String& pattern,
        /* [in] */ const String& match,
        /* [in] */ Int32 type);

public:
    /**
     * Pattern type: the given pattern must exactly match the string it is
     * tested against.
     */
    static const Int32 PATTERN_LITERAL = 0;

    /**
     * Pattern type: the given pattern must match the
     * beginning of the string it is tested against.
     */
    static const Int32 PATTERN_PREFIX = 1;

    /**
     * Pattern type: the given pattern is interpreted with a
     * simple glob syntax for matching against the string it is tested against.
     * In this syntax, you can use the '*' character to match against zero or
     * more occurrences of the character immediately before.  If the
     * character before it is '.' it will match any character.  The character
     * '\' can be used as an escape.  This essentially provides only the '*'
     * wildcard part of a normal regexp.
     */
    static const Int32 PATTERN_SIMPLE_GLOB = 2;

private:
    String mPattern;
    Int32 mType;
};

} // namespace Os
} // namespace Droid
} // namespace Elastos

#endif //__PATTERNMATCHER_H__
