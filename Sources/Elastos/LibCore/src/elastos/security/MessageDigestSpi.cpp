
#include "Elastos.CoreLibrary.IO.h"
#include "MessageDigestSpi.h"
#include <elastos/utility/logging/Logger.h>

using Elastos::IO::IBuffer;
using Elastos::Utility::Logging::Logger;

namespace Elastos {
namespace Security {

ECode MessageDigestSpi::EngineGetDigestLength(
    /* [out] */ Int32* len)
{
    VALIDATE_NOT_NULL(len)
    *len = 0;
    return NOERROR;
}

ECode MessageDigestSpi::EngineUpdate(
    /* [in] */ IByteBuffer* input)
{
    Boolean result;
    IBuffer::Probe(input)->HasRemaining(&result);
    if (!result) {
        return NOERROR;
    }
    AutoPtr< ArrayOf<Byte> > tmp;
    IBuffer::Probe(input)->HasArray(&result);
    if (result) {
        input->GetArray((ArrayOf<Byte>**)&tmp);
        Int32 offset, position, limit;
        IBuffer::Probe(input)->GetArrayOffset(&offset);
        IBuffer::Probe(input)->GetPosition(&position);
        IBuffer::Probe(input)->GetLimit(&limit);
        FAIL_RETURN(EngineUpdate(tmp, offset + position, limit - position));
        IBuffer::Probe(input)->SetPosition(limit);
    }
    else {
        Int32 limit, position;
        IBuffer::Probe(input)->GetLimit(&limit);
        IBuffer::Probe(input)->GetPosition(&position);
        tmp = ArrayOf<Byte>::Alloc(limit - position);
        input->Get(tmp);
        FAIL_RETURN(EngineUpdate(tmp, 0, tmp->GetLength()));
    }
    return NOERROR;
}

ECode MessageDigestSpi::EngineDigest(
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
        Logger::E("MessageDigestSpi", "The value of len parameter is less than the actual digest length");
        return E_DIGEST_EXCEPTION;
    }
    if (offset < 0) {
        EngineReset();
        Logger::E("MessageDigestSpi", "offset < 0");
        return E_DIGEST_EXCEPTION;
    }
    if (offset + len > buf->GetLength()) {
        EngineReset();
        Logger::E("MessageDigestSpi", "offset + len > buf.length");
        return E_DIGEST_EXCEPTION;
    }
    AutoPtr<ArrayOf<Byte> > tmp;
    FAIL_RETURN(EngineDigest((ArrayOf<Byte>**)&tmp));
    if (len < tmp->GetLength()) {
        Logger::E("MessageDigestSpi", "The value of len parameter is less than the actual digest length");
        return E_DIGEST_EXCEPTION;
    }
    buf->Copy(offset, tmp, 0, tmp->GetLength());
    *ed = tmp->GetLength();
    return NOERROR;
}

} // namespace Security
} // namespace Elastos