
#include "speech/srec/CUlawEncoderInputStreamHelper.h"
#include "speech/srec/CUlawEncoderInputStream.h"
#include "ext/frameworkext.h"

namespace Elastos {
namespace Droid {
namespace Speech {
namespace Srec {

ECode CUlawEncoderInputStreamHelper::Encode(
    /* [in] */ ArrayOf<Byte>* pcmBuf,
    /* [in] */ Int32 pcmOffset,
    /* [in] */ ArrayOf<Byte>* ulawBuf,
    /* [in] */ Int32 ulawOffset,
    /* [in] */ Int32 length,
    /* [in] */ Int32 max)
{
    CUlawEncoderInputStream::Encode(pcmBuf, pcmOffset, ulawBuf, ulawOffset, length, max);
    return NOERROR;
}

ECode CUlawEncoderInputStreamHelper::MaxAbsPcm(
    /* [in] */ ArrayOf<Byte>* pcmBuf,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 length,
    /* [out] */ Int32* ret)
{
    VALIDATE_NOT_NULL(ret);
    *ret = CUlawEncoderInputStream::MaxAbsPcm(pcmBuf, offset, length);
    return NOERROR;
}

}//namespace Srec
}//namespace Speech
}//namespace Droid
}//namespace Elastos

