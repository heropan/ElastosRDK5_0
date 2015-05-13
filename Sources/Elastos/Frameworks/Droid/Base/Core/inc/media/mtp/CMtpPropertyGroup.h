
#ifndef __CMTPPROPERTYGROUP_H__
#define __CMTPPROPERTYGROUP_H__

#include "_CMtpPropertyGroup.h"
#include "ext/frameworkext.h"
#include <elastos/List.h>

using Elastos::Utility::List;
using Elastos::Droid::Content::IIContentProvider;
using Elastos::Droid::Net::IUri;
using Elastos::Droid::Mtp::IMtpDatabase;
using Elastos::Droid::Mtp::IMtpPropertyList;

namespace Elastos {
namespace Droid {
namespace Mtp {

CarClass(CMtpPropertyGroup)
{
public:
    CMtpPropertyGroup();

    CARAPI constructor(
        /* [in] */ IMtpDatabase* database,
        /* [in] */ IIContentProvider* provider,
        /* [in] */ const String& volume,
        /* [in] */ ArrayOf<Int32>* properties);

    CARAPI GetPropertyList(
        /* [in] */ Int32 handle,
        /* [in] */ Int32 format,
        /* [in] */ Int32 depth,
        /* [out] */ IMtpPropertyList** list);

private:
    class Property
        : public ElRefBase
    {
    public:
        // MTP property code
        Int32 mCode;
        // MTP data type
        Int32 mType;
        // column index for our query
        Int32 mColumn;

        Property(
            /* [in] */ Int32 code,
            /* [in] */ Int32 type,
            /* [in] */ Int32 column);
    };

private:
    CARAPI_(AutoPtr<Property>) CreateProperty(
        /* [in] */ Int32 code,
        /* [in] */ List<String>* columns);

    CARAPI_(String) QueryString(
        /* [in] */ Int32 id,
        /* [in] */ const String& column);

    CARAPI_(String) QueryAudio(
        /* [in] */ Int32 id,
        /* [in] */ const String& column);

    CARAPI_(String) QueryGenre(
        /* [in] */ Int32 id);

    CARAPI_(Int64) QueryInt64(
        /* [in] */ Int32 id,
        /* [in] */ const String& column);

    static CARAPI_(String) NameFromPath(
        /* [in] */ const String& path);

    CARAPI_(String) NativeFormatDateTime(
        /* [in] */ Int64 seconds);

private:
    AutoPtr<IMtpDatabase> mDatabase;
    AutoPtr<IIContentProvider> mProvider;
    String mVolumeName;
    AutoPtr<IUri> mUri;

    // list of all properties in this group
    AutoPtr<ArrayOf<Property*> > mProperties;

    // list of columns for database query
    AutoPtr<ArrayOf<String> > mColumns;

    static const String TAG;
    static const String ID_WHERE;
    static const String FORMAT_WHERE;
    static const String ID_FORMAT_WHERE;
    static const String PARENT_WHERE;
    static const String PARENT_FORMAT_WHERE;
};

} // namespace Mtp
} // namepsace Droid
} // namespace Elastos

#endif // __CMTPPROPERTYGROUP_H__
