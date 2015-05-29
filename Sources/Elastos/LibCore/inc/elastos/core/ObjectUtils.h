#ifndef __ELASTOS_CORE_OBJECTUTILS_H__
#define __ELASTOS_CORE_OBJECTUTILS_H__

#include <elastos.h>

using namespace Elastos;

namespace Elastos {
namespace Core {

class ObjectUtils
{
public:
    // From String without error code
    //
    static CARAPI_(Boolean) Equals(
        /* [in] */ IInterface* first,
        /* [in] */ IInterface* second);

    static CARAPI_(Int32) GetHashCode(
        /* [in] */ IInterface* first);

    static CARAPI_(String) ToString(
        /* [in] */ IInterface* first);

private:
    ObjectUtils() {}

    ~ObjectUtils() {}
};

} // namespace Core
} // namespace Elastos

#endif // __ELASTOS_CORE_OBJECTUTILS_H__
