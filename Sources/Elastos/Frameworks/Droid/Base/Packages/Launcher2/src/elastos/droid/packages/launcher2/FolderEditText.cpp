
#include "elastos/droid/packages/launcher2/FolderEditText.h"
#include "Elastos.Droid.Service.h"
#include "R.h"

namespace Elastos {
namespace Droid {
namespace Packages {
namespace Launcher2 {

CAR_INTERFACE_IMPL(FolderEditText, EditText, IFolderEditText);

ECode FolderEditText::constructor(
    /* [in] */ IContext* context)
{
    return EditText::constructor(context);
}

ECode FolderEditText::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    return EditText::constructor(context, attrs);
}

ECode FolderEditText::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
{
    return EditText::constructor(context, attrs, defStyle);
}

ECode FolderEditText::SetFolder(
    /* [in] */ IFolder* folder)
{
    mFolder = folder;
    return NOERROR;
}

ECode FolderEditText::OnKeyPreIme(
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event,
    /* [out] */ Boolean* result)
{
    // Catch the back button on the soft keyboard so that we can just close the activity
    Int32 code;
    event->GetKeyCode(&code);
    if (code == IKeyEvent::KEYCODE_BACK) {
        mFolder->DoneEditingFolderName(TRUE);
    }
    return EditText::OnKeyPreIme(keyCode, event, result);
}

} // namespace Launcher2
} // namespace Packages
} // namespace Droid
} // namespace Elastos