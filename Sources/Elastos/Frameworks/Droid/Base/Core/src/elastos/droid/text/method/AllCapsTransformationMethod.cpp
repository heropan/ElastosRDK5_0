#include "text/method/AllCapsTransformationMethod.h"
#include "content/res/CConfiguration.h"
#include <elastos/utility/logging/Logger.h>

using Elastos::Droid::Content::Res::IConfiguration;
using Elastos::Droid::Content::Res::CConfiguration;
using Elastos::Droid::Content::Res::IResources;
using Elastos::Core::CString;
using Elastos::Utility::Logging::Logger;

namespace Elastos {
namespace Droid {
namespace Text {
namespace Method {

const String AllCapsTransformationMethod::TAG = "AllCapsTransformationMethod";

CAR_INTERFACE_IMPL_3(AllCapsTransformationMethod, Object, IAllCapsTransformationMethod, ITransformationMethod2, ITransformationMethod)

AllCapsTransformationMethod::AllCapsTransformationMethod()
    : mEnabled(FALSE)
    , mLocale(NULL)
{}

AllCapsTransformationMethod::~AllCapsTransformationMethod()
{}

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
    /* [out] */ ICharSequence** csq)
{
    if (mEnabled) {
        if(source != NULL) {
            //Java:    source.toString().toUpperCase(mLocale);
            String strSource;
            source->ToString(&strSource);
            AutoPtr<ICharSequence> cs;
            CString::New(strSource.ToUpperCase(), (ICharSequence**)&cs);
            *csq = cs;
            REFCOUNT_ADD(*csq);
            return NOERROR
        } else {
            *csq = NULL;
            return NOERROR;
        }
    }
    Logger::W(TAG, String("Caller did not enable length changes; not transforming text\n") );
    *csq = source;
    REFCOUNT_ADD(*csq);
    return NOERROR;
}

ECode AllCapsTransformationMethod::OnFocusChanged(
    /* [in] */ IView* view,
    /* [in] */ ICharSequence* sourceText,
    /* [in] */ Boolean focused,
    /* [in] */ Int32 direction,
    /* [in] */ IRect* previouslyFocusedRect)
{
    return NOERROR;
}

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
