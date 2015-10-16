
#include "elastos/droid/speech/srec/CRecognizerHelper.h"
#include "elastos/droid/speech/srec/CRecognizer.h"
#include "elastos/droid/ext/frameworkext.h"

namespace Elastos {
namespace Droid {
namespace Speech {
namespace Srec {

ECode CRecognizerHelper::GetConfigDir(
    /* [in] */ ILocale* locale,
    /* [out] */ String* ret)
{
    VALIDATE_NOT_NULL(ret);
    *ret=CRecognizer::GetConfigDir(locale);
    return NOERROR;
}

ECode CRecognizerHelper::EventToString(
    /* [in] */ Int32 event,
    /* [out] */ String* ret)
{
    VALIDATE_NOT_NULL(ret);
    *ret=CRecognizer::EventToString(event);
    return NOERROR;
}

}//namespace Srec
}//namespace Speech
}//namespace Droid
}//namespace Elastos

