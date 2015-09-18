
#ifndef __ELASTOS_DROID_CONTENT_PM_CINSTRUMENTATIONINFO_H__
#define __ELASTOS_DROID_CONTENT_PM_CINSTRUMENTATIONINFO_H__

#include "_Elastos_Droid_Content_Pm_CInstrumentationInfo.h"
#include "PackageItemInfo.h"

namespace Elastos {
namespace Droid {
namespace Content {
namespace Pm {

/**
 * Information you can retrieve about a particular piece of test
 * instrumentation.  This corresponds to information collected
 * from the AndroidManifest.xml's &lt;instrumentation&gt; tag.
 */
CarClass(CInstrumentationInfo)
    , public PackageItemInfo
    , public IInstrumentationInfo
{
public:
    CAR_INTERFACE_DECL()

    CAR_OBJECT_DECL()

    CInstrumentationInfo();

    ~CInstrumentationInfo();

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ IInstrumentationInfo* orig);

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
        /* [out] */ IDrawable** logo);

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

    CARAPI GetTargetPackage(
        /* [out] */ String* pkg);

    CARAPI SetTargetPackage(
        /* [in] */ const String& pkg);

    CARAPI GetSourceDir(
        /* [out] */ String* sourceDir);

    CARAPI SetSourceDir(
        /* [in] */ const String& sourceDir);

    CARAPI GetPublicSourceDir(
        /* [out] */ String* pubSrcDir);

    CARAPI SetPublicSourceDir(
        /* [in] */ const String& pubSrcDir);

    CARAPI GetDataDir(
        /* [out] */ String* dataDir);

    CARAPI SetDataDir(
        /* [in] */ const String& dataDir);

    CARAPI GetNativeLibraryDir(
        /* [out] */ String* libraryDir);

    CARAPI SetNativeLibraryDir(
        /* [in] */ const String& libraryDir);

    CARAPI GetHandleProfiling(
        /* [out] */ Boolean* handleProfiling);

    CARAPI SetHandleProfiling(
        /* [in] */ Boolean handleProfiling);

    CARAPI GetFunctionalTest(
        /* [out] */ Boolean* functionalTest);

    CARAPI SetFunctionalTest(
        /* [in] */ Boolean functionalTest);

public:
    /**
     * The name of the application package being instrumented.  From the
     * "package" attribute.
     */
    String mTargetPackage;

    /**
     * Full path to the location of this package.
     */
    String mSourceDir;

    /**
     * Full path to the location of the publicly available parts of this package (i.e. the resources
     * and manifest).  For non-forward-locked apps this will be the same as {@link #sourceDir).
     */
    String mPublicSourceDir;
    /**
     * Full path to a directory assigned to the package for its persistent
     * data.
     */
    String mDataDir;

    /**
     * Full path to the directory where the native JNI libraries are stored.
     *
     * {@hide}
     */
    String mNativeLibraryDir;

    /**
     * Specifies whether or not this instrumentation will handle profiling.
     */
    Boolean mHandleProfiling;

    /** Specifies whether or not to run this instrumentation as a functional test */
    Boolean mFunctionalTest;
};

} // namespace Pm
} // namespace Content
} // namespace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_CONTENT_PM_CINSTRUMENTATIONINFO_H__
