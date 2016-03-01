
#include "Elastos.Droid.Accounts.h"
#include "Elastos.Droid.App.h"
#include "Elastos.Droid.Content.h"
#include "Elastos.Droid.Location.h"
#include "Elastos.Droid.Os.h"
#include "Elastos.Droid.View.h"
#include "Elastos.Droid.Widget.h"
#include "elastos/droid/ext/frameworkext.h"
#include "elastos/droid/view/InputChannel.h"
#include "elastos/droid/view/CInputChannel.h"
#include "elastos/droid/view/NativeInputChannel.h"
#include <elastos/utility/logging/Slogger.h>
#include <input/InputTransport.h>

using Elastos::Utility::Logging::Slogger;
using android::String8;
using android::sp;
using android::status_t;

namespace Elastos {
namespace Droid {
namespace View {

CAR_INTERFACE_IMPL_2(InputChannel, Object, IInputChannel, IParcelable)

typedef void (*InputChannelObjDisposeCallback)(
    /* [in] */ IInputChannel* inputChannelObj,
    /* [in] */ const android::sp<android::InputChannel>& inputChannel,
    /* [in] */ void* data);

#if 0
class NativeInputChannel
{
public:
    NativeInputChannel(
        /* [in] */ const sp<android::InputChannel>& inputChannel);

    ~NativeInputChannel();

    inline android::sp<android::InputChannel> getInputChannel()
    { return mInputChannel; }

    void setDisposeCallback(
        /* [in] */ InputChannelObjDisposeCallback callback,
        /* [in] */ void* data);

    void invokeAndRemoveDisposeCallback(
        /* [in] */ IInputChannel* obj);

private:
    android::sp<android::InputChannel> mInputChannel;
    InputChannelObjDisposeCallback mDisposeCallback;
    void* mDisposeData;
};

// ----------------------------------------------------------------------------

NativeInputChannel::NativeInputChannel(
        /* [in] */ const sp<android::InputChannel>& inputChannel) :
    mInputChannel(inputChannel), mDisposeCallback(NULL) {
}

NativeInputChannel::~NativeInputChannel() {
}

void NativeInputChannel::setDisposeCallback(
        /* [in] */ InputChannelObjDisposeCallback callback,
        /* [in] */ void* data)
{
    mDisposeCallback = callback;
    mDisposeData = data;
}

void NativeInputChannel::invokeAndRemoveDisposeCallback(
        /* [in] */ IInputChannel* obj)
{
    if (mDisposeCallback) {
        mDisposeCallback(obj, mInputChannel, mDisposeData);
        mDisposeCallback = NULL;
        mDisposeData = NULL;
    }
}
#endif

// ----------------------------------------------------------------------------

const char* InputChannel::TAG = "InputChannel";
const Boolean InputChannel::DEBUG;

InputChannel::InputChannel() :
    mNative(0)
{
}

InputChannel::~InputChannel()
{
    Dispose();
}

ECode InputChannel::constructor()
{
    return NOERROR;
}

ECode InputChannel::OpenInputChannelPair(
    /* [in] */ const String& _name,
    /* [out, callee] */ ArrayOf<IInputChannel*>** icp)
{
    VALIDATE_NOT_NULL(icp);

    if (_name.IsNull()) {
        SLOGGERE(TAG, "OpenInputChannelPair: name must not be null")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    if (DEBUG) {
        SLOGGERD(TAG, "OpenInputChannelPair: Opening input channel pair '%s'", _name.string())
    }

    return NativeOpenInputChannelPair(_name, icp);
}

ECode InputChannel::GetName(
    /* [out] */ String* name)
{
    VALIDATE_NOT_NULL(name);

    ECode ec = NativeGetName(name);
    if (name->IsNull())
        *name = String("uninitialized");

    return ec;
}

ECode InputChannel::Dispose()
{
    return NativeDispose(FALSE);
}

ECode InputChannel::TransferTo(
    /* [in] */ IInputChannel* outParameter)
{
    if (outParameter == NULL) {
        SLOGGERE(TAG, "outParameter must not be null.");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    return NativeTransferTo(outParameter);
}

ECode InputChannel::ReadFromParcel(
    /* [in] */ IParcel *in)
{
    if (in == NULL) {
        SLOGGERD("InputChannel", "ReadFromParcel: in must not be null")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    return NativeReadFromParcel(in);
}

ECode InputChannel::WriteToParcel(
    /* [in] */ IParcel *dest)
{
    if (dest == NULL) {
        SLOGGERE(TAG, "dest must not be null.");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    return NativeWriteToParcel(dest);

    // if ((flags & PARCELABLE_WRITE_RETURN_VALUE) != 0) {
    //     dispose();
    // }
}

ECode InputChannel::Dup(
    /* [out] */ IInputChannel** target)
{
    CInputChannel::New(target);
    NativeDup(*target);
    return NOERROR;
}

ECode InputChannel::GetNativeInputChannel(
    /* [out] */ Handle64* ptr)
{
    VALIDATE_NOT_NULL(ptr);
    *ptr = mNative;
    return NOERROR;
}

ECode InputChannel::NativeOpenInputChannelPair(
    /* [in] */ const String& nameStr,
    /* [out] */ ArrayOf<IInputChannel*>** pair)
{
    const char* nameChars = nameStr.string();
    String8 name(nameChars);

    sp<android::InputChannel> serverChannel;
    sp<android::InputChannel> clientChannel;
    status_t result = android::InputChannel::openInputChannelPair(name, serverChannel, clientChannel);

    if (result) {
        String8 message;
        message.appendFormat("Could not open input channel pair.  status=%d", result);
        *pair = NULL;
        return E_RUNTIME_EXCEPTION;
    }

    AutoPtr<ArrayOf<IInputChannel*> > rArray = ArrayOf<IInputChannel*>::Alloc(2);
    NativeInputChannel* tmpServer = new NativeInputChannel(serverChannel);
    AutoPtr<IInputChannel> serverChannelObj;
    CInputChannel::New((IInputChannel**)&serverChannelObj);
    ((InputChannel*)serverChannelObj.Get())->mNative = reinterpret_cast<Handle64>(tmpServer);

    NativeInputChannel* tmpClient = new NativeInputChannel(clientChannel);
    AutoPtr<IInputChannel> clientChannelObj;
    CInputChannel::New((IInputChannel**)&clientChannelObj);
    ((InputChannel*)clientChannelObj.Get())->mNative = reinterpret_cast<Handle64>(tmpClient);



    rArray->Set(0, serverChannelObj);
    rArray->Set(1, clientChannelObj);
    *pair = rArray;
    REFCOUNT_ADD(*pair)
    return NOERROR;
}

ECode InputChannel::NativeDispose(
    /* [in] */ Boolean finalized)
{
    NativeInputChannel* nativeInputChannel = reinterpret_cast<NativeInputChannel*>(mNative);
    if (nativeInputChannel) {
        if (finalized) {
            ALOGW("Input channel object '%s' was finalized without being disposed!",
                    nativeInputChannel->getInputChannel()->getName().string());
        }

        nativeInputChannel->invokeAndRemoveDisposeCallback(this);

        mNative = 0;
        delete nativeInputChannel;
    }
    return NOERROR;
}

ECode InputChannel::NativeTransferTo(
    /* [in] */ IInputChannel* other)
{
    InputChannel* otherObj = (InputChannel*)other;

    if (otherObj->mNative != 0) {
        SLOGGERE("InputChannel", "Other object already has a native input channel.")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    otherObj->mNative = mNative;
    mNative = 0;
    return NOERROR;
}

ECode InputChannel::NativeReadFromParcel(
    /* [in] */ IParcel* in)
{
    if (mNative != 0) {
        SLOGGERE("InputChannel",
                "This object already has a native input channel.");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    if (in != NULL) {
        Boolean isInitialized = FALSE;
        in->ReadBoolean(&isInitialized);
        if (isInitialized) {
            String rStr;
            in->ReadString(&rStr);
            const char* tmpStr = rStr.string();

            String8 name = String8(tmpStr);
            Int32 rawFd;
            in->ReadFileDescriptor(&rawFd);
            Int32 dupFd = dup(rawFd);
            if (dupFd < 0) {
                ALOGE("Error %d dup channel fd %d.", errno, rawFd);
                ALOGE("Could not read input channel file descriptors from parcel.");
                return E_RUNTIME_EXCEPTION;
            }

            android::InputChannel* inputChannel = new android::InputChannel(name, dupFd);
            NativeInputChannel* nativeInputChannel = new NativeInputChannel(inputChannel);
            mNative = reinterpret_cast<Handle64>(nativeInputChannel);
        }
    }
    return NOERROR;
}

ECode InputChannel::NativeWriteToParcel(
    /* [in] */ IParcel* dest)
{

    if (0 != mNative) {
        NativeInputChannel* pNative = reinterpret_cast<NativeInputChannel*>(mNative);
        android::sp<android::InputChannel> inputChannel = pNative->getInputChannel();
        dest->WriteBoolean(TRUE);
        dest->WriteString(String(inputChannel->getName().string()));
        dest->WriteDupFileDescriptor(inputChannel->getFd());
    }
    else {
        dest->WriteInt32(FALSE);
    }

    // if ((flags & PARCELABLE_WRITE_RETURN_VALUE) != 0) {
    //     dispose();
    // }

    return NOERROR;
}

ECode InputChannel::NativeDup(
    /* [in] */ IInputChannel* otherObj)
{
    NativeInputChannel* nativeInputChannel = reinterpret_cast<NativeInputChannel*>(mNative);
    if (nativeInputChannel) {
        InputChannel* impl= (InputChannel*)otherObj;
        NativeInputChannel* otherChannel = new NativeInputChannel(nativeInputChannel->getInputChannel()->dup());
        impl->mNative =reinterpret_cast<Handle64>(otherChannel);
    }
    return NOERROR;
}

ECode InputChannel::NativeGetName(
    /* [out] */ String* name)
{
    if (0 == mNative) {
        *name = NULL;
    }
    else {
        NativeInputChannel* pNative = reinterpret_cast<NativeInputChannel*>(mNative);
        *name = String(pNative->getInputChannel()->getName().string());
    }
    return NOERROR;
}


} // namespace View
} // namepsace Droid
} // namespace Elastos
