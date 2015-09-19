
#include "ext/frameworkext.h"
#include "content/pm/CInstrumentationInfo.h"

namespace Elastos {
namespace Droid {
namespace Content {
namespace Pm {

CAR_INTERFACE_DECL(CInstrumentationInfo, PackageItemInfo, IInstrumentationInfo)

CAR_OBJECT_DECL(CInstrumentationInfo)

CInstrumentationInfo::CInstrumentationInfo()
    : mHandleProfiling(FALSE)
    , mFunctionalTest(FALSE)
{}

CInstrumentationInfo::~CInstrumentationInfo()
{}

ECode CInstrumentationInfo::constructor()
{
    return PackageItemInfo::constructor();
}

ECode CInstrumentationInfo::constructor(
    /* [in] */ IInstrumentationInfo* orig)
{
    assert(orig != NULL);
    PackageItemInfo::constructor(orig);
    orig->GetTargetPackage(&mTargetPackage);
    orig->GetSourceDir(&mSourceDir);
    orig->GetPublicSourceDir(&mPublicSourceDir);
    orig->GetDataDir(&mDataDir);
    orig->GetNativeLibraryDir(&mNativeLibraryDir);
    orig->GetHandleProfiling(&mHandleProfiling);
    orig->GetFunctionalTest(&mFunctionalTest);
    return NOERROR;
}

ECode CInstrumentationInfo::ToString(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str);
    // return "InstrumentationInfo{"
    //         + Integer.toHexString(System.identityHashCode(this))
    //         + " " + packageName + "}";
    assert(0);
    return E_NOT_IMPLEMENTED;
}

ECode CInstrumentationInfo::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    assert(source != NULL);
    PackageItemInfo::ReadFromParcel(source);
    source->ReadString(&mTargetPackage);
    source->ReadString(&mSourceDir);
    source->ReadString(&mPublicSourceDir);
    source->ReadString(&mDataDir);
    source->ReadString(&mNativeLibraryDir);
    source->ReadBoolean(&mHandleProfiling);
    source->ReadBoolean(&mFunctionalTest);
    return NOERROR;
}

ECode CInstrumentationInfo::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    assert(dest != NULL);
    PackageItemInfo::WriteToParcel(dest);
    dest->WriteString(mTargetPackage);
    dest->WriteString(mSourceDir);
    dest->WriteString(mPublicSourceDir);
    dest->WriteString(mDataDir);
    dest->WriteString(mNativeLibraryDir);
    dest->WriteBoolean(mHandleProfiling);
    dest->WriteBoolean(mFunctionalTest);
    return NOERROR;
}

ECode CInstrumentationInfo::LoadLabel(
    /* [in] */ IPackageManager* pm,
    /* [out] */ ICharSequence** label)
{
    VALIDATE_NOT_NULL(label);
    return PackageItemInfo::LoadLabel(pm, label);
}

ECode CInstrumentationInfo::LoadIcon(
    /* [in] */ IPackageManager* pm,
    /* [out] */ IDrawable** icon)
{
    VALIDATE_NOT_NULL(icon);
    return PackageItemInfo::LoadIcon(pm, icon);
}

ECode CInstrumentationInfo::LoadLogo(
    /* [in] */ IPackageManager* pm,
    /* [out] */ IDrawable** logo)
{
    VALIDATE_NOT_NULL(logo);
    return PackageItemInfo::LoadLogo(pm, logo);
}

ECode CInstrumentationInfo::LoadXmlMetaData(
    /* [in] */ IPackageManager* pm,
    /* [in] */ const String& name,
    /* [out] */ IXmlResourceParser** resource)
{
    VALIDATE_NOT_NULL(resource);
    return PackageItemInfo::LoadXmlMetaData(pm, name,resource);
}

ECode CInstrumentationInfo::GetName(
    /* [out] */ String* name)
{
    VALIDATE_NOT_NULL(name);
    return PackageItemInfo::GetName(name);
}

ECode CInstrumentationInfo::SetName(
    /* [in] */ const String& name)
{
    return PackageItemInfo::SetName(name);
}

ECode CInstrumentationInfo::GetPackageName(
    /* [out] */ String* name)
{
    VALIDATE_NOT_NULL(name);
    return PackageItemInfo::GetPackageName(name);
}

ECode CInstrumentationInfo::SetPackageName(
    /* [in] */ const String& name)
{
    return PackageItemInfo::SetPackageName(name);
}

ECode CInstrumentationInfo::GetLabelRes(
    /* [out] */ Int32* labelRes)
{
    VALIDATE_NOT_NULL(labelRes);
    return PackageItemInfo::GetLabelRes(labelRes);
}

ECode CInstrumentationInfo::SetLabelRes(
    /* [in] */ Int32 labelRes)
{
    return PackageItemInfo::SetLabelRes(labelRes);
}

ECode CInstrumentationInfo::GetNonLocalizedLabel(
    /* [out] */ ICharSequence** label)
{
    VALIDATE_NOT_NULL(label);
    return PackageItemInfo::GetNonLocalizedLabel(label);
}

ECode CInstrumentationInfo::SetNonLocalizedLabel(
    /* [in] */ ICharSequence* label)
{
    return PackageItemInfo::SetNonLocalizedLabel(label);
}

ECode CInstrumentationInfo::GetIcon(
    /* [out] */ Int32* icon)
{
    VALIDATE_NOT_NULL(icon);
    return PackageItemInfo::GetIcon(icon);
}

ECode CInstrumentationInfo::SetIcon(
    /* [in] */ Int32 icon)
{
    return PackageItemInfo::SetIcon(icon);
}

ECode CInstrumentationInfo::GetLogo(
    /* [out] */ Int32* logo)
{
    VALIDATE_NOT_NULL(logo);
    return PackageItemInfo::GetLogo(logo);
}

ECode CInstrumentationInfo::SetLogo(
    /* [in] */ Int32 logo)
{
    return PackageItemInfo::SetLogo(logo);
}

ECode CInstrumentationInfo::GetMetaData(
    /* [out] */ IBundle** metaData)
{
    VALIDATE_NOT_NULL(metaData);
    return PackageItemInfo::GetMetaData(metaData);
}

ECode CInstrumentationInfo::SetMetaData(
    /* [in] */ IBundle* metaData)
{
    return PackageItemInfo::SetMetaData(metaData);
}

ECode CInstrumentationInfo::GetTargetPackage(
    /* [out] */ String* pkg)
{
    VALIDATE_NOT_NULL(pkg);
    *pkg = mTargetPackage;
    return NOERROR;
}

ECode CInstrumentationInfo::SetTargetPackage(
    /* [in] */ const String& pkg)
{
    mTargetPackage = pkg;
    return NOERROR;
}

ECode CInstrumentationInfo::GetSourceDir(
    /* [out] */ String* sourceDir)
{
    VALIDATE_NOT_NULL(sourceDir);
    *sourceDir = mSourceDir;
    return NOERROR;
}

ECode CInstrumentationInfo::SetSourceDir(
    /* [in] */ const String& sourceDir)
{
    mSourceDir = sourceDir;
    return NOERROR;
}

ECode CInstrumentationInfo::GetPublicSourceDir(
    /* [out] */ String* pubSrcDir)
{
    VALIDATE_NOT_NULL(pubSrcDir);
    *pubSrcDir = mPublicSourceDir;
    return NOERROR;
}

ECode CInstrumentationInfo::SetPublicSourceDir(
    /* [in] */ const String& pubSrcDir)
{
    mPublicSourceDir = pubSrcDir;
    return NOERROR;
}

ECode CInstrumentationInfo::GetDataDir(
    /* [out] */ String* dataDir)
{
    VALIDATE_NOT_NULL(dataDir);
    *dataDir = mDataDir;
    return NOERROR;
}

ECode CInstrumentationInfo::SetDataDir(
    /* [in] */ const String& dataDir)
{
    mDataDir = dataDir;
    return NOERROR;
}

ECode CInstrumentationInfo::GetNativeLibraryDir(
    /* [out] */ String* libraryDir)
{
    VALIDATE_NOT_NULL(libraryDir);
    *libraryDir = mNativeLibraryDir;
    return NOERROR;
}

ECode CInstrumentationInfo::SetNativeLibraryDir(
    /* [in] */ const String& libraryDir)
{
    mNativeLibraryDir = libraryDir;
    return NOERROR;
}

ECode CInstrumentationInfo::GetHandleProfiling(
    /* [out] */ Boolean* handleProfiling)
{
    VALIDATE_NOT_NULL(handleProfiling);
    *handleProfiling = mHandleProfiling;
    return NOERROR;
}

ECode CInstrumentationInfo::SetHandleProfiling(
    /* [in] */ Boolean handleProfiling)
{
    mHandleProfiling = handleProfiling;
    return NOERROR;
}

ECode CInstrumentationInfo::GetFunctionalTest(
    /* [out] */ Boolean* functionalTest)
{
    VALIDATE_NOT_NULL(functionalTest);
    *functionalTest = mFunctionalTest;
    return NOERROR;
}

ECode CInstrumentationInfo::SetFunctionalTest(
    /* [in] */ Boolean functionalTest)
{
    mFunctionalTest = functionalTest;
    return NOERROR;
}

} // namespace Pm
} // namespace Content
} // namespace Droid
} // namespace Elastos
