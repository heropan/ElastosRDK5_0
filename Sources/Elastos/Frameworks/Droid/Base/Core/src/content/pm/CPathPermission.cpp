
#include "ext/frameworkext.h"
#include "content/pm/CPathPermission.h"

namespace Elastos {
namespace Droid {
namespace Content {
namespace Pm {

ECode CPathPermission::constructor()
{
    return PatternMatcher::constructor();
}

ECode CPathPermission::constructor(
    /* [in] */ const String& pattern,
    /* [in] */ Int32 type,
    /* [in] */ const String& readPermission,
    /* [in] */ const String& writePermission)
{
    PatternMatcher::constructor(pattern, type);
    mReadPermission = readPermission;
    mWritePermission = writePermission;
    return NOERROR;
}

ECode CPathPermission::GetReadPermission(
    /* [out] */ String* readPermission)
{
    VALIDATE_NOT_NULL(readPermission);
    *readPermission = mReadPermission;
    return NOERROR;
}

ECode CPathPermission::GetWritePermission(
    /* [out] */ String* writePermission)
{
    VALIDATE_NOT_NULL(writePermission);
    *writePermission = mWritePermission;
    return NOERROR;
}

ECode CPathPermission::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    assert(source != NULL);
    PatternMatcher::ReadFromParcel(source);
    source->ReadString(&mReadPermission);
    source->ReadString(&mWritePermission);
    return NOERROR;
}

ECode CPathPermission::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    assert(dest != NULL);
    PatternMatcher::WriteToParcel(dest);
    dest->WriteString(mReadPermission);
    dest->WriteString(mWritePermission);
    return NOERROR;
}

ECode CPathPermission::GetPath(
    /* [out] */ String* path)
{
    return PatternMatcher::GetPath(path);
}

ECode CPathPermission::GetType(
    /* [out] */ Int32* type)
{
    return PatternMatcher::GetType(type);
}

ECode CPathPermission::Match(
    /* [in] */ const String& str,
    /* [out] */ Boolean* isMatch)
{
    return PatternMatcher::Match(str, isMatch);
}

ECode CPathPermission::ToString(
    /* [out] */ String* str)
{
    return PatternMatcher::ToString(str);
}

} // namespace Pm
} // namespace Content
} // namespace Droid
} // namespace Elastos
