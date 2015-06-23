
#include "gesture/CGestureLibraries.h"

using Elastos::IO::IFileInputStream;
using Elastos::IO::CFileInputStream;
using Elastos::IO::IFileOutputStream;
using Elastos::IO::CFileOutputStream;

namespace Elastos {
namespace Droid {
namespace Gesture {

CGestureLibraries::CGestureLibraries()
{}

ECode CGestureLibraries::FromFile(
    /* [in] */ const String& path,
    /* [out] */ IGestureLibrary **instance)
{
    AutoPtr<IFile> file;
    CFile::New(path, (IFile**)&file);
    return FromFile(file, instance);
}

ECode CGestureLibraries::FromFile(
    /* [in] */ IFile *path,
    /* [out] */ IGestureLibrary **instance)
{
    VALIDATE_NOT_NULL(instance);
    AutoPtr<FileGestureLibrary> fgl = new FileGestureLibrary(path);
    *instance = (IGestureLibrary*)fgl;
    REFCOUNT_ADD(*instance);
    return NOERROR;
}

ECode CGestureLibraries::FromPrivateFile(
    /* [in] */ IContext *context,
    /* [in] */ const String& name,
    /* [out] */ IGestureLibrary **instance)
{
    AutoPtr<IFile> file;
    context->GetFileStreamPath(name, (IFile**)&file);
    return FromFile(file, instance);
}

ECode CGestureLibraries::FromRawResource(
    /* [in] */ IContext *context,
    /* [in] */ Int32 resourceId,
    /* [out] */ IGestureLibrary **instance)
{
    AutoPtr<ResourceGestureLibrary> rgl = new ResourceGestureLibrary(context, resourceId);
    *instance = (IGestureLibrary*)rgl;
    REFCOUNT_ADD(*instance);
    return NOERROR;
}

CAR_INTERFACE_IMPL(CGestureLibraries::FileGestureLibrary, IGestureLibrary)

CGestureLibraries::FileGestureLibrary::FileGestureLibrary(
    /* [in] */ IFile *path)
    : mPath(path)
{}

ECode CGestureLibraries::FileGestureLibrary::IsReadOnly(
    /* [out] */ Boolean *isReadOnly)
{
    VALIDATE_NOT_NULL(isReadOnly);
    Boolean b;
    mPath->CanWrite(&b);
    *isReadOnly = !b;
    return NOERROR;
}

ECode CGestureLibraries::FileGestureLibrary::Save(
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

ECode CGestureLibraries::FileGestureLibrary::Load(
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

ECode CGestureLibraries::FileGestureLibrary::SetOrientationStyle(
    /* [in] */ Int32 style)
{
    return GestureLibrary::SetOrientationStyle(style);
}

ECode CGestureLibraries::FileGestureLibrary::GetOrientationStyle(
    /* [out] */ Int32 *style)
{
    return GestureLibrary::GetOrientationStyle(style);
}

ECode CGestureLibraries::FileGestureLibrary::SetSequenceType(
    /* [in] */ Int32 type)
{
    return GestureLibrary::SetSequenceType(type);
}

ECode CGestureLibraries::FileGestureLibrary::GetSequenceType(
    /* [out] */ Int32 *type)
{
    return GestureLibrary::GetSequenceType(type);
}

ECode CGestureLibraries::FileGestureLibrary::GetGestureEntries(
    /* [out] */ IObjectContainer **entries)
{
    return GestureLibrary::GetGestureEntries(entries);
}

ECode CGestureLibraries::FileGestureLibrary::Recognize(
    /* [in] */ IGesture *gesture,
    /* [out] */ IArrayList **predictions)
{
    return GestureLibrary::Recognize(gesture, predictions);
}

ECode CGestureLibraries::FileGestureLibrary::AddGesture(
    /* [in] */  const String& entryName,
    /* [in] */  IGesture *gesture)
{
    return GestureLibrary::AddGesture(entryName, gesture);
}

ECode CGestureLibraries::FileGestureLibrary::RemoveGesture(
    /* [in] */ const String& entryName,
    /* [in] */ IGesture *gesture)
{
    return GestureLibrary::RemoveGesture(entryName, gesture);
}

ECode CGestureLibraries::FileGestureLibrary::RemoveEntry(
    /* [in] */ const String& entryName)
{
    return GestureLibrary::RemoveEntry(entryName);
}

ECode CGestureLibraries::FileGestureLibrary::GetGestures(
    /* [in] */ const String& entryName,
    /* [out] */ IObjectContainer **gestures)
{
    return GestureLibrary::GetGestures(entryName, gestures);
}

CAR_INTERFACE_IMPL(CGestureLibraries::ResourceGestureLibrary, IGestureLibrary)

CGestureLibraries::ResourceGestureLibrary::ResourceGestureLibrary(
    /* [in] */ IContext *context,
    /* [in] */ Int32 resourceId)
    : mResourceId(resourceId)
{
//    mContext = new WeakReference<Context>(context);
}

ECode CGestureLibraries::ResourceGestureLibrary::IsReadOnly(
    /* [out] */ Boolean *isReadOnly)
{
    VALIDATE_NOT_NULL(isReadOnly);
    *isReadOnly = TRUE;
    return NOERROR;
}

ECode CGestureLibraries::ResourceGestureLibrary::Save(
    /* [out] */ Boolean *isSaved)
{
    VALIDATE_NOT_NULL(isSaved);
    *isSaved = FALSE;
    return NOERROR;
}

ECode CGestureLibraries::ResourceGestureLibrary::Load(
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

ECode CGestureLibraries::ResourceGestureLibrary::SetOrientationStyle(
    /* [in] */ Int32 style)
{
    return GestureLibrary::SetOrientationStyle(style);
}

ECode CGestureLibraries::ResourceGestureLibrary::GetOrientationStyle(
    /* [out] */ Int32 *style)
{
    return GestureLibrary::GetOrientationStyle(style);
}

ECode CGestureLibraries::ResourceGestureLibrary::SetSequenceType(
    /* [in] */ Int32 type)
{
    return GestureLibrary::SetSequenceType(type);
}

ECode CGestureLibraries::ResourceGestureLibrary::GetSequenceType(
    /* [out] */ Int32 *type)
{
    return GestureLibrary::GetSequenceType(type);
}

ECode CGestureLibraries::ResourceGestureLibrary::GetGestureEntries(
    /* [out] */ IObjectContainer **entries)
{
    return GestureLibrary::GetGestureEntries(entries);
}

ECode CGestureLibraries::ResourceGestureLibrary::Recognize(
    /* [in] */ IGesture *gesture,
    /* [out] */ IArrayList **predictions)
{
    return GestureLibrary::Recognize(gesture, predictions);
}

ECode CGestureLibraries::ResourceGestureLibrary::AddGesture(
    /* [in] */  const String& entryName,
    /* [in] */  IGesture *gesture)
{
    return GestureLibrary::AddGesture(entryName, gesture);
}

ECode CGestureLibraries::ResourceGestureLibrary::RemoveGesture(
    /* [in] */ const String& entryName,
    /* [in] */ IGesture *gesture)
{
    return GestureLibrary::RemoveGesture(entryName, gesture);
}

ECode CGestureLibraries::ResourceGestureLibrary::RemoveEntry(
    /* [in] */ const String& entryName)
{
    return GestureLibrary::RemoveEntry(entryName);
}

ECode CGestureLibraries::ResourceGestureLibrary::GetGestures(
    /* [in] */ const String& entryName,
    /* [out] */ IObjectContainer **gestures)
{
    return GestureLibrary::GetGestures(entryName, gestures);
}

}//namespace Gesture
}//namespace Droid
}//namespace Elastos
