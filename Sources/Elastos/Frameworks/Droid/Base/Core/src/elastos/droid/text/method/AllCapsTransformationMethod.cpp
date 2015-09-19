#include "text/method/AllCapsTransformationMethod.h"
#include "content/res/CConfiguration.h"
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

const CString AllCapsTransformationMethod::TAG = "AllCapsTransformationMethod";

AllCapsTransformationMethod::AllCapsTransformationMethod()
{}

AllCapsTransformationMethod::AllCapsTransformationMethod(
    /* [in] */ IContext* context)
{
    Init(context);
}

void AllCapsTransformationMethod::Init(
    /* [in] */ IContext* context)
{
    AutoPtr<IResources> res;
    context->GetResources((IResources**)&res);
    AutoPtr<IConfiguration> config;
    res->GetConfiguration((IConfiguration**)&config);
    config->GetLocale((ILocale**)&mLocale);
    mEnabled = FALSE;
}

AutoPtr<ICharSequence> AllCapsTransformationMethod::GetTransformation(
    /* [in] */ ICharSequence* source,
    /* [in] */ IView* view)
{
    if (mEnabled) {
        if(source != NULL) {
            //Java:    source.toString().toUpperCase(mLocale);
            String strSource;
            source->ToString(&strSource);
            AutoPtr<ICharSequence> cs;
            CString::New(strSource.ToUpperCase(), (ICharSequence**)&cs);
            return cs;
        } else {
            return NULL;
        }
    }
    Logger::W(TAG, String("Caller did not enable length changes; not transforming text\n") );
    return source;
}

void AllCapsTransformationMethod::OnFocusChanged(
    /* [in] */ IView* view,
    /* [in] */ ICharSequence* sourceText,
    /* [in] */ Boolean focused,
    /* [in] */ Int32 direction,
    /* [in] */ IRect* previouslyFocusedRect)
{}

void AllCapsTransformationMethod::SetLengthChangesAllowed(
    /* [in] */ Boolean allowLengthChanges)
{
    mEnabled = allowLengthChanges;
}



} // namespace Method
} // namespace Text
} // namepsace Droid
} // namespace Elastos
