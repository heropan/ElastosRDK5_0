#include "CTestTrace.h"

using Elastos::Sql::SQLite::EIID_ITrace;
using Elastos::Sql::SQLite::EIID_ICallback;

namespace Elastos {
namespace Sql {
namespace SQLite {

ECode CTestTrace::Trace(
    /* [in] */ const String& stmt)
{
    traceCalled = TRUE;
    buf.AppendString(stmt);
    return NOERROR;
}

PInterface CTestTrace::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_ITrace) {
        return (ITrace*)this;
    } else if (riid == EIID_ICallback) {
        return (ICallback*)this;
    } else {
        return NULL;
    }
}

ECode CTestTrace::Columns(
    /* [in] */ const ArrayOf<String>& coldata)
{
    return NOERROR;
}

ECode CTestTrace::Types(
    /* [in] */ const ArrayOf<String>& types)
{
    return NOERROR;
}

ECode CTestTrace::Newrow(
    /* [in] */ const ArrayOf<String>& rowdata,
    /* [out] */ Boolean* value)
{
    *value = FALSE;
    return NOERROR;
}

ECode CTestTrace::constructor()
{
    traceCalled = FALSE;
    return NOERROR;
}

String CTestTrace::GetTrace()
{
    return buf.ToString();
}

} // namespace SQLite
} // namespace Sql
} // namespace Elastos
