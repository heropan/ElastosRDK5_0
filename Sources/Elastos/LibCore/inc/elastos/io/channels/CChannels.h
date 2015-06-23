
#ifndef __ELASTOS_IO_CCHANNELS_H__
#define __ELASTOS_IO_CCHANNELS_H__

#include "_Elastos_IO_Channels_CChannels.h"
#include "Singleton.h"
#include "AbstractInterruptibleChannel.h"

using Elastos::Core::Singleton;
using Elastos::IO::Charset::ICharsetEncoder;
using Elastos::IO::Charset::ICharsetDecoder;
using Elastos::IO::Channels::Spi::AbstractInterruptibleChannel;

namespace Elastos {
namespace IO {
namespace Channels{

CarClass(CChannels)
    , public Singleton
    , public IChannels
{
public:
    CAR_INTERFACE_DECL()

    CAR_SINGLETON_DECL()

    CARAPI NewInputStream(
        /* [in] */ IReadableByteChannel * pChannel,
        /* [out] */ IInputStream ** ppInputStream);

    CARAPI NewOutputStream(
        /* [in] */ IWritableByteChannel * pChannel,
        /* [out] */ IOutputStream ** ppOutputStream);

    CARAPI NewChannel(
        /* [in] */ IInputStream * pInputStream,
        /* [out] */ IReadableByteChannel ** ppInputChannel);

    CARAPI NewChannel(
        /* [in] */ IOutputStream * pOutputStream,
        /* [out] */ IWritableByteChannel ** ppOutputChannel);

    CARAPI NewReader(
        /* [in] */ IReadableByteChannel * pChannel,
        /* [in] */ ICharsetDecoder * pDecoder,
        /* [in] */ Int32 minBufferCapacity,
        /* [out] */ IReader ** ppReader);

    CARAPI NewReader(
        /* [in] */ IReadableByteChannel * pChannel,
        /* [in] */ const String& charsetName,
        /* [out] */ IReader ** ppReader);

    CARAPI NewWriter(
        /* [in] */ IWritableByteChannel * pChannel,
        /* [in] */ ICharsetEncoder * pEncoder,
        /* [in] */ Int32 minBufferCapacity,
        /* [out] */ IWriter ** ppWriter);

    CARAPI NewWriter(
        /* [in] */ IWritableByteChannel * pChannel,
        /* [in] */ const String& charsetName,
        /* [out] */ IWriter ** ppWriter);
};

} // namespace Channels
} // namespace IO
} // namespace Elastos

#endif // __ELASTOS_IO_CCHANNELS_H__
