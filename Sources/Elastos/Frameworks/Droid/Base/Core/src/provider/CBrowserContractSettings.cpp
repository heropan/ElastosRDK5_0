
#include "provider/CBrowserContractSettings.h"


ECode CBrowserContractSettings::constructor()
{
    return NOERROR;
}

ECode CBrowserContractSettings::GetCONTENT_URI(
    /* [out] */ IUri** uri)
{
//    *uri =Uri.withAppendedPath(AUTHORITY_URI, "settings");
}

ECode CBrowserContractSettings::IsSyncEnabled(
    /* [in] */ IContext* context,
    /* [out] */ Boolean* res)
{
//     Cursor cursor = null;
//     try {
//         cursor = context.getContentResolver().query(CONTENT_URI, new String[] { VALUE },
//                 KEY + "=?", new String[] { KEY_SYNC_ENABLED }, null);
//         if (cursor == null || !cursor.moveToFirst()) {
//             return false;
//         }
//         return cursor.getInt(0) != 0;
//     } finally {
//         if (cursor != null) cursor.close();
//     }
}

ECode CBrowserContractSettings::SetSyncEnabled(
    /* [in] */ IContext* context,
    /* [in] */ Boolean enabled)
{
//     ContentValues values = new ContentValues();
//     values.put(KEY, KEY_SYNC_ENABLED);
//     values.put(VALUE, enabled ? 1 : 0);
//     context.getContentResolver().insert(CONTENT_URI, values);
}