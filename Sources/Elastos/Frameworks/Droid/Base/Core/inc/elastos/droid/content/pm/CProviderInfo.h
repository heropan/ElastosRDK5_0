
#ifndef __ELASTOS_DROID_CONTENT_PM_CPROVIDERINFO_H__
#define __ELASTOS_DROID_CONTENT_PM_CPROVIDERINFO_H__

#include "_Elastos_Droid_Content_Pm_CProviderInfo.h"
#include "ComponentInfo.h"

using Elastos::Droid::Os::IPatternMatcher;

namespace Elastos {
namespace Droid {
namespace Content {
namespace Pm {

/**
 * Holds information about a specific
 * {@link android.content.ContentProvider content provider}. This is returned by
 * {@link android.content.pm.PackageManager#resolveContentProvider(java.lang.String, int)
 * PackageManager.resolveContentProvider()}.
 */
CarClass(CProviderInfo), public ComponentInfo
{
public:
    CAR_INTERFACE_DECL()

    CAR_OBJECT_DECL()

    CProviderInfo();

    ~CProviderInfo();

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ IProviderInfo* orig);

    CARAPI ReadFromParcel(
        /* [in] */ IParcel* source);

    CARAPI WriteToParcel(
        /* [in] */ IParcel* dest);

    CARAPI ToString(
        /* [out] */ String* str);

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

    CARAPI GetAuthority(
        /* [out] */ String* authority);

    CARAPI SetAuthority(
        /* [in] */ const String& authority);

    CARAPI GetReadPermission(
        /* [out] */ String* readPermission);

    CARAPI SetReadPermission(
        /* [in] */ const String& readPermission);

    CARAPI GetWritePermission(
        /* [out] */ String* writePermission);

    CARAPI SetWritePermission(
        /* [in] */ const String& writePermission);

    CARAPI GetGrantUriPermissions(
        /* [out] */ Boolean* grantUriPermissions);

    CARAPI SetGrantUriPermissions(
        /* [in] */ Boolean grantUriPermissions);

    CARAPI GetUriPermissionPatterns(
        /* [out, callee] */ ArrayOf<IPatternMatcher*>** patterns);

    CARAPI SetUriPermissionPatterns(
        /* [in] */ ArrayOf<IPatternMatcher*>* patterns);

    CARAPI GetPathPermissions(
        /* [out, callee] */ ArrayOf<IPathPermission*>** pathPermissions);

    CARAPI SetPathPermissions(
        /* [in] */ ArrayOf<IPathPermission*>* pathPermissions);

    CARAPI GetMultiprocess(
        /* [out] */ Boolean* multiprocess);

    CARAPI SetMultiprocess(
        /* [in] */ Boolean multiprocess);

    CARAPI GetInitOrder(
        /* [out] */ Int32* initOrder);

    CARAPI SetInitOrder(
        /* [in] */ Int32 initOrder);

    CARAPI GetFlags(
        /* [out] */ Int32* flags);

    CARAPI SetFlags(
        /* [in] */ Int32 flags);

    CARAPI GetIsSyncable(
        /* [out] */ Boolean* isSyncable);

    CARAPI SetIsSyncable(
        /* [in] */ Boolean isSyncable);

public:
    /** The name provider is published under content:// */
    String mAuthority;

    /** Optional permission required for read-only access this content
     * provider. */
    String mReadPermission;

    /** Optional permission required for read/write access this content
     * provider. */
    String mWritePermission;

    /** If true, additional permissions to specific Uris in this content
     * provider can be granted, as per the
     * {@link android.R.styleable#AndroidManifestProvider_grantUriPermissions
     * grantUriPermissions} attribute.
     */
    Boolean mGrantUriPermissions;

    /**
     * If non-null, these are the patterns that are allowed for granting URI
     * permissions.  Any URI that does not match one of these patterns will not
     * allowed to be granted.  If null, all URIs are allowed.  The
     * {@link PackageManager#GET_URI_PERMISSION_PATTERNS
     * PackageManager.GET_URI_PERMISSION_PATTERNS} flag must be specified for
     * this field to be filled in.
     */
    AutoPtr< ArrayOf<IPatternMatcher*> > mUriPermissionPatterns;

    /**
     * If non-null, these are path-specific permissions that are allowed for
     * accessing the provider.  Any permissions listed here will allow a
     * holding client to access the provider, and the provider will check
     * the URI it provides when making calls against the patterns here.
     */
    AutoPtr< ArrayOf<IPathPermission*> > mPathPermissions;

    /** If true, this content provider allows multiple instances of itself
     *  to run in different process.  If false, a single instances is always
     *  run in {@link #processName}. */
    Boolean mMultiprocess;

    /** Used to control initialization order of single-process providers
     *  running in the same process.  Higher goes first. */
    Int32 mInitOrder;

    /**
     * Options that have been set in the provider declaration in the
     * manifest.
     * These include: {@link #FLAG_SINGLE_USER}.
     */
    Int32 mFlags;

    /**
     * Whether or not this provider is syncable.
     * @deprecated This flag is now being ignored. The current way to make a provider
     * syncable is to provide a SyncAdapter service for a given provider/account type.
     */
    Boolean mIsSyncable;
};

} // namespace Pm
} // namespace Content
} // namespace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_CONTENT_PM_CPROVIDERINFO_H__
