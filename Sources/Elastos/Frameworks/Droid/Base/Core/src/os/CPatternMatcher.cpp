
#include "os/CPatternMatcher.h"

namespace Elastos {
namespace Droid {
namespace Os {

ECode CPatternMatcher::constructor()
{
    return PatternMatcher::constructor();
}

ECode CPatternMatcher::constructor(
    /* [in] */ const String& pattern,
    /* [in] */ Int32 type)
{
    return PatternMatcher::constructor(pattern, type);
}

ECode CPatternMatcher::GetPath(
    /* [out] */ String* path)
{
    return PatternMatcher::GetPath(path);
}

ECode CPatternMatcher::GetType(
    /* [out] */ Int32* type)
{
    return PatternMatcher::GetType(type);
}

ECode CPatternMatcher::Match(
    /* [in] */ const String& str,
    /* [out] */ Boolean* isMatch)
{
    return PatternMatcher::Match(str, isMatch);
}

ECode CPatternMatcher::ToString(
    /* [out] */ String* str)
{
    return PatternMatcher::ToString(str);
}

ECode CPatternMatcher::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    assert(source != NULL && "CPatternMatcher::ReadFromParcel: source cann't be null.");
    return PatternMatcher::ReadFromParcel(source);
}

ECode CPatternMatcher::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    assert(dest != NULL && "CPatternMatcher::WriteToParcel: dest cann't be null.");
    return PatternMatcher::WriteToParcel(dest);
}

} // namespace Os
} // namespace Droid
} // namespace Elastos
