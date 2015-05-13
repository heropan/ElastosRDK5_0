
#ifndef __CLIBCORE_H__
#define __CLIBCORE_H__

#include "_CLibcore.h"

namespace Libcore {
namespace IO {

CarClass(CLibcore)
{
public:
    CARAPI GetOs(
        /* [out] */ IOs ** os);

public:
    static AutoPtr<IOs> sOs;
};

} // namespace IO
} // namespace Libcore

#endif // __CLIBCORE_H__
