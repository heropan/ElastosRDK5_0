
#include "elastos/droid/widget/QuickContactBadge.h"
#include "elastos/droid/content/CIntent.h"
#include "elastos/droid/net/Uri.h"
#include "elastos/droid/os/CBundle.h"
#include "elastos/droid/R.h"

using Elastos::Droid::Content::CIntent;
using Elastos::Droid::Content::IIntent;
using Elastos::Droid::Net::Uri;
using Elastos::Droid::Os::CBundle;
using Elastos::Droid::R;
using Elastos::Droid::View::Accessibility::IAccessibilityRecord;
using Elastos::Droid::View::EIID_IViewOnClickListener;
using Elastos::Core::CString;
using Elastos::Core::ICharSequence;
using Elastos::IO::ICloseable;

namespace Elastos {
namespace Droid {
namespace Widget {

const Int32 QuickContactBadge::TOKEN_EMAIL_LOOKUP = 0;
const Int32 QuickContactBadge::TOKEN_PHONE_LOOKUP = 1;
const Int32 QuickContactBadge::TOKEN_EMAIL_LOOKUP_AND_TRIGGER = 2;
const Int32 QuickContactBadge::TOKEN_PHONE_LOOKUP_AND_TRIGGER = 3;
const String QuickContactBadge::EXTRA_URI_CONTENT("uri_content");

const String QuickContactBadge::EMAIL_LOOKUP_PROJECTION[] = {
        // RawContacts.CONTACT_ID, //TODO
        // Contacts.LOOKUP_KEY, //TODO
};

const Int32 QuickContactBadge::EMAIL_ID_COLUMN_INDEX = 0;
const Int32 QuickContactBadge::EMAIL_LOOKUP_STRING_COLUMN_INDEX = 1;

static const String PHONE_LOOKUP_PROJECTION[] = {
        // PhoneLookup._ID, //TODO
        // PhoneLookup.LOOKUP_KEY, //TODO
};

const Int32 QuickContactBadge::PHONE_ID_COLUMN_INDEX = 0;
const Int32 QuickContactBadge::PHONE_LOOKUP_STRING_COLUMN_INDEX = 1;


// ===================================================
//          QuickContactBadge::QueryHandler::
// ===================================================
QuickContactBadge::QueryHandler::QueryHandler(
    /* [in] */ IContentResolver* cr,
    /* [in] */ QuickContactBadge* host)
    : mHost(host)
{
    AsyncQueryHandler::constructor(cr);
}

ECode QuickContactBadge::QueryHandler::OnQueryComplete(
    /* [in] */ Int32 token,
    /* [in] */ IInterface* cookie,
    /* [in] */ ICursor* cursor)
{
    AutoPtr<IUri> lookupUri;
    AutoPtr<IUri> createUri;
    Boolean trigger = FALSE;
    AutoPtr<IBundle> extras;
    if (cookie != NULL) {
        extras = IBundle::Probe(cookie);
    }
    else {
        CBundle::New((IBundle**)&extras);
    }
    // try {
    String value;
    Boolean succeeded = FALSE;
    switch(token) {
        case TOKEN_PHONE_LOOKUP_AND_TRIGGER:
            trigger = TRUE;
            extras->GetString(EXTRA_URI_CONTENT, &value);
            Uri::FromParts(String("tel"), value, String(NULL), (IUri**)&createUri);

            //$FALL-THROUGH$
        case TOKEN_PHONE_LOOKUP: {
            if (cursor != NULL && (cursor->MoveToFirst(&succeeded), succeeded)) {
                Int64 contactId = 0;
                cursor->GetInt64(PHONE_ID_COLUMN_INDEX, &contactId);
                String lookupKey;
                cursor->GetString(PHONE_LOOKUP_STRING_COLUMN_INDEX, &lookupKey);
                assert(0 && "TODO");
                // Contacts::GetLookupUri(contactId, lookupKey, (IUri**)&lookupUri);
            }

            break;
        }
        case TOKEN_EMAIL_LOOKUP_AND_TRIGGER:
            trigger = TRUE;
            extras->GetString(EXTRA_URI_CONTENT, &value);
            Uri::FromParts(String("mailto"), value, String(NULL), (IUri**)&createUri);

            //$FALL-THROUGH$
        case TOKEN_EMAIL_LOOKUP: {
            if (cursor != NULL && (cursor->MoveToFirst(&succeeded), succeeded)) {
                Int64 contactId = 0;
                cursor->GetInt64(EMAIL_ID_COLUMN_INDEX, &contactId);
                String lookupKey;
                cursor->GetString(EMAIL_LOOKUP_STRING_COLUMN_INDEX, &lookupKey);
                assert(0 && "TODO");
                // Contacts::GetLookupUri(contactId, lookupKey, (IUri**)&lookupUri);
            }
            break;
        }
    }
    // } finally {
    if (cursor != NULL) {
        ICloseable::Probe(cursor)->Close();
    }
    // }

    mHost->mContactUri = lookupUri;
    mHost->OnContactUriChanged();

    if (trigger && lookupUri != NULL) {
        assert(0 && "TODO");
        // Found contact, so trigger QuickContact
        // QuickContact.showQuickContact(getContext(), QuickContactBadge.this, lookupUri,
        //         QuickContact.MODE_LARGE, mExcludeMimes);
    }
    else if (createUri != NULL) {
        // Prompt user to add this person to contacts
        AutoPtr<IIntent> intent;
        assert(0 && "TODO");
        // CIntent::New(Intents.SHOW_OR_CREATE_CONTACT, createUri, (IIntent**)&intent);
        if (extras != NULL) {
            extras->Remove(EXTRA_URI_CONTENT);
            intent->PutExtras(extras);
        }
        AutoPtr<IContext> context;
        mHost->GetContext((IContext**)&context);
        context->StartActivity(intent);
    }
    return NOERROR;
}

// ===================================================
//          QuickContactBadge::QuickContactBadgeClickListener
// ===================================================
CAR_INTERFACE_IMPL(QuickContactBadge::QuickContactBadgeClickListener, Object, IViewOnClickListener);
QuickContactBadge::QuickContactBadgeClickListener::QuickContactBadgeClickListener(
    /* [in] */ QuickContactBadge* host)
    : mHost(host)
{
}

ECode QuickContactBadge::QuickContactBadgeClickListener::OnClick(
    /* [in] */ IView* v)
{
    return mHost->OnClick(v);
}

CAR_INTERFACE_IMPL_2(QuickContactBadge, ImageView, IQuickContactBadge, IViewOnClickListener);
ECode QuickContactBadge::constructor(
    /* [in] */ IContext* context)
{
    return constructor(context, NULL);
}

ECode QuickContactBadge::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    return constructor(context, attrs, 0);
}

ECode QuickContactBadge::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyleAttr)
{
    return constructor(context, attrs, defStyleAttr, 0);
}

ECode QuickContactBadge::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyleAttr,
    /* [in] */ Int32 defStyleRes)
{
    ImageView::constructor(context, attrs, defStyleAttr, defStyleRes);

    AutoPtr<ITypedArray> styledAttributes;
    AutoPtr<ArrayOf<Int32> > attrIds = ArrayOf<Int32>::Alloc(
            const_cast<Int32 *>(R::styleable::Theme),
            ArraySize(R::styleable::Theme));
    mContext->ObtainStyledAttributes(attrIds, (ITypedArray**)&styledAttributes);
    styledAttributes->GetDrawable(R::styleable::Theme_quickContactBadgeOverlay, (IDrawable**)&mOverlay);
    styledAttributes->Recycle();

    Boolean mode = FALSE;
    if (IsInEditMode(&mode), !mode) {
        AutoPtr<IContentResolver> resolver;
        mContext->GetContentResolver((IContentResolver**)&resolver);
        mQueryHandler = new QueryHandler(resolver, this);
    }

    AutoPtr<QuickContactBadgeClickListener> listener = new QuickContactBadgeClickListener(this);
    SetOnClickListener(listener);
    return NOERROR;
}

ECode QuickContactBadge::DrawableStateChanged()
{
    ImageView::DrawableStateChanged();
    Boolean isStateful = FALSE;
    if (mOverlay != NULL && (mOverlay->IsStateful(&isStateful) , isStateful)) {
        AutoPtr<ArrayOf<Int32> > drawableState;
        GetDrawableState((ArrayOf<Int32>**)&drawableState);
        mOverlay->SetState(drawableState, &isStateful);
        Invalidate();
    }
    return NOERROR;
}

ECode QuickContactBadge::DrawableHotspotChanged(
    /* [in] */ Float x,
    /* [in] */ Float y)
{
    ImageView::DrawableHotspotChanged(x, y);

    if (mOverlay != NULL) {
        mOverlay->SetHotspot(x, y);
    }
    return NOERROR;
}

ECode QuickContactBadge::SetMode(
    /* [in] */ Int32 size)
{
    return NOERROR;
}

void QuickContactBadge::OnDraw(
    /* [in] */ ICanvas* canvas)
{
    ImageView::OnDraw(canvas);

    Boolean enabled = FALSE;
    if (IsEnabled(&enabled), !enabled) {
        return;
    }

    if (mOverlay == NULL) {
        return;
    }
    Int32 width = 0, height = 0;
    mOverlay->GetIntrinsicWidth(&width);
    mOverlay->GetIntrinsicHeight(&height);
    if (width == 0 || height == 0) {
        return;
    }

    GetWidth(&width);
    GetHeight(&height);
    mOverlay->SetBounds(0, 0, width, height);

    if (mPaddingTop == 0 && mPaddingLeft == 0) {
        mOverlay->Draw(canvas);
    } else {
        Int32 saveCount = 0, tmp = 0;
        canvas->GetSaveCount(&saveCount);
        canvas->Save(&tmp);
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
        mContext->GetDrawable(R::drawable::ic_contact_picture, (IDrawable**)&mDefaultAvatar);
    }
    return SetImageDrawable(mDefaultAvatar);
}

ECode QuickContactBadge::AssignContactUri(
    /* [in] */ IUri* contactUri)
{
    mContactUri = contactUri;
    mContactEmail = NULL;
    mContactPhone = NULL;
    OnContactUriChanged();
    return NOERROR;
}

ECode QuickContactBadge::AssignContactFromEmail(
    /* [in] */ const String& emailAddress,
    /* [in] */ Boolean lazyLookup)
{
    return AssignContactFromEmail(emailAddress, lazyLookup, NULL);
}

ECode QuickContactBadge::AssignContactFromEmail(
    /* [in] */ const String& emailAddress,
    /* [in] */ Boolean lazyLookup,
    /* [in] */ IBundle* extras)
{
    mContactEmail = emailAddress;
    mExtras = extras;
    if (!lazyLookup && mQueryHandler != NULL) {
        assert(0 && "TODO");
        // mQueryHandler->StartQuery(TOKEN_EMAIL_LOOKUP, NULL,
        //         Uri::WithAppendedPath(Email.CONTENT_LOOKUP_URI, Uri.encode(mContactEmail)),
        //         EMAIL_LOOKUP_PROJECTION, NULL, NULL, NULL);
    } else {
        mContactUri = NULL;
        OnContactUriChanged();
    }
    return NOERROR;
}

ECode QuickContactBadge::AssignContactFromPhone(
    /* [in] */ const String& phoneNumber,
    /* [in] */ Boolean lazyLookup)
{
    AutoPtr<IBundle> bundle;
    CBundle::New((IBundle**)&bundle);
    return AssignContactFromPhone(phoneNumber, lazyLookup, bundle);
}

ECode QuickContactBadge::AssignContactFromPhone(
    /* [in] */ const String& phoneNumber,
    /* [in] */ Boolean lazyLookup,
    /* [in] */ IBundle* extras)
{
    mContactPhone = phoneNumber;
    mExtras = extras;
    if (!lazyLookup && mQueryHandler != NULL) {
        assert(0 && "TODO");
        // mQueryHandler->StartQuery(TOKEN_PHONE_LOOKUP, NULL,
        //         Uri.withAppendedPath(PhoneLookup.CONTENT_FILTER_URI, mContactPhone),
        //         PHONE_LOOKUP_PROJECTION, NULL, NULL, NULL);
    } else {
        mContactUri = NULL;
        OnContactUriChanged();
    }
    return NOERROR;
}

ECode QuickContactBadge::SetOverlay(
    /* [in] */ IDrawable* overlay)
{
    mOverlay = overlay;
    return NOERROR;
}

void QuickContactBadge::OnContactUriChanged()
{
    SetEnabled(IsAssigned());
}

ECode QuickContactBadge::OnClick(
    /* [in] */ IView* v)
{
    // If contact has been assigned, mExtras should no longer be null, but do a null check
    // anyway just in case assignContactFromPhone or Email was called with a null bundle or
    // wasn't assigned previously.
    AutoPtr<IBundle> extras;
    if (mExtras == NULL) {
        CBundle::New((IBundle**)&extras);
    }
    else {
        extras = mExtras;
    }
    if (mContactUri != NULL) {
        assert(0 && "TODO");
        // QuickContact.showQuickContact(getContext(), QuickContactBadge.this, mContactUri,
        //         QuickContact.MODE_LARGE, mExcludeMimes);
    }
    else if (mContactEmail != NULL && mQueryHandler != NULL) {
        extras->PutString(EXTRA_URI_CONTENT, mContactEmail);
        assert(0 && "TODO");
        // mQueryHandler->StartQuery(TOKEN_EMAIL_LOOKUP_AND_TRIGGER, extras,
        //         Uri.withAppendedPath(Email.CONTENT_LOOKUP_URI, Uri.encode(mContactEmail)),
        //         EMAIL_LOOKUP_PROJECTION, NULL, NULL, NULL);
    }
    else if (mContactPhone != NULL && mQueryHandler != NULL) {
        extras->PutString(EXTRA_URI_CONTENT, mContactPhone);
        assert(0 && "TODO");
        // mQueryHandler->StartQuery(TOKEN_PHONE_LOOKUP_AND_TRIGGER, extras,
        //         Uri.withAppendedPath(PhoneLookup.CONTENT_FILTER_URI, mContactPhone),
        //         PHONE_LOOKUP_PROJECTION, NULL, NULL, NULL);
    }
    else {
        // If a contact hasn't been assigned, don't react to click.
        return NOERROR;
    }
    return NOERROR;
}

ECode QuickContactBadge::OnInitializeAccessibilityEvent(
    /* [in] */ IAccessibilityEvent* event)
{
    ImageView::OnInitializeAccessibilityEvent(event);
    AutoPtr<ICharSequence> seq;
    CString::New(String("CQuickContactBadge"), (ICharSequence**)&seq);
    IAccessibilityRecord::Probe(event)->SetClassName(seq);
    return NOERROR;
}

ECode QuickContactBadge::OnInitializeAccessibilityNodeInfo(
    /* [in] */ IAccessibilityNodeInfo* info)
{
    ImageView::OnInitializeAccessibilityNodeInfo(info);
    AutoPtr<ICharSequence> seq;
    CString::New(String("CQuickContactBadge"), (ICharSequence**)&seq);
    info->SetClassName(seq);
    return NOERROR;
}

ECode QuickContactBadge::SetExcludeMimes(
    /* [in] */ ArrayOf<String>* excludeMimes)
{
    mExcludeMimes = excludeMimes;
    return NOERROR;
}

} // namespace Widget
} // namespace Droid
} // namespace Elastos
