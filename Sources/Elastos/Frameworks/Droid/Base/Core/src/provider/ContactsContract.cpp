
#include "provider/ContactsContract.h"
#include "provider/ContactsContractProfile.h"
#include "net/Uri.h"
#include "text/TextUtils.h"
#ifdef ELASTOS_CORE
#include "Elastos.Core_server.h"
#include "Pattern.h"
#else
#include "Elastos.Core.h"
#endif
#include <elastos/StringUtils.h>
#include <elastos/Math.h>

using Elastos::Core::Math;
using Elastos::Core::IInteger32;
using Elastos::Core::CInteger32;
using Elastos::Core::StringUtils;
using Elastos::Utility::Regex::IMatcher;
using Elastos::Droid::Net::Uri;
using Elastos::Droid::Text::TextUtils;
using Elastos::Droid::Provider::IContactsContractProfile;
#ifdef ELASTOS_CORE
using Elastos::Utility::Regex::Pattern;
#else
using Elastos::Utility::Regex::IPatternHelper;
using Elastos::Utility::Regex::CPatternHelper;
#endif

namespace Elastos {
namespace Droid {
namespace Provider {

static AutoPtr<IPattern> initSPLITPATTERN()
{
    AutoPtr<IPattern> pattern;
#ifdef ELASTOS_CORE
    Pattern::Compile(String("([\\w-\\.]+)@((?:[\\w]+\\.)+)([a-zA-Z]{2,4})|[\\w]+"), (IPattern**)&pattern);
#else
    AutoPtr<IPatternHelper> helper;
    CPatternHelper::AcquireSingleton((IPatternHelper**)&helper);
    helper->Compile(String("([\\w-\\.]+)@((?:[\\w]+\\.)+)([a-zA-Z]{2,4})|[\\w]+"), (IPattern**)&pattern);
#endif
    return pattern;
}

AutoPtr<IPattern> ContactsContract::SPLIT_PATTERN = initSPLITPATTERN();

ECode ContactsContract::GetAUTHORITYURI(
    /* [out] */ IUri** uri)
{
    VALIDATE_NOT_NULL(uri);

    return Uri::Parse(String("content://") + IContactsContract::AUTHORITY, uri);
}

ECode ContactsContract::IsProfileId(
    /* [in] */ Int64 id,
    /* [out] */ Boolean* res)
{
    VALIDATE_NOT_NULL(res);

    Int64 _id;
    FAIL_RETURN(ContactsContractProfile::GetMINID(&_id))
    *res =  id >= _id;
    return NOERROR;
}

ECode ContactsContract::Snippetize(
    /* [in] */ const String& content,
    /* [in] */ const String& displayName,
    /* [in] */ const String& query,
    /* [in] */ Char32 snippetStartMatch,
    /* [in] */ Char32 snippetEndMatch,
    /* [in] */ const String& snippetEllipsis,
    /* [in] */ Int32 snippetMaxTokens,
    /* [out] */ String* snippet)
{
    VALIDATE_NOT_NULL(snippet);

    String lowerQuery = query.IsNull() ? query.ToLowerCase() : String(NULL);
    if (TextUtils::IsEmpty(content) || TextUtils::IsEmpty(query) ||
            TextUtils::IsEmpty(displayName) || !content.ToLowerCase().Contains(lowerQuery)) {
        *snippet = NULL;
        return NOERROR;
    }

    // If the display name already contains the query term, return empty - snippets should
    // not be needed in that case.
    String lowerDisplayName = !displayName.IsNull() ? displayName.ToLowerCase() : String("");
    List<String> nameTokens;
    AutoPtr<List<IInteger32*> > nameTokenOffsets;
    FAIL_RETURN(Split(lowerDisplayName.Trim(), &nameTokens, (List<IInteger32*>*)nameTokenOffsets))
    List<String>::Iterator it = nameTokens.Begin();
    for (; it != nameTokens.End(); ++it) {
        if ((*it).StartWith(lowerQuery)) {
            *snippet = NULL;
            return NOERROR;
        }
    }

    AutoPtr<ArrayOf<String> > contentLines;
    FAIL_RETURN(StringUtils::Split(content, String("\n"), (ArrayOf<String>**)&contentLines))

    // Locate the lines of the content that contain the query term.
    for (Int32 i = 0; i < contentLines->GetLength(); i++) {
        String contentLine = (*contentLines)[i];
        if (contentLine.ToLowerCase().Contains(lowerQuery)) {

            // Line contains the query string - now search for it at the start of tokens.
            List<String> lineTokens;
            AutoPtr<List<IInteger32*> > tokenOffsets;
            FAIL_RETURN(Split(contentLine, &lineTokens, (List<IInteger32*>*)tokenOffsets))

            // As we find matches against the query, we'll populate this list with the marked
            // (or unchanged) tokens.
            List<String> markedTokens;

            Int32 firstToken = -1;
            Int32 lastToken = -1;
            for (UInt32 i = 0; i < lineTokens.GetSize(); i++) {
                String token = lineTokens[i];
                String lowerToken = token.ToLowerCase();
                if (lowerToken.StartWith(lowerQuery)) {

                    // Query term matched; surround the token with match markers.
                    markedTokens.PushBack(String(String("").Append(snippetStartMatch) + token + String("").Append(snippetEndMatch)));

                    // If this is the first token found with a match, mark the token
                    // positions to use for assembling the snippet.
                    if (firstToken == -1) {
                        firstToken = Elastos::Core::Math::Max(0, i - (Int32) Elastos::Core::Math::Floor(
                                    Elastos::Core::Math::Abs(snippetMaxTokens) / 2.0));
                        lastToken = Elastos::Core::Math::Min(lineTokens.GetSize(), firstToken +
                                    Elastos::Core::Math::Abs(snippetMaxTokens));
                    }
                }
                else {
                    markedTokens.PushBack(token);
                }
            }

            // Assemble the snippet by piecing the tokens back together.
            if (firstToken > -1) {
                StringBuilder sb;
                if (firstToken > 0) {
                    FAIL_RETURN(sb.AppendString(snippetEllipsis))
                }
                for (Int32 i = firstToken; i < lastToken; i++) {
                    String markedToken = markedTokens[i];
                    String originalToken = lineTokens[i];
                    FAIL_RETURN(sb.AppendString(markedToken))
                    if (i < lastToken - 1) {
                        // Add the characters that appeared between this token and the next.
                        Int32 start, end;
                        FAIL_RETURN((*tokenOffsets)[i]->GetValue(&start))
                        FAIL_RETURN((*tokenOffsets)[i + 1]->GetValue(&end))
                        FAIL_RETURN(sb.AppendString(contentLine.Substring(start + originalToken.GetLength(), end)))
                    }
                }
                if (lastToken < lineTokens.GetSize()) {
                    FAIL_RETURN(sb.AppendString(snippetEllipsis))
                }
                *snippet =  sb.ToString();
                return NOERROR;
            }
        }
    }
    *snippet = NULL;
    return NOERROR;
}

ECode ContactsContract::Split(
    /* [in] */ const String& content,
    /* [in] */ List<String>* tokens,
    /* [in] */ List<IInteger32*>* offsets)
{
    AutoPtr<IMatcher> matcher;
    FAIL_RETURN(SPLIT_PATTERN->Matcher(content, (IMatcher**)&matcher))

    Boolean result;
    while ((matcher->Find(&result),result)) {
        String text;
        FAIL_RETURN(matcher->Group(&text))
        tokens->PushBack(text);
        Int32 index;
        FAIL_RETURN(matcher->Start(&index))
        AutoPtr<IInteger32> _index;
        FAIL_RETURN(CInteger32::New(index, (IInteger32**)&_index))
        offsets->PushBack(_index);
    }
    return NOERROR;
}

} //Provider
} //Droid
} //Elastos