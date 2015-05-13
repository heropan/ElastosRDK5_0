
#include "app/ActivityManagerTaskThumbnails.h"
#include <ext/frameworkext.h>

namespace Elastos {
namespace Droid {
namespace App {

ActivityManagerTaskThumbnails::ActivityManagerTaskThumbnails()
    : mNumSubThumbbails(0)
{
}

ECode ActivityManagerTaskThumbnails::Init()
{
    return NOERROR;
}

ECode ActivityManagerTaskThumbnails::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    VALIDATE_NOT_NULL(dest);

    if (mMainThumbnail != NULL) {
        FAIL_RETURN(dest->WriteInt32(1));
//        mMainThumbnail->WriteToParcel(dest);
    } else {
        FAIL_RETURN(dest->WriteInt32(0));
    }
    FAIL_RETURN(dest->WriteInt32(mNumSubThumbbails));
    FAIL_RETURN(dest->WriteInterfacePtr((IInterface*)mRetriever));

    return NOERROR;
}

ECode ActivityManagerTaskThumbnails::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    VALIDATE_NOT_NULL(source);

    Int32 value;
    FAIL_RETURN(source->ReadInt32(&value));
    if (value != 0) {
//        mainThumbnail = Bitmap.CREATOR.createFromParcel(source);
    } else {
        mMainThumbnail = NULL;
    }
    FAIL_RETURN(source->ReadInt32(&mNumSubThumbbails));
//    retriever = IThumbnailRetriever.Stub.asInterface(source.readStrongBinder());

    return NOERROR;
}

ECode ActivityManagerTaskThumbnails::GetMainThumbnail(
    /* [out] */ IBitmap** thumbnail)
{
    VALIDATE_NOT_NULL(thumbnail);
    *thumbnail = mMainThumbnail;
    INTERFACE_ADDREF(*thumbnail)
    return NOERROR;
}

ECode ActivityManagerTaskThumbnails::SetMainThumbnail(
    /* [in] */ IBitmap* thumbnail)
{
    mMainThumbnail = thumbnail;
    return NOERROR;
}

ECode ActivityManagerTaskThumbnails::GetNumSubThumbbails(
    /* [out] */ Int32* num)
{
    VALIDATE_NOT_NULL(num);
    *num = mNumSubThumbbails;
    return NOERROR;
}

ECode ActivityManagerTaskThumbnails::SetNumSubThumbbails(
    /* [in] */ Int32 num)
{
    mNumSubThumbbails = num;
    return NOERROR;
}

/** @hide */
ECode ActivityManagerTaskThumbnails::GetRetriever(
    /* [out] */ IThumbnailRetriever** retriever)
{
    VALIDATE_NOT_NULL(retriever);
    *retriever = mRetriever;
    INTERFACE_ADDREF(*retriever)
    return NOERROR;
}

ECode ActivityManagerTaskThumbnails::SetRetriever(
    /* [in] */ IThumbnailRetriever* retriever)
{
    mRetriever = retriever;
    return NOERROR;
}

ECode ActivityManagerTaskThumbnails::GetSubThumbnail(
    /* [in] */ Int32 index,
    /* [out] */ IBitmap** thumbnail)
{
    VALIDATE_NOT_NULL(thumbnail);

//     try {
    ECode ec = mRetriever->GetThumbnail(index, thumbnail);
    if (ec == E_REMOTE_EXCEPTION) {
        ec = NOERROR;
        *thumbnail = NULL;
    }
//     } catch (RemoteException e) {
//         return null;
//     }
    return ec;
}

} // namespace App
} // namespace Droid
} // namespace Elastos

