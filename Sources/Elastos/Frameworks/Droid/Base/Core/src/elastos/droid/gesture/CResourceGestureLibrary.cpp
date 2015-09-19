#include "CResourceGestureLibrary.h"

namespace Elastos {
namespace Droid {
namespace Gesture {

ECode CResourceGestureLibrary::constructor(
    /* [in] */ IContext *context,
    /* [in] */ Int32 resourceId)
{
    GestureLibraries::ResourceGestureLibrary::
            ResourceGestureLibrary(context, resourceId);
    return NOERROR;
}

ECode CResourceGestureLibrary::Save(
    /* [out] */ Boolean *isSaved)
{
    return GestureLibraries::ResourceGestureLibrary::Save(isSaved);
}

ECode CResourceGestureLibrary::Load(
    /* [out] */ Boolean *isLoaded)
{
    return GestureLibraries::ResourceGestureLibrary::Load(isLoaded);
}

ECode CResourceGestureLibrary::IsReadOnly(
    /* [out] */ Boolean *isReadOnly)
{
    return GestureLibraries::ResourceGestureLibrary::IsReadOnly(isReadOnly);
}

ECode CResourceGestureLibrary::GetLearner(
    /* [out] */ ILearner **learner)
{
    return E_NOT_IMPLEMENTED;
}

ECode CResourceGestureLibrary::SetOrientationStyle(
    /* [out] */ Int32 style)
{
    return E_NOT_IMPLEMENTED;
}

ECode CResourceGestureLibrary::GetOrientationStyle(
    /* [out] */ Int32 *style)
{
    return E_NOT_IMPLEMENTED;
}

ECode CResourceGestureLibrary::SetSequenceType(
    /* [in] */ Int32 type)
{
    return E_NOT_IMPLEMENTED;
}

ECode CResourceGestureLibrary::GetSequenceType(
    /* [out] */ Int32 *type)
{
    return E_NOT_IMPLEMENTED;
}

ECode CResourceGestureLibrary::GetGestureEntries(
    /* [out] */ IObjectContainer **entries)
{
    return E_NOT_IMPLEMENTED;
}

ECode CResourceGestureLibrary::Recognize(
    /* [in] */ IGesture *gesture,
    /* [out] */ IObjectContainer **predictions)
{
    return E_NOT_IMPLEMENTED;
}

ECode CResourceGestureLibrary::AddGesture(
    /* [in] */ const String& entryName,
    /* [in] */ IGesture *gesture)
{
    return E_NOT_IMPLEMENTED;
}

ECode CResourceGestureLibrary::RemoveGesture(
    /* [in] */ const String& entryName,
    /* [in] */ IGesture *gesture)
{
    return E_NOT_IMPLEMENTED;
}

ECode CResourceGestureLibrary::RemoveEntry(
    /* [in] */ const String& entryName)
{
    return E_NOT_IMPLEMENTED;
}

ECode CResourceGestureLibrary::GetGestures(
    /* [in] */ const String& entryName,
    /* [out] */ IObjectContainer **gestures)
{
    return E_NOT_IMPLEMENTED;
}


}
}
}
