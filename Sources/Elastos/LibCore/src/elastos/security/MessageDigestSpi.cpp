
#include "MessageDigestSpi.h"
#include <cutils/log.h>
#include <cmdef.h>

namespace Elastos {
namespace Security {
//5ADF9E34-6626-84B4-6EDC-C27A2FF98A96
extern "C" const InterfaceID EIID_MessageDigestSpi =
    { 0x5ADF9E34, 0x6626, 0x84B4, { 0x6E, 0xDC, 0xC2, 0x7A, 0x2F, 0xF9, 0x8A, 0x96 } };

ECode MessageDigestSpi::EngineGetDigestLength(
        /* [out] */ Int32* len)
{
    VALIDATE_NOT_NULL(len)
    *len = 0;
    return NOERROR;
}

ECode MessageDigestSpi::EngineUpdateEx2(
    /* [in] */ IByteBuffer* input)
{
    Boolean hasRemaining;
    input->HasRemaining(&hasRemaining);
    if (!hasRemaining) {
            return NOERROR;
    }
    AutoPtr<ArrayOf<Byte> > tmp;
    input->HasArray(&hasRemaining);
    Int32 offset, position, limit;
    input->GetArrayOffset(&offset);
    input->GetPosition(&position);
    input->GetLimit(&limit);
    if (hasRemaining) {
        input->GetArray((ArrayOf<Byte>**)&tmp);
        EngineUpdateEx(tmp, offset+position, limit-position);
        input->SetPosition(limit);
    } else {
        tmp = ArrayOf<Byte>::Alloc(limit - position);
        input->GetBytes(tmp);
        EngineUpdateEx(tmp, 0, tmp->GetLength());
    }
    return NOERROR;
}

ECode MessageDigestSpi::EngineDigestEx(
    /* [in, out] */ ArrayOf<Byte>* buf,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 len,
    /* [out] */ Int32* ed)
{
    VALIDATE_NOT_NULL(ed)
    Int32 length;
    EngineGetDigestLength(&length);
    if (len < length) {
        EngineReset();
        ALOGE("The value of len parameter is less than the actual digest length");
        return E_DIGEST_EXCEPTION;
    }
    if (offset < 0) {
        EngineReset();
        ALOGE("offset < 0");
        return E_DIGEST_EXCEPTION;
    }
    if (offset + len > buf->GetLength()) {
        EngineReset();
        ALOGE("offset + len > buf.length");
        return E_DIGEST_EXCEPTION;
    }
    AutoPtr<ArrayOf<Byte> > tmp;
    EngineDigest((ArrayOf<Byte>**)&tmp);
    if (len < tmp->GetLength()) {
        ALOGE("The value of len parameter is less than the actual digest length");
        return E_DIGEST_EXCEPTION;
    }
    buf->Copy(offset, tmp, 0, tmp->GetLength());
    *ed = tmp->GetLength();
    return NOERROR;
}

} // namespace Security
} // namespace Elastos