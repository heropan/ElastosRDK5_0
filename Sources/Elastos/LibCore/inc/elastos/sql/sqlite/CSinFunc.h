
#ifndef __CSINFUNC_H__
#define __CSINFUNC_H__

#include "_Elastos_Sql_SQLite_CSinFunc.h"
#include <coredef.h>

namespace Elastos {
namespace Sql {
namespace SQLite {

CarClass(CSinFunc)
    , public Object
    , public IFunction
{
public:
    CAR_OBJECT_DECL();

    CAR_INTERFACE_DECL();

    CARAPI Function(
        /* [in] */ IFunctionContext* fc,
        /* [in] */ const ArrayOf<String>& args);

    CARAPI Step(
        /* [in] */ IFunctionContext* fc,
        /* [in] */ const ArrayOf<String>& args);

    CARAPI LastStep(
        /* [in] */ IFunctionContext* fc);

    CARAPI constructor();

public:
    Double testDouble;
    Boolean functionCalled;
};

} // namespace SQLite
} // namespace Sql
} // namespace Elastos

#endif // __CSINFUNC_H__
