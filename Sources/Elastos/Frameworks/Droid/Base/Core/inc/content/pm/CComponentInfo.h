
#ifndef __CCOMPONENTINFO_H__
#define __CCOMPONENTINFO_H__

#include "_CComponentInfo.h"
#include "ComponentInfo.h"

namespace Elastos {
namespace Droid {
namespace Content {
namespace Pm {

/**
 * Base class containing information common to all application components
 * ({@link ActivityInfo}, {@link ServiceInfo}).  This class is not intended
 * to be used by itself; it is simply here to share common definitions
 * between all application components.  As such, it does not itself
 * implement Parcelable, but does provide convenience methods to assist
 * in the implementation of Parcelable in subclasses.
 */
CarClass(CComponentInfo), public ComponentInfo
{
public:
    CComponentInfo();

    ~CComponentInfo();

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ IComponentInfo* orig);

    CARAPI LoadLabel(
        /* [in] */ IPackageManager* pm,
        /* [out] */ ICharSequence** label);

    /**
     * Return whether this component and its enclosing application are enabled.
     */
    CARAPI IsEnabled(
        /* [out] */ Boolean* isEnable);

    /**
     * Return the icon resource identifier to use for this component.  If
     * the component defines an icon, that is used; else, the application
     * icon is used.
     *
     * @return The icon associated with this component.
     */
    CARAPI GetIconResource(
        /* [out] */ Int32* icon);

    CARAPI WriteToParcel(
        /* [in] */ IParcel* dest);

    CARAPI ReadFromParcel(
        /* [in] */ IParcel* src);

    CARAPI LoadIcon(
        /* [in] */ IPackageManager* pm,
        /* [out] */ IDrawable** icon);

    CARAPI LoadLogo(
        /* [in] */ IPackageManager* pm,
        /* [out] */ IDrawable** icon);

    CARAPI LoadXmlMetaData(
        /* [in] */ IPackageManager* pm,
        /* [in] */ const String& name,
        /* [out] */ IXmlResourceParser** resource);

    CARAPI GetName(
        /* [out] */ String* name);

    CARAPI SetName(
        /* [in] */ const String& name);

    CARAPI GetPackageName(
        /* [out] */ String* name);

    CARAPI SetPackageName(
        /* [in] */ const String& name);

    CARAPI GetLabelRes(
        /* [out] */ Int32* labelRes);

    CARAPI SetLabelRes(
        /* [in] */ Int32 labelRes);

    CARAPI GetNonLocalizedLabel(
        /* [out] */ ICharSequence** label);

    CARAPI SetNonLocalizedLabel(
        /* [in] */ ICharSequence* label);

    CARAPI GetIcon(
        /* [out] */ Int32* icon);

    CARAPI SetIcon(
        /* [in] */ Int32 icon);

    CARAPI GetLogo(
        /* [out] */ Int32* logo);

    CARAPI SetLogo(
        /* [in] */ Int32 logo);

    CARAPI GetMetaData(
        /* [out] */ IBundle** metaData);

    CARAPI SetMetaData(
        /* [in] */ IBundle* metaData);

    CARAPI GetApplicationInfo(
        /* [out] */ IApplicationInfo** info);

    CARAPI SetApplicationInfo(
        /* [in] */ IApplicationInfo* info);

    CARAPI GetProcessName(
        /* [out] */ String* processName);

    CARAPI SetProcessName(
        /* [in] */ const String& processName);

    CARAPI GetDescriptionRes(
        /* [out] */ Int32* desRes);

    CARAPI SetDescriptionRes(
        /* [in] */ Int32 desRes);

    CARAPI GetEnabled(
        /* [out] */ Boolean* enabled);

    CARAPI SetEnabled(
        /* [in] */ Boolean enabled);

    CARAPI GetExported(
        /* [out] */ Boolean* exported);

    CARAPI SetExported(
        /* [in] */ Boolean exported);
};

} // namespace Pm
} // namespace Content
} // namespace Droid
} // namespace Elastos

#endif // __CCOMPONENTINFO_H__
