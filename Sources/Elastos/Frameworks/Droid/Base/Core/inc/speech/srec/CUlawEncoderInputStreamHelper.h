
#ifndef __CULAWENCODERINPUTSTREAMHELPER_H__
#define __CULAWENCODERINPUTSTREAMHELPER_H__

#include "_CUlawEncoderInputStreamHelper.h"

namespace Elastos {
namespace Droid {
namespace Speech {
namespace Srec {

CarClass(CUlawEncoderInputStreamHelper)
{
public:
    CARAPI Encode(
        /* [in] */ ArrayOf<Byte>* pcmBuf,
        /* [in] */ Int32 pcmOffset,
        /* [in] */ ArrayOf<Byte>* ulawBuf,
        /* [in] */ Int32 ulawOffset,
        /* [in] */ Int32 length,
        /* [in] */ Int32 max);

    CARAPI MaxAbsPcm(
        /* [in] */ ArrayOf<Byte>* pcmBuf,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 length,
        /* [out] */ Int32* ret);

};

}//namespace Srec
}//namespace Speech
}//namespace Droid
}//namespace Elastos

#endif // __CULAWENCODERINPUTSTREAMHELPER_H__
