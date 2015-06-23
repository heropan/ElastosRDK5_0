
#include "ext/frameworkext.h"
#include "app/CAlertDialogBuilder.h"
#include "webkit/CPlugin.h"
#include "R.h"

using Elastos::Core::CStringWrapper;
using Elastos::Core::ICharSequence;
using Elastos::Droid::App::CAlertDialogBuilder;
using Elastos::Droid::App::IAlertDialogBuilder;
using Elastos::Droid::Content::EIID_IDialogInterfaceOnClickListener;

namespace Elastos {
namespace Droid {
namespace Webkit {

//===============================================================
//                    CPlugin::DefaultClickHandler
//===============================================================
UInt32 CPlugin::DefaultClickHandler::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 CPlugin::DefaultClickHandler::Release()
{
    return ElRefBase::Release();
}

ECode CPlugin::DefaultClickHandler::GetInterfaceID(
    /* [in] */ IInterface* Object,
    /* [out] */ InterfaceID* IID)
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

PInterface CPlugin::DefaultClickHandler::Probe(
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

ECode CPlugin::DefaultClickHandler::HandleClickEvent(
    /* [in] */ IContext* context)
{
    // Show a simple popup dialog containing the description
    // string of the plugin.
    AutoPtr<IAlertDialogBuilder> builder;
    CAlertDialogBuilder::New(context, (IAlertDialogBuilder**)&builder);
    AutoPtr<ICharSequence> name;
    CStringWrapper::New(mOwner->mName, (ICharSequence**)&name);
    builder->SetTitle(name);
    AutoPtr<ICharSequence> description;
    CStringWrapper::New(mOwner->mDescription, (ICharSequence**)&description);
    builder->SetMessage(description);
    builder->SetPositiveButton(R::string::ok, this);
    builder->SetCancelable(FALSE);
    return builder->Show((IAlertDialog**)&mDialog);
}

/**
 * @hide
 * @deprecated This interface was intended to be used by Gears. Since Gears was
 * deprecated, so is this class.
 */
ECode CPlugin::DefaultClickHandler::OnClick(
    /* [in] */ IDialogInterface* dialog,
    /* [in] */ Int32 which)
{
    mDialog->Dismiss();
    mDialog = NULL;
    return NOERROR;
}


//===============================================================
//                            CPlugin
//===============================================================
ECode CPlugin::constructor(
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
    return NOERROR;
}

ECode CPlugin::ToString(
    /* [out] */ String* name)
{
    VALIDATE_NOT_NULL(name);
    *name = mName;
    return NOERROR;
}

ECode CPlugin::GetName(
    /* [out] */ String* name)
{
    VALIDATE_NOT_NULL(name);
    *name = mName;
    return NOERROR;
}

ECode CPlugin::GetPath(
    /* [out] */ String* path)
{
    VALIDATE_NOT_NULL(path);
    *path = mPath;
    return NOERROR;
}

ECode CPlugin::GetFileName(
    /* [out] */ String* fileName)
{
    VALIDATE_NOT_NULL(fileName);
    *fileName = mFileName;
    return NOERROR;
}

ECode CPlugin::GetDescription(
    /* [out] */ String* desc)
{
    VALIDATE_NOT_NULL(desc);
    *desc = mDescription;
    return NOERROR;
}

ECode CPlugin::SetName(
    /* [in] */ const String& name)
{
    mName = name;
    return NOERROR;
}

ECode CPlugin::SetPath(
    /* [in] */ const String& path)
{
    mPath = path;
    return NOERROR;
}

ECode CPlugin::SetFileName(
    /* [in] */ const String& fileName)
{
    mFileName = fileName;
    return NOERROR;
}

ECode CPlugin::SetDescription(
    /* [in] */ const String& description)
{
    mDescription = description;
    return NOERROR;
}

ECode CPlugin::SetClickHandler(
    /* [in] */ IPreferencesClickHandler* handler)
{
    mHandler = handler;
    return NOERROR;
}

ECode CPlugin::DispatchClickEvent(
    /* [in] */ IContext* context)
{
    if (mHandler != NULL) {
        mHandler->HandleClickEvent(context);
    }
    return NOERROR;
}

} // namespace Webkit
} // namespace Droid
} // namespace Elastos
