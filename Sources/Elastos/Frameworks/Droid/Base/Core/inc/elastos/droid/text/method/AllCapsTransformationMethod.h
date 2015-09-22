#ifndef __ELASTOS_DROID_TEXT_METHOD_AllCapsTransformationMethod_H__
#define __ELASTOS_DROID_TEXT_METHOD_AllCapsTransformationMethod_H__

#include "Elastos.Droid.Core_server.h"

using Elastos::Droid::Content::IContext;
using Elastos::Droid::Graphics::IRect;
using Elastos::Droid::View::IView;
using Libcore::ICU::ILocale;
using Elastos::Core::ICharSequence;


namespace Elastos {
namespace Droid {
namespace Text {
namespace Method {

/**
 * Transforms source text into an ALL CAPS string, locale-aware.
 *
 * @hide
 */
class AllCapsTransformationMethod
    : public Object
    , public IAllCapsTransformationMethod
    , public ITransformationMethod2
    , public ITransformationMethod
{
public:
    CAR_INTERFACE_DECL()

    AllCapsTransformationMethod();

    virtual ~AllCapsTransformationMethod();

    CARAPI constructor(
        /* [in] */ IContext* context);

    //@Override
    CARAPI GetTransformation(
        /* [in] */ ICharSequence* source,
        /* [in] */ IView* view,
        /* [out] */ ICharSequence** csq);

    //@Override
    CARAPI OnFocusChanged(
        /* [in] */ IView* view,
        /* [in] */ ICharSequence* sourceText,
        /* [in] */ Boolean focused,
        /* [in] */ Int32 direction,
        /* [in] */ IRect* previouslyFocusedRect);

    //@Override
    CARAPI SetLengthChangesAllowed(
        /* [in] */ Boolean allowLengthChanges);

private:
    static const String TAG;// = "AllCapsTransformationMethod";

    Boolean mEnabled;//=FALSE;
    AutoPtr<ILocale> mLocale;
};


} // namespace Method
} // namespace Text
} // namepsace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_TEXT_METHOD_AllCapsTransformationMethod_H__
