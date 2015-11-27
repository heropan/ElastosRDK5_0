
#include "elastos/droid/speech/srec/CRecognizerHelper.h"
#include "elastos/droid/speech/srec/CRecognizer.h"
#include "elastos/droid/ext/frameworkext.h"

namespace Elastos {
namespace Droid {
namespace Speech {
namespace Srec {

CAR_SINGLETON_IMPL(CRecognizerHelper)

CAR_INTERFACE_IMPL(CRecognizerHelper, Singleton, IRecognizerHelper)

ECode CRecognizerHelper::GetConfigDir(
    /* [in] */ ILocale* locale,
    /* [out] */ String* ret)
{
    VALIDATE_NOT_NULL(ret);
    *ret = Recognizer::GetConfigDir(locale);
    return NOERROR;
}

ECode CRecognizerHelper::EventToString(
    /* [in] */ Int32 event,
    /* [out] */ String* ret)
{
    VALIDATE_NOT_NULL(ret);
    *ret = Recognizer::EventToString(event);
    return NOERROR;
}

} // namespace Srec
} // namespace Speech
} // namespace Droid
} // namespace Elastos
