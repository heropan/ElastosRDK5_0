
#ifndef __CKEYSTORECALLBACKHANDLERPROTECTION_H__
#define __CKEYSTORECALLBACKHANDLERPROTECTION_H__

#include "_CKeyStoreCallbackHandlerProtection.h"

using Elastosx::Security::Auth::Callback::ICallbackHandler;

namespace Elastos {
namespace Security {

CarClass(CKeyStoreCallbackHandlerProtection)
{
public:
    CARAPI GetCallbackHandler(
        /* [out] */ ICallbackHandler **handler);

    CARAPI constructor(
        /* [in] */ ICallbackHandler *handler);

private:
    // Store CallbackHandler
    const AutoPtr<ICallbackHandler> mCallbackHandler;
};

}
}

#endif // __CKEYSTORECALLBACKHANDLERPROTECTION_H__
