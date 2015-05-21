#ifndef __CPATHCLASSLOADER_H__
#define __CPATHCLASSLOADER_H__

#include "_CPathClassLoader.h"

using Elastos::Core::IClassLoader;

namespace Elastos {
namespace Core {

CarClass(CPathClassLoader) , public IClassLoader
{
public:
    CARAPI constructor(
        /* [in] */ const String& path);

    CARAPI LoadClass(
        /* [in] */ const String& className,
        /* [out] */ IClassInfo** klass);

private:
    String mPath;
};

} // namespace Core
} // namespace Elastos

#endif //__CPATHCLASSLOADER_H__
