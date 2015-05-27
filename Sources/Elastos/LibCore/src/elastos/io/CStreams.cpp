
#include "coredef.h"
#include "CStreams.h"
#include <elastos/Math.h>
#include <elastos/StringBuilder.h>
#include <CByteArrayOutputStream.h>
#include <CStringWriter.h>

using Elastos::Core::StringBuilder;

namespace Elastos {
namespace IO {

AutoPtr<ArrayOf<Byte> > CStreams::mSkipBuffer = NULL;

ECode CStreams::ReadSingleByte(
    /* [in] */ IInputStream* in,
    /* [out] */ Int32* singleByte)
{
    VALIDATE_NOT_NULL(in);
    VALIDATE_NOT_NULL(singleByte);

    AutoPtr<ArrayOf<Byte> > buffer = ArrayOf<Byte>::Alloc(1);
    Int32 result;
    FAIL_RETURN(in->ReadBytesEx(buffer.Get(), 0, 1, &result));
    *singleByte = (result != -1) ? (*buffer)[0] & 0xff : -1;
    return NOERROR;
}

ECode CStreams::WriteSingleByte(
    /* [in] */ IOutputStream* out,
    /* [in] */ Int32 b)
{
    VALIDATE_NOT_NULL(out);

    AutoPtr<ArrayOf<Byte> > buffer = ArrayOf<Byte>::Alloc(1);
    (*buffer)[0] = (Byte)(b & 0xff);
    return out->WriteBytes(*(buffer.Get()));
}

ECode CStreams::ReadFully(
    /* [in] */ IInputStream* in,
    /* [out, callee] */ ArrayOf<Byte>** dst)
{
    VALIDATE_NOT_NULL(in);
    VALIDATE_NOT_NULL(dst);
    ECode ec = ReadFullyNoClose(in, dst);
    FAIL_RETURN(in->Close())
    return ec;

}

ECode CStreams::ReadFully(
    /* [in] */ IInputStream* in,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 byteCount,
    /* [out] */ ArrayOf<Byte>* dst)
{
    if (byteCount == 0) {
        return NOERROR;
    }
    if (in == NULL) {
        return E_NULL_POINTER_EXCEPTION;
    }
    if (dst == NULL) {
        return E_NULL_POINTER_EXCEPTION;
    }

    if ((offset | byteCount) < 0 || offset > dst->GetLength() ||
            dst->GetLength() - offset < byteCount) {
        return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }

    Int32 bytesRead;
    while (byteCount > 0) {
        FAIL_RETURN(in->ReadBytesEx(dst, offset, byteCount, &bytesRead));
        if (bytesRead < 0) {
            return E_EOF_EXCEPTION;
        }
        offset += bytesRead;
        byteCount -= bytesRead;
    }

    return NOERROR;
}

ECode CStreams::ReadFullyCloseWhenDone(
    /* [in] */ IInputStream* in,
    /* [out, callee] */ ArrayOf<Byte>** bytes)
{
    VALIDATE_NOT_NULL(bytes);
    *bytes = NULL;
    VALIDATE_NOT_NULL(in);

    ECode ec = ReadFullyNoClose(in, bytes);
    in->Close();
    return ec;
}

ECode CStreams::ReadFullyNoClose(
    /* [in] */ IInputStream* in,
    /* [out, callee] */ ArrayOf<Byte>** byteArray)
{
    VALIDATE_NOT_NULL(byteArray);
    *byteArray = NULL;
    VALIDATE_NOT_NULL(in);

    AutoPtr<IByteArrayOutputStream> bytes;
    CByteArrayOutputStream::New((IByteArrayOutputStream**)&bytes);
    AutoPtr<ArrayOf<Byte> > buffer = ArrayOf<Byte>::Alloc(1024);
    Int32 count;
    FAIL_RETURN(in->ReadBytes(buffer, &count));
    while (count != -1) {
        FAIL_RETURN(bytes->WriteBytesEx(*buffer.Get(), 0, count));
        FAIL_RETURN(in->ReadBytes(buffer, &count));
    }
    return bytes->ToByteArray(byteArray);
}

ECode CStreams::ReadFullyFromReader(
    /* [in] */ IReader* reader,
    /* [out] */ String* result)//todo: how to write simple when fail??????
{
    VALIDATE_NOT_NULL(result);
    *result = NULL;
    VALIDATE_NOT_NULL(reader);

    AutoPtr<ICloseable> ic = (ICloseable*)reader->Probe(EIID_ICloseable);
    //try {
    AutoPtr<IStringWriter> writer;
    CStringWriter::New((IStringWriter**)&writer);
    AutoPtr<ArrayOf<Char32> > buffer = ArrayOf<Char32>::Alloc(1024);
    AutoPtr<IStringBuffer> strBuf;
    Int32 count;
    ECode ec = reader->ReadChars(buffer.Get(), &count);
    if (FAILED(ec)) {
        goto finally;
    }
    while (count != -1) {
        ec = writer->WriteCharsEx(*buffer.Get(), 0, count);
        if (FAILED(ec)) {
            goto finally;
        }
        ec = reader->ReadChars(buffer, &count);
        if (FAILED(ec)) {
            goto finally;
        }
    }

    ec = writer->GetBuffer((IStringBuffer**)&strBuf);
    if (FAILED(ec)) {
        goto finally;
    }

    ec = strBuf->ToString(result);

finally:
    if (ic != NULL) {
        ic->Close();
    }
    return ec;
}

ECode CStreams::SkipAll(
    /* [in] */ IInputStream* in)
{
    VALIDATE_NOT_NULL(in);
    using Elastos::Core::Math;
    Int32 count;
    Int64 skipped;
    do {
        FAIL_RETURN(in->Skip(Math::INT64_MAX_VALUE, &skipped));
        FAIL_RETURN(in->Read(&count));
    }
    while (count != -1);
    return NOERROR;
}

ECode CStreams::SkipByReading(
    /* [in] */ IInputStream* in,
    /* [in] */ Int64 byteCount,
    /* [out] */ Int64* result)
{
    VALIDATE_NOT_NULL(result);
    *result = -1;
    VALIDATE_NOT_NULL(in);

    using Elastos::Core::Math;
    if (mSkipBuffer.Get() == NULL) {
        mSkipBuffer = ArrayOf<Byte>::Alloc(4096);
    }
    Int64 skipBufferLength = mSkipBuffer->GetLength();
    Int64 skipped = 0;
    Int32 toRead, read;
    ECode ec = NOERROR;
    while (skipped < byteCount) {
        toRead = (Int32)Min((Int64)(byteCount - skipped), skipBufferLength);
        // FAIL_RETURN(in->ReadBytesEx(mSkipBuffer, 0, toRead, &read));
        in->ReadBytesEx(mSkipBuffer, 0, toRead, &read);
        if (read == -1) {
            break;
        }
        skipped += read;
        if (read < toRead) {
            break;
        }
    }

    *result = skipped;
    return ec;
}

ECode CStreams::Copy(
    /* [in] */ IInputStream* in,
    /* [in] */ IOutputStream* out,
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(in);
    VALIDATE_NOT_NULL(out);
    VALIDATE_NOT_NULL(number);

    Int32 total = 0;
    ArrayOf_<Byte, 8192> buffer;
    Int32 c;
    FAIL_RETURN(in->ReadBytes(&buffer, &c));
    while (c != -1) {
        total += c;
        FAIL_RETURN(out->WriteBytesEx(buffer, 0, c));
        FAIL_RETURN(in->ReadBytes(&buffer, &c));
    }

    *number = total;
    return NOERROR;
}

ECode CStreams::ReadAsciiLine(
    /* [in] */ IInputStream* in,
    /* [out] */ String* characters)
{
    VALIDATE_NOT_NULL(in);
    VALIDATE_NOT_NULL(characters);
    //TODO: support UTF-8 here instead

    StringBuilder result(80);
    while (TRUE) {
        Int32 c;
        FAIL_RETURN(in->Read(&c));
        if (c == -1) {
            return E_EOF_EXCEPTION;
//            throw new EOFException();
        }
        else if (c == '\n') {
            break;
        }

        result.AppendChar((Char32)c);
    }
    Int32 length = 0;
    result.GetLength(&length);
    if (length > 0) {
        Char32 ch = result.GetChar(length - 1);
        if (ch == '\r') {
            result.DeleteChar(length - 1);
        }
    }

    result.ToString(characters);
    return NOERROR;
}

} // namespace IO
} // namespace Elastos
