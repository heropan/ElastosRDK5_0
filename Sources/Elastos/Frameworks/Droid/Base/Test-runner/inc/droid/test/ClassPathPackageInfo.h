
#ifndef __CLASSPATHPACKAGEINFO_H__
#define __CLASSPATHPACKAGEINFO_H__

#include <ext/frameworkdef.h>
#include "Elastos.Droid.Test_server.h"
#include "droid/test/ClassPathPackageInfoSource.h"

using Elastos::Utility::ISet;

namespace Elastos {
namespace Droid {
namespace Test {

class ClassPathPackageInfoSource;

class ClassPathPackageInfo
    : public ElRefBase
    , public IInterface
{
public:
    ClassPathPackageInfo(
        /* [in] */ ClassPathPackageInfoSource* source,
        /* [in] */ const String& packageName,
        /* [in] */ ISet* subpackageNames,
        /* [in] */ ISet* topLevelClasses);

    CAR_INTERFACE_DECL()

    CARAPI_(AutoPtr<ISet>) GetSubpackages();

    CARAPI_(AutoPtr<ISet>) GetTopLevelClassesRecursive();

private:
    CARAPI_(void) AddTopLevelClassesTo(
        /* [in] */ ISet* set);

private:
    AutoPtr<ClassPathPackageInfoSource> mSource;
    String mPackageName;
    AutoPtr<ISet> mSubpackageNames;
    AutoPtr<ISet> mTopLevelClasses;
};

} // namespace Test
} // namespace Droid
} // namespace Elastos

#endif //__CLASSPATHPACKAGEINFO_H__
