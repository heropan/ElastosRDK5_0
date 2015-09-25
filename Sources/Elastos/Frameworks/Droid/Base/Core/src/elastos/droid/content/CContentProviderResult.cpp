
#include "elastos/droid/content/CContentProviderResult.h"
#include "elastos/droid/content/ContentProvider.h"
//#include "net/Uri.h"
#include <elastos/core/StringUtils.h>
#include <elastos/core/StringBuilder.h>

//using Elastos::Droid::Net::Uri;
using Elastos::Core::StringUtils;
using Elastos::Core::StringBuilder;

namespace Elastos {
namespace Droid {
namespace Content {

CAR_INTERFACE_IMPL_2(CContentProviderResult, Object, IContentProviderResult, IParcelable)

CAR_OBJECT_IMPL(CContentProviderResult)

CContentProviderResult::CContentProviderResult()
    : mCount(0)
{}

CContentProviderResult::~CContentProviderResult()
{}

ECode CContentProviderResult::constructor()
{
    return NOERROR;
}

ECode CContentProviderResult::constructor(
    /* [in] */ IUri* uri)
{
    VALIDATE_NOT_NULL(uri)
    mUri = uri;
    return NOERROR;
}

ECode CContentProviderResult::constructor(
    /* [in] */ Int32 count)
{
    mCount = count;
    return NOERROR;
}

ECode CContentProviderResult::constructor(
    /* [in] */ IContentProviderResult* cpr,
    /* [in] */ Int32 userId)
{
    VALIDATE_NOT_NULL(cpr)
    CContentProviderResult* obj = (CContentProviderResult*)cpr;
    mUri = ContentProvider::MaybeAddUserId(obj->mUri, userId);
    return NOERROR;
}

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

ECode CContentProviderResult::SetUri(
    /* [in] */ IUri* uri)
{
    mUri = uri;
    return NOERROR;
}

ECode CContentProviderResult::SetCount(
    /* [in] */ Int32 count)
{
    mCount = count;
    return NOERROR;
}

ECode CContentProviderResult::ToString(
    /* [out] */ String* result)
{
    VALIDATE_NOT_NULL(result)

    if (NULL != mUri) {
        StringBuilder sb("CContentProviderResult(uri=");
        sb.Append(Object::ToString(mUri));
        sb.Append(")");
        *result = sb.ToString();
    }
    else {
        StringBuilder sb("ContentProviderResult(count=");
        sb.Append(StringUtils::ToString(mCount));
        sb.Append(")");
        *result = sb.ToString();
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
        assert(0 && "TODO");
        // Uri::ReadFromParcel(source, (IUri**)&mUri);
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

}
}
}

