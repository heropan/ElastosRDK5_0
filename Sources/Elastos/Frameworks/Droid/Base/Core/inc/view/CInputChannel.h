
#ifndef __CINPUTCHANNEL_H__
#define __CINPUTCHANNEL_H__

#include "_CInputChannel.h"
#include "view/NativeInputChannel.h"

namespace Elastos {
namespace Droid {
namespace View {

CarClass(CInputChannel)
{
private:
    static const char* TAG;

    static const Boolean DEBUG = FALSE;

public:
    /**
     * Creates an uninitialized input channel.
     * It can be initialized by reading from a Parcel or by transferring the state of
     * another input channel into this one.
     */
    CInputChannel();

    ~CInputChannel();

    CARAPI constructor();

    static CARAPI OpenInputChannelPair(
        /* [in] */ const String& name,
        /* [out] */ CInputChannel** inputChannel0,
        /* [out] */ CInputChannel** inputChannel1);

    /**
     * Gets the name of the input channel.
     * @return The input channel name.
     */
    CARAPI GetName(
        /* [out] */ String* name);

    /**
     * Disposes the input channel.
     * Explicitly releases the reference this object is holding on the input channel.
     * When all references are released, the input channel will be closed.
     */
    CARAPI Dispose();

    CARAPI TransferTo(
        /* [in] */ IInputChannel* outParameter);

    CARAPI GetNativeInputChannel(
        /* [out] */ Handle32* nativeInputChannel);

    CARAPI ReadFromParcel(
        /* [in] */ IParcel *source);

    CARAPI WriteToParcel(
        /* [in] */ IParcel *dest);

private:
    NativeInputChannel* mNative; // used by native code
};

} // namespace View
} // namepsace Droid
} // namespace Elastos

#endif //__CINPUTCHANNEL_H__
