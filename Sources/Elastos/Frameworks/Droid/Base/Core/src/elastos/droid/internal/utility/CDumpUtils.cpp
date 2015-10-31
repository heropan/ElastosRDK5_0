
#include "elastos/droid/internal/utility/CDumpUtils.h"
// #include "elastos/droid/internal/utility/CFastPrintWriter.h"

// using Elastos::Droid::Internal::Utility::CFastPrintWriter;
using Elastos::IO::CStringWriter;
using Elastos::IO::ICloseable;
using Elastos::IO::IWriter;

namespace Elastos {
namespace Droid {
namespace Internal {
namespace Utility {

CAR_INTERFACE_IMPL(CDumpUtils, Singleton, IDumpUtils)

CAR_SINGLETON_IMPL(CDumpUtils)

CDumpUtils::DumpRunnable::DumpRunnable(
    /* [in] */ IDump* dump,
    /* [in] */ IStringWriter* sw)
    : mDump(dump)
    , mSw(sw)
{
}

ECode CDumpUtils::DumpRunnable::Run()
{
    AutoPtr<IPrintWriter> lpw;
    // CFastPrintWriter::New(IWriter::Probe(mSw), (IPrintWriter**)&lpw);
    mDump->Dump(lpw);
    ICloseable::Probe(lpw)->Close();
    return NOERROR;
}

void CDumpUtils::_DumpAsync(
    /* [in] */ IHandler* handler,
    /* [in] */ IDump* dump,
    /* [in] */ IPrintWriter* pw,
    /* [in] */ Int64 timeout)
{
    AutoPtr<IStringWriter> sw;
    CStringWriter::New((IStringWriter**)&sw);
    AutoPtr<DumpRunnable> runnable = new DumpRunnable(dump, sw);
    Boolean res;
    handler->RunWithScissors(runnable, timeout, &res);
    if (res) {
        String str;
        IObject::Probe(sw)->ToString(&str);
        pw->Print(str);
    }
    else {
        pw->Println(String("... timed out"));
    }
}

ECode CDumpUtils::DumpAsync(
    /* [in] */ IHandler* handler,
    /* [in] */ IDump* dump,
    /* [in] */ IPrintWriter* pw,
    /* [in] */ Int64 timeout)
{
    _DumpAsync(handler, dump, pw, timeout);
    return NOERROR;
}

} // namespace Utility
} // namespace Internal
} // namespace Droid
} // namespace Elastos
