#ifndef __CDATAGRAMCHANNELHELPER_H__
#define __CDATAGRAMCHANNELHELPER_H__

#include "_CDatagramChannelHelper.h"

namespace Elastos {
namespace IO {
namespace Channels {

CarClass(CDatagramChannelHelper)
{
public:

    /**
     * Creates an opened and not-connected datagram channel.
     * <p>
     * This channel is created by calling the <code>openDatagramChannel</code>
     * method of the default {@link SelectorProvider} instance.
     *
     * @return the new channel which is open but not connected.
     * @throws IOException
     *             if some I/O error occurs.
     */
    CARAPI Open(
        /* [out] */ IDatagramChannel** channel);

};

} // namespace Channels
} // namespace IO
} // namespace Elastos

#endif // __CDATAGRAMCHANNELHELPER_H__
