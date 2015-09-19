
#include "ext/frameworkext.h"
#include "graphics/CRegion.h"
#include "graphics/CRect.h"
#include "graphics/CPath.h"
#include "graphics/GraphicsNative.h"
#include <binder/Parcel.h>
#include <skia/core/SkRegion.h>
#include <skia/core/SkPath.h>

namespace Elastos {
namespace Droid {
namespace Graphics {

ECode CRegion::constructor()
{
    mNativeRegion = NativeConstructor();
    return NOERROR;
}

ECode CRegion::constructor(
    /* [in] */ IRegion* region)
{
    mNativeRegion = NativeConstructor();
    NativeSetRegion(mNativeRegion, ((CRegion*)region)->mNativeRegion);
    return NOERROR;
}

ECode CRegion::constructor(
    /* [in] */ IRect* r)
{
    mNativeRegion = NativeConstructor();
    CRect* rect = (CRect*)r;
    NativeSetRect(mNativeRegion, rect->mLeft, rect->mTop, rect->mRight, rect->mBottom);
    return NOERROR;
}

ECode CRegion::constructor(
    /* [in] */ Int32 left,
    /* [in] */ Int32 top,
    /* [in] */ Int32 right,
    /* [in] */ Int32 bottom)
{
    mNativeRegion = NativeConstructor();
    NativeSetRect(mNativeRegion, left, top, right, bottom);
    return NOERROR;
}

ECode CRegion::constructor(
    /* [in] */ Int32 ni)
{
    if ((SkRegion*)ni == NULL) {
        //throw new RuntimeException();
        return E_RUNTIME_EXCEPTION;
    }

    mNativeRegion = ni;
    return NOERROR;
}

CRegion::~CRegion()
{
    NativeDestructor(mNativeRegion);
}

ECode CRegion::SetEmpty()
{
    NativeSetRect(mNativeRegion, 0, 0, 0, 0);
    return NOERROR;
}

ECode CRegion::Set(
    /* [in] */ IRegion* src,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    *result = NativeSetRegion(mNativeRegion, ((CRegion*)src)->mNativeRegion);
    return NOERROR;
}

ECode CRegion::Set(
    /* [in] */ IRect* r,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    CRect* rect = (CRect*)r;
    *result = NativeSetRect(mNativeRegion, rect->mLeft, rect->mTop, rect->mRight, rect->mBottom);
    return NOERROR;
}

ECode CRegion::Set(
    /* [in] */ Int32 left,
    /* [in] */ Int32 top,
    /* [in] */ Int32 right,
    /* [in] */ Int32 bottom,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    *result = NativeSetRect(mNativeRegion, left, top, right, bottom);
    return NOERROR;
}

ECode CRegion::SetPath(
    /* [in] */ IPath* path,
    /* [in] */ IRegion* clip,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    *result = NativeSetPath(mNativeRegion, ((CPath*)path)->Ni(), ((CRegion*)clip)->mNativeRegion);
    return NOERROR;
}

ECode CRegion::IsEmpty(
    /* [out] */ Boolean* isEmpty)
{
    VALIDATE_NOT_NULL(isEmpty);

    *isEmpty = ((SkRegion*)mNativeRegion)->isEmpty();
    return NOERROR;
}

ECode CRegion::IsRect(
    /* [out] */ Boolean* isRect)
{
    VALIDATE_NOT_NULL(isRect);

    *isRect = ((SkRegion*)mNativeRegion)->isRect();
    return NOERROR;
}

ECode CRegion::IsComplex(
    /* [out] */ Boolean* isComplex)
{
    VALIDATE_NOT_NULL(isComplex);

    *isComplex = ((SkRegion*)mNativeRegion)->isComplex();
    return NOERROR;
}

ECode CRegion::GetBounds(
    /* [out] */ IRect** bounds)
{
    CRect::New(bounds);
    NativeGetBounds(mNativeRegion, *bounds);
    return NOERROR;
}

ECode CRegion::GetBounds(
    /* [in, out] */ IRect* r,
    /* [out] */ Boolean* result)
{
    if (r == NULL) {
        //throw new NullPointerException();
        return E_NULL_POINTER_EXCEPTION;
    }
    *result = NativeGetBounds(mNativeRegion, r);
    return NOERROR;
}

/**
 * Return the boundary of the region as a new Path. If the region is empty,
 * the path will also be empty.
 */
ECode CRegion::GetBoundaryPath(
    /* [out] */ IPath** path)
{
    VALIDATE_NOT_NULL(path);

    CPath::New(path);
    NativeGetBoundaryPath(mNativeRegion, ((CPath*)*path)->Ni());
    return NOERROR;
}

/**
 * Set the path to the boundary of the region. If the region is empty, the
 * path will also be empty.
 */
ECode CRegion::GetBoundaryPath(
    /* [in, out] */ IPath* path,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    *result = NativeGetBoundaryPath(mNativeRegion, ((CPath*)path)->Ni());
    return NOERROR;
}

ECode CRegion::Contains(
    /* [in] */ Int32 x,
    /* [in] */ Int32 y,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    *result = ((SkRegion*)mNativeRegion)->contains(x, y);
    return NOERROR;
}

ECode CRegion::QuickContains(
    /* [in] */ IRect* r,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    CRect* rect = (CRect*)r;
    return QuickContains(rect->mLeft, rect->mTop, rect->mRight, rect->mBottom, result);
}

ECode CRegion::QuickContains(
    /* [in] */ Int32 left,
    /* [in] */ Int32 top,
    /* [in] */ Int32 right,
    /* [in] */ Int32 bottom,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    *result = ((SkRegion*)mNativeRegion)->quickContains(left, top, right, bottom);
    return NOERROR;
}

ECode CRegion::QuickReject(
    /* [in] */ IRect* r,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    CRect* rect = (CRect*)r;
    return QuickReject(rect->mLeft, rect->mTop, rect->mRight, rect->mBottom, result);
}

ECode CRegion::QuickReject(
    /* [in] */ Int32 left,
    /* [in] */ Int32 top,
    /* [in] */ Int32 right,
    /* [in] */ Int32 bottom,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    SkIRect ir;
    ir.set(left, top, right, bottom);
    *result = ((SkRegion*)mNativeRegion)->quickReject(ir);
    return NOERROR;
}

ECode CRegion::QuickReject(
    /* [in] */ IRegion* rgn,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    *result = ((SkRegion*)mNativeRegion)->quickReject(*(SkRegion*)((CRegion*)rgn)->mNativeRegion);
    return NOERROR;
}

ECode CRegion::Translate(
    /* [in] */ Int32 dx,
    /* [in] */ Int32 dy)
{
    return Translate(dx, dy, NULL);
}

ECode CRegion::Translate(
    /* [in] */ Int32 dx,
    /* [in] */ Int32 dy,
    /* [in] */ IRegion* dst)
{
    if (dst) {
        ((SkRegion*)mNativeRegion)->translate(dx, dy, (SkRegion*)((CRegion*)dst)->mNativeRegion);
    }
    else {
        ((SkRegion*)mNativeRegion)->translate(dx, dy);
    }

    return NOERROR;
}

ECode CRegion::Scale(
    /* [in] */ Float scale)
{
    return Scale(scale, NULL);
}

// Scale the rectangle by given scale and set the reuslt to the dst.
static void ScaleRect(
    /* [in] */ SkIRect* dst,
    /* [in] */ const SkIRect& src,
    /* [in] */ Float scale)
{
   dst->fLeft = (Int32)::roundf(src.fLeft * scale);
   dst->fTop = (Int32)::roundf(src.fTop * scale);
   dst->fRight = (Int32)::roundf(src.fRight * scale);
   dst->fBottom = (Int32)::roundf(src.fBottom * scale);
}

// Scale the region by given scale and set the reuslt to the dst.
// dest and src can be the same region instance.
static void ScaleRgn(
    /* [in] */ SkRegion* dst,
    /* [in] */ const SkRegion& src,
    /* [in] */ Float scale)
{
   SkRegion tmp;
   SkRegion::Iterator iter(src);

   for (; !iter.done(); iter.next()) {
       SkIRect r;
       ScaleRect(&r, iter.rect(), scale);
       tmp.op(r, SkRegion::kUnion_Op);
   }
   dst->swap(tmp);
}

ECode CRegion::Scale(
    /* [in] */ Float scale,
    /* [in] */ IRegion* dst)
{
    if (dst) {
        ScaleRgn((SkRegion*)((CRegion*)dst)->mNativeRegion, *(SkRegion*)mNativeRegion, scale);
    }
    else {
        ScaleRgn((SkRegion*)mNativeRegion, *(SkRegion*)mNativeRegion, scale);
    }

    return NOERROR;
}

ECode CRegion::Union(
    /* [in] */ IRect* r,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    return Op(r, RegionOp_UNION, result);
}

ECode CRegion::Op(
    /* [in] */ IRect* r,
    /* [in] */ RegionOp op,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    CRect* rect = (CRect*)r;
    *result = NativeOp(mNativeRegion, rect->mLeft, rect->mTop, rect->mRight, rect->mBottom, op);
    return NOERROR;
}

ECode CRegion::Op(
    /* [in] */ Int32 left,
    /* [in] */ Int32 top,
    /* [in] */ Int32 right,
    /* [in] */ Int32 bottom,
    /* [in] */ RegionOp op,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    *result = NativeOp(mNativeRegion, left, top, right, bottom, op);
    return NOERROR;
}

ECode CRegion::Op(
    /* [in] */ IRegion* region,
    /* [in] */ RegionOp op,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    return Op((IRegion*)this, region, op, result);
}

ECode CRegion::Op(
    /* [in] */ IRect* rect,
    /* [in] */ IRegion* region,
    /* [in] */ RegionOp op,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    *result = NativeOp(mNativeRegion, rect, ((CRegion*)region)->mNativeRegion, op);
    return NOERROR;
}

ECode CRegion::Op(
    /* [in] */ IRegion* region1,
    /* [in] */ IRegion* region2,
    /* [in] */ RegionOp op,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    *result = NativeOp(
                mNativeRegion,
                ((CRegion*)region1)->mNativeRegion,
                ((CRegion*)region2)->mNativeRegion,
                op);
    return NOERROR;
}

ECode CRegion::Equals(
    /* [in] */ IRegion* r,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = FALSE;
    if (r != NULL) {
        *result = NativeEquals(mNativeRegion, ((CRegion*)r)->mNativeRegion);
    }
    return NOERROR;
}

ECode CRegion::Equals(
    /* [in] */ IInterface* region,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    return Equals(IRegion::Probe(region), result);
}

ECode CRegion::GetHashCode(
    /* [out] */ Int32* hash)
{
    VALIDATE_NOT_NULL(hash);
    *hash = (Int32)mNativeRegion;
    return NOERROR;
}

ECode CRegion::ToString(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str);

    *str = NativeToString(mNativeRegion);
    return NOERROR;
}

ECode CRegion::GetNativeRegion(
    /* [out] */ Handle32* region)
{
    VALIDATE_NOT_NULL(region);

    *region = (Handle32)mNativeRegion;
     return NOERROR;
}

ECode CRegion::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    mNativeRegion = NativeCreateFromParcel(source);
    return NOERROR;
}

ECode CRegion::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    if (!NativeWriteToParcel(mNativeRegion, dest)) {
        //throw new RuntimeException();
        return E_RUNTIME_EXCEPTION;
    }
    return NOERROR;
}

Int32 CRegion::Ni()
{
    return mNativeRegion;
}

Boolean CRegion::NativeEquals(
    /* [in] */ Int32 region1,
    /* [in] */ Int32 region2)
{
    return *(SkRegion*)region1 == *(SkRegion*)region2;
}

Int32 CRegion::NativeConstructor()
{
    return (Int32)new SkRegion;
}

void CRegion::NativeDestructor(
    /* [in] */ Int32 region)
{
    SkASSERT(region);
    delete (SkRegion*)region;
}

Boolean CRegion::NativeSetRegion(
    /* [in] */ Int32 dst,
    /* [in] */ Int32 src)
{
    SkASSERT(dst && src);
    *(SkRegion*)dst = *(SkRegion*)src;
    return TRUE;
}

Boolean CRegion::NativeSetRect(
    /* [in] */ Int32 dst,
    /* [in] */ Int32 left,
    /* [in] */ Int32 top,
    /* [in] */ Int32 right,
    /* [in] */ Int32 bottom)
{
    return ((SkRegion*)dst)->setRect(left, top, right, bottom);
}

Boolean CRegion::NativeSetPath(
    /* [in] */ Int32 dst,
    /* [in] */ Int32 path,
    /* [in] */ Int32 clip)
{
    SkASSERT(dst && path && clip);
    return ((SkRegion*)dst)->setPath(*(SkPath*)path, *(SkRegion*)clip);
}

Boolean CRegion::NativeGetBounds(
    /* [in] */ Int32 region,
    /* [out] */ IRect* rect)
{
    GraphicsNative::SkIRect2IRect(((SkRegion*)region)->getBounds(), rect);
    return !((SkRegion*)region)->isEmpty();
}

Boolean CRegion::NativeGetBoundaryPath(
    /* [in] */ Int32 region,
    /* [out] */ Int32 path)
{
    return ((SkRegion*)region)->getBoundaryPath((SkPath*)path);
}

Boolean CRegion::NativeOp(
    /* [in] */ Int32 dst,
    /* [in] */ Int32 left,
    /* [in] */ Int32 top,
    /* [in] */ Int32 right,
    /* [in] */ Int32 bottom,
    /* [in] */ RegionOp op)
{
    SkIRect ir;
    ir.set(left, top, right, bottom);
    return ((SkRegion*)dst)->op(ir, (SkRegion::Op)op);
}

Boolean CRegion::NativeOp(
    /* [in] */ Int32 dst,
    /* [in] */ IRect* rect,
    /* [in] */ Int32 region,
    /* [in] */ RegionOp op)
{
    SkIRect ir;
    GraphicsNative::IRect2SkIRect(rect, &ir);
    return ((SkRegion*)dst)->op(ir, *(SkRegion*)region, (SkRegion::Op)op);
}

Boolean CRegion::NativeOp(
    /* [in] */ Int32 dst,
    /* [in] */ Int32 region1,
    /* [in] */ Int32 region2,
    /* [in] */ RegionOp op)
{
    return ((SkRegion*)dst)->op(*(SkRegion*)region1, *(SkRegion*)region2, (SkRegion::Op)op);
}

Int32 CRegion::NativeCreateFromParcel(
    /* [in] */ IParcel* parcel)
{
    if (parcel == NULL) {
        return 0;
    }

    Handle32 data;
    parcel->GetElementPayload(&data);
    android::Parcel* p = (android::Parcel*)data;

    SkRegion* region = new SkRegion;
    size_t size = p->readInt32();
    region->unflatten(p->readInplace(size));

    return (Int32)region;
}

Boolean CRegion::NativeWriteToParcel(
    /* [in] */ Int32 region,
    /* [out] */ IParcel* parcel)
{
    if (parcel == NULL) {
        return FALSE;
    }

    Handle32 data;
    parcel->GetElementPayload(&data);
    android::Parcel* p = (android::Parcel*)data;

    size_t size = ((SkRegion*)region)->flatten(NULL);
    p->writeInt32(size);
    ((SkRegion*)region)->flatten(p->writeInplace(size));

    return TRUE;
}

String CRegion::NativeToString(
    /* [in] */ Int32 region)
{
    char* str = ((SkRegion*)region)->toString();
    if (str == NULL) {
        return String(NULL);
    }
    String result(str);
    free(str);
    return result;
}

} // namespace Graphics
} // namepsace Droid
} // namespace Elastos
