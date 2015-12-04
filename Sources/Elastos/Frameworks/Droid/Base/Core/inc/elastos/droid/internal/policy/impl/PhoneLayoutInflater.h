#ifndef __ELASTOS_DROID_INTERNAL_POLICY_IMPL_PHONELAYOUTINFLATER_H__
#define __ELASTOS_DROID_INTERNAL_POLICY_IMPL_PHONELAYOUTINFLATER_H__

#include "elastos/droid/ext/frameworkext.h"
#include "elastos/droid/view/LayoutInflater.h"

using Elastos::Droid::Content::IContext;
using Elastos::Droid::Utility::IAttributeSet;
using Elastos::Droid::View::LayoutInflater;
using Elastos::Droid::View::IView;

namespace Elastos {
namespace Droid {
namespace Internal {
namespace Policy {
namespace Impl {

class PhoneLayoutInflater
    : public LayoutInflater
    , public IPhoneLayoutInflater
{
public:
    CAR_INTERFACE_DECL()

    PhoneLayoutInflater();

    /**
      * Instead of instantiating directly, you should retrieve an instance
      * through {@link Context#getSystemService}
      *
      * @param context The Context in which in which to find resources and other
      *                application-specific things.
      *
      * @see Context#getSystemService
      */
    CARAPI constructor(
        /* [in] */ IContext* context);


    /** Override onCreateView to instantiate names that correspond to the
         widgets known to the Widget factory. If we don't find a match,
         call through to our super class.
     */
    // @Override protected View onCreateView(String name, AttributeSet attrs) throws ClassNotFoundException {
    virtual CARAPI OnCreateView(
        /* [in] */ const String& name,
        /* [in] */ IAttributeSet* attrs,
        /* [out] */ IView** view);

    virtual CARAPI CloneInContext(
        /* [in] */ IContext* newContext,
        /* [out] */ ILayoutInflater** result);

protected:
    PhoneLayoutInflater(
        /* [in] */ ILayoutInflater* original,
        /* [in] */ IContext* newContext);

private:
    static CARAPI_(AutoPtr< ArrayOf<String> >) MiddleInitSclassprefixlist();

private:
    static AutoPtr< ArrayOf<String> > sClassPrefixList;
};

} // namespace Impl
} // namespace Policy
} // namespace Internal
} // namespace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_INTERNAL_POLICY_IMPL_PHONELAYOUTINFLATER_H__

