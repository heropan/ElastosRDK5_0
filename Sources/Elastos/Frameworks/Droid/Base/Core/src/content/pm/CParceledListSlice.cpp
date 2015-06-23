
#include <binder/Parcel.h>
#include "ext/frameworkext.h"
#include "content/pm/CParceledListSlice.h"
#include "os/CParcel.h"

using Elastos::Droid::Os::CParcel;

namespace Elastos {
namespace Droid {
namespace Content {
namespace Pm {

const Int32 CParceledListSlice::MAX_IPC_SIZE;

CParceledListSlice::CParceledListSlice()
    : mNumItems(0)
    , mIsLastSlice(FALSE)
{}

CParceledListSlice::~CParceledListSlice()
{}

ECode CParceledListSlice::constructor()
{
    return CParcel::New((IParcel**)&mParcel);
}

ECode CParceledListSlice::constructor(
    /* [in] */ IParcel* p,
    /* [in] */ Int32 numItems,
    /* [in] */ Boolean lastSlice)
{
    mParcel = p;
    mNumItems = numItems;
    mIsLastSlice = lastSlice;
    return NOERROR;
}

ECode CParceledListSlice::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    Int32 numItems;
    source->ReadInt32(&numItems);
    Boolean lastSlice;
    source->ReadBoolean(&lastSlice);

    if (numItems > 0) {
        android::Parcel* _src;
        source->GetElementPayload((Handle32*)&_src);
        Int32 parcelSize = _src->readInt32();

        // Advance within this Parcel
        Int32 offset = _src->dataPosition();
        _src->setDataPosition(offset + parcelSize);

        android::Parcel* _dest;
        mParcel->GetElementPayload((Handle32*)&_dest);
        _dest->setDataPosition(0);
        _dest->appendFrom(_src, offset, parcelSize);
        _dest->setDataPosition(0);

        mNumItems = numItems;
        mIsLastSlice = lastSlice;
        return NOERROR;
    }
    else {
        return NOERROR;
    }
}

ECode CParceledListSlice::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    dest->WriteInt32(mNumItems);
    dest->WriteBoolean(mIsLastSlice);

    if (mNumItems > 0) {
        android::Parcel* _src;
        mParcel->GetElementPayload((Handle32*)&_src);
        android::Parcel* _dest;
        dest->GetElementPayload((Handle32*)&_dest);
        Int32 parcelSize = _src->dataSize();
        _dest->writeInt32(parcelSize);
        _dest->appendFrom(_src, 0, parcelSize);
    }

    mNumItems = 0;
    mParcel = NULL;
    return NOERROR;

}

ECode CParceledListSlice::Append(
    /* [in] */ IParcelable* item,
    /* [out] */ Boolean* result)
{
     VALIDATE_NOT_NULL(result);

    if (mParcel == NULL) {
        // throw new IllegalStateException("ParceledListSlice has already been recycled");
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    item->WriteToParcel(mParcel/*, PARCELABLE_WRITE_RETURN_VALUE*/);
    mNumItems++;

    Int32 size;
    mParcel->GetElementSize(&size);
    *result = size > MAX_IPC_SIZE;
    return NOERROR;
}

ECode CParceledListSlice::PopulateList(
    /* [in, out] */ IObjectContainer* list,
    /* [in] */ const ClassID& clsid,
    /* [out] */ IInterface** obj)
{
    VALIDATE_NOT_NULL(obj);

    mParcel->SetDataPosition(0);

    AutoPtr<IParcelable> item;
    for (Int32 i = 0; i < mNumItems; i++) {
        item = NULL;
        FAIL_RETURN(_CObject_CreateInstance(clsid, RGM_SAME_DOMAIN,
                EIID_IParcelable, (IInterface**)&item));
        item->ReadFromParcel(mParcel);
        list->Add(item);
    }

    mParcel = NULL;

    *obj = item;
    REFCOUNT_ADD(*obj);
    return NOERROR;
}

ECode CParceledListSlice::SetLastSlice(
    /* [in] */ Boolean lastSlice)
{
    mIsLastSlice = lastSlice;
    return NOERROR;
}

ECode CParceledListSlice::IsLastSlice(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    *result = mIsLastSlice;
    return NOERROR;
}

} // namespace Pm
} // namespace Content
} // namespace Droid
} // namespace Elastos
