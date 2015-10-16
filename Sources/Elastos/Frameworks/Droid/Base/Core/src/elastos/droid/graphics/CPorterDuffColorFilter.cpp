
#include "elastos/droid/graphics/CPorterDuffColorFilter.h"
#include <skia/core/SkColorFilter.h>
#include <skia/effects/SkPorterDuff.h>

namespace Elastos {
namespace Droid {
namespace Graphics {

CAR_OBJECT_IMPL(CPorterDuffColorFilter);
CPorterDuffColorFilter::CPorterDuffColorFilter()
    : mColor(0)
    , mMode(-1)
{}

PInterface CPorterDuffColorFilter::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_ColorFilter) {
        return reinterpret_cast<PInterface>((ColorFilter*)this);
    }
    else if (riid == EIID_IPorterDuffColorFilter) {
        return (IPorterDuffColorFilter*)this;
    }
    return ColorFilter::Probe(riid);
}

UInt32 CPorterDuffColorFilter::AddRef()
{
    return ColorFilter::AddRef();
}

UInt32 CPorterDuffColorFilter::Release()
{
    return ColorFilter::Release();
}

ECode CPorterDuffColorFilter::GetInterfaceID(
    /* [in] */ IInterface* object,
    /* [out] */ InterfaceID* iid)
{
    return ColorFilter::GetInterfaceID(object, iid);
}

ECode CPorterDuffColorFilter::constructor(
    /* [in] */ Int32 srcColor,
    /* [in] */ PorterDuffMode mode)
{
    mColor = srcColor;
    mMode = mode;
    Update();
    return NOERROR;
}

ECode CPorterDuffColorFilter::GetColor(
    /* [out] */ Int32* color)
{
    VALIDATE_NOT_NULL(color);
    *color = mColor;
    return NOERROR;
}

ECode CPorterDuffColorFilter::SetColor(
    /* [in] */ Int32 color)
{
    mColor = color;
    Update();
    return NOERROR;
}

ECode CPorterDuffColorFilter::GetMode(
    /* [out] */ PorterDuffMode* mode)
{
    VALIDATE_NOT_NULL(mode);
    *mode = mMode;
    return NOERROR;
}

ECode CPorterDuffColorFilter::SetMode(
    /* [in] */ PorterDuffMode mode)
{
    mMode = mode;
    Update();
    return NOERROR;
}

void CPorterDuffColorFilter::Update()
{
    DestroyFilter(mNativeInstance);
    mNativeInstance = NativeCreatePorterDuffFilter(mColor, mMode);
}

ECode CPorterDuffColorFilter::Equals(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* equals)
{
    assert(0 && "TODO");
    // if (this == object) {
    //     return true;
    // }
    // if (object == null || getClass() != object.getClass()) {
    //     return false;
    // }
    // final PorterDuffColorFilter other = (PorterDuffColorFilter) object;
    // if (mColor != other.mColor || mMode != other.mMode) {
    //     return false;
    // }
    // return true;
    return NOERROR;
}

ECode CPorterDuffColorFilter::GetHashCode(
    /* [out] */ Int32* code)
{
    VALIDATE_NOT_NULL(code);
    // return 31 *  mMode.hashCode() + mColor;
    *code = 31 *  mMode + mColor;
    return NOERROR;
}

Int64 CPorterDuffColorFilter::NativeCreatePorterDuffFilter(
    /* [in] */ Int32 srcColor,
    /* [in] */ Int32 porterDuffMode)
{
    assert(0 && "need jni.");
}

} // namespace Graphics
} // namepsace Droid
} // namespace Elastos
