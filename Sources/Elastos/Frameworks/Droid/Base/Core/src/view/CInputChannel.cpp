
#include "ext/frameworkext.h"
#include "view/CInputChannel.h"
#include <elastos/utility/logging/Slogger.h>

//using Elastos::Core;
using Elastos::Utility::Logging::Slogger;

namespace Elastos {
namespace Droid {
namespace View {

const char* CInputChannel::TAG = "InputChannel";
const Boolean CInputChannel::DEBUG;

CInputChannel::CInputChannel() :
    mNative(NULL)
{
}

CInputChannel::~CInputChannel()
{
    Dispose();
}

ECode CInputChannel::constructor()
{
    return NOERROR;
}

ECode CInputChannel::OpenInputChannelPair(
    /* [in] */ const String& _name,
    /* [out] */ CInputChannel** inputChannel0,
    /* [out] */ CInputChannel** inputChannel1)
{
    VALIDATE_NOT_NULL(inputChannel0);
    VALIDATE_NOT_NULL(inputChannel1);

    if (_name.IsNull()) {
        Slogger::E(TAG, "name must not be null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    if (DEBUG) {
        Slogger::D(TAG, "Opening input channel pair '%s'", _name.string());
    }

    android::String8 name((const char*)_name);

    android::sp<android::InputChannel> serverChannel;
    android::sp<android::InputChannel> clientChannel;
    android::status_t result = android::InputChannel::openInputChannelPair(
            name, serverChannel, clientChannel);

    if (result) {
        Slogger::E(TAG, "Could not open input channel pair.  status=%d" + result);
        return E_RUNTIME_EXCEPTION;
    }

    FAIL_RETURN(CInputChannel::NewByFriend(inputChannel0));
    (*inputChannel0)->mNative = new NativeInputChannel(serverChannel);
    FAIL_RETURN(CInputChannel::NewByFriend(inputChannel1));
    (*inputChannel1)->mNative = new NativeInputChannel(clientChannel);

    return NOERROR;
}

ECode CInputChannel::GetName(
    /* [out] */ String* name)
{
    VALIDATE_NOT_NULL(name);
    if (!mNative) {
        *name = NULL;
    }
    else {
        *name = String(mNative->getInputChannel()->getName().string());
    }

    if (name->IsNull())
        *name = String("uninitialized");

    return NOERROR;
}

ECode CInputChannel::Dispose()
{
    if (mNative) {
        mNative->invokeAndRemoveDisposeCallback(this);

        delete mNative;
        mNative = NULL;
    }
    return NOERROR;
}

ECode CInputChannel::TransferTo(
    /* [in] */ IInputChannel* outParameter)
{
    if (outParameter == NULL) {
        Slogger::E(TAG, "outParameter must not be null.");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    CInputChannel* obj = (CInputChannel*)outParameter;
    if (obj->mNative != NULL) {
        Slogger::E(TAG, "Other object already has a native input channel.");
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    obj->mNative = mNative;
    mNative = NULL;
    return NOERROR;
}

ECode CInputChannel::GetNativeInputChannel(
    /* [out] */ Handle32* nativeInputChannel)
{
    VALIDATE_NOT_NULL(nativeInputChannel);
    *nativeInputChannel = (Handle32)mNative;
    return NOERROR;
}

ECode CInputChannel::ReadFromParcel(
    /* [in] */ IParcel *source)
{
    if (source == NULL) {
        Slogger::E(TAG, "source must not be null.");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    if (mNative != NULL) {
        Slogger::E(TAG, "This object already has a native input channel.");
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    Int32 isInitialized;
    source->ReadInt32(&isInitialized);
    if (isInitialized) {
        String _name;
        source->ReadString(&_name);
        Int32 _fd;
        source->ReadFileDescriptor(&_fd);
        int32_t fd = dup(_fd);
        if (fd < 0) {
            Slogger::E(TAG, "Could not read input channel file descriptors from parcel.");
            return E_RUNTIME_EXCEPTION;
        }

        android::String8 name((const char*)_name);
        android::InputChannel* inputChannel = new android::InputChannel(name, fd);
        NativeInputChannel* nativeInputChannel = new NativeInputChannel(inputChannel);

        mNative = nativeInputChannel;
    }
    return NOERROR;
}

ECode CInputChannel::WriteToParcel(
    /* [in] */ IParcel *dest)
{
    if (dest == NULL) {
        Slogger::E(TAG, "dest must not be null.");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    if (mNative) {
        android::sp<android::InputChannel> inputChannel = mNative->getInputChannel();

        dest->WriteInt32(1);
        dest->WriteString(String(inputChannel->getName().string()));
        dest->WriteDupFileDescriptor(inputChannel->getFd());
    }
    else {
        dest->WriteInt32(0);
    }

    // if ((flags & PARCELABLE_WRITE_RETURN_VALUE) != 0) {
    //     dispose();
    // }

    return NOERROR;
}

} // namespace View
} // namepsace Droid
} // namespace Elastos
