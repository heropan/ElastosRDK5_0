
#include "CSinFunc.h"
#include <elastos/Math.h>
#include <elastos/StringUtils.h>

using Elastos::Core::StringUtils;

namespace Elastos {
namespace Sql {
namespace SQLite {

ECode CSinFunc::constructor()
{
    testDouble = 3.0;
    functionCalled = FALSE;
    return NOERROR;
}

ECode CSinFunc::Function(
    /* [in] */ IFunctionContext* fc,
    /* [in] */ const ArrayOf<String>& args)
{
    Double d = StringUtils::ParseDouble(args[0]);
    functionCalled = TRUE;
    VALIDATE_NOT_NULL(fc);
    fc->SetResultEx2(Elastos::Core::Math::Sin(d));
    return NOERROR;
}

ECode CSinFunc::Step(
    /* [in] */ IFunctionContext* fc,
    /* [in] */ const ArrayOf<String>& args)
{
    return NOERROR;
}

ECode CSinFunc::LastStep(
    /* [in] */ IFunctionContext* fc)
{
    return NOERROR;
}

} // namespace SQLite
} // namespace Sql
} // namespace Elastos