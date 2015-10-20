#ifndef __ELASTOS_DROID_TEXT_METHOD_AllCapsTransformationMethod_H__
#define __ELASTOS_DROID_TEXT_METHOD_AllCapsTransformationMethod_H__

#include "Elastos.Droid.Core_server.h"

using Elastos::Utility::ILocale;
using Elastos::Core::ICharSequence;
using Elastos::Droid::Content::IContext;
using Elastos::Droid::Graphics::IRect;
using Elastos::Droid::View::IView;

namespace Elastos {
namespace Droid {
namespace Text {
namespace Method {

/**
 * Transforms source text into an ALL CAPS string, locale-aware.
 *
 * @hide
 */
class AllCapsTransformationMethod{
public:
    AllCapsTransformationMethod(
        /* [in] */ IContext* context);

    CARAPI_(void) Init(
    /* [in] */ IContext* context);

    //@Override
    CARAPI_(AutoPtr<ICharSequence>) GetTransformation(
        /* [in] */ ICharSequence* source,
        /* [in] */ IView* view);

    //@Override
    CARAPI_(void) OnFocusChanged(
        /* [in] */ IView* view,
        /* [in] */ ICharSequence* sourceText,
        /* [in] */ Boolean focused,
        /* [in] */ Int32 direction,
        /* [in] */ IRect* previouslyFocusedRect);

    //@Override
    CARAPI_(void) SetLengthChangesAllowed(
        /* [in] */ Boolean allowLengthChanges);

protected:
    AllCapsTransformationMethod();

private:
    static const CString TAG;// = "AllCapsTransformationMethod";

    Boolean mEnabled;//=FALSE;
    AutoPtr<ILocale> mLocale;
};


} // namespace Method
} // namespace Text
} // namepsace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_TEXT_METHOD_AllCapsTransformationMethod_H__
