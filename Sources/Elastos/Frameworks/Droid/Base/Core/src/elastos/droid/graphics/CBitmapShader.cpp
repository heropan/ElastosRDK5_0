
#include "ext/frameworkext.h"
#include "graphics/CBitmapShader.h"
#include "graphics/CBitmap.h"
#include <skia/core/SkShader.h>
#include <skia/core/SkBitmap.h>

namespace Elastos {
namespace Droid {
namespace Graphics {

CAR_OBJECT_IMPL(CBitmapShader);
ECode CBitmapShader::constructor(
    /* [in] */ IBitmap* bitmap,
    /* [in] */ ShaderTileMode tileX,
    /* [in] */ ShaderTileMode tileY)
{
    mBitmap = bitmap;
    mTileX = tileX;
    mTileY = tileY;
    Int64 b = ((CBitmap*)bitmap)->Ni();
    Init(NativeCreate(b, (Int32)tileX, (Int32)tileY));
    return NOERROR;
}

AutoPtr<IShader> CBitmapShader::Copy()
{
    AutoPtr<IBitmapShader> copy;
    CBitmapShader::New(mBitmap, mTileX, mTileY, (IBitmapShader**)&copy);
    CopyLocalMatrix(IShader::Probe(copy));
    return IShader::Probe(copy);
}

PInterface CBitmapShader::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_Shader) {
        return reinterpret_cast<PInterface>((Shader*)this);
    }
    else if (riid == EIID_IBitmapShader) {
        return ((IBitmapShader*)this);
    }
    return Shader::Probe(riid);
}

UInt32 CBitmapShader::AddRef()
{
    return Shader::AddRef();
}

UInt32 CBitmapShader::Release()
{
    return Shader::Release();
}

ECode CBitmapShader::GetInterfaceID(
    /* [in] */ IInterface* object,
    /* [out] */ InterfaceID* iid)
{
    return Shader::GetInterfaceID(object, iid);
}

Int64 CBitmapShader::NativeCreate(
    /* [in] */ Int64 bitmapHandle,
    /* [in] */ Int32 tileModeX,
    /* [in] */ Int32 tileModeY)
{
    const SkBitmap* bitmap = reinterpret_cast<SkBitmap*>(bitmapHandle);
    SkShader* s = SkShader::CreateBitmapShader(*bitmap,
            (SkShader::TileMode)tileModeX,
            (SkShader::TileMode)tileModeY);
    return reinterpret_cast<Int64>(s);
}

} // namespace Graphics
} // namepsace Droid
} // namespace Elastos