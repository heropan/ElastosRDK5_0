#include "CMockFunction.h"

namespace Elastos {
namespace Sql {
namespace SQLite {

Boolean CMockFunction::getAggValueCalled = FALSE;
Boolean CMockFunction::functionCalled = FALSE;
Boolean CMockFunction::stepCalled = FALSE;
Boolean CMockFunction::lastStepCalled = FALSE;

ECode CMockFunction::constructor()
{
    return NOERROR;
}

ECode CMockFunction::Function(
    /* [in] */ IFunctionContext* fc,
    /* [in] */ const ArrayOf<String>& args)
{
    functionCalled = TRUE;
    if (args.GetLength() > 0) {
        String str = args[0].ToLowerCase();
        fc->SetResult(str);
    }
    return NOERROR;
}

ECode CMockFunction::Step(
    /* [in] */ IFunctionContext* fc,
    /* [in] */ const ArrayOf<String>& args)
{
    stepCalled = TRUE;
    for (Int32 i = 0; i < args.GetLength(); i++) {
        acc.AppendString(args[i]);
        acc.AppendCStr(" ");
    }
    return NOERROR;
}

ECode CMockFunction::LastStep(
    /* [in] */ IFunctionContext* fc)
{
    lastStepCalled = TRUE;
    fc->SetResult(acc.ToString());
    return NOERROR;
}

ECode CMockFunction::GetAggValue(
    /* [out] */ String * str)
{
    VALIDATE_NOT_NULL(str);
    getAggValueCalled = TRUE;
    *str = acc.ToString();
    return NOERROR;
}

ECode CMockFunction::PrintCalled()
{
    printf("functionCalled:%d, stepCalled:%d, lastStepCalled:%d \n",
            functionCalled,stepCalled,lastStepCalled);
    return NOERROR;
}

} // namespace SQLite
} // namespace Sql
} // namespace Elastos
