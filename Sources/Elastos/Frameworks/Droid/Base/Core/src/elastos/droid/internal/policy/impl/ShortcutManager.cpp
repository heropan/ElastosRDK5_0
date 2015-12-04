
#include "elastos/droid/internal/policy/impl/ShortcutManager.h"

namespace Elastos {
namespace Droid {
namespace Internal {
namespace Policy {
namespace Impl {

//=====================================================================
//                           ShortcutManager
//=====================================================================
const String ShortcutManager::TAG("ShortcutManager");
const Int32 ShortcutManager::COLUMN_SHORTCUT;
const Int32 ShortcutManager::COLUMN_INTENT;
AutoPtr< ArrayOf<String> > ShortcutManager::sProjection = ShortcutManager::MiddleInitSprojection();

ShortcutManager::ShortcutManager(
    /* [in] */ IContext* context,
    /* [in] */ IHandler* handler)
{
    // ==================before translated======================
    // super(handler);
    //
    // mContext = context;
    // mShortcutIntents = new SparseArray<Intent>();
}

ECode ShortcutManager::Observe()
{
    // ==================before translated======================
    // mCursor = mContext.getContentResolver().query(
    //         Settings.Bookmarks.CONTENT_URI, sProjection, null, null, null);
    // mCursor.registerContentObserver(this);
    // updateShortcuts();
    assert(0);
    return NOERROR;
}

ECode ShortcutManager::OnChange(
    /* [in] */ Boolean selfChange)
{
    // ==================before translated======================
    // updateShortcuts();
    assert(0);
    return NOERROR;
}

ECode ShortcutManager::GetIntent(
    /* [in] */ IKeyCharacterMap* kcm,
    /* [in] */ Int32 keyCode,
    /* [in] */ Int32 metaState,
    /* [out] */ Intent** result)
{
    VALIDATE_NOT_NULL(kcm);
    VALIDATE_NOT_NULL(result);
    // ==================before translated======================
    // Intent intent = null;
    //
    // // First try the exact keycode (with modifiers).
    // int shortcut = kcm.get(keyCode, metaState);
    // if (shortcut != 0) {
    //     intent = mShortcutIntents.get(shortcut);
    // }
    //
    // // Next try the primary character on that key.
    // if (intent == null) {
    //     shortcut = Character.toLowerCase(kcm.getDisplayLabel(keyCode));
    //     if (shortcut != 0) {
    //         intent = mShortcutIntents.get(shortcut);
    //     }
    // }
    //
    // return intent;
    assert(0);
    return NOERROR;
}

AutoPtr< ArrayOf<String> > ShortcutManager::MiddleInitSprojection()
{
    // ==================before translated======================
    // String[] result = new String[] {
    //          Settings.Bookmarks.SHORTCUT, Settings.Bookmarks.INTENT
    //      };
    assert(0);
    AutoPtr< ArrayOf<String> > empty;
    return empty;
}

void ShortcutManager::UpdateShortcuts()
{
    // ==================before translated======================
    // Cursor c = mCursor;
    // if (!c.requery()) {
    //     Log.e(TAG, "ShortcutObserver could not re-query shortcuts.");
    //     return;
    // }
    //
    // mShortcutIntents.clear();
    // while (c.moveToNext()) {
    //     int shortcut = c.getInt(COLUMN_SHORTCUT);
    //     if (shortcut == 0) continue;
    //     String intentURI = c.getString(COLUMN_INTENT);
    //     Intent intent = null;
    //     try {
    //         intent = Intent.getIntent(intentURI);
    //     } catch (URISyntaxException e) {
    //         Log.w(TAG, "Intent URI for shortcut invalid.", e);
    //     }
    //     if (intent == null) continue;
    //     mShortcutIntents.put(shortcut, intent);
    // }
    assert(0);
}

} // namespace Impl
} // namespace Policy
} // namespace Internal
} // namespace Droid
} // namespace Elastos


