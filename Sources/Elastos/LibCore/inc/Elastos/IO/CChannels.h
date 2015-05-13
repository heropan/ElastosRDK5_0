
#ifndef __CCHANNELS_H__
#define __CCHANNELS_H__

#include "_CChannels.h"

using Elastos::IO::Charset::ICharsetEncoder;
using Elastos::IO::Charset::ICharsetDecoder;

namespace Elastos {
namespace IO {
namespace Channels{

CarClass(CChannels)
{
public:
    CChannels();

    ~CChannels();

    CARAPI NewInputStream(
        /* [in] */ IReadableByteChannel * pChannel,
        /* [out] */ IInputStream ** ppInputStream);

    CARAPI NewOutputStream(
        /* [in] */ IWritableByteChannel * pChannel,
        /* [out] */ IOutputStream ** ppOutputStream);

    CARAPI NewChannel(
        /* [in] */ IInputStream * pInputStream,
        /* [out] */ IReadableByteChannel ** ppInputChannel);

    CARAPI NewChannelEx(
        /* [in] */ IOutputStream * pOutputStream,
        /* [out] */ IWritableByteChannel ** ppOutputChannel);

    CARAPI NewReader(
        /* [in] */ IReadableByteChannel * pChannel,
        /* [in] */ ICharsetDecoder * pDecoder,
        /* [in] */ Int32 minBufferCapacity,
        /* [out] */ IReader ** ppReader);

    CARAPI NewReaderEx(
        /* [in] */ IReadableByteChannel * pChannel,
        /* [in] */ const String& charsetName,
        /* [out] */ IReader ** ppReader);

    CARAPI NewWriter(
        /* [in] */ IWritableByteChannel * pChannel,
        /* [in] */ ICharsetEncoder * pEncoder,
        /* [in] */ Int32 minBufferCapacity,
        /* [out] */ IWriter ** ppWriter);

    CARAPI NewWriterEx(
        /* [in] */ IWritableByteChannel * pChannel,
        /* [in] */ const String& charsetName,
        /* [out] */ IWriter ** ppWriter);

    CARAPI constructor();

private:
    // TODO: Add your private member variables here.
};

} // namespace Channels
} // namespace IO
} // namespace Elastos

#endif // __CCHANNELS_H__
