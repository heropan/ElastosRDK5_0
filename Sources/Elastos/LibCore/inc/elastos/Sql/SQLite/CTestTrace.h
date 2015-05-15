
#ifndef __CTESTTRACE_H__
#define __CTESTTRACE_H__

#include "_CTestTrace.h"
#include <cmdef.h>
#include <elastos/StringBuffer.h>

using Elastos::Core::StringBuffer;

namespace Elastos {
namespace Sql {
namespace SQLite {

CarClass(CTestTrace)
{
public:

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
