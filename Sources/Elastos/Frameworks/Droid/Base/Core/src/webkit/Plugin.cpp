
#include "webkit/Plugin.h"

using Elastos::Droid::Content::EIID_IDialogInterfaceOnClickListener;

namespace Elastos {
namespace Droid {
namespace Webkit {

//===============================================================
//                    Plugin::DefaultClickHandler
//===============================================================
UInt32 Plugin::DefaultClickHandler::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 Plugin::DefaultClickHandler::Release()
{
    return ElRefBase::Release();
}

ECode Plugin::DefaultClickHandler::GetInterfaceID(
    /* [in] */ IInterface *Object,
    /* [out] */ InterfaceID *IID)
{
    VALIDATE_NOT_NULL(IID);

    if (Object == (IInterface*)(IPreferencesClickHandler*)this) {
        *IID = EIID_IPreferencesClickHandler;
    }
    else if (Object == (IInterface*)(IDialogInterfaceOnClickListener*)this) {
        *IID = EIID_IDialogInterfaceOnClickListener;
    }
    else {
        return E_INVALID_ARGUMENT;
    }

    return NOERROR;
}

PInterface Plugin::DefaultClickHandler::Probe(
    /* [in]  */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)(IPreferencesClickHandler*)this;
    }
    else if (riid == EIID_IPreferencesClickHandler) {
        return (IPreferencesClickHandler*)this;
    }
    else if (riid == EIID_IDialogInterfaceOnClickListener) {
        return (IDialogInterfaceOnClickListener*)this;
    }

    return NULL;
}

ECode Plugin::DefaultClickHandler::HandleClickEvent(
    /* [in] */ IContext* context)
{
    assert(0);
    // Show a simple popup dialog containing the description
    // string of the plugin.
//    if (mDialog == NULL) {
//        mDialog = new AlertDialog.Builder(context)
//                .setTitle(mName)
//                .setMessage(mDescription)
//                .setPositiveButton(R.string.ok, this)
//                .setCancelable(false)
//                .show();
//    }
    return E_NOT_IMPLEMENTED;
}

/**
 * @hide
 * @deprecated This interface was intended to be used by Gears. Since Gears was
 * deprecated, so is this class.
 */
ECode Plugin::DefaultClickHandler::OnClick(
    /* [in] */ IDialogInterface* dialog,
    /* [in] */ Int32 which)
{
    mDialog->Dismiss();
    mDialog = NULL;
    return NOERROR;
}

//===============================================================
//                             Plugin
//===============================================================
Plugin::Plugin()
{
}

/**
 * @hide
 * @deprecated This interface was intended to be used by Gears. Since Gears was
 * deprecated, so is this class.
 */
Plugin::Plugin(
    /* [in] */ const String& name,
    /* [in] */ const String& path,
    /* [in] */ const String& fileName,
    /* [in] */ const String& description)
{
    Init(name, path, fileName, description);
}

void Plugin::Init(
    /* [in] */ const String& name,
    /* [in] */ const String& path,
    /* [in] */ const String& fileName,
    /* [in] */ const String& description)
{
    mName = name;
    mPath = path;
    mFileName = fileName;
    mDescription = description;
    mHandler = new DefaultClickHandler(this);
}

/**
 * @hide
 * @deprecated This interface was intended to be used by Gears. Since Gears was
 * deprecated, so is this class.
 */
String Plugin::ToString()
{
    return mName;
}

/**
 * @hide
 * @deprecated This interface was intended to be used by Gears. Since Gears was
 * deprecated, so is this class.
 */
String Plugin::GetName()
{
    return mName;
}

/**
 * @hide
 * @deprecated This interface was intended to be used by Gears. Since Gears was
 * deprecated, so is this class.
 */
String Plugin::GetPath()
{
    return mPath;
}

/**
 * @hide
 * @deprecated This interface was intended to be used by Gears. Since Gears was
 * deprecated, so is this class.
 */
String Plugin::GetFileName()
{
    return mFileName;
}

/**
 * @hide
 * @deprecated This interface was intended to be used by Gears. Since Gears was
 * deprecated, so is this class.
 */
String Plugin::GetDescription()
{
    return mDescription;
}

/**
 * @hide
 * @deprecated This interface was intended to be used by Gears. Since Gears was
 * deprecated, so is this class.
 */
ECode Plugin::SetName(
    /* [in] */ const String& name)
{
    mName = name;
    return NOERROR;
}

/**
 * @hide
 * @deprecated This interface was intended to be used by Gears. Since Gears was
 * deprecated, so is this class.
 */
ECode Plugin::SetPath(
    /* [in] */ const String& path)
{
    mPath = path;
    return NOERROR;
}

/**
 * @hide
 * @deprecated This interface was intended to be used by Gears. Since Gears was
 * deprecated, so is this class.
 */
ECode Plugin::SetFileName(
    /* [in] */ const String& fileName)
{
    mFileName = fileName;
    return NOERROR;
}

/**
 * @hide
 * @deprecated This interface was intended to be used by Gears. Since Gears was
 * deprecated, so is this class.
 */
ECode Plugin::SetDescription(
    /* [in] */ const String& description)
{
    mDescription = description;
    return NOERROR;
}

/**
 * @hide
 * @deprecated This interface was intended to be used by Gears. Since Gears was
 * deprecated, so is this class.
 */
ECode Plugin::SetClickHandler(
    /* [in] */ IPreferencesClickHandler* handler)
{
    mHandler = handler;
    return NOERROR;
}

/**
* Invokes the click handler for this plugin.
*
* @hide
* @deprecated This interface was intended to be used by Gears. Since Gears was
* deprecated, so is this class.
*/
ECode Plugin::DispatchClickEvent(
    /* [in] */ IContext* context)
{
    if (mHandler != NULL) {
        mHandler->HandleClickEvent(context);
    }
    return NOERROR;
}

} // namespace Webkit
} // namepsace Droid
} // namespace Elastos