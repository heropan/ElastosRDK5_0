#include "elastos/droid/text/method/AllCapsTransformationMethod.h"
#include "elastos/droid/content/res/CConfiguration.h"
#include <elastos/utility/logging/Logger.h>

using Elastos::Core::CString;
using Elastos::Utility::Logging::Logger;
using Elastos::Droid::Content::Res::IConfiguration;
using Elastos::Droid::Content::Res::CConfiguration;
using Elastos::Droid::Content::Res::IResources;

namespace Elastos {
namespace Droid {
namespace Text {
namespace Method {

const String AllCapsTransformationMethod::TAG = String("AllCapsTransformationMethod");

AllCapsTransformationMethod::AllCapsTransformationMethod()
    : mEnabled(FALSE), mLocale(NULL)
{}

AllCapsTransformationMethod::~AllCapsTransformationMethod()
{}

CAR_INTERFACE_IMPL_3(AllCapsTransformationMethod, Object, IAllCapsTransformationMethod, ITransformationMethod2, ITransformationMethod)

ECode AllCapsTransformationMethod::constructor(
    /* [in] */ IContext* context)
{
    AutoPtr<IResources> res;
    context->GetResources((IResources**)&res);
    AutoPtr<IConfiguration> config;
    res->GetConfiguration((IConfiguration**)&config);
    config->GetLocale((ILocale**)&mLocale);
    mEnabled = FALSE;
    return NOERROR;
}

ECode AllCapsTransformationMethod::GetTransformation(
    /* [in] */ ICharSequence* source,
    /* [in] */ IView* view,
    /* [out] */ ICharSequence** ret)
{
    VALIDATE_NOT_NULL(ret)
    if (mEnabled) {
        if(source != NULL) {
            //Java:    source.toString().toUpperCase(mLocale);
            String strSource;
            source->ToString(&strSource);
            AutoPtr<ICharSequence> cs;
            CString::New(strSource.ToUpperCase(), (ICharSequence**)&cs);
            *ret = cs;
            REFCOUNT_ADD(*ret);
            return NOERROR;
        } else {
            *ret = NULL;
            return NOERROR;
        }
    }
    Logger::W(TAG, String("Caller did not enable length changes; not transforming text\n") );
    *ret = source;
    REFCOUNT_ADD(*ret);
    return NOERROR;
}

ECode AllCapsTransformationMethod::OnFocusChanged(
    /* [in] */ IView* view,
    /* [in] */ ICharSequence* sourceText,
    /* [in] */ Boolean focused,
    /* [in] */ Int32 direction,
    /* [in] */ IRect* previouslyFocusedRect)
{}

ECode AllCapsTransformationMethod::SetLengthChangesAllowed(
    /* [in] */ Boolean allowLengthChanges)
{
    mEnabled = allowLengthChanges;
    return NOERROR;
}



} // namespace Method
} // namespace Text
} // namepsace Droid
} // namespace Elastos
