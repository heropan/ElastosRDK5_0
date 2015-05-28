
#include "coredef.h"
#include "InputStreamReader.h"
#include "Charset.h"
#include "CCoderResult.h"
#include "CharBuffer.h"
#include "ByteBuffer.h"
#include "CCodingErrorAction.h"
#include <elastos/Math.h>
#include <elastos/Character.h>

using Elastos::Core::Character;
using Elastos::IO::Charset::CCoderResult;
using Elastos::IO::Charset::CCodingErrorAction;
using Elastos::IO::Charset::Charset;
using Elastos::IO::Charset::ICharset;
using Elastos::IO::Charset::ICoderResult;
using Elastos::IO::Charset::ICodingErrorAction;

namespace Elastos {
namespace IO {

InputStreamReader::InputStreamReader()
    : mEndOfInput(FALSE)
{
    ASSERT_SUCCEEDED(ByteBuffer::Allocate(8192, (IByteBuffer**)&mBytes));
}

ECode InputStreamReader::Init(
    /* [in] */ IInputStream *in)
{
    AutoPtr<ICharset> ch;
    FAIL_RETURN(Charset::Charset::DefaultCharset((ICharset**)&ch));

    return Init(in, (ICharset*)ch);
}

ECode InputStreamReader::Init(
    /* [in] */ IInputStream *in,
    /* [in] */ const String &enc)
{
    AutoPtr<IInterface> obj;
    in->GetLock((IInterface**)&obj);
    LockObject* lockobj = (LockObject*)obj.Get();
    Reader::Init(lockobj);

    if (enc.IsNull()) {
        return E_NULL_POINTER_EXCEPTION;
//        throw new NullPointerException();
    }

    mIn = in;
    AutoPtr<ICharset> charset;
    FAIL_RETURN(Charset::Charset::ForName(enc, (ICharset**)&charset));
    FAIL_RETURN(charset->NewDecoder((ICharsetDecoder**)&mDecoder));
    AutoPtr<ICodingErrorAction> action;
    FAIL_RETURN(CCodingErrorAction::New((ICodingErrorAction** )&action))

    AutoPtr<ICodingErrorAction> replace;
    action->GetREPLACE((ICodingErrorAction **)&replace);

    AutoPtr<ICharsetDecoder> malformDecoder;
    mDecoder->OnMalformedInput(replace, (ICharsetDecoder **)&malformDecoder);

    AutoPtr<ICharsetDecoder> unMappableDecoder;
    malformDecoder->OnUnmappableCharacter(replace, (ICharsetDecoder **)&unMappableDecoder);
    return mBytes->SetLimit(0);
}

ECode InputStreamReader::Init(
    /* [in] */ IInputStream* in,
    /* [in] */ ICharset* charset)
{
    AutoPtr<IInterface> obj;
    in->GetLock((IInterface**)&obj);
    LockObject* lockobj = (LockObject*)obj.Get();
    Reader::Init(lockobj);

    mIn = in;
    charset->NewDecoder((ICharsetDecoder**)&mDecoder);
    AutoPtr<ICodingErrorAction> action;
    FAIL_RETURN(CCodingErrorAction::New((ICodingErrorAction** )&action))

    AutoPtr<ICodingErrorAction> replace;
    action->GetREPLACE((ICodingErrorAction **)&replace);

    AutoPtr<ICharsetDecoder> malformDecoder;
    mDecoder->OnMalformedInput(replace, (ICharsetDecoder **)&malformDecoder);

    AutoPtr<ICharsetDecoder> unMappableDecoder;
    malformDecoder->OnUnmappableCharacter(replace, (ICharsetDecoder **)&unMappableDecoder);
    return mBytes->SetLimit(0);
}

ECode InputStreamReader::Close()
{
    Object::Autolock lock(mLock);
    if (mDecoder != NULL) {
        mDecoder->Reset();
    }
    mDecoder = NULL;
    if (mIn != NULL) {
        FAIL_RETURN(mIn->Close());
        mIn = NULL;
    }
    return NOERROR;
}

ECode InputStreamReader::GetEncoding(
    /* [out] */ String* encoding)
{
    VALIDATE_NOT_NULL(encoding);
    if (!IsOpen()) {
        *encoding = String(NULL);
        return NOERROR;
    }
 //   return HistoricalNamesUtil.getHistoricalName(decoder.charset().name());
    *encoding = String("UTF-8");

    return NOERROR;
}

ECode InputStreamReader::Read(
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);
    Object::Autolock lock(mLock);
    if (!IsOpen()) {
        return E_IO_EXCEPTION;
//        throw new IOException("InputStreamReader is closed");
    }
    ArrayOf_<Char32, 1> buf;
    Int32 number;
    FAIL_RETURN(ReadChars(&buf, 0, 1, &number));
    *value = number != -1 ? buf[0] : -1;

    return NOERROR;
}

ECode InputStreamReader::ReadChars(
    /* [out] */ ArrayOf<Char32>* buffer,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 count,
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(buffer);
    VALIDATE_NOT_NULL(number);
    *number = 0;

    Object::Autolock lock(mLock);

    if (!IsOpen()) {
        return E_IO_EXCEPTION;
//        throw new IOException("InputStreamReader is closed");
    }
    // RI exception compatibility so we can run more tests.
    if (offset < 0) {
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
//        throw new IndexOutOfBoundsException();
    }
    if (buffer == NULL) {
        return E_NULL_POINTER_EXCEPTION;
//        throw new NullPointerException("buffer == null");
    }
    if (count < 0 || offset > buffer->GetLength() - count) {
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
//        throw new IndexOutOfBoundsException();
    }
    if (count == 0) {
        return NOERROR;
    }

    AutoPtr<ICharBuffer> out;
    FAIL_RETURN(CharBuffer::WrapArray(buffer, offset, count, (ICharBuffer**)&out));
    AutoPtr<ICoderResult> result, resultTmp;
    CCoderResult::GetUNDERFLOW((ICoderResult**)&result);
    resultTmp = result;
    // bytes.remaining() indicates number of bytes in buffer
    // when 1-st time entered, it'll be equal to zero
    Boolean hasRemaining;
    mBytes->HasRemaining(&hasRemaining);
    Boolean needInput = !hasRemaining;

    Int32 size;
    Int32 was_red;

    while(out->HasRemaining(&hasRemaining), hasRemaining) {
        // fill the buffer if needed
        if (needInput) {
            Int32 num, position;
            FAIL_RETURN(mIn->Available(&num));
            FAIL_RETURN(out->GetPosition(&position));
            if (num == 0 && position > offset) {
                // we could return the result without blocking read
                break;
            }

            Int32 capacity, limit, arrayOffset;
            mBytes->GetCapacity(&capacity);
            mBytes->GetLimit(&limit);
            mBytes->GetArrayOffset(&arrayOffset);
            Int32 to_read = capacity - limit;
            Int32 off = arrayOffset + limit;
            AutoPtr<ArrayOf<Byte> > buf;
            mBytes->GetArray((ArrayOf<Byte>**)&buf);
            mIn->ReadBytes(buf, off, to_read, &was_red);
            if (was_red == -1) {
                mEndOfInput = TRUE;
                break;
            }
            else if (was_red == 0) {
                break;
            }
            mBytes->SetLimit(limit + was_red);
            needInput = FALSE;
        }

        // decode bytes
        result = NULL;
        mDecoder->Decode(mBytes, out, FALSE, (ICoderResult**)&result);
        Boolean isUnderflow;
        if (result->IsUnderflow(&isUnderflow), isUnderflow) {
            Int32 capacity, limit, pos;
            mBytes->GetLimit(&limit);
            mBytes->GetCapacity(&capacity);
            // compact the buffer if no space left
            if (limit == capacity) {
                mBytes->Compact();
                mBytes->GetPosition(&pos);
                mBytes->SetLimit(pos);
                mBytes->SetPosition(0);
            }
            needInput = TRUE;
        }
        else {
            break;
        }
    }

    if (_CObject_Compare(result, resultTmp) && mEndOfInput) {
        result = NULL;
        mDecoder->Decode(mBytes, out, TRUE, (ICoderResult**)&result);
        AutoPtr<ICoderResult> resultFlush;
        mDecoder->Flush(out, (ICoderResult**)&resultFlush);
        mDecoder->Reset();

        Int32 pos, arrayOffset, limit;
        mBytes->GetArrayOffset(&arrayOffset);
        mBytes->GetPosition(&pos);
        mBytes->GetLimit(&limit);
        mBytes->SetPosition(Elastos::Core::Math::Min(pos, limit) - arrayOffset);
    }
    Boolean isMalformed, isUnmappable;
    if ((result->IsMalformed(&isMalformed), isMalformed) || (result->IsUnmappable(&isUnmappable), isUnmappable)) {
        return result->ThrowException();
    }

    Int32 charBufPos;
    out->GetPosition(&charBufPos);
    *number = charBufPos - offset == 0 ? -1 : charBufPos - offset;
    return NOERROR;
}

Boolean InputStreamReader::IsOpen()
{
    return mIn != NULL;
}

ECode InputStreamReader::IsReady(
    /* [out] */ Boolean* ready)
{
    VALIDATE_NOT_NULL(ready);
    Object::Autolock lock(mLock);
    if (mIn == NULL) {
        return E_IO_EXCEPTION;
//        throw new IOException("InputStreamReader is closed");
    }

    Boolean hasRemaining;
    mBytes->HasRemaining(&hasRemaining);
    Int32 number;
    mIn->Available(&number);
    *ready = hasRemaining || number > 0;

    return NOERROR;
}

} // namespace IO
} // namespace Elastos
