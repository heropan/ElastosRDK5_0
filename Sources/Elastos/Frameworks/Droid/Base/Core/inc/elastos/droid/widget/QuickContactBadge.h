
#ifndef __ELASTOS_DROID_WIDGET_QUICKCONTACTBADGE_H__
#define __ELASTOS_DROID_WIDGET_QUICKCONTACTBADGE_H__

#include "elastos/droid/ext/frameworkext.h"
#include "widget/ImageView.h"

namespace Elastos {
namespace Droid {
namespace Widget {

class QuickContactBadge : public ImageView
{
private:
    class QueryHandler : public AsyncQueryHandler
    {
    public:
        QueryHandler(
            /* [in] */ IContentResolver* cr);

    protected:
        CARAPI_(void) OnQueryComplete(
            /* [in] */ Int32 token,
            /* [in] */ IInterface* cookie,
            /* [in] */ ICursor* cursor);
    };

public:
    QuickContactBadge(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs = NULL,
        /* [in] */ Int32 defStyle = 0);


    virtual CARAPI SetMode(
        /* [in] */ Int32 size);

    virtual CARAPI SetImageToDefault();

    virtual CARAPI AssignContactUri(
        /* [in] */ IUri* contactUri);

    virtual CARAPI AssignContactFromEmail(
        /* [in] */ const String& emailAddress,
        /* [in] */ Boolean lazyLookup);

    virtual CARAPI AssignContactFromPhone(
        /* [in] */ const String& phoneNumber,
        /* [in] */ Boolean lazyLookup);

    virtual CARAPI OnClick(
        /* [in] */ IView* v);

    virtual CARAPI OnInitializeAccessibilityEvent(
        /* [in] */ IAccessibilityEvent* event);

    virtual CARAPI OnInitializeAccessibilityNodeInfo(
        /* [in] */ IAccessibilityNodeInfo* info);

    virtual CARAPI SetExcludeMimes(
        /* [in] */ const ArrayOf<String> excludeMimes);

protected:
    QuickContactBadge();

    CARAPI Init(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs = NULL,
        /* [in] */ Int32 defStyle = 0);

    CARAPI_(void) DrawableStateChanged();

    CARAPI_(void) OnDraw(
        /* [in] */ ICanvas* canvas);

private:
    CARAPI_(Boolean) IsAssigned();

    CARAPI_(void) OnContactUriChanged();

private:
    AutoPtr<IUri> mContactUri;
    String mContactEmail;
    String mContactPhone;
    AutoPtr<IDrawable> mOverlay;
    AutoPtr<QueryHandler> mQueryHandler;
    AutoPtr<IDrawable> mDefaultAvatar;

protected:
    AutoPtr< ArrayOf<String> > mExcludeMimes;

public:
    static const Int32 TOKEN_EMAIL_LOOKUP = 0;
    static const Int32 TOKEN_PHONE_LOOKUP = 1;
    static const Int32 TOKEN_EMAIL_LOOKUP_AND_TRIGGER = 2;
    static const Int32 TOKEN_PHONE_LOOKUP_AND_TRIGGER = 3;

    static const String EMAIL_LOOKUP_PROJECTION[]; /*= new String[] {
        RawContacts.CONTACT_ID,
            Contacts.LOOKUP_KEY,
    };*/
    static const Int32 EMAIL_ID_COLUMN_INDEX = 0;
    static const Int32 EMAIL_LOOKUP_STRING_COLUMN_INDEX = 1;

    static const String PHONE_LOOKUP_PROJECTION[]; /*= new String[] {
        PhoneLookup._ID,
            PhoneLookup.LOOKUP_KEY,
    };*/
    static const Int32 PHONE_ID_COLUMN_INDEX = 0;
    static const Int32 PHONE_LOOKUP_STRING_COLUMN_INDEX = 1;
};

} // namespace Widget
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_WIDGET_QUICKCONTACTBADGE_H__
