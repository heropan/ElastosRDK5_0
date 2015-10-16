
#include "elastos/droid/provider/CBrowserContractBookmarks.h"


ECode CBrowserContractBookmarks::GetCONTENT_URI(
    /* [out] */ IUri** uri)
{
//    *uri = Uri.withAppendedPath(AUTHORITY_URI, "bookmarks");
}

ECode CBrowserContractBookmarks::GetCONTENT_URIDEFAULTFOLDER(
        /* [out] */ IUri** uri)
{
//    *uri = Uri.withAppendedPath(CONTENT_URI, "folder");
}

ECode CBrowserContractBookmarks::constructor()
{
    return NOERROR;
}

/**
 * Builds a URI that points to a specific folder.
 * @param folderId the ID of the folder to point to
 * @hide
 */
ECode CBrowserContractBookmarks::BuildFolderUri(
    /* [in] */ Int64 folderId,
    /* [out] */ IUri** uri)
{
//    return ContentUris.withAppendedId(CONTENT_URI_DEFAULT_FOLDER, folderId);
}
