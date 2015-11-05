
#include "elastos/droid/gesture/GestureLibraries.h"

using Elastos::IO::IFileInputStream;
using Elastos::IO::CFileInputStream;
using Elastos::IO::IFileOutputStream;
using Elastos::IO::CFileOutputStream;

namespace Elastos {
namespace Droid {
namespace Gesture {

CAR_INTERFACE_IMPL(GestureLibraries, Object, IGestureLibraries);

GestureLibraries::GestureLibraries()
{}

GestureLibraries::~GestureLibraries()
{}

ECode GestureLibraries::FromFile(
    /* [in] */ const String& path,
    /* [out] */ IGestureLibrary **instance)
{
    AutoPtr<IFile> file;
    CFile::New(path, (IFile**)&file);
    return FromFile(file, instance);
}

ECode GestureLibraries::FromFile(
    /* [in] */ IFile *path,
    /* [out] */ IGestureLibrary **instance)
{
    VALIDATE_NOT_NULL(instance);
    AutoPtr<FileGestureLibrary> fgl = new FileGestureLibrary(path);
    *instance = (IGestureLibrary*)fgl;
    REFCOUNT_ADD(*instance);
    return NOERROR;
}

ECode GestureLibraries::FromPrivateFile(
    /* [in] */ IContext *context,
    /* [in] */ const String& name,
    /* [out] */ IGestureLibrary **instance)
{
    AutoPtr<IFile> file;
    context->GetFileStreamPath(name, (IFile**)&file);
    return FromFile(file, instance);
}

ECode GestureLibraries::FromRawResource(
    /* [in] */ IContext *context,
    /* [in] */ Int32 resourceId,
    /* [out] */ IGestureLibrary **instance)
{
    AutoPtr<ResourceGestureLibrary> rgl = new ResourceGestureLibrary(context, resourceId);
    *instance = (IGestureLibrary*)rgl;
    REFCOUNT_ADD(*instance);
    return NOERROR;
}

CAR_INTERFACE_IMPL(GestureLibraries::FileGestureLibrary, IGestureLibrary)

GestureLibraries::FileGestureLibrary::FileGestureLibrary(
    /* [in] */ IFile *path)
    : mPath(path)
{}

ECode GestureLibraries::FileGestureLibrary::IsReadOnly(
    /* [out] */ Boolean *isReadOnly)
{
    VALIDATE_NOT_NULL(isReadOnly);
    Boolean b;
    mPath->CanWrite(&b);
    *isReadOnly = !b;
    return NOERROR;
}

ECode GestureLibraries::FileGestureLibrary::Save(
    /* [out] */ Boolean *isSaved)
{
    VALIDATE_NOT_NULL(isSaved);
    Boolean b;
    mStore->HasChanged(&b);
    if (!b) {
        *isSaved = TRUE;
        return NOERROR;
    }

    const AutoPtr<IFile> file = mPath;

    const AutoPtr<IFile> parentFile;
    file->GetParentFile((IFile**)&parentFile);
    parentFile->Exists(&b);
    if (!b) {
        parentFile->Mkdirs(&b);
        if (!b) {
            *isSaved = FALSE;
            return NOERROR;
        }
    }

    Boolean result = FALSE;
    //try {
    //noinspection ResultOfMethodCallIgnored
    file->CreateNewFile(&b);
    AutoPtr<IFileOutputStream> fos;
    CFileOutputStream::New(file, (IFileOutputStream**)&fos);
    mStore->Save(fos, TRUE);
    result = TRUE;
    //} catch (FileNotFoundException e) {
    //    Log.d(LOG_TAG, "Could not save the gesture library in " + mPath, e);
    //} catch (IOException e) {
    //    Log.d(LOG_TAG, "Could not save the gesture library in " + mPath, e);
    //}
    *isSaved = result;
    return NOERROR;
}

ECode GestureLibraries::FileGestureLibrary::Load(
    /* [out] */ Boolean *isLoaded)
{
    VALIDATE_NOT_NULL(isLoaded);
    Boolean result = FALSE;
    const AutoPtr<IFile> file = mPath;
    Boolean a, b;
    file->Exists(&a);
    file->CanRead(&b);
    if (a && b) {
        //try {
        AutoPtr<IFileInputStream> fis;
        CFileInputStream::New(file, (IFileInputStream**)&fis);
        mStore->Load(fis, TRUE);
        result = TRUE;
        //} catch (FileNotFoundException e) {
        //    Log.d(LOG_TAG, "Could not load the gesture library from " + mPath, e);
        //} catch (IOException e) {
        //    Log.d(LOG_TAG, "Could not load the gesture library from " + mPath, e);
        //}
    }
    *isLoaded = result;
    return NOERROR;
}

ECode GestureLibraries::FileGestureLibrary::SetOrientationStyle(
    /* [in] */ Int32 style)
{
    return GestureLibrary::SetOrientationStyle(style);
}

ECode GestureLibraries::FileGestureLibrary::GetOrientationStyle(
    /* [out] */ Int32 *style)
{
    return GestureLibrary::GetOrientationStyle(style);
}

ECode GestureLibraries::FileGestureLibrary::SetSequenceType(
    /* [in] */ Int32 type)
{
    return GestureLibrary::SetSequenceType(type);
}

ECode GestureLibraries::FileGestureLibrary::GetSequenceType(
    /* [out] */ Int32 *type)
{
    return GestureLibrary::GetSequenceType(type);
}

ECode GestureLibraries::FileGestureLibrary::GetGestureEntries(
    /* [out] */ IObjectContainer **entries)
{
    return GestureLibrary::GetGestureEntries(entries);
}

ECode GestureLibraries::FileGestureLibrary::Recognize(
    /* [in] */ IGesture *gesture,
    /* [out] */ IArrayList **predictions)
{
    return GestureLibrary::Recognize(gesture, predictions);
}

ECode GestureLibraries::FileGestureLibrary::AddGesture(
    /* [in] */  const String& entryName,
    /* [in] */  IGesture *gesture)
{
    return GestureLibrary::AddGesture(entryName, gesture);
}

ECode GestureLibraries::FileGestureLibrary::RemoveGesture(
    /* [in] */ const String& entryName,
    /* [in] */ IGesture *gesture)
{
    return GestureLibrary::RemoveGesture(entryName, gesture);
}

ECode GestureLibraries::FileGestureLibrary::RemoveEntry(
    /* [in] */ const String& entryName)
{
    return GestureLibrary::RemoveEntry(entryName);
}

ECode GestureLibraries::FileGestureLibrary::GetGestures(
    /* [in] */ const String& entryName,
    /* [out] */ IObjectContainer **gestures)
{
    return GestureLibrary::GetGestures(entryName, gestures);
}

CAR_INTERFACE_IMPL(GestureLibraries::ResourceGestureLibrary, IGestureLibrary)

GestureLibraries::ResourceGestureLibrary::ResourceGestureLibrary(
    /* [in] */ IContext *context,
    /* [in] */ Int32 resourceId)
    : mResourceId(resourceId)
{
//    mContext = new WeakReference<Context>(context);
}

GestureLibraries::ResourceGestureLibrary::~ResourceGestureLibrary()
{}

ECode CResourceGestureLibrary::constructor(
    /* [in] */ IContext *context,
    /* [in] */ Int32 resourceId)
{
    GestureLibraries::ResourceGestureLibrary::
            ResourceGestureLibrary(context, resourceId);
    return NOERROR;
}

ECode GestureLibraries::ResourceGestureLibrary::IsReadOnly(
    /* [out] */ Boolean *isReadOnly)
{
    VALIDATE_NOT_NULL(isReadOnly);
    *isReadOnly = TRUE;
    return NOERROR;
}

ECode GestureLibraries::ResourceGestureLibrary::Save(
    /* [out] */ Boolean *isSaved)
{
    VALIDATE_NOT_NULL(isSaved);
    *isSaved = FALSE;
    return NOERROR;
}

ECode GestureLibraries::ResourceGestureLibrary::Load(
    /* [out] */ Boolean *isLoaded)
{
    VALIDATE_NOT_NULL(isLoaded);
    Boolean result = FALSE;
//    final Context context = mContext.get();
//    if (context != null) {
//        final InputStream in = context.getResources().openRawResource(mResourceId);
        //try {
//        mStore->Load(in, TRUE);
//        result = TRUE;
        //} catch (IOException e) {
        //    Log.d(LOG_TAG, "Could not load the gesture library from raw resource " +
        //            context.getResources().getResourceName(mResourceId), e);
        //}
//    }
    *isLoaded = result;
    return NOERROR;
}

ECode GestureLibraries::ResourceGestureLibrary::SetOrientationStyle(
    /* [in] */ Int32 style)
{
    return GestureLibrary::SetOrientationStyle(style);
}

ECode GestureLibraries::ResourceGestureLibrary::GetOrientationStyle(
    /* [out] */ Int32 *style)
{
    return GestureLibrary::GetOrientationStyle(style);
}

ECode GestureLibraries::ResourceGestureLibrary::SetSequenceType(
    /* [in] */ Int32 type)
{
    return GestureLibrary::SetSequenceType(type);
}

ECode GestureLibraries::ResourceGestureLibrary::GetSequenceType(
    /* [out] */ Int32 *type)
{
    return GestureLibrary::GetSequenceType(type);
}

ECode GestureLibraries::ResourceGestureLibrary::GetGestureEntries(
    /* [out] */ IObjectContainer **entries)
{
    return GestureLibrary::GetGestureEntries(entries);
}

ECode GestureLibraries::ResourceGestureLibrary::Recognize(
    /* [in] */ IGesture *gesture,
    /* [out] */ IArrayList **predictions)
{
    return GestureLibrary::Recognize(gesture, predictions);
}

ECode GestureLibraries::ResourceGestureLibrary::AddGesture(
    /* [in] */  const String& entryName,
    /* [in] */  IGesture *gesture)
{
    return GestureLibrary::AddGesture(entryName, gesture);
}

ECode GestureLibraries::ResourceGestureLibrary::RemoveGesture(
    /* [in] */ const String& entryName,
    /* [in] */ IGesture *gesture)
{
    return GestureLibrary::RemoveGesture(entryName, gesture);
}

ECode GestureLibraries::ResourceGestureLibrary::RemoveEntry(
    /* [in] */ const String& entryName)
{
    return GestureLibrary::RemoveEntry(entryName);
}

ECode GestureLibraries::ResourceGestureLibrary::GetGestures(
    /* [in] */ const String& entryName,
    /* [out] */ IObjectContainer **gestures)
{
    return GestureLibrary::GetGestures(entryName, gestures);
}

} // namespace Gesture
} // namespace Droid
} // namespace Elastos
