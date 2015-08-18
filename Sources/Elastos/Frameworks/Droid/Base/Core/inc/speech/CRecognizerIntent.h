
#ifndef __ELASTOS_DROID_SPEECH_CRECOGNIZERINTENT_H__
#define __ELASTOS_DROID_SPEECH_CRECOGNIZERINTENT_H__

#include "_Elastos_Droid_Speech_CRecognizerIntent.h"

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

#endif // __ELASTOS_DROID_SPEECH_CRECOGNIZERINTENT_H__
