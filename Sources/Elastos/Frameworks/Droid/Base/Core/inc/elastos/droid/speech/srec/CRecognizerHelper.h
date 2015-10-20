
#ifndef __ELASTOS_DROID_SPEECH_SREC_CRECOGNIZERHELPER_H__
#define __ELASTOS_DROID_SPEECH_SREC_CRECOGNIZERHELPER_H__

#include "_Elastos_Droid_Speech_Srec_CRecognizerHelper.h"

using Elastos::Utility::ILocale;

namespace Elastos {
namespace Droid {
namespace Speech {
namespace Srec {

CarClass(CRecognizerHelper)
{
public:
    CARAPI GetConfigDir(
        /* [in] */ ILocale* locale,
        /* [out] */ String* ret);

    CARAPI EventToString(
        /* [in] */ Int32 event,
        /* [out] */ String* ret);

};

}//namespace Srec
}//namespace Speech
}//namespace Droid
}//namespace Elastos

#endif // __ELASTOS_DROID_SPEECH_SREC_CRECOGNIZERHELPER_H__
