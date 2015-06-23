#ifndef __HH_CREATEOUTPUTSTREAMADAPTOR_H
#define __HH_CREATEOUTPUTSTREAMADAPTOR_H

#include "Elastos.Droid.Core_server.h"
#include <Elastos.CoreLibrary.h>
#include <skia/core/SkStream.h>

using Elastos::IO::IInputStream;
using Elastos::IO::IOutputStream;

namespace Elastos {
namespace Droid {
namespace Graphics {

SkStream* CreateInputStreamAdaptor(
    /* [in] */ IInputStream* stream,
    /* [in] */ ArrayOf<Byte>* storage,
    /* [in] */ Int32 markSize = 0);

SkWStream* CreateOutputStreamAdaptor(
    /* [in] */ IOutputStream* stream,
    /* [in] */ ArrayOf<Byte>* storage);

} // namespace Graphics
} // namespace Droid
} // namespace Elastos

#endif // __HH_CREATEOUTPUTSTREAMADAPTOR_H