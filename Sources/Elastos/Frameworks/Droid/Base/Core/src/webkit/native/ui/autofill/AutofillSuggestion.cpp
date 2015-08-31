
namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Ui {
namespace Autofill {

AutofillSuggestion::AutofillSuggestion(
    /* in */ String name,
    /* in */ String label,
    /* in */ Int32 uniqueId)
    : mLabel(name)
    , mSublabel(label)
    , mUniqueId(uniqueId)
{
}

//@Override
String AutofillSuggestion::GetLabel()
{
    return mLabel;
}

//@Override
String AutofillSuggestion::GetSublabel()
{
    return mSublabel;
}

//@Override
Boolean AutofillSuggestion::IsEnabled()
{
    return TRUE;
}

//@Override
Boolean AutofillSuggestion::IsGroupHeader()
{
    return FALSE;
}

Int32 AutofillSuggestion::GetUniqueId()
{
    return mUniqueId;
}

} // namespace Autofill
} // namespace Ui
} // namespace Webkit
} // namespace Droid
} // namespace Elastos

