
#ifndef __ELASTOS_IO_CCODINGERRORACTION_H__
#define __ELASTOS_IO_CCODINGERRORACTION_H__

#include "_Elastos_IO_CCodingErrorAction.h"

namespace Elastos {
namespace IO {
namespace Charset {

CarClass(CCodingErrorAction)
{
public:
    CARAPI constructor();

    CARAPI GetIGNORE(
        /* [out] */ ICodingErrorAction** ignore);

    CARAPI GetREPLACE(
        /* [out] */ ICodingErrorAction** replace);

    CARAPI GetREPORT(
        /* [out] */ ICodingErrorAction** report);

    CARAPI ToString(
        /* [out] */ String* toString);

private:
    friend AutoPtr<ICodingErrorAction> CreateAction(
        /* [in] */ const String& action);

private:
    static const AutoPtr<ICodingErrorAction> IGNORE;
    static const AutoPtr<ICodingErrorAction> REPLACE;
    static const AutoPtr<ICodingErrorAction> REPORT;

    String mAction;
};

} // namespace Charset
} // namespace IO
} // namespace Elastos

#endif // __ELASTOS_IO_CCODINGERRORACTION_H__
