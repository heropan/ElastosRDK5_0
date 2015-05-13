
#ifndef __CRECOGNIZERINTENT_H__
#define __CRECOGNIZERINTENT_H__

#include "_CRecognizerIntent.h"

#include "Elastos.Droid.Core_server.h"

using Elastos::Droid::Content::IContext;
using Elastos::Droid::Content::IIntent;

namespace Elastos {
namespace Droid {
namespace Speech {

CarClass(CRecognizerIntent)
{
public:
    CARAPI GetVoiceDetailsIntent(
        /* [in] */ IContext * pContext,
        /* [out] */ IIntent ** ppRet);

private:
    // TODO: Add your private member variables here.
};

}//namespace Speech
}//namespace Droid
}//namespace Elastos

#endif // __CRECOGNIZERINTENT_H__
