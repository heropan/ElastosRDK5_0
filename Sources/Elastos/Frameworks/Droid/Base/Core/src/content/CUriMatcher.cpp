
#include "content/CUriMatcher.h"

using Elastos::Utility::Regex::IPatternHelper;
using Elastos::Utility::Regex::CPatternHelper;
using Elastos::Utility::Regex::IPattern;

namespace Elastos {
namespace Droid {
namespace Content {

AutoPtr<IPattern> CUriMatcher::PATH_SPLIT_PATTERN;

AutoPtr<IPattern> CUriMatcher::GetSplitPattern()
{
    if (PATH_SPLIT_PATTERN.Get() == NULL) {
        AutoPtr<IPatternHelper> helper;
        CPatternHelper::AcquireSingleton((IPatternHelper**)&helper);
        helper->Compile(String("/"), (IPattern**)&PATH_SPLIT_PATTERN);
    }

    return PATH_SPLIT_PATTERN;
}

CUriMatcher::CUriMatcher()
    : mCode(IUriMatcher::NO_MATCH)
    , mWhich(-1)
{
}

CUriMatcher::~CUriMatcher()
{
    mChildren.Clear();
}

ECode CUriMatcher::AddURI(
    /* [in] */ const String& authority,
    /* [in] */ const String& path,
    /* [in] */ Int32 code)
{
    if (code < 0) {
        // throw new IllegalArgumentException("code " + code + " is invalid: it must be positive");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    AutoPtr<ArrayOf<String> > tokens;

    if (!path.IsNull()) {
        FAIL_RETURN(GetSplitPattern()->Split(path, (ArrayOf<String>**)&tokens))
    }

    Int32 numTokens = tokens != NULL ? tokens->GetLength() : 0;
    AutoPtr<CUriMatcher> node = this;
    String token;

    for (Int32 i = -1; i < numTokens; i++) {
        if (i < 0) {
            token = authority;
        }
        else {
            token = (*tokens)[i];
        }

        List<AutoPtr<CUriMatcher> >& children = node->mChildren;
        Int32 numChildren = children.GetSize();
        AutoPtr<CUriMatcher> child;
        Int32 j = 0;
        List<AutoPtr<CUriMatcher> >::Iterator it;

        for (it = children.Begin(); it != children.End(); it++, j++) {
            child = *it;
            if (token.Equals(child->mText)) {
                node = child;
                break;
            }
        }

        if (j == numChildren) {
            // Child not found, create it
            child = new CUriMatcher();

            if (token.Equals("#")) {
                child->mWhich = NUMBER;
            }
            else if (token.Equals("*")) {
                child->mWhich = TEXT;
            }
            else {
                child->mWhich = EXACT;
            }

            child->mText = token;
            node->mChildren.PushBack(child);
            node = child;
        }
    }

    node->mCode = code;
    return NOERROR;
}

ECode CUriMatcher::Match(
    /* [in] */ IUri* uri,
    /* [out] */ Int32* matchCode)
{
    VALIDATE_NOT_NULL(matchCode)
    *matchCode = NULL;
    VALIDATE_NOT_NULL(uri)

    AutoPtr<ArrayOf<String> > pathSegments;
    FAIL_RETURN(uri->GetPathSegments((ArrayOf<String>**)&pathSegments))
    Int32 length = pathSegments->GetLength();
    AutoPtr<CUriMatcher> node = this;
    String authority;
    FAIL_RETURN(uri->GetAuthority(&authority))

    if (0 == length && authority.IsNull()) {
        *matchCode = node->mCode;
        return NOERROR;
    }

    String u;
    Char32 c;
    AutoPtr<ArrayOf<Char32> > chars;
    for (Int32 i = -1; i < length; i++) {
        u = i < 0 ? authority : (*pathSegments)[i];

        List<AutoPtr<CUriMatcher> >& list = node->mChildren;
        if (list.IsEmpty()) break;
        node = NULL;
        List<AutoPtr<CUriMatcher> >::Iterator it;

        for (it = list.Begin(); it != list.End(); it++) {
            AutoPtr<CUriMatcher> n = *it;
            Int32 lk = 0;
            //which_switch:
            switch (n->mWhich) {
            case EXACT:
                if (n->mText.Equals(u)) {
                    node = n;
                }
                break;
            case NUMBER:
                chars = u.GetChars();
                lk = chars->GetLength();
                for (Int32 k = 0; k < lk; k++) {
                    c = (*chars)[i];
                    if (c < '0' || c > '9') {
                        //break which_switch;
                        goto which_switch;
                    }
                }
                node = n;
                break;
            case TEXT:
                node = n;
                break;
            }

which_switch:
            if (NULL != node) {
                break;
            }
        }

        if (NULL == node) {
            *matchCode = IUriMatcher::NO_MATCH;
            return NOERROR;
        }
    }

    *matchCode = node->mCode;
    return NOERROR;
}

ECode CUriMatcher::constructor(
    /* [in] */ Int32 code)
{
    mCode = code;
    return NOERROR;
}

}
}
}

