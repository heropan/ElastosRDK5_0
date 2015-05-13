
#ifndef __CRECOGNIZERHELPER_H__
#define __CRECOGNIZERHELPER_H__

#include "_CRecognizerHelper.h"

using Libcore::ICU::ILocale;

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

#endif // __CRECOGNIZERHELPER_H__
