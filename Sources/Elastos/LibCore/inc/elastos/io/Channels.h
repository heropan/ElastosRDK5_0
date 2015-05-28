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
    static CARAPI NewChannel(
        IOutputStream* outputStream,
        IWritableByteChannel** outputChannel);
    */
    /*
    static CARAPI NewReader(
        IReadableByteChannel * pChannel,
        ICharsetDecoder * pDecoder,
        Int32 minBufferCapacity,
        IReader ** ppReader);

    static CARAPI NewReader(
        IReadableByteChannel * pChannel,
        const String& charsetName,
        IReader ** ppReader);

    static CARAPI NewWriter(
        IWritableByteChannel * pChannel,
        ICharsetEncoder * pEncoder,
        Int32 minBufferCapacity,
        IWriter ** ppWriter);

    static CARAPI NewWriter(
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
            IBuffer* res = ((IBuffer*)targetPtr->Probe(EIID_IBuffer));
            if (res) {
                res->GetRemaining(&bytesRemain);
            }
            AutoPtr<ArrayOf<Byte> > buff = ArrayOf<Byte>::Alloc(bytesRemain);
            int readCount = 0;
            mInputStreamPtr->Read(buff, &readCount);

            if (readCount > 0) {
                // targetPtr->PutBytes(0, readCount, *buff);
                targetPtr->PutBytes(*buff, 0, readCount);
            }
            return readCount;
        }

    protected:
        // @Override
        virtual ECode ImplCloseChannel()
        {
            AutoPtr<ICloseable> res = (ICloseable*) mInputStreamPtr->Probe(EIID_ICloseable);
            if(res) res->Close();
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
            IBuffer* res = ((IBuffer*)source->Probe(EIID_IBuffer));
            if (res) {
                res->GetRemaining(&bytesRemain);
            }

            if (bytesRemain == 0) {
                return 0;
            }

            AutoPtr<ArrayOf<Byte> > buff = ArrayOf<Byte>::Alloc(bytesRemain);
            source->GetByte(buff->GetPayload());
            mOutputStream->Write(*buff, 0, bytesRemain);

            return bytesRemain;
        }

        protected:
        ECode ImplCloseChannel()
        {
            AutoPtr<ICloseable> res = (ICloseable*) mOutputStream->Probe(EIID_ICloseable);
            if(res) res->Close();
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
