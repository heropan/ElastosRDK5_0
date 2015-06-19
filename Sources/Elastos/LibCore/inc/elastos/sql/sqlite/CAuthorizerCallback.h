
#ifndef __CAUTHORIZERCALLBACK_H__
#define __CAUTHORIZERCALLBACK_H__

#include "_Elastos_Sql_SQLite_CAuthorizerCallback.h"
#include "Object.h"


namespace Elastos {
namespace Sql {
namespace SQLite {

CarClass(CAuthorizerCallback)
    , public Object
    , public IAuthorizer
    , public ICallback
{
public:
    CAR_OBJECT_DECL();

    CAR_INTERFACE_DECL();

    CARAPI Authorize(
        /* [in] */ Int32 what,
        /* [in] */ const String& arg1,
        /* [in] */ const String& arg2,
        /* [in] */ const String& arg3,
        /* [in] */ const String& arg4,
        /* [out] */ Int32* code);

    CARAPI Columns(
        /* [in] */ const ArrayOf<String>& coldata);

    CARAPI Types(
        /* [in] */ const ArrayOf<String>& types);

    CARAPI Newrow(
        /* [in] */ const ArrayOf<String>& rowdata,
        /* [out] */ Boolean* value);

    CARAPI constructor();

    CARAPI_(Boolean) WasCalled();

private:
    Boolean isAuthorizing;
};

} // namespace SQLite
} // namespace Sql
} // namespace Elastos

#endif // __CAUTHORIZERCALLBACK_H__
