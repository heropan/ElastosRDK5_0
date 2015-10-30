
#include "elastos/droid/graphics/CPoint.h"
#include "elastos/droid/graphics/CreateOutputStreamAdaptor.h"
#include "elastos/droid/webkit/ViewStateSerializer.h"
#include "elastos/droid/webkit/CWebViewCore.h"

using Elastos::Droid::Graphics::CPoint;
using Elastos::Droid::Graphics::IPoint;
using Elastos::Droid::Graphics::CreateOutputStreamAdaptor;
using Elastos::IO::CDataInputStream;
using Elastos::IO::IDataInputStream;
using Elastos::IO::CDataOutputStream;
using Elastos::IO::IDataOutputStream;

#ifdef __cplusplus
extern "C"
{
#endif
    extern void Elastos_ViewStateSerializer_nativeDumpLayerHierarchy(Int32 jbaseLayer,
            Int32 level, IInterface* jstream, ArrayOf<Byte>* jstorage, SkWStream *stream);
    extern Boolean Elastos_ViewStateSerializer_nativeSerializeViewState(Int32 jbaseLayer, IInterface* jstream, ArrayOf<Byte>* jstorage, SkWStream *stream);
    extern Int32 Elastos_ViewStateSerializer_nativeDeserializeViewState(Int32 version,IInterface* jstream, ArrayOf<Byte>* jstorage);
#ifdef __cplusplus
}
#endif

namespace Elastos {
namespace Droid {
namespace Webkit {

const Int32 ViewStateSerializer::WORKING_STREAM_STORAGE;

const Int32 ViewStateSerializer::VERSION;

ViewStateSerializer::ViewStateSerializer()
{
}

Boolean ViewStateSerializer::SerializeViewState(
    /* [in] */ IOutputStream* stream,
    /* [in] */ CWebViewCoreDrawData* draw)
{
    assert(stream);
    assert(draw);
    Int32 baseLayer = draw->mBaseLayer;
    if (baseLayer == 0) {
        return FALSE;
    }
    AutoPtr<IDataOutputStream> dos;
    CDataOutputStream::New(stream, (IDataOutputStream**)&dos);
    dos->Write(VERSION);
    Int32 x, y;
    draw->mContentSize->GetX(&x);
    draw->mContentSize->GetY(&y);
    dos->Write(x);
    dos->Write(y);
    AutoPtr< ArrayOf<Byte> > array = ArrayOf<Byte>::Alloc(WORKING_STREAM_STORAGE);
    return NativeSerializeViewState(baseLayer, dos, array);
}

AutoPtr<CWebViewCoreDrawData> ViewStateSerializer::DeserializeViewState(
    /* [in] */ IInputStream* stream)
{
    AutoPtr<IDataInputStream> dis;
    CDataInputStream::New(stream, (IDataInputStream**)&dis);
    Int32 version;
    dis->Read(&version);
    if (version > VERSION) {
        //throw new IOException("Unexpected version: " + version);
        assert(0);
    }
    Int32 contentWidth;
    Int32 contentHeight;
    dis->Read(&contentWidth);
    dis->Read(&contentHeight);
    AutoPtr< ArrayOf<Byte> > array = ArrayOf<Byte>::Alloc(WORKING_STREAM_STORAGE);
    Int32 baseLayer = NativeDeserializeViewState(version, dis, array);
    AutoPtr<CWebViewCoreDrawData> draw = new CWebViewCoreDrawData();
    draw->mViewState = new CWebViewCoreViewState();
    CPoint::New(contentWidth, contentHeight, (IPoint**)&(draw->mContentSize));
    draw->mBaseLayer = baseLayer;
    stream->Close();
    return draw;
}

void ViewStateSerializer::DumpLayerHierarchy(
    /* [in] */ Int32 baseLayer,
    /* [in] */ IOutputStream* out,
    /* [in] */ Int32 level)
{
    AutoPtr< ArrayOf<Byte> > array = ArrayOf<Byte>::Alloc(WORKING_STREAM_STORAGE);
    NativeDumpLayerHierarchy(baseLayer, level, out, array);
}

void ViewStateSerializer::NativeDumpLayerHierarchy(
    /* [in] */ Int32 jbaseLayer,
    /* [in] */ Int32 level,
    /* [in] */ IOutputStream* jstream,
    /* [in] */ ArrayOf<Byte>* jstorage)
{
    SkWStream *stream = CreateOutputStreamAdaptor(jstream, jstorage);
    Elastos_ViewStateSerializer_nativeDumpLayerHierarchy(jbaseLayer, level, jstream, jstorage, stream);
}

Boolean ViewStateSerializer::NativeSerializeViewState(
    /* [in] */ Int32 jbaseLayer,
    /* [in] */ IOutputStream* jstream,
    /* [in] */ ArrayOf<Byte>* jstorage)
{
    SkWStream *stream = CreateOutputStreamAdaptor(jstream, jstorage);
    return Elastos_ViewStateSerializer_nativeSerializeViewState(jbaseLayer, jstream, jstorage, stream);
}

// Returns a pointer to the BaseLayer
Int32 ViewStateSerializer::NativeDeserializeViewState(
    /* [in] */ Int32 version,
    /* [in] */ IInputStream* jstream,
    /* [in] */ ArrayOf<Byte>* jstorage)
{
    return Elastos_ViewStateSerializer_nativeDeserializeViewState(version, jstream, jstorage);
}

ECode ViewStateSerializer::ToString(
    /* [out] */ String* info)
{
    VALIDATE_NOT_NULL(info);
    *info = "ViewStateSerializer";
    return NOERROR;
}

} // namespace Webkit
} // namespace Droid
} // namespace Elastos
