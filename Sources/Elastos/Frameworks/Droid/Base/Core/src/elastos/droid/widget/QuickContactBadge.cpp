#include "elastos/droid/widget/QuickContactBadge.h"

namespace Elastos {
namespace Droid {
namespace Widget {

const Int32 QuickContactBadge::TOKEN_EMAIL_LOOKUP;
const Int32 QuickContactBadge::TOKEN_PHONE_LOOKUP = 1;
const Int32 QuickContactBadge::TOKEN_EMAIL_LOOKUP_AND_TRIGGER = 2;
const Int32 QuickContactBadge::TOKEN_PHONE_LOOKUP_AND_TRIGGER = 3;

const String QuickContactBadge::EMAIL_LOOKUP_PROJECTION[] = {

};

const Int32 QuickContactBadge::EMAIL_ID_COLUMN_INDEX = 0;
const Int32 QuickContactBadge::EMAIL_LOOKUP_STRING_COLUMN_INDEX = 1;

static const String PHONE_LOOKUP_PROJECTION[] = {

};

const Int32 QuickContactBadge::PHONE_ID_COLUMN_INDEX = 0;
const Int32 QuickContactBadge::PHONE_LOOKUP_STRING_COLUMN_INDEX = 1;

QuickContactBadge::QuickContactBadge(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
{
    ImageView::Init(context, attrs, defStyle);

    TypedArray styledAttributes = mContext.obtainStyledAttributes(R.styleable.Theme);
    mOverlay = styledAttributes.getDrawable(
            com.android.internal.R.styleable.Theme_quickContactBadgeOverlay);
    styledAttributes.recycle();

    mQueryHandler = new QueryHandler(mContext.getContentResolver());
    setOnClickListener(this);
}

QuickContactBadge()
{

}

CARAPI Init(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs = NULL,
    /* [in] */ Int32 defStyle = 0)
{

}

void QuickContactBadge::DrawableStateChanged()
{
    ImageView::DrawableStateChanged();
    Boolean isStateful = FALSE;
    if (mOverlay != NULL && (mOverlay->IsStateful(&isStateful) , isStateful)) {
        mOverlay->SetState(GetDrawableState(), isStateful);
        Invalidate();
    }
}

ECode QuickContactBadge::SetMode(
    /* [in] */ Int32 size)
{
}

void QuickContactBadge::OnDraw(
    /* [in] */ ICanvas* canvas)
{
    ImageView::OnDraw(canvas);

    if (!IsEnabled()) {
        return;
    }

    if (mOverlay == NULL) {
        return;
    }
    Int32 IntrinsicWidth = 0, IntrinsicHeight = 0;
    mOverlay->GetIntrinsicWidth(&IntrinsicWidth);
    mOverlay->GetIntrinsicHeight(&IntrinsicHeight);
    if (IntrinsicWidth== 0 || IntrinsicHeight == 0) {
        return;
    }

    mOverlay->SetBounds(0, 0, GetWidth(), GetHeight());

    if (mPaddingTop == 0 && mPaddingLeft == 0) {
        mOverlay->Draw(canvas);
    } else {
        Int32 saveCount = 0;
        canvas->GetSaveCount(&saveCount);
        canvas->Save();
        canvas->Translate(mPaddingLeft, mPaddingTop);
        mOverlay->Draw(canvas);
        canvas->RestoreToCount(saveCount);
    }
}

Boolean QuickContactBadge::IsAssigned()
{
    return mContactUri != NULL || mContactEmail != NULL || mContactPhone != NULL;
}

ECode QuickContactBadge::SetImageToDefault()
{
    if (mDefaultAvatar == NULL) {
        GetResources()->GetDrawable(R::drawable::ic_contact_picture, (IDrawable**)&mDefaultAvatar);
    }
    SetImageDrawable(mDefaultAvatar);
}

ECode QuickContactBadge::AssignContactUri(
    /* [in] */ IUri* contactUri)
{
    mContactUri = contactUri;
    mContactEmail = NULL;
    mContactPhone = NULL;
    OnContactUriChanged();
}

/**
 * Assign a contact based on an email address. This should only be used when
 * the contact's URI is not available, as an extra query will have to be
 * performed to lookup the URI based on the email.
 *
 * @param emailAddress The email address of the contact.
 * @param lazyLookup If this is true, the lookup query will not be performed
 * until this view is clicked.
 */
ECode QuickContactBadge::AssignContactFromEmail(
    /* [in] */ const String& emailAddress,
    /* [in] */ Boolean lazyLookup)
{
    mContactEmail = emailAddress;
    if (!lazyLookup) {
        AutoPtr<IUri> uri;
        Uri::WithAppendedPath(Email.CONTENT_LOOKUP_URI, Uri.encode(mContactEmail), (IUri**)&uri);
        mQueryHandler->startQuery(TOKEN_EMAIL_LOOKUP, NULL,
                ,
                EMAIL_LOOKUP_PROJECTION, null, null, null);
    } else {
        mContactUri = null;
        OnContactUriChanged();
    }
}

/**
 * Assign a contact based on a phone number. This should only be used when
 * the contact's URI is not available, as an extra query will have to be
 * performed to lookup the URI based on the phone number.
 *
 * @param phoneNumber The phone number of the contact.
 * @param lazyLookup If this is true, the lookup query will not be performed
 * until this view is clicked.
 */
ECode QuickContactBadge::AssignContactFromPhone(
    /* [in] */ const String& phoneNumber,
    /* [in] */ Boolean lazyLookup)
{
    mContactPhone = phoneNumber;
    if (!lazyLookup) {
        mQueryHandler.startQuery(TOKEN_PHONE_LOOKUP, null,
                Uri.withAppendedPath(PhoneLookup.CONTENT_FILTER_URI, mContactPhone),
                PHONE_LOOKUP_PROJECTION, null, null, null);
    } else {
        mContactUri = null;
        onContactUriChanged();
    }
}

void QuickContactBadge::OnContactUriChanged()
{
    SetEnabled(IsAssigned());
}

ECode QuickContactBadge::OnClick(
    /* [in] */ IView* v)
{
    if (mContactUri != null) {
        QuickContact.showQuickContact(getContext(), QuickContactBadge.this, mContactUri,
                QuickContact.MODE_LARGE, mExcludeMimes);
    } else if (mContactEmail != null) {
        mQueryHandler.startQuery(TOKEN_EMAIL_LOOKUP_AND_TRIGGER, mContactEmail,
                Uri.withAppendedPath(Email.CONTENT_LOOKUP_URI, Uri.encode(mContactEmail)),
                EMAIL_LOOKUP_PROJECTION, null, null, null);
    } else if (mContactPhone != null) {
        mQueryHandler.startQuery(TOKEN_PHONE_LOOKUP_AND_TRIGGER, mContactPhone,
                Uri.withAppendedPath(PhoneLookup.CONTENT_FILTER_URI, mContactPhone),
                PHONE_LOOKUP_PROJECTION, null, null, null);
    } else {
        // If a contact hasn't been assigned, don't react to click.
        return;
    }
}

ECode QuickContactBadge::OnInitializeAccessibilityEvent(
    /* [in] */ IAccessibilityEvent* event)
{
    ImageView::OnInitializeAccessibilityEvent(event);
    AutoPtr<ICharSequence> seq;
    CStringWrapper::New(String("CQuickContactBadge"), (ICharSequence**)&seq);
    event->SetClassName(seq);
    return NOERROR;
}

ECode QuickContactBadge::OnInitializeAccessibilityNodeInfo(
    /* [in] */ IAccessibilityNodeInfo* info)
{
    ImageView::OnInitializeAccessibilityNodeInfo(info);
    AutoPtr<ICharSequence> seq;
    CStringWrapper::New(String("CQuickContactBadge"), (ICharSequence**)&seq);
    info->SetClassName(seq);
    return NOERROR;
}

/**
 * Set a list of specific MIME-types to exclude and not display. For
 * example, this can be used to hide the {@link Contacts#CONTENT_ITEM_TYPE}
 * profile icon.
 */
ECode QuickContactBadge::SetExcludeMimes(
    /* [in] */ const ArrayOf<String>& excludeMimes)
{
    mExcludeMimes = excludeMimes;
}

// ===================================================
//          QuickContactBadge::QueryHandler::
// ===================================================

QuickContactBadge::QueryHandler::QueryHandler(
    /* [in] */ IContentResolver* cr)
{
    super(cr);
}

void QuickContactBadge::QueryHandler::OnQueryComplete(
    /* [in] */ Int32 token,
    /* [in] */ IInterface* cookie,
    /* [in] */ ICursor* cursor)
{
    Uri lookupUri = NULL;
    Uri createUri = NULL;
    boolean trigger = FALSE;

    try {
        switch(token) {
            case TOKEN_PHONE_LOOKUP_AND_TRIGGER:
                trigger = TRUE;
                createUri = Uri.fromParts("tel", (String)cookie, null);

                //$FALL-THROUGH$
            case TOKEN_PHONE_LOOKUP: {
                if (cursor != null && cursor.moveToFirst()) {
                    long contactId = cursor.getLong(PHONE_ID_COLUMN_INDEX);
                    String lookupKey = cursor.getString(PHONE_LOOKUP_STRING_COLUMN_INDEX);
                    lookupUri = Contacts.getLookupUri(contactId, lookupKey);
                }

                break;
            }
            case TOKEN_EMAIL_LOOKUP_AND_TRIGGER:
                trigger = TRUE;
                createUri = Uri.fromParts("mailto", (String)cookie, null);

                //$FALL-THROUGH$
            case TOKEN_EMAIL_LOOKUP: {
                if (cursor != null && cursor.moveToFirst()) {
                    long contactId = cursor.getLong(EMAIL_ID_COLUMN_INDEX);
                    String lookupKey = cursor.getString(EMAIL_LOOKUP_STRING_COLUMN_INDEX);
                    lookupUri = Contacts.getLookupUri(contactId, lookupKey);
                }
                break;
            }
        }
    } finally {
        if (cursor != null) {
            cursor.close();
        }
    }

    mContactUri = lookupUri;
    onContactUriChanged();

    if (trigger && lookupUri != null) {
        // Found contact, so trigger QuickContact
        QuickContact.showQuickContact(getContext(), QuickContactBadge.this, lookupUri,
                QuickContact.MODE_LARGE, mExcludeMimes);
    } else if (createUri != null) {
        // Prompt user to add this person to contacts
        final Intent intent = new Intent(Intents.SHOW_OR_CREATE_CONTACT, createUri);
        getContext().startActivity(intent);
    }
}

} // namespace Widget
} // namespace Droid
} // namespace Elastos

