
#ifndef __CSERVICEINFO_H__
#define __CSERVICEINFO_H__

#include "_CServiceInfo.h"
#include "ComponentInfo.h"

namespace Elastos {
namespace Droid {
namespace Content {
namespace Pm {

/**
 * Information you can retrieve about a particular application
 * service. This corresponds to information collected from the
 * AndroidManifest.xml's &lt;service&gt; tags.
 */
CarClass(CServiceInfo), public ComponentInfo
{
public:
    CServiceInfo();

    ~CServiceInfo();

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ IServiceInfo* orig);

    CARAPI Dump(
        /* [in] */ IPrinter * pw,
        /* [in] */ const String& prefix);

    CARAPI ToString(
        /* [out] */ String* str);

    CARAPI ReadFromParcel(
        /* [in] */ IParcel* source);

    CARAPI WriteToParcel(
        /* [in] */ IParcel* dest);

    CARAPI LoadLabel(
        /* [in] */ IPackageManager* pm,
        /* [out] */ ICharSequence** label);

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

    CARAPI IsEnabled(
        /* [out] */ Boolean* isEnable);

    CARAPI GetIconResource(
        /* [out] */ Int32* icon);

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

    CARAPI GetPermission(
        /* [out] */ String* permission);

    CARAPI SetPermission(
        /* [in] */ const String& permission);

    CARAPI GetFlags(
        /* [out] */ Int32* flags);

    CARAPI SetFlags(
        /* [in] */ Int32 flags);

public:
    /**
     * Optional name of a permission required to be able to access this
     * Service.  From the "permission" attribute.
     */
    String mPermission;

    /**
     * Options that have been set in the service declaration in the
     * manifest.
     * These include:
     * {@link #FLAG_STOP_WITH_TASK}, {@link #FLAG_ISOLATED_PROCESS},
     * {@link #FLAG_SINGLE_USER}.
     */
    Int32 mFlags;
};

} // namespace Pm
} // namespace Content
} // namespace Droid
} // namespace Elastos

#endif // __CSERVICEINFO_H__
