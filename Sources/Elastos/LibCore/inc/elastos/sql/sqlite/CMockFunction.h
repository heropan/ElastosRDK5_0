
#ifndef __CMOCKFUNCTION_H__
#define __CMOCKFUNCTION_H__

#include "_Elastos_Sql_SQLite_CMockFunction.h"
#include <elastos/core/StringBuffer.h>
#include "Object.h"

using Elastos::Core::StringBuffer;

namespace Elastos {
namespace Sql {
namespace SQLite {

CarClass(CMockFunction)
    , public Object
    , public IMockFunction
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

    CARAPI GetAggValue(
        /* [out] */ String * str);

    CARAPI PrintCalled();

private:
    StringBuffer acc;

public:
    static Boolean getAggValueCalled;
    static Boolean functionCalled;
    static Boolean stepCalled;
    static Boolean lastStepCalled;
};

} // namespace SQLite
} // namespace Sql
} // namespace Elastos

#endif // __CMOCKFUNCTION_H__
