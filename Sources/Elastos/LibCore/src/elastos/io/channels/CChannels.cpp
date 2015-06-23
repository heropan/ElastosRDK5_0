
#include "CChannels.h"
#include "Channels.h"
#include "ByteBuffer.h"
#include "InputStream.h"
#include "OutputStream.h"
// #include "CStreams.h"
#include "elastos/core/Math.h"

using Elastos::IO::InputStream;
using Elastos::IO::OutputStream;

namespace Elastos {
namespace IO {
namespace Channels{

class IChannelInputStream;
class IChannelOutputStream;

CAR_INTERFACE_IMPL(CChannels, Singleton, IChannels)

CAR_SINGLETON_IMPL(CChannels)

ECode CChannels::NewChannel(
    /* [in] */ IInputStream* inputStream,
    /* [out] */ IReadableByteChannel** inputChannel)
{
    // return new InputStreamChannel(inputStream);
    return NOERROR;
}

ECode CChannels::NewChannel(
    /* [in] */ IOutputStream* outputStream,
    /* [out] */ IWritableByteChannel** outputChannel)
{
    // return new OutputStreamChannel(outputStream);
    return NOERROR;
}

ECode CChannels::NewInputStream(
    /* [in] */ IReadableByteChannel * pChannel,
    /* [out] */ IInputStream ** ppInputStream)
{
    // return new ChannelInputStream(channel);
    return E_NOT_IMPLEMENTED;
}

ECode CChannels::NewOutputStream(
    /* [in] */ IWritableByteChannel * pChannel,
    /* [out] */ IOutputStream ** ppOutputStream)
{
    // return new ChannelOutputStream(channel);
    return E_NOT_IMPLEMENTED;
}

ECode CChannels::NewReader(
    /* [in] */ IReadableByteChannel * pChannel,
    /* [in] */ ICharsetDecoder * pDecoder,
    /* [in] */ Int32 minBufferCapacity,
    /* [out] */ IReader ** ppReader)
{
    /*
     * This method doesn't honor minBufferCapacity. Ignoring that parameter
     * saves us from having to add a hidden constructor to InputStreamReader.
     */
    // return new InputStreamReader(new ChannelInputStream(channel), decoder);
    return E_NOT_IMPLEMENTED;
}

ECode CChannels::NewReader(
    /* [in] */ IReadableByteChannel * pChannel,
    /* [in] */ const String& charsetName,
    /* [out] */ IReader ** ppReader)
{
    // if (charsetName == null) {
    //     throw new NullPointerException("charsetName == null");
    // }
    // return newReader(channel, Charset.forName(charsetName).newDecoder(), -1);
    return E_NOT_IMPLEMENTED;
}

ECode CChannels::NewWriter(
    /* [in] */ IWritableByteChannel * pChannel,
    /* [in] */ ICharsetEncoder * pEncoder,
    /* [in] */ Int32 minBufferCapacity,
    /* [out] */ IWriter ** ppWriter)
{
    // return new OutputStreamWriter(new ChannelOutputStream(channel), encoder);
    return E_NOT_IMPLEMENTED;
}

ECode CChannels::NewWriter(
    /* [in] */ IWritableByteChannel * pChannel,
    /* [in] */ const String& charsetName,
    /* [out] */ IWriter ** ppWriter)
{
    // if (charsetName == null) {
    //     throw new NullPointerException("charsetName == null");
    // }
    // return newWriter(channel, Charset.forName(charsetName).newEncoder(), -1);
    return E_NOT_IMPLEMENTED;
}

static ECode CheckBlocking(IChannel *channel)
{
    if(ISelectableChannel::Probe(channel) != NULL)
    {
        Boolean ret;
        ((ISelectableChannel*)channel)->IsBlocking(&ret);
        if(ret)
        {
            return E_ILLEGAL_BLOCKING_MODE_EXCEPTION;
        }
    }
    return NOERROR;
}

class IChannelInputStream : public IInputStream
{
private:
     IReadableByteChannel *mChannel;

public:
    IChannelInputStream(IReadableByteChannel *channel)
    {
        assert(NULL != channel);
        mChannel = channel;
    }

    ECode Read()
    {
        Int32 nRead;
        assert(0 && "TODO"); //wangguangming
        // AutoPtr<IStreams> stream;
        // CStreams::AcquireSingleton((IStreams**)&stream);
        // stream->ReadSingleByte(this, &nRead);
        return nRead;
    }

    ECode Read(ArrayOf<Byte> *target, Int32 offset, Int32 length)
    {
        IByteBuffer *buffer;
        ByteBuffer::Wrap(target, offset, length, &buffer);
        CheckBlocking(IChannel::Probe(mChannel));
        Int32 nRead;
        mChannel->Read(buffer, &nRead);
        return nRead;
    }

    ECode Available()
    {
        if(IFileChannel::Probe(mChannel) != NULL)
        {
            /* !!! NOT SURE */
            IFileChannel *fileChannel = (IFileChannel*)mChannel;

            Int64 result, size, position;
            fileChannel->GetSize(&size);
            fileChannel->GetPosition(&position);
            result = size - position;
            return result > Elastos::Core::Math::INT32_MAX_VALUE ? Elastos::Core::Math::INT32_MAX_VALUE : (Int32)result;
        }
        else
        {
            Int32 ret;
            IInputStream::Available(&ret);
            return ret;
        }
    }

    ECode Close()
    {
        ICloseable::Probe(mChannel)->Close();
        return NOERROR;
    }

};

class IChannelOutputStream : public IOutputStream
{
private:
        IWritableByteChannel *mChannel;
        Object mLock;

public:
        IChannelOutputStream(IWritableByteChannel *channel)
        {
            assert(NULL != channel);
            REFCOUNT_ADD(channel);
            mChannel = channel;
        }

        virtual ~IChannelOutputStream()
        {
            if (NULL != mChannel)
            {
                mChannel->Release();
            }
        }

        ECode Write(Int32 oneByte)
        {
            Int32 value;
            mLock.Lock();
            value = oneByte;
            Write(value);
            mLock.Unlock();
            return NOERROR;
        }

        ECode Write(ArrayOf<Byte>* source, Int32 offset, Int32 length)
        {
            AutoPtr<IByteBuffer> buffer;
            ByteBuffer::Wrap(source, offset, length, (IByteBuffer **)&buffer);
            CheckBlocking(IChannel::Probe(mChannel));
            Int32 total = 0;
            Int32 nWrite = 0;
            while(total < length)
            {
               mChannel->Write(buffer, &nWrite);
               total += nWrite;
            }
            return NOERROR;
        }

        ECode Close()
        {
            ICloseable::Probe(mChannel)->Close();
            return NOERROR;
        }
};

} // namespace Channels
} // namespace IO
} // namespace Elastos
