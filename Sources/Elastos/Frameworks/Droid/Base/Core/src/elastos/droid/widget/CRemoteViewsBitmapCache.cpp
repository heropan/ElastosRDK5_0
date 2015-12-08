#include "elastos/droid/widget/CRemoteViewsBitmapCache.h"
#include "elastos/droid/widget/RemoteViews.h"
#include "elastos/droid/graphics/CBitmap.h"
#include <elastos/utility/etl/Algorithm.h>
#include <elastos/utility/logging/Slogger.h>

using Elastos::Utility::Logging::Slogger;
using Elastos::Droid::Graphics::CBitmap;

namespace Elastos {
namespace Droid {
namespace Widget {

ECode CRemoteViewsBitmapCache::constructor()
{
    return NOERROR;
}

ECode CRemoteViewsBitmapCache::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    mBitmaps.Clear();

    Int32 count, tmp;
    IParcelable* parcelable;
    source->ReadInt32(&count);
    for (Int32 i = 0; i < count; i++) {
        source->ReadInt32(&tmp);
        AutoPtr<IBitmap> bmp;
        if (tmp != 0) {
            CBitmap::New((IBitmap**)&bmp);
            parcelable = IParcelable::Probe(bmp.Get());
            parcelable->ReadFromParcel(source);
        }

        mBitmaps.PushBack(bmp);
    }

    return NOERROR;
}

ECode CRemoteViewsBitmapCache::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    Int32 count = mBitmaps.GetSize();
    dest->WriteInt32(count);
    IParcelable* parcelable;
    for (Int32 i = 0; i < count; i++) {
        if (mBitmaps[i] != NULL) {
            dest->WriteInt32(1);
            parcelable = IParcelable::Probe(mBitmaps[i].Get());
            parcelable->WriteToParcel(dest);
        }
        else {
            dest->WriteInt32(0);
        }
    }
    return NOERROR;
}

ECode CRemoteViewsBitmapCache::GetBitmapId(
    /* [in] */ IBitmap* b,
    /* [out] */ Int32* id)
{
    VALIDATE_NOT_NULL(id);
    *id = -1;

    if (b != NULL) {
        List<AutoPtr<IBitmap> >::Iterator it = mBitmaps.Begin();
        Int32 i = 0;
        for (; it != mBitmaps.End(); ++it, ++i) {
            if (b == (*it).Get()) {
                *id = i;
                return NOERROR;
            }
        }

        AutoPtr<IBitmap> newBmp = b;
        mBitmaps.PushBack(newBmp);
        *id = i;
        return NOERROR;
    }
    return NOERROR;
}

ECode CRemoteViewsBitmapCache::GetBitmapForId(
    /* [in] */ Int32 id,
    /* [out] */ IBitmap** bitmap)
{
    VALIDATE_NOT_NULL(bitmap);
    *bitmap = NULL;

    if (id >= 0 && id < mBitmaps.GetSize()) {
        *bitmap = mBitmaps[id];
        REFCOUNT_ADD(*bitmap);
    }
    return NOERROR;
}

ECode CRemoteViewsBitmapCache::Assimilate(
    /* [in] */ IRemoteViewsBitmapCache* bitmapCache)
{
    VALIDATE_NOT_NULL(bitmapCache);

    CRemoteViewsBitmapCache* other = (CRemoteViewsBitmapCache*)bitmapCache;

    List<AutoPtr<IBitmap> >::Iterator otherIt = other->mBitmaps.Begin();
    Boolean contains = FALSE;
    AutoPtr<IBitmap> b;
    List<AutoPtr<IBitmap> >::Iterator it;

    for (; otherIt != other->mBitmaps.End(); ++otherIt) {
        AutoPtr<IBitmap> b = *otherIt;
        it = Find(mBitmaps.Begin(), mBitmaps.End(), b);
        contains = (it != mBitmaps.End());

        if (!contains) {
            mBitmaps.PushBack(b);
        }
    }
    return NOERROR;
}

void CRemoteViewsBitmapCache::AddBitmapMemory(
    /* [in] */ MemoryUsageCounter* memoryCounter)
{
    if (memoryCounter != NULL) {
        List<AutoPtr<IBitmap> >::Iterator it = mBitmaps.Begin();
        for (; it != mBitmaps.End(); ++it) {
            memoryCounter->AddBitmapMemory(*it);
        }
    }
}


}// namespace Widget
}// namespace Droid
}// namespace Elastos
