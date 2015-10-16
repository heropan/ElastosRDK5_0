
#include "elastos/droid/ext/frameworkext.h"
#include "elastos/droid/media/CMediaCodec.h"
#include <media/stagefright/foundation/AMessage.h>
#include <media/Media_Utils.h>
#include <media/ICrypto.h>
#include <gui/ISurface.h>
#include <gui/ISurfaceTexture.h>
#include <elastos/utility/logging/Slogger.h>
#include <elastos/utility/logging/Logger.h>

using Elastos::Utility::IObjectStringMap;
using Elastos::Utility::Logging::Slogger;
using Elastos::IO::IByteBufferHelper;
using Elastos::IO::CByteBufferHelper;
using Elastos::IO::IByteBuffer;
using Elastos::IO::ByteOrder;
using Elastos::IO::IByteOrderHelper;
using Elastos::IO::CByteOrderHelper;
using Elastos::Core::CString;
using Elastos::Core::ICharSequence;

namespace Elastos {
namespace Droid {
namespace Media {

const String CMediaCodec::TAG("CMediaCodec");

//====================================================================
//              CMediaCodec
//====================================================================
ECode CMediaCodec::CreateDecoderByType(
    /* [in] */ const String& type,
    /* [out] */ IMediaCodec** result)
{
    VALIDATE_NOT_NULL(result);
    AutoPtr<CMediaCodec> obj;
    CMediaCodec::NewByFriend(type, TRUE /* nameIsType */, FALSE /* encoder */, (CMediaCodec**)&obj);
    *result = (IMediaCodec*)obj.Get();
    REFCOUNT_ADD(*result);
    return NOERROR;
}

ECode CMediaCodec::CreateEncoderByType(
    /* [in] */ const String& type,
    /* [out] */ IMediaCodec** result)
{
    VALIDATE_NOT_NULL(result);
    AutoPtr<CMediaCodec> obj;
    CMediaCodec::NewByFriend(type, TRUE /* nameIsType */, TRUE /* encoder */, (CMediaCodec**)&obj);
    *result = (IMediaCodec*)obj.Get();
    REFCOUNT_ADD(*result);
    return NOERROR;
}

ECode CMediaCodec::CreateByCodecName(
    /* [in] */ const String& name,
    /* [out] */ IMediaCodec** result)
{
    VALIDATE_NOT_NULL(result);
    AutoPtr<CMediaCodec> obj;
    CMediaCodec::NewByFriend(name, FALSE /* nameIsType */, FALSE /* encoder */, (CMediaCodec**)&obj);
    *result = (IMediaCodec*)obj.Get();
    REFCOUNT_ADD(*result);
    return NOERROR;
}

CMediaCodec::CMediaCodec()
    : mNativeContext(0)
{
}

CMediaCodec::~CMediaCodec()
{
    NativeFinalize();
}

ECode CMediaCodec::constructor(
    /* [in] */ const String& name,
    /* [in] */ Boolean nameIsType,
    /* [in] */ Boolean encoder)
{
    NativeSetup(name, nameIsType, encoder);
    return NOERROR;
}

void CMediaCodec::Finalize()
{
    NativeFinalize();
}

ECode CMediaCodec::ReleaseResources()
{
    mNativeContext = 0;
    return NOERROR;
}

ECode CMediaCodec::Configure(
    /* [in] */ IMediaFormat* format,
    /* [in] */ ISurface* surface,
    /* [in] */ IMediaCrypto* crypto,
    /* [in] */ Int32 flags)
{
    AutoPtr<ArrayOf<String> > keys;
    AutoPtr<ArrayOf<IInterface*> > values;

    if (format != NULL) {
        AutoPtr<IObjectStringMap> formatMap;
        format->GetMap((IObjectStringMap**)&formatMap);

        Int32 tempsize;
        formatMap->GetSize(&tempsize);
        formatMap->GetKeys((ArrayOf<String>**)&keys);

        values = ArrayOf<IInterface*>::Alloc(tempsize);

        Int32 keylen = keys->GetLength();
        for (Int32 i = 0; i < keylen; i++) {
            AutoPtr<IInterface> value;
            formatMap->Get((*keys)[i],(IInterface**)&value);
            values->Set(i, value);
        }
    }

    return NativeConfigure(keys, values, surface, crypto, flags);
}

ECode CMediaCodec::NativeConfigure(
    /* [in] */ ArrayOf<String>* keys,
    /* [in] */ ArrayOf<IInterface*>* values,
    /* [in] */ ISurface* surface,
    /* [in] */ IMediaCrypto* crypto,
    /* [in] */ Int32 flags)
{
    android::sp<android::AMessage> format;
    FAIL_RETURN(Media_Utils::ConvertKeyValueArraysToMessage(keys, values, &format));

    android::sp<android::ISurfaceTexture> surfaceTexture;
    if (surface != NULL) {
        Handle32 tempsurface;
        surface->GetSurface(&tempsurface);
        android::sp<android::ISurface> surface( (android::ISurface*)  tempsurface);
        if (surface != NULL) {
            surfaceTexture = surface->getSurfaceTexture();
        }
        else {
            Slogger::E(TAG, "The surface has been released!");
            return E_ILLEGAL_ARGUMENT_EXCEPTION;
        }
    }

    android::sp<android::ICrypto> mycrypto;
    if (crypto != NULL) {
        mycrypto = JCrypto::GetCrypto();
    }

    android::sp<android::SurfaceTextureClient> client;
    if (surfaceTexture != NULL) {
        mSurfaceTextureClient = new android::SurfaceTextureClient(surfaceTexture);
    }
    else {
        mSurfaceTextureClient.clear();
    }

    android::status_t err = mCodec->configure(format, mSurfaceTextureClient, mycrypto, flags);
    return Media_Utils::ThrowExceptionAsNecessary(err);
}

ECode CMediaCodec::Start()
{
    if (mCodec == NULL) {
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    android::status_t err = mCodec->start();
    return Media_Utils::ThrowExceptionAsNecessary(err);
}

ECode CMediaCodec::Stop()
{
    mSurfaceTextureClient.clear();

    if (mCodec == NULL) {
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    android::status_t err = mCodec->stop();
    return Media_Utils::ThrowExceptionAsNecessary(err);
}

ECode CMediaCodec::Flush()
{
    if (mCodec == NULL) {
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    android::status_t err = mCodec->flush();
    return Media_Utils::ThrowExceptionAsNecessary(err);
}

ECode CMediaCodec::QueueInputBuffer(
    /* [in] */ Int32 index,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 size,
    /* [in] */ Int64 timestampUs,
    /* [in] */ Int32 flags)
{
    if (mCodec == NULL) {
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    android::AString errorDetailMsg;
    android::status_t err = mCodec->queueInputBuffer(
            index, offset, size, timestampUs, flags, &errorDetailMsg);
    return Media_Utils::ThrowExceptionAsNecessary(err);
}

ECode CMediaCodec::QueueSecureInputBuffer(
    /* [in] */ Int32 index,
    /* [in] */ Int32 offset,
    /* [in] */ IMediaCodecCryptoInfo* info,
    /* [in] */ Int64 presentationTimeUs,
    /* [in] */ Int32 flags)
{
    if (mCodec == NULL) {
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    Int32 numSubSamples;
    info->GetNumSubSamples(&numSubSamples);

    AutoPtr<ArrayOf<Int32> > numBytesOfClearDataObj;
    info->GetNumBytesOfClearData((ArrayOf<Int32>**)&numBytesOfClearDataObj);

    AutoPtr<ArrayOf<Int32> > numBytesOfEncryptedDataObj;
    info->GetNumBytesOfEncryptedData((ArrayOf<Int32>**)&numBytesOfEncryptedDataObj);

    AutoPtr<ArrayOf<Byte> > keyObj;
    info->GetKey((ArrayOf<Byte>**)&keyObj);

    AutoPtr<ArrayOf<Byte> > ivObj;
    info->GetInitVector((ArrayOf<Byte>**)&ivObj);

    Int32 mode;
    info->GetMode(&mode);

    android::status_t err = android::OK;

    android::CryptoPlugin::SubSample* subSamples = NULL;

    byte* key = NULL;
    byte* iv = NULL;

    if (numSubSamples <= 0) {
        err = -EINVAL;
    }
    else if (numBytesOfClearDataObj == NULL && numBytesOfEncryptedDataObj == NULL) {
        err = -EINVAL;
    }
    else if (numBytesOfEncryptedDataObj != NULL
        && numBytesOfEncryptedDataObj->GetLength() < numSubSamples) {
        err = -ERANGE;
    }
    else if (numBytesOfClearDataObj != NULL
        && numBytesOfClearDataObj->GetLength() < numSubSamples) {
        err = -ERANGE;
    }
    else {
        Int32* numBytesOfClearData = NULL;
        if (numBytesOfClearDataObj != NULL)
            numBytesOfClearData = numBytesOfClearDataObj->GetPayload();

        Int32 *numBytesOfEncryptedData = NULL;
        if (numBytesOfEncryptedDataObj != NULL)
            numBytesOfEncryptedData = numBytesOfEncryptedDataObj->GetPayload();

        subSamples = new android::CryptoPlugin::SubSample[numSubSamples];
        for (Int32 i = 0; i < numSubSamples; ++i) {
            if (numBytesOfClearData == NULL)
                subSamples[i].mNumBytesOfClearData = 0;
            else
                subSamples[i].mNumBytesOfClearData = numBytesOfClearData[i];

            if (numBytesOfEncryptedData == NULL)
                subSamples[i].mNumBytesOfEncryptedData = 0;
            else
                subSamples[i].mNumBytesOfEncryptedData = numBytesOfEncryptedData[i];
        }
    }//end else

    if (err == android::OK && keyObj != NULL) {
        if (keyObj->GetLength() != 16){
            err = -EINVAL;
        }
        else {
            key = keyObj->GetPayload();
        }
    }

    if (err == android::OK && ivObj != NULL) {
        if (ivObj->GetLength() != 16){
            err = -EINVAL;
        }
        else {
            iv = ivObj->GetPayload();
        }
    }

    android::AString errorDetailMsg;
    if (err == android::OK) {
        mCodec->queueSecureInputBuffer(
            index,
            offset,
            subSamples,
            numSubSamples,
            (const uint8_t *)key,
            (const uint8_t *)iv,
            (android::CryptoPlugin::Mode)mode,
            presentationTimeUs,
            flags,
            &errorDetailMsg);
    }

    if (subSamples != NULL) {
        delete[] subSamples;
        subSamples = NULL;
    }

    return Media_Utils::ThrowExceptionAsNecessary(err);
}

ECode CMediaCodec::DequeueInputBuffer(
    /* [in] */ Int64 timeoutUs,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    *result =  -1;

    if (mCodec == NULL) {
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    size_t index;
    android::status_t err = mCodec->dequeueInputBuffer(&index, timeoutUs);
    if (err == android::OK) {
        *result = index;
    }

    return Media_Utils::ThrowExceptionAsNecessary(err);
}

ECode CMediaCodec::DequeueOutputBuffer(
    /* [in] */ IMediaCodecBufferInfo* info,
    /* [in] */ Int64 timeoutUs,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    *result = -1;
    VALIDATE_NOT_NULL(info);

    if (mCodec == NULL) {
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    size_t size, offset;
    int64_t timeUs;
    uint32_t flags;
    size_t index;
    android::status_t err = mCodec->dequeueOutputBuffer(&index, &offset, &size, &timeUs, &flags, timeoutUs);
    if (err == android::OK) {
        info->Set(offset, size, timeUs, flags);
        *result = index;
    }

    return Media_Utils::ThrowExceptionAsNecessary(err);
}

ECode CMediaCodec::ReleaseOutputBuffer(
    /* [in] */ Int32 index,
    /* [in] */ Boolean render)
{
    if (mCodec == NULL) {
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    android::status_t err = render
        ? mCodec->renderOutputBufferAndRelease(index)
        : mCodec->releaseOutputBuffer(index);
    return Media_Utils::ThrowExceptionAsNecessary(err);
}

ECode CMediaCodec::GetOutputFormat(
    /* [out] */ IMediaFormat** result)
{
    VALIDATE_NOT_NULL(result)
    *result = NULL;

    if (mCodec == NULL) {
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    android::sp<android::AMessage> msg;
    android::status_t err = mCodec->getOutputFormat(&msg);
    if (err != android::OK) {
        return Media_Utils::ThrowExceptionAsNecessary(err);
    }

    AutoPtr<IObjectStringMap> tempmap;
    FAIL_RETURN(Media_Utils::ConvertMessageToMap(msg, (IObjectStringMap**)&tempmap));
    return CMediaFormat::New(tempmap, (IMediaFormat**)result);
}

ECode CMediaCodec::GetInputBuffers(
    /* [out] */ ArrayOf<IByteBuffer*>** result)
{
    VALIDATE_NOT_NULL(result);
    *result = NULL;

    if (mCodec == NULL) {
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    AutoPtr<ArrayOf<IByteBuffer*> > temp;
    GetBuffers(TRUE /* input */, (ArrayOf<IByteBuffer*>**)&temp);
    *result = temp;
    REFCOUNT_ADD(*result);
    return NOERROR;
}

ECode CMediaCodec::GetOutputBuffers(
    /* [out] */ ArrayOf<IByteBuffer*>** result)
{
    VALIDATE_NOT_NULL(result);
    *result = NULL;

    if (mCodec == NULL) {
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    AutoPtr<ArrayOf<IByteBuffer*> > temp;
    GetBuffers(FALSE /* input */, (ArrayOf<IByteBuffer*>**)&temp);
    *result = temp;
    REFCOUNT_ADD(*result);
    return NOERROR;
}

ECode CMediaCodec::SetVideoScalingMode(
    /* [in] */ Int32 mode)
{
    if (mCodec == NULL) {
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    if (mode != NATIVE_WINDOW_SCALING_MODE_SCALE_TO_WINDOW
            && mode != NATIVE_WINDOW_SCALING_MODE_SCALE_CROP) {
        Slogger::E(TAG, "mode error");
        return E_INVALID_ARGUMENT;
    }

    if (mSurfaceTextureClient != NULL) {
        native_window_set_scaling_mode(mSurfaceTextureClient.get(),mode);
    }

    return NOERROR;
}

ECode CMediaCodec::GetBuffers(
    /* [in] */ Boolean input,
    /* [out, callee] */ ArrayOf<IByteBuffer*>** result)
{
    VALIDATE_NOT_NULL(result);
    *result = NULL;

    if (mCodec == NULL) {
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    android::Vector<android::sp<android::ABuffer> > buffers;
    android::status_t err;

    if (input == TRUE) {
        err = mCodec->getInputBuffers(&buffers);
    }
    else {
        err = mCodec->getOutputBuffers(&buffers);
    }

    if (err != android::OK) {
        Slogger::E(TAG, "get buffer error");
        return Media_Utils::ThrowExceptionAsNecessary(err);
    }

    AutoPtr<ArrayOf<IByteBuffer*> > bufArray = ArrayOf<IByteBuffer*>::Alloc(buffers.size());

    AutoPtr<IByteOrderHelper> helper;
    CByteOrderHelper::AcquireSingleton((IByteOrderHelper**)&helper );
    ByteOrder nativeByteOrderObj;
    helper->GetNativeOrder(&nativeByteOrderObj);

    AutoPtr<IByteBufferHelper> bbhelper;
    CByteBufferHelper::AcquireSingleton((IByteBufferHelper**)&bbhelper);

    for (Int32 i = 0; i < buffers.size(); ++i) {
        const android::sp<android::ABuffer> &buffer = buffers.itemAt(i);

        AutoPtr<IByteBuffer> bytebuffer;
        // bbhelper->AllocateDirect(buffer->capacity() , (IByteBuffer**)&bytebuffer);

        ArrayOf<Byte> bytes((Byte*)buffer->base(), buffer->capacity());
        bbhelper->WrapArray(&bytes , (IByteBuffer**)&bytebuffer);
        bytebuffer->SetOrder(nativeByteOrderObj);

        bufArray->Set(i, bytebuffer);
    }

    *result = bufArray;
    REFCOUNT_ADD(*result);
    return NOERROR;
}

void CMediaCodec::NativeInit()
{
    //Do nothing
    return;
}

void CMediaCodec::NativeSetup(
    /* [in] */ const String& name,
    /* [in] */ Boolean nameIsType,
    /* [in] */ Boolean encoder)
{
    const char *tmp = name.string();
    mLooper = new android::ALooper;
    mLooper->setName("MediaCodec_looper");
    mLooper->start(
        false,  // runOnCallingThread
        false,  // canCallJava
        android::PRIORITY_DEFAULT);

    if (nameIsType){
        mCodec = android::MediaCodec::CreateByType(mLooper,name,encoder);
    }
    else{
        mCodec = android::MediaCodec::CreateByComponentName(mLooper,name);
    }

    return;
}

void CMediaCodec::NativeFinalize()
{
    //Do nothing
    ReleaseResources();
    return;
}

} // namespace Media
} // namepsace Droid
} // namespace Elastos
