
#include "elastos/droid/content/pm/CPackageParserHelper.h"
#include "elastos/droid/content/pm/PackageParser.h"

namespace Elastos {
namespace Droid {
namespace Content {
namespace Pm {

CAR_INTERFACE_IMPL(CPackageParserHelper, Singleton, IPackageParserHelper)

CAR_SINGLETON_IMPL(CPackageParserHelper)

ECode CPackageParserHelper::IsApkFile(
    /* [in] */ IFile* file,
    /* [out] */ Boolean* res)
{
    VALIDATE_NOT_NULL(res)
    *res = PackageParser::IsApkFile(file);
    return NOERROR;
}

ECode CPackageParserHelper::ParsePackageLite(
    /* [in] */ IFile* packageFile,
    /* [in] */ Int32 flags,
    /* [out] */ IPackageLite** res)
{
    VALIDATE_NOT_NULL(res)
    AutoPtr< ArrayOf<Byte> > readBuffer = ArrayOf<Byte>::Alloc(PackageParser::CERTIFICATE_BUFFER_SIZE);
    PackageParser::ParsePackageLite(packageFile, flags, readBuffer, res);
    return NOERROR;
}

} // namespace Pm
} // namespace Content
} // namespace Droid
} // namespace Elastos