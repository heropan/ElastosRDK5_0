
#include "ext/frameworkext.h"
#include "graphics/CPathMeasure.h"
#include <skia/core/SkPath.h>
#include <skia/core/SkPathMeasure.h>

namespace Elastos {
namespace Droid {
namespace Graphics {

struct PathMeasurePair
{
    PathMeasurePair() {}
    PathMeasurePair(const SkPath& path, Boolean forceClosed)
        : fPath(path), fMeasure(fPath, forceClosed) {}

    SkPath fPath;      // copy of the user's path
    SkPathMeasure fMeasure;   // this guy points to fPath
};

CPathMeasure::~CPathMeasure()
{
    NativeDestroy(mNativeInstance);
}

ECode CPathMeasure::constructor()
{
    mPath = NULL;
    mNativeInstance = NativeCreate(0, FALSE);
    return NOERROR;
}

ECode CPathMeasure::constructor(
    /* [in] */ IPath* path,
    /* [in] */ Boolean forceClosed)
{
    // The native implementation does not copy the path, prevent it from being GC'd
    mPath = path;
    mNativeInstance = NativeCreate(
                        path != NULL ? ((CPath*)path)->Ni() : 0,
                        forceClosed);
    return NOERROR;
}

ECode CPathMeasure::SetPath(
    /* [in] */ IPath* path,
    /* [in] */ Boolean forceClosed)
{
    mPath = path;
    NativeSetPath(
            mNativeInstance,
            path != NULL ? ((CPath*)path)->Ni() : 0,
            forceClosed);
    return NOERROR;
}

ECode CPathMeasure::GetLength(
    /* [out] */ Float* length)
{
    VALIDATE_NOT_NULL(length);

    *length = NativeGetLength(mNativeInstance);
    return NOERROR;
}

ECode CPathMeasure::GetPosTan(
    /* [in] */ Float distance,
    /* [out] */ ArrayOf<Float>* pos,
    /* [out] */ ArrayOf<Float>* tan,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    if ((pos != NULL && pos->GetLength() < 2) ||
        (tan != NULL && tan->GetLength() < 2)) {
        // throw new ArrayIndexOutOfBoundsException();
        return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }
    *result = NativeGetPosTan(mNativeInstance, distance, pos, tan);
    return NOERROR;
}

ECode CPathMeasure::GetMatrix(
    /* [in] */ Float distance,
    /* [in] */ IMatrix* matrix,
    /* [in] */ Int32 flags,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    *result = NativeGetMatrix(mNativeInstance, distance,
            ((Matrix*)matrix->Probe(EIID_Matrix))->Ni(), flags);
    return NOERROR;
}

ECode CPathMeasure::GetSegment(
    /* [in] */ Float startD,
    /* [in] */ Float stopD,
    /* [in] */ IPath* dst,
    /* [in] */ Boolean startWithMoveTo,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    *result = NativeGetSegment(mNativeInstance, startD, stopD,
            ((CPath*)dst)->Ni(), startWithMoveTo);
    return NOERROR;
}

ECode CPathMeasure::IsClosed(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    *result = NativeIsClosed(mNativeInstance);
    return NOERROR;
}

ECode CPathMeasure::NextContour(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    *result = NativeNextContour(mNativeInstance);
    return NOERROR;
}

Int32 CPathMeasure::NativeCreate(
    /* [in] */ Int32 nativePath,
    /* [in] */ Boolean forceClosed)
{
    return nativePath ? (Int32)new PathMeasurePair(*(SkPath*)nativePath, forceClosed) : (Int32)new PathMeasurePair;
}

void CPathMeasure::NativeSetPath(
    /* [in] */ Int32 nativeInstance,
    /* [in] */ Int32 nativePath,
    /* [in] */ Boolean forceClosed)
{
    PathMeasurePair* pair = (PathMeasurePair*)nativeInstance;
    if (0 == nativePath) {
        pair->fPath.reset();
    }
    else {
        pair->fPath = *(SkPath*)nativePath;
    }
    pair->fMeasure.setPath(&pair->fPath, forceClosed);
}

Float CPathMeasure::NativeGetLength(
    /* [in] */ Int32 nativeInstance)
{
    PathMeasurePair* pair = (PathMeasurePair*)nativeInstance;
    return SkScalarToFloat(pair->fMeasure.getLength());;
}

static void ConvertTwoElemFloatArray(
    /* [out] */ ArrayOf<Float>*  array,
    /* [in] */ SkScalar* src)
{
    assert(array != NULL);

    if (array->GetLength() < 2) {
        sk_throw();
    }
    (*array)[0] = SkScalarToFloat(src[0]);
    (*array)[1] = SkScalarToFloat(src[1]);
}

Boolean CPathMeasure::NativeGetPosTan(
    /* [in] */ Int32 nativeInstance,
    /* [in] */ Float distance,
    /* [out] */ ArrayOf<Float>* pos,
    /* [out] */ ArrayOf<Float>* tan)
{
    PathMeasurePair* pair = (PathMeasurePair*)nativeInstance;
    SkScalar tmpPos[2], tmpTan[2];
    SkScalar* posPtr = pos ? tmpPos : NULL;
    SkScalar* tanPtr = tan ? tmpTan : NULL;

    if (!pair->fMeasure.getPosTan(SkFloatToScalar(distance), (SkPoint*)posPtr,
            (SkVector*)tanPtr)) {
        return FALSE;
    }

    if (pos) {
        ConvertTwoElemFloatArray(pos, tmpPos);
    }
    if (tan) {
        ConvertTwoElemFloatArray(tan, tmpTan);
    }
    return TRUE;
}

Boolean CPathMeasure::NativeGetMatrix(
    /* [in] */ Int32 nativeInstance,
    /* [in] */ Float distance,
    /* [out] */ Int32 nativeMatrix,
    /* [in] */ Int32 flags)
{
    PathMeasurePair* pair = (PathMeasurePair*)nativeInstance;
    return pair->fMeasure.getMatrix(SkFloatToScalar(distance), (SkMatrix*)nativeMatrix,
        (SkPathMeasure::MatrixFlags)flags);
}

Boolean CPathMeasure::NativeGetSegment(
    /* [in] */ Int32 nativeInstance,
    /* [in] */ Float startF,
    /* [in] */ Float stopF,
    /* [out] */ Int32 nativePath,
    /* [in] */ Boolean startWithMoveTo)
{
    PathMeasurePair* pair = (PathMeasurePair*)nativeInstance;
    return pair->fMeasure.getSegment(SkFloatToScalar(startF), SkFloatToScalar(stopF),
            (SkPath*)nativePath, startWithMoveTo);
}

Boolean CPathMeasure::NativeIsClosed(
    /* [in] */ Int32 nativeInstance)
{
    PathMeasurePair* pair = (PathMeasurePair*)nativeInstance;
    return pair->fMeasure.isClosed();;
}

Boolean CPathMeasure::NativeNextContour(
    /* [in] */ Int32 nativeInstance)
{
    PathMeasurePair* pair = (PathMeasurePair*)nativeInstance;
    return pair->fMeasure.nextContour();;
}

void CPathMeasure::NativeDestroy(
    /* [in] */ Int32 nativeInstance)
{
    delete (PathMeasurePair*)nativeInstance;
}

} // namespace Graphics
} // namepsace Droid
} // namespace Elastos
