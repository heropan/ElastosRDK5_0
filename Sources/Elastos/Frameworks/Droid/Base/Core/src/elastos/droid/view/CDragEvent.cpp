#include "view/CDragEvent.h"
#include <elastos/utility/logging/Logger.h>
//#include "content/CClipDescription"

using Elastos::Droid::Content::IClipData;
using Elastos::Droid::Content::IClipDescription;
using Elastos::Utility::Logging::Logger;

namespace Elastos {
namespace Droid {
namespace View {

const Int32 CDragEvent::MAX_RECYCLED;
const Boolean CDragEvent::TRACK_RECYCLED_LOCATION;
Mutex CDragEvent::sRecyclerLock;
Int32 CDragEvent::sRecyclerUsed = 0;
AutoPtr<CDragEvent> CDragEvent::sRecyclerTop;

ECode CDragEvent::constructor()
{
    return NOERROR;
}

ECode CDragEvent::GetAction(
    /* [out] */ Int32* action)
{
    VALIDATE_NOT_NULL(action);
    *action = mAction;
    return NOERROR;
}

ECode CDragEvent::SetAction(
    /* [in] */ Int32 action)
{
    mAction = action;
    return NOERROR;
}

ECode CDragEvent::GetX(
    /* [out] */ Float* x)
{
    VALIDATE_NOT_NULL(x);
    *x = mX;
    return NOERROR;
}

ECode CDragEvent::GetY(
    /* [out] */ Float* y)
{
    VALIDATE_NOT_NULL(y);
    *y = mY;
    return NOERROR;
}

ECode CDragEvent::SetX(
    /* [in] */ Float x)
{
    mX = x;
    return NOERROR;
}

ECode CDragEvent::SetY(
    /* [in] */ Float y)
{
    mY = y;
    return NOERROR;
}

ECode CDragEvent::GetClipData(
    /* [out] */ IClipData** clipData)
{
    VALIDATE_NOT_NULL(clipData);
    *clipData = mClipData;
    REFCOUNT_ADD(*clipData);
    return NOERROR;
}

ECode CDragEvent::GetClipDescription(
    /* [out] */ IClipDescription** description)
{
    VALIDATE_NOT_NULL(description);
    *description = mClipDescription;
    REFCOUNT_ADD(*description);
    return NOERROR;
}

ECode CDragEvent::GetLocalState(
    /* [out] */ IInterface** localState)
{
    VALIDATE_NOT_NULL(localState);
    *localState = mLocalState;
    REFCOUNT_ADD(*localState);
    return NOERROR;
}

ECode CDragEvent::SetLocalState(
    /* [in] */ IInterface* localState)
{
    mLocalState = localState;
    return NOERROR;
}

ECode CDragEvent::GetResult(
    /* [out] */ Boolean* dragResult)
{
    VALIDATE_NOT_NULL(dragResult);
    *dragResult = mDragResult;
    return NOERROR;
}

ECode CDragEvent::Recycle()
{
    // Ensure recycle is only called once!
    /*if (TRACK_RECYCLED_LOCATION) {
        if (NULL != mRecycledLocation) {
            //throw new RuntimeException(toString() + " recycled twice!", mRecycledLocation);
            Logger::E(String("DragEvent"), String("recycled twice!"));
            return E_RUNTIME_EXCEPTION;
        }
        mRecycledLocation = new RuntimeException("Last recycled here");
    } else */
    {
        if (mRecycled) {
            Logger::E("DragEvent", "recycled twice!");
            return E_RUNTIME_EXCEPTION;
        }
        mRecycled = TRUE;
    }

    //mClipData = NULL;
    //mClipDescription = NULL;
    //mLocalState = NULL;

    AutoLock lock(sRecyclerLock);
    if (sRecyclerUsed < MAX_RECYCLED) {
        sRecyclerUsed++;
        mNext = sRecyclerTop;
        sRecyclerTop = this;
    }

    return NOERROR;
}

ECode CDragEvent::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

/**
 * Creates a {@link android.os.Parcel} object from this DragEvent object.
 * @param dest A {@link android.os.Parcel} object in which to put the DragEvent object.
 * @param flags Flags to store in the Parcel.
 */
ECode CDragEvent::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    VALIDATE_NOT_NULL(dest);
    FAIL_RETURN(dest->WriteInt32(mAction));
    FAIL_RETURN(dest->WriteFloat(mX));
    FAIL_RETURN(dest->WriteFloat(mY));
    FAIL_RETURN(dest->WriteInt32(mDragResult ? 1 : 0));
    if (NULL == mClipData) {
        FAIL_RETURN(dest->WriteInt32(0));
    }
    else {
        FAIL_RETURN(dest->WriteInt32(1));
        assert(IParcelable::Probe(mClipData) != NULL);
        IParcelable::Probe(mClipData)->WriteToParcel(dest);
    }
    if (NULL == mClipDescription) {
        FAIL_RETURN(dest->WriteInt32(0));
    }
    else {
        FAIL_RETURN(dest->WriteInt32(1));
        IParcelable::Probe(mClipDescription)->WriteToParcel(dest);
    }
    return NOERROR;
}

ECode CDragEvent::Init(
        /* [in] */ Int32 action,
        /* [in] */ Float x,
        /* [in] */ Float y,
        /* [in] */ IClipDescription * description,
        /* [in] */ IClipData* data,
        /* [in] */ IInterface* localState,
        /* [in] */ Boolean result)
{
    mAction = action;
    mX = x;
    mY = y;
    mClipDescription = description;
    mClipData = data;
    mLocalState = localState;
    mDragResult = result;
    return NOERROR;
}


ECode CDragEvent::Obtain(
    /* [out] */ CDragEvent** event)
{
    VALIDATE_NOT_NULL(event);
    return Obtain(0, 0.0f, 0.0f, NULL, NULL, NULL, FALSE, event);
}

ECode CDragEvent::Obtain(
    /* [in] */ Int32 action,
    /* [in] */ Float x,
    /* [in] */ Float y,
    /* [in] */ IInterface* localState,
    /* [in] */ IClipDescription* description,
    /* [in] */ IClipData* data,
    /* [in] */ Boolean result,
    /* [out] */ CDragEvent** event)
{

    VALIDATE_NOT_NULL(event);
    {
        AutoLock lock(sRecyclerLock);

        if (sRecyclerTop == NULL) {
            FAIL_RETURN(CDragEvent::NewByFriend(event));
            (*event)->Init(action, x, y, description, data, localState, result);
            return NOERROR;

        }
        *event = sRecyclerTop;
        REFCOUNT_ADD(*event);
        sRecyclerTop = (*event)->mNext;
        sRecyclerUsed -= 1;
    }
    (*event)->mRecycled = FALSE;
    (*event)->mNext = NULL;
    (*event)->Init(action, x, y, description, data, localState, result);
    return NOERROR;
}

ECode CDragEvent::Obtain(
    /* [in] */ CDragEvent* source,
    /* [out] */ CDragEvent** event)
{
    if (source == NULL) {
        Logger::E("CDragEvent", "other drag event must not be null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    return(Obtain(source->mAction, source->mX, source->mY,
        source->mLocalState, source->mClipDescription, source->mClipData,
        source->mDragResult, event));
}

} // View
} // Droid
} // Elastos

