// wuweizuo automatic build .cpp file from .java file.

#include "SelectActionModeCallback.h"

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Content {
namespace Browser {

//=====================================================================
//                       SelectActionModeCallback
//=====================================================================
Boolean SelectActionModeCallback::OnCreateActionMode(
    /* [in] */ IActionMode* mode,
    /* [in] */ IMenu* menu)
{
    // ==================before translated======================
    // mode.setTitle(null);
    // mode.setSubtitle(null);
    // mEditable = mActionHandler.isSelectionEditable();
    // mIsPasswordType = mActionHandler.isSelectionPassword();
    // createActionMenu(mode, menu);
    // return true;
    assert(0);
    return FALSE;
}

Boolean SelectActionModeCallback::OnPrepareActionMode(
    /* [in] */ IActionMode* mode,
    /* [in] */ IMenu* menu)
{
    // ==================before translated======================
    // boolean isEditableNow = mActionHandler.isSelectionEditable();
    // boolean isPasswordNow = mActionHandler.isSelectionPassword();
    // if (mEditable != isEditableNow || mIsPasswordType != isPasswordNow) {
    //     mEditable = isEditableNow;
    //     mIsPasswordType = isPasswordNow;
    //     menu.clear();
    //     createActionMenu(mode, menu);
    //     return true;
    // }
    // return false;
    assert(0);
    return FALSE;
}

Boolean SelectActionModeCallback::OnActionItemClicked(
    /* [in] */ IActionMode* mode,
    /* [in] */ IMenuItem* item)
{
    // ==================before translated======================
    // int id = item.getItemId();
    //
    // if (id == R.id.select_action_menu_select_all) {
    //     mActionHandler.selectAll();
    // } else if (id == R.id.select_action_menu_cut) {
    //     mActionHandler.cut();
    // } else if (id == R.id.select_action_menu_copy) {
    //     mActionHandler.copy();
    //     mode.finish();
    // } else if (id == R.id.select_action_menu_paste) {
    //     mActionHandler.paste();
    // } else if (id == R.id.select_action_menu_share) {
    //     mActionHandler.share();
    //     mode.finish();
    // } else if (id == R.id.select_action_menu_web_search) {
    //     mActionHandler.search();
    //     mode.finish();
    // } else {
    //     return false;
    // }
    // return true;
    assert(0);
    return FALSE;
}

ECode SelectActionModeCallback::OnDestroyActionMode(
    /* [in] */ IActionMode* mode)
{
    VALIDATE_NOT_NULL(mode);
    // ==================before translated======================
    // mActionHandler.onDestroyActionMode();
    assert(0);
    return NOERROR;
}

SelectActionModeCallback::SelectActionModeCallback(
    /* [in] */ IContext* context,
    /* [in] */ ActionHandler* actionHandler,
    /* [in] */ Boolean incognito)
{
    // ==================before translated======================
    // mContext = context;
    // mActionHandler = actionHandler;
    // mIncognito = incognito;
}

AutoPtr<IContext> SelectActionModeCallback::GetContext()
{
    // ==================before translated======================
    // return mContext;
    assert(0);
    AutoPtr<IContext> empty;
    return empty;
}

ECode SelectActionModeCallback::CreateActionMenu(
    /* [in] */ IActionMode* mode,
    /* [in] */ IMenu* menu)
{
    VALIDATE_NOT_NULL(mode);
    VALIDATE_NOT_NULL(menu);
    // ==================before translated======================
    // mode.getMenuInflater().inflate(R.menu.select_action_menu, menu);
    // if (!mEditable || !canPaste()) {
    //     menu.removeItem(R.id.select_action_menu_paste);
    // }
    //
    // if (!mEditable) {
    //     menu.removeItem(R.id.select_action_menu_cut);
    // }
    //
    // if (mEditable || !mActionHandler.isShareAvailable()) {
    //     menu.removeItem(R.id.select_action_menu_share);
    // }
    //
    // if (mEditable || mIncognito || !mActionHandler.isWebSearchAvailable()) {
    //     menu.removeItem(R.id.select_action_menu_web_search);
    // }
    // if (mIsPasswordType) {
    //     menu.removeItem(R.id.select_action_menu_copy);
    //     menu.removeItem(R.id.select_action_menu_cut);
    // }
    assert(0);
    return NOERROR;
}

Boolean SelectActionModeCallback::CanPaste()
{
    // ==================before translated======================
    // ClipboardManager clipMgr = (ClipboardManager)
    //         getContext().getSystemService(Context.CLIPBOARD_SERVICE);
    // return clipMgr.hasPrimaryClip();
    assert(0);
    return FALSE;
}

} // namespace Browser
} // namespace Content
} // namespace Webkit
} // namespace Droid
} // namespace Elastos


