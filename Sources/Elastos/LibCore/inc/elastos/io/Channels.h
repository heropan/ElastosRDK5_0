#ifndef __CHANNELS_H__
#define __CHANNELS_H__

#include <Elastos.CoreLibrary_server.h>
#include "AbstractInterruptibleChannel.h"

using Elastos::IO::Channels::Spi::AbstractInterruptibleChannel;

namespace Elastos {
namespace IO {
namespace Channels{

class Channels
{
public:
    /*
    Channels();
    ~Channels();
    */
    /** Channels just provides only static functions, so use implicited
     *  constructor
     */
    /*
    static CARAPI NewInputStream(
        IReadableByteChannel * pChannel,
        IInputStream ** ppInputStream);

    static CARAPI NewOutputStream(
        IWritableByteChannel * pChannel,
        IOutputStream ** ppOutputStream);
    */
    /*
    static CARAPI NewChannel(
        IInputStream* inputStream,
        IReadableByteChannel** inputChannel);
    */

    static IReadableByteChannel* NewChannel(IInputStream* inputStream);
    static IWritableByteChannel* NewChannel(IOutputStream* outputStream);

    /*
    static CARAPI NewChannelEx(
        IOutputStream* outputStream,
        IWritableByteChannel** outputChannel);
    */
    /*
    static CARAPI NewReader(
        IReadableByteChannel * pChannel,
        ICharsetDecoder * pDecoder,
        Int32 minBufferCapacity,
        IReader ** ppReader);

    static CARAPI NewReaderEx(
        IReadableByteChannel * pChannel,
        const String& charsetName,
        IReader ** ppReader);

    static CARAPI NewWriter(
        IWritableByteChannel * pChannel,
        ICharsetEncoder * pEncoder,
        Int32 minBufferCapacity,
        IWriter ** ppWriter);

    static CARAPI NewWriterEx(
        IWritableByteChannel * pChannel,
        const String& charsetName,
        IWriter ** ppWriter);
    */
private:
    /*
     * Wrapper class used for newChannel(InputStream inputStream)
     */
    ///*
    class ReadableByteChannelImpl : public AbstractInterruptibleChannel
    {
    public:
        ReadableByteChannelImpl(IInputStream* inputStreamPtr) : AbstractInterruptibleChannel()
        {
            mInputStreamPtr = inputStreamPtr;
        }

        int Read(IByteBuffer* targetPtr) {
            if (!IsOpen()) {
                return -1;
            }

            int bytesRemain;
            targetPtr->GetRemaining(&bytesRemain);
            AutoPtr<ArrayOf<Byte> > buff = ArrayOf<Byte>::Alloc(bytesRemain);
            int readCount = 0;
            mInputStreamPtr->ReadBytes(buff, &readCount);

            if (readCount > 0) {
                // targetPtr->PutBytesEx(0, readCount, *buff);
                targetPtr->PutBytesEx(*buff, 0, readCount);
            }
            return readCount;
        }

    protected:
        // @Override
        virtual ECode ImplCloseChannel()
        {
            mInputStreamPtr->Close();
            return NOERROR;
        }

    private:
        IInputStream*       mInputStreamPtr;
    };  // end of class ReadableByteChannelImpl
    //*/
    /*
     * Wrapper class used for newChannel(OutputStream outputStream)
     */
    ///*
    class WritableByteChannelImpl : public AbstractInterruptibleChannel
    {
    public:
        WritableByteChannelImpl(IOutputStream* outputStream) : AbstractInterruptibleChannel()
        {
            mOutputStream = outputStream;
        }

        int Write(IByteBuffer* source) {
            if (!IsOpen()) {
                return -1;
            }
            int bytesRemain;
            source->GetRemaining(&bytesRemain);

            if (bytesRemain == 0) {
                return 0;
            }

            AutoPtr<ArrayOf<Byte> > buff = ArrayOf<Byte>::Alloc(bytesRemain);
            source->GetByte(buff->GetPayload());
            mOutputStream->WriteBytesEx(*buff, 0, bytesRemain);

            return bytesRemain;
        }

        protected:
        ECode ImplCloseChannel()
        {
            mOutputStream->Close();
            return NOERROR;
        }

    private:
        IOutputStream*  mOutputStream;
    };
    //*/
    // TODO: Add your private member variables here.
};

} // namespace Channels
} // namespace IO
} // namespace Elastos

#endif // __CHANNELS_H__


