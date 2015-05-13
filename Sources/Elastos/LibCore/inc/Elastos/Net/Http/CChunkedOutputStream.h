
#ifndef __CCHUNKEDOUTPUTSTREAM_H__
#define __CCHUNKEDOUTPUTSTREAM_H__

#include "_CChunkedOutputStream.h"
#include "AbstractHttpOutputStream.h"

using Elastos::IO::IByteArrayOutputStream;

namespace Elastos {
namespace Net {
namespace Http {

CarClass(CChunkedOutputStream), public AbstractHttpOutputStream
{
public:
    CARAPI constructor(
        /* [in] */ IOutputStream* socketOut,
        /* [in] */ Int32 maxChunkLength);

    CARAPI Write(
        /* [in] */ Int32 oneByte);

    CARAPI WriteBytes(
        /* [in] */ const ArrayOf<Byte>& buffer);

    CARAPI WriteBytesEx(
        /* [in] */ const ArrayOf<Byte>& buffer,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 count);

    CARAPI CheckError(
        /* [out] */ Boolean* hasError);

    CARAPI Close();

    CARAPI Flush();

    CARAPI GetLock(
        /* [out] */ IInterface** lockobj);

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

private:
    CARAPI_(Int32) DataLength(
        /* [in] */ Int32 dataPlusHeaderLength);

    CARAPI_(void) WriteHex(
        /* [in] */ Int32 i);

    CARAPI_(void) WriteBufferedChunkToSocket();

private:
    static const Byte CRLF[2]; // = { '\r', '\n' };
    static const Byte HEX_DIGITS[16]; // = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};
    static const Byte FINAL_CHUNK[5]; // = new byte[] { '0', '\r', '\n', '\r', '\n' };

    /** Scratch space for up to 8 hex digits, and then a constant CRLF */
    Byte mHex[10]; // = { 0, 0, 0, 0, 0, 0, 0, 0, '\r', '\n' };

    AutoPtr<IOutputStream> mSocketOut;
    Int32 mMaxChunkLength;
    AutoPtr<IByteArrayOutputStream> mBufferedChunk;
};

} // namespace Http
} // namespace Net
} // namespace Elastos

#endif //__CCHUNKEDOUTPUTSTREAM_H__
