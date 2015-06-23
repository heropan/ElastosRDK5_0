
#include "content/CContentProviderResult.h"
//***#include "net/CStringUri.h"
//***#include "net/COpaqueUri.h"
//***#include "net/CHierarchicalUri.h"
//***#include "net/CPart.h"
//***#include "net/CPathPart.h"
#include <elastos/core/StringUtils.h>

using Elastos::Core::StringUtils;
namespace Elastos {
namespace Droid {
namespace Content {

CContentProviderResult::CContentProviderResult()
    : mUri(NULL)
    , mCount(0)
{}

CContentProviderResult::~CContentProviderResult()
{}

ECode CContentProviderResult::GetUri(
    /* [out] */ IUri** uri)
{
    VALIDATE_NOT_NULL(uri)
    *uri = mUri;
    REFCOUNT_ADD(*uri)
    return NOERROR;
}

ECode CContentProviderResult::GetCount(
    /* [out] */ Int32* count)
{
    VALIDATE_NOT_NULL(count)
    *count = mCount;
    return NOERROR;
}

ECode CContentProviderResult::ToString(
    /* [out] */ String* result)
{
    VALIDATE_NOT_NULL(result)

    if (NULL != mUri) {
        String str1("CContentProviderResult(uri=");
        String str2;
        String str3(")");
        FAIL_RETURN(mUri->ToString(&str2));
        str1.Append(str2);
        str1.Append(str3);
        *result = str1;
    }
    else {
        String str1("ContentProviderResult(count=");
        String str2;
        String str3(")");
        str2 = StringUtils::Int32ToString(mCount);
        str1.Append(str2);
        str1.Append(str3);
        *result = str1;
    }

    return NOERROR;
}

ECode CContentProviderResult::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    VALIDATE_NOT_NULL(source)
    Int32 tmpType = 0;
    source->ReadInt32(&tmpType);

    if (1 == tmpType) {
        source->ReadInt32(&mCount);
        mUri = NULL;
    }
    else {
        mCount = 0;
        Int32 type = 0;
        FAIL_RETURN(source->ReadInt32(&type))
        String str1;

        if (1 == type) {
            FAIL_RETURN(source->ReadString(&str1));
//***            FAIL_RETURN(CStringUri::New(str1, (IUri**)&mUri));
        }
        else if (2 == type) {
            FAIL_RETURN(source->ReadString(&str1));
//***            AutoPtr<IPart> part1;
//***            FAIL_RETURN(Part::ReadFrom(source, (IPart**)&part1));
//***            AutoPtr<IPart> part2;
//***            FAIL_RETURN(Part::ReadFrom(source, (IPart**)&part2));
//***            FAIL_RETURN(COpaqueUri::New(str1, part1, part2, (IUri**)&mUri));
        }
        else if (3 == type) {
            FAIL_RETURN(source->ReadString(&str1));
//***            AutoPtr<IPart> part1;
//***            FAIL_RETURN(Part::ReadFrom(source, (IPart**)&part1));
//***            AutoPtr<IPathPart> pathPart1;
//***            FAIL_RETURN(PathPart::ReadFrom(source, (IPathPart**)&pathPart1));
//***            AutoPtr<IPart> part2;
//***            FAIL_RETURN(Part::ReadFrom(source, (IPart**)&part2));
//***            AutoPtr<IPart> part3;
//***            FAIL_RETURN(Part::ReadFrom(source, (IPart**)&part3));
//***            FAIL_RETURN(CHierarchicalUri::New(str1, part1, pathPart1, part2, part3, (IUri**)&mUri));
        }
    }

    return NOERROR;
}

ECode CContentProviderResult::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    VALIDATE_NOT_NULL(dest)

    if (NULL == mUri) {
        dest->WriteInt32(1);
        dest->WriteInt32(mCount);
    }
    else {
        dest->WriteInt32(2);
        AutoPtr<IParcelable> parcelable = (IParcelable*) mUri->Probe(EIID_IParcelable);
        FAIL_RETURN(parcelable->WriteToParcel(dest))
    }

    return NOERROR;
}

ECode CContentProviderResult::constructor(
    /* [in] */ IUri* uri)
{
    VALIDATE_NOT_NULL(uri)
    mUri = uri;
    mCount = 0;
    return NOERROR;
}

ECode CContentProviderResult::constructor(
    /* [in] */ Int32 count)
{
    mCount = count;
    mUri = NULL;
    return NOERROR;
}

ECode CContentProviderResult::constructor()
{
    return NOERROR;
}

}
}
}

