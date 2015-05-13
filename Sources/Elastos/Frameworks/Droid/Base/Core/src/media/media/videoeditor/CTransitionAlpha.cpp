
#include "CTransitionAlpha.h"
#include "graphics/CBitmapFactoryOptions.h"
#include "graphics/CBitmapFactory.h"

using Elastos::IO::IFile;
using Elastos::IO::CFile;
using Elastos::IO::IFileOutputStream;
using Elastos::IO::CFileOutputStream;
using Elastos::IO::IDataOutputStream;
using Elastos::IO::CDataOutputStream;
using Elastos::IO::IByteBufferHelper;
using Elastos::IO::CByteBufferHelper;
using Elastos::IO::IByteBuffer;
using Elastos::IO::IInt32Buffer;
using Elastos::Droid::Graphics::IBitmapFactoryOptions;
using Elastos::Droid::Graphics::CBitmapFactoryOptions;
using Elastos::Droid::Graphics::IBitmapFactory;
using Elastos::Droid::Graphics::CBitmapFactory;

namespace Elastos {
namespace Droid {
namespace Media {
namespace Videoeditor {

ITRANSITION_METHODS_IMPL(CTransitionAlpha, Transition);

PInterface CTransitionAlpha::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_Transition) {
        return reinterpret_cast<PInterface>((Transition*)this);
    }
    return _CTransitionAlpha::Probe(riid);
}

CTransitionAlpha::CTransitionAlpha()
    : mMaskFilename(NULL)
    , mBlendingPercent(0)
    , mIsInvert(FALSE)
    , mWidth(0)
    , mHeight(0)
    , mRGBMaskFile(NULL)
{
}

CTransitionAlpha::CTransitionAlpha(
    /* [in] */ const String& transitionId,
    /* [in] */ IMediaItem* afterMediaItem,
    /* [in] */ IMediaItem* beforeMediaItem,
    /* [in] */ Int64 durationMs,
    /* [in] */ Int32 behavior,
    /* [in] */ const String& maskFilename,
    /* [in] */ Int32 blendingPercent,
    /* [in] */ Boolean invert)
    : mMaskFilename(NULL)
    , mBlendingPercent(0)
    , mIsInvert(FALSE)
    , mWidth(0)
    , mHeight(0)
    , mRGBMaskFile(NULL)
{
}

ECode CTransitionAlpha::constructor(
    /* [in] */ const String& transitionId,
    /* [in] */ IMediaItem* afterMediaItem,
    /* [in] */ IMediaItem* beforeMediaItem,
    /* [in] */ Int64 durationMs,
    /* [in] */ Int32 behavior,
    /* [in] */ const String& maskFilename,
    /* [in] */ Int32 blendingPercent,
    /* [in] */ Boolean invert)
{
    Init(transitionId, afterMediaItem, beforeMediaItem, durationMs, behavior);

    /**
     * Generate a RGB file for the supplied mask file
     */
    AutoPtr<IBitmapFactoryOptions> dbo;
    CBitmapFactoryOptions::New((IBitmapFactoryOptions**)&dbo);
    dbo->SetInJustDecodeBounds(TRUE);
    AutoPtr<IFile> file;
    CFile::New(maskFilename, (IFile**)&file);
    Boolean b;
    file->Exists(&b);
    if (!b) {
        //throw new IllegalArgumentException("File not Found " + maskFilename);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    AutoPtr<IBitmapFactory> bitmapFactory;
    CBitmapFactory::AcquireSingleton((IBitmapFactory**)&bitmapFactory);
    AutoPtr<IBitmap> tmp;
    bitmapFactory->DecodeFile(maskFilename, dbo, (IBitmap**)&tmp);

    dbo->GetOutWidth(&mWidth);
    dbo->GetOutHeight(&mHeight);

    String path;
    mNativeHelper->GetProjectPath(&path);
    mRGBMaskFile = path + "/" + "mask" + transitionId + ".rgb";

    AutoPtr<IFileOutputStream> fl;

    //try{
        CFileOutputStream::New(mRGBMaskFile, (IFileOutputStream**)&fl);
    //} catch (IOException e) {
        /* catch IO exception */
    //}
    AutoPtr<IDataOutputStream> dos;
    CDataOutputStream::New(fl, (IDataOutputStream**)&dos);

    if (fl != NULL) {
        /**
         * Write to rgb file
         */
        AutoPtr<IBitmap> imageBitmap;
        bitmapFactory->DecodeFileEx(maskFilename, (IBitmap**)&imageBitmap);
        AutoPtr<ArrayOf<Int32> > framingBuffer = ArrayOf<Int32>::Alloc(mWidth);

        AutoPtr<IByteBufferHelper> helper;
        CByteBufferHelper::AcquireSingleton((IByteBufferHelper**)&helper);
        AutoPtr<IByteBuffer> byteBuffer;
        helper->Allocate(framingBuffer->GetLength() * 4, (IByteBuffer**)&byteBuffer);
        AutoPtr<IInt32Buffer> intBuffer;

        AutoPtr<ArrayOf<Byte> > array;
        byteBuffer->GetArray((ArrayOf<Byte>**)&array);
        Int32 tmp = 0;
        while (tmp < mHeight) {
            imageBitmap->GetPixels(*framingBuffer, 0, mWidth, 0, tmp,mWidth, 1);
            byteBuffer->AsInt32Buffer((IInt32Buffer**)&intBuffer);
            intBuffer->PutInt32sEx(*framingBuffer, 0, mWidth);
            //try {
                dos->WriteBytes(*array);
            //} catch (IOException e) {
                /* catch file write error */
            //}
            tmp += 1;
        }

        imageBitmap->Recycle();
        //try{
            fl->Close();
        //}catch (IOException e) {
            /* file close error */
        //}
    }

    /**
     * Capture the details
     */
    mMaskFilename = maskFilename;
    mBlendingPercent = blendingPercent;
    mIsInvert = invert;

    return NOERROR;
}

ECode CTransitionAlpha::GetRGBFileWidth(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);

    *result = mWidth;
    return NOERROR;
}

ECode CTransitionAlpha::GetRGBFileHeight(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);

    *result = mHeight;
    return NOERROR;
}

ECode CTransitionAlpha::GetPNGMaskFilename(
    /* [out] */ String* result)
{
    VALIDATE_NOT_NULL(result);

    *result = mRGBMaskFile;
    return NOERROR;
}

/**
 * Get the blending percentage
 *
 * @return The blending percentage
 */
 CTransitionAlpha::GetBlendingPercent(
    /* [out] */ Int32* result)
 {
    VALIDATE_NOT_NULL(result);

    *result = mBlendingPercent;
    return NOERROR;
}

/**
 * Get the filename of the mask.
 *
 * @return The mask filename
 */
ECode CTransitionAlpha::GetMaskFilename(
    /* [out] */ String* result)
{
    VALIDATE_NOT_NULL(result);

    *result = mMaskFilename;
    return NOERROR;
}

/**
 * Check if the alpha blending direction is inverted.
 *
 * @return true if the direction of the alpha blending is inverted
 */
ECode CTransitionAlpha::IsInvert(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    *result = mIsInvert;
    return NOERROR;
}

ECode CTransitionAlpha::Generate()
{
    return Transition::Generate();
}

} // namespace Videoeditor
} // namespace Media
} // namepsace Droid
} // namespace Elastos
