#include "CFileGestureLibrary.h"

namespace Elastos {
namespace Droid {
namespace Gesture {

ECode CFileGestureLibrary::constructor(
    /* [in] */ IFile *path)
{
    GestureLibraries::FileGestureLibrary::FileGestureLibrary(path);
    return NOERROR;
}

ECode CFileGestureLibrary::Save(
    /* [out] */ Boolean *isSaved)
{
    return GestureLibraries::FileGestureLibrary::Save(isSaved);
}

ECode CFileGestureLibrary::Load(
    /* [out] */ Boolean *isLoaded)
{
    return GestureLibraries::FileGestureLibrary::Load(isLoaded);
}

ECode CFileGestureLibrary::IsReadOnly(
    /* [out] */ Boolean *isReadOnly)
{
    return GestureLibraries::FileGestureLibrary::IsReadOnly(isReadOnly);
}

ECode CFileGestureLibrary::GetLearner(
    /* [out] */ ILearner **learner)
{
    return E_NOT_IMPLEMENTED;
}

ECode CFileGestureLibrary::SetOrientationStyle(
    /* [out] */ Int32 style)
{
    return E_NOT_IMPLEMENTED;
}

ECode CFileGestureLibrary::GetOrientationStyle(
    /* [out] */ Int32 *style)
{
    return E_NOT_IMPLEMENTED;
}

ECode CFileGestureLibrary::SetSequenceType(
    /* [in] */ Int32 type)
{
    return E_NOT_IMPLEMENTED;
}

ECode CFileGestureLibrary::GetSequenceType(
    /* [out] */ Int32 *type)
{
    return E_NOT_IMPLEMENTED;
}

ECode CFileGestureLibrary::GetGestureEntries(
    /* [out] */ IObjectContainer **entries)
{
    return E_NOT_IMPLEMENTED;
}

ECode CFileGestureLibrary::Recognize(
    /* [in] */ IGesture *gesture,
    /* [out] */ IObjectContainer **predictions)
{
    return E_NOT_IMPLEMENTED;
}

ECode CFileGestureLibrary::AddGesture(
    /* [in] */ const String& entryName,
    /* [in] */ IGesture *gesture)
{
    return E_NOT_IMPLEMENTED;
}

ECode CFileGestureLibrary::RemoveGesture(
    /* [in] */ const String& entryName,
    /* [in] */ IGesture *gesture)
{
    return E_NOT_IMPLEMENTED;
}

ECode CFileGestureLibrary::RemoveEntry(
    /* [in] */ const String& entryName)
{
    return E_NOT_IMPLEMENTED;
}

ECode CFileGestureLibrary::GetGestures(
    /* [in] */ const String& entryName,
    /* [out] */ IObjectContainer **gestures)
{
    return E_NOT_IMPLEMENTED;
}


}
}
}
