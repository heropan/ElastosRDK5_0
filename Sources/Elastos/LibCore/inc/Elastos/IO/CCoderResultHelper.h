#ifndef __CCODERRESULTHELPER_H__
#define __CCODERRESULTHELPER_H__

#include "_CCoderResultHelper.h"

namespace Elastos {
namespace IO {
namespace Charset {

CarClass(CCoderResultHelper)
{
public:

    CARAPI GetUNDERFLOW(
        /* [out] */ ICoderResult** result);

    CARAPI GetOVERFLOW(
        /* [out] */ ICoderResult** result);

    CARAPI MalformedForLength(
        /* [in] */ Int32 length,
        /* [out] */ ICoderResult** result);

    CARAPI UnmappableForLength(
        /* [in] */ Int32 length,
        /* [out] */ ICoderResult** result);
};

} // namespace Charset
} // namespace IO
} // namespace Elastos

#endif //__CCODERRESULTHELPER_H__
