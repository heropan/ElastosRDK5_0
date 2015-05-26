
#ifndef __CTESTTRACE_H__
#define __CTESTTRACE_H__

#include "_Elastos_Sql_SQLite_CTestTrace.h"
#include <coredef.h>
#include <elastos/StringBuffer.h>

using Elastos::Core::StringBuffer;

namespace Elastos {
namespace Sql {
namespace SQLite {

CarClass(CTestTrace)
    , public Object
    , public ITrace
    , public ICallback
{
public:
    CAR_OBJECT_DECL();

    CAR_INTERFACE_DECL();

    CARAPI Trace(
        /* [in] */ const String& stmt);

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI Columns(
        /* [in] */ const ArrayOf<String>& coldata);

    CARAPI Types(
        /* [in] */ const ArrayOf<String>& types);

    CARAPI Newrow(
        /* [in] */ const ArrayOf<String>& rowdata,
        /* [out] */ Boolean* value);

    CARAPI constructor();

    CARAPI_(String) GetTrace();

public:
    Boolean traceCalled;

private:
    StringBuffer buf;
};

} // namespace SQLite
} // namespace Sql
} // namespace Elastos

#endif // __CTESTTRACE_H__
