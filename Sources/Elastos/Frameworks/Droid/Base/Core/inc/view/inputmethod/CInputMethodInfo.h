
#ifndef __ELASTOS_DROID_VIEW_INPUTMETHOD_CINPUTMETHODINFO_H__
#define  __ELASTOS_DROID_VIEW_INPUTMETHOD_CINPUTMETHODINFO_H__

#include "ext/frameworkdef.h"
#include "_CInputMethodInfo.h"
#include "content/pm/CResolveInfo.h"
#include <elastos/utility/etl/List.h>

using Elastos::Utility::Etl::List;
using Elastos::Utility::IObjectStringMap;
using Elastos::Droid::Content::IContext;
using Elastos::Droid::Content::IComponentName;
using Elastos::Droid::Content::Pm::IResolveInfo;
using Elastos::Droid::Content::Pm::CResolveInfo;
using Elastos::Droid::Content::Pm::IServiceInfo;
using Elastos::Droid::Content::Pm::IPackageManager;
using Elastos::Droid::View::InputMethod::IInputMethodSubtype;

namespace Elastos {
namespace Droid {
namespace View {
namespace InputMethod {

CarClass(CInputMethodInfo)
{
public:
    CInputMethodInfo();

    CARAPI constructor();

    /**
     * Constructor.
     *
     * @param context The Context in which we are parsing the input method.
     * @param service The ResolveInfo returned from the package manager about
     * this input method's component.
     */
    CARAPI constructor(
        /* [in] */ IContext* ctx,
        /* [in] */ IResolveInfo* service);

    /**
     * Constructor.
     *
     * @param context The Context in which we are parsing the input method.
     * @param service The ResolveInfo returned from the package manager about
     * this input method's component.
     * @param additionalSubtypes additional subtypes being added to this InputMethodInfo
     * @hide
     */
    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IResolveInfo* service,
        /* [in] */ IObjectStringMap* additionalSubtypesMap);

    CARAPI constructor(
        /* [in] */ const String& packageName,
        /* [in] */ const String& className,
        /* [in] */ ICharSequence* label,
        /* [in] */ const String& settingsActivity);

    CARAPI ReadFromParcel(
        /* [in] */ IParcel *source);

    CARAPI WriteToParcel(
        /* [in] */ IParcel *dest);

    CARAPI GetId(
        /* [out] */ String* id);

    CARAPI GetPackageName(
        /* [out] */ String* name);

    CARAPI GetServiceName(
        /* [out] */ String* name);

    CARAPI GetServiceInfo(
        /* [out] */ IServiceInfo** info);

    CARAPI GetComponent(
        /* [out] */ IComponentName** name);

    CARAPI LoadLabel(
        /* [in] */ IPackageManager* pm,
        /* [out] */ ICharSequence** str);

    CARAPI LoadIcon(
        /* [in] */ IPackageManager* pm,
        /* [out] */ IDrawable** drawable);

    CARAPI GetSettingsActivity(
        /* [out] */ String* settings);

    CARAPI GetSubtypeCount(
        /* [out] */ Int32* count);

    CARAPI GetSubtypeAt(
        /* [in] */ Int32 index,
        /* [out] */ IInputMethodSubtype** subtype);

    CARAPI GetIsDefaultResourceId(
        /* [out] */ Int32* id);

    CARAPI IsAuxiliaryIme(
        /* [in] */ Boolean* auxIme);

    CARAPI GetHashCode(
        /* [out] */ Int32* code);

    CARAPI Equals(
        /* [in] */ IInputMethodInfo* o,
        /* [out] */ Boolean* e);

    CARAPI Equals(
        /* [in] */ IInterface* o,
        /* [out] */ Boolean* e);

private:
    CARAPI Init(
        /* [in] */ IContext* context,
        /* [in] */ IResolveInfo* service,
        /* [in] */ IObjectStringMap* additionalSubtypesMap);

private:
    static const String TAG;

    /**
     * The Service that implements this input method component.
     */
    AutoPtr<CResolveInfo> mService;

    /**
     * The unique string Id to identify the input method.  This is generated
     * from the input method component.
     */
    String mId;

    /**
     * The input method setting activity's name, used by the system settings to
     * launch the setting activity of this input method.
     */
    String mSettingsActivityName;

    /**
     * The resource in the input method's .apk that holds a boolean indicating
     * whether it should be considered the default input method for this
     * system.  This is a resource ID instead of the final value so that it
     * can change based on the configuration (in particular locale).
     */
    Int32 mIsDefaultResId;

    /**
     * The array of the subtypes.
     */
    List<AutoPtr<IInputMethodSubtype> > mSubtypes;

    Boolean mIsAuxIme;
};

} // namespace InputMethod
} // namespace View
} // namespace Droid
} // namespace Elastos

#endif  //__ELASTOS_DROID_VIEW_INPUTMETHOD_CINPUTMETHODINFO_H__
