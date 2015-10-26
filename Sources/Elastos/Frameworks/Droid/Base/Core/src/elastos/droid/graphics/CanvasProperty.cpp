
#include "elastos/droid/graphics/NativePaint.h"
#include "elastos/droid/graphics/CanvasProperty.h"
#include "elastos/droid/graphics/Paint.h"

namespace Elastos {
namespace Droid {
namespace Graphics {

class CanvasPropertyPaint : public VirtualLightRefBase {
    PREVENT_COPY_AND_ASSIGN(CanvasPropertyPaint);
public:
    CanvasPropertyPaint(const SkPaint& initialValue) : value(initialValue) {}

    SkPaint value;
};

CAR_INTERFACE_IMPL(CanvasProperty, Object, ICanvasProperty);
AutoPtr<CanvasProperty> CanvasProperty::CreateFloat(
    /* [in] */ Float initialValue)
{
    return new CanvasProperty(nCreateFloat(initialValue));
}

AutoPtr<CanvasProperty> CanvasProperty::CreatePaint(
    /* [in] */ IPaint* initialValue)
{
    Paint* paint = (Paint*)initialValue;
    return new CanvasProperty(nCreatePaint(paint->mNativePaint));
}

ECode CanvasProperty::GetNativeContainer(
    /* [out] */ Int64* native)
{
    VALIDATE_NOT_NULL(native);
    assert(0 && "TODO");
    // return mProperty.get();
    return NOERROR;
}

CanvasProperty::CanvasProperty(
    /* [in] */ Int64 nativeContainer)
{
    assert(0 && "TODO");
    // mProperty = new VirtualRefBasePtr(nativeContainer);
}

Int64 CanvasProperty::nCreateFloat(
    /* [in] */ Float initialValue)
{
    return reinterpret_cast<Int64>(new CanvasPropertyPrimitive(initialValue));
}

Int64 CanvasProperty::nCreatePaint(
    /* [in] */ Int64 initialValuePaintPtr)
{
    const NativePaint* paint = reinterpret_cast<const NativePaint*>(initialValuePaintPtr);
    return reinterpret_cast<Int64>(new CanvasPropertyPaint(*paint));
}

} // namespace Graphics
} // namespace Droid
} // namespace Elastos
