
#ifndef __CFUNCTIONCONTEXT_H__
#define __CFUNCTIONCONTEXT_H__

#include "_CFunctionContext.h"

namespace Elastos {
namespace Sql {
namespace SQLite {

CarClass(CFunctionContext)
{
public:
    CARAPI SetResult(
        /* [in] */ const String& ret);

    CARAPI SetResultEx(
        /* [in] */ Int32 i);

    CARAPI SetResultEx2(
        /* [in] */ Double d);

    CARAPI SetError(
        /* [in] */ const String& err);

    CARAPI SetResultEx3(
        /* [in] */ const ArrayOf<Byte>& b);

    CARAPI SetResultZeroblob(
        /* [in] */ Int32 n);

    CARAPI Count(
        /* [out] */ Int32* count);
    CFunctionContext();

private:
    static CARAPI_(Boolean) Internal_init();

private:
    static Boolean isinit;

public:
    Int64 mHandle;
};

} // namespace SQLite
} // namespace Sql
} // namespace Elastos

#endif // __CFUNCTIONCONTEXT_H__
