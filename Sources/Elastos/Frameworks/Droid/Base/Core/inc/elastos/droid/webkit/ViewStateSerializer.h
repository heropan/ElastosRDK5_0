
#ifndef __ELASTOS_DROID_WEBKIT_VIEWSTATESERIALIZER_H__
#define __ELASTOS_DROID_WEBKIT_VIEWSTATESERIALIZER_H__

#include "elastos/droid/ext/frameworkext.h"

using Elastos::IO::IOutputStream;
using Elastos::IO::IInputStream;

namespace Elastos {
namespace Droid {
namespace Webkit {

class CWebViewCoreDrawData;

/**
 * @hide
 */
class ViewStateSerializer : public ElRefBase
{
public:
    static CARAPI_(Boolean) SerializeViewState(
        /* [in] */ IOutputStream* stream,
        /* [in] */ CWebViewCoreDrawData* draw);

    static CARAPI_(AutoPtr<CWebViewCoreDrawData>) DeserializeViewState(
        /* [in] */ IInputStream* stream);

    static CARAPI_(void) DumpLayerHierarchy(
        /* [in] */ Int32 baseLayer,
        /* [in] */ IOutputStream* out,
        /* [in] */ Int32 level);

public:
    static const Int32 VERSION = 1;

private:
    ViewStateSerializer();

    static CARAPI_(void) NativeDumpLayerHierarchy(
        /* [in] */ Int32 baseLayer,
        /* [in] */ Int32 level,
        /* [in] */ IOutputStream* out,
        /* [in] */ ArrayOf<Byte>* storage);

    static CARAPI_(Boolean) NativeSerializeViewState(
        /* [in] */ Int32 baseLayer,
        /* [in] */ IOutputStream* stream,
        /* [in] */ ArrayOf<Byte>* storage);

    // Returns a pointer to the BaseLayer
    static CARAPI_(Int32) NativeDeserializeViewState(
        /* [in] */ Int32 version,
        /* [in] */ IInputStream* stream,
        /* [in] */ ArrayOf<Byte>* storage);

private:
    static const Int32 WORKING_STREAM_STORAGE = 16 * 1024;
};

} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_WEBKIT_VIEWSTATESERIALIZER_H__
