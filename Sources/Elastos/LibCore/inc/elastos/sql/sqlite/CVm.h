
#ifndef __CVM_H__
#define __CVM_H__

#include "_Elastos_Sql_SQLite_CVm.h"
#include "Object.h"

using Elastos::Core::Object;

namespace Elastos {
namespace Sql {
namespace SQLite {

CarClass(CVm)
    , public Object
    , public IVm
{
public:
    CAR_OBJECT_DECL();

    CAR_INTERFACE_DECL();

    CARAPI Step(
        /* [in] */ ICallback * cb,
        /* [out] */ Boolean * value);

    CARAPI Compile(
        /* [out] */ Boolean * value);

    CARAPI Stop();

    CARAPI Finalize();

    CVm();

public:
    Int32 mError_code;
    Int64 mHandle;

private:
    static CARAPI_(Boolean) Internal_init();

private:
    static Boolean isinit;
};

} // namespace SQLite
} // namespace Sql
} // namespace Elastos

#endif // __CVM_H__
