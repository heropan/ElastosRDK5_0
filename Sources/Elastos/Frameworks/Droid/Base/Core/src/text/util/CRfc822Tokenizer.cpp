#include "Elastos.Core.h"
#include "text/util/CRfc822Tokenizer.h"
#include "text/util/CRfc822Token.h"

using Elastos::Core::CString;
using Elastos::Core::CObjectContainer;
using Elastos::Droid::Text::Utility::CRfc822Token;

namespace Elastos {
namespace Droid {
namespace Text {
namespace Utility {

ECode CRfc822Tokenizer::Tokenize(
    /* [in] */ ICharSequence* text,
    /* [in, out] */ IObjectContainer* out)
{
    VALIDATE_NOT_NULL(text);
    VALIDATE_NOT_NULL(out);
    StringBuilder name;
    StringBuilder address;
    StringBuilder comment;

    Int32 i = 0;
    Int32 cursor;
    text->GetLength(&cursor);

    while (i < cursor) {
        Char32 c;
        text->GetCharAt(i, &c);

        if (c == ',' || c == ';') {
            i++;

            Char32 cc;
            while(i < cursor && (text->GetCharAt(i, &cc), cc) == ' ') {
                i++;
            }
            Crunch(name);
            if (address.GetLength() > 0) {
                AutoPtr<IRfc822Token> rfc822Token;
                CRfc822Token::New(name.ToString(), address.ToString(), comment.ToString(), (IRfc822Token**)&rfc822Token);
                out->Add(rfc822Token.Get());
            }
            else if (name.GetLength() > 0) {
                AutoPtr<IRfc822Token> rfc822Token;
                CRfc822Token::New(String(NULL), name.ToString(), comment.ToString(), (IRfc822Token**)&rfc822Token);
                out->Add(rfc822Token.Get());
            }

            name.Reset();
            address.Reset();
            comment.Reset();
        }
        else if (c == '"') {
            i++;

            while (i < cursor) {
                text->GetCharAt(i, &c);

                if (c == '"') {
                    i++;
                    break;
                }
                else if (c == '\\') {
                    if (i + 1 < cursor) {
                        Char32 cc;
                        name.AppendChar((text->GetCharAt(i + 1, &cc), cc));
                    }
                    i += 2;
                }
                else {
                    name.AppendChar(c);
                    i++;
                }
            }
        }
        else if (c == '(') {
            Int32 level = 1;
            i++;

            while (i < cursor && level > 0) {
                text->GetCharAt(i, &c);

                if (c == ')') {
                    if (level > 1) {
                        comment.AppendChar(c);
                    }

                    level--;
                    i++;
                }
                else if (c == '(') {
                    comment.AppendChar(c);
                    level++;
                    i++;
                }
                else if (c == '\\') {
                    if (i + 1 < cursor) {
                        Char32 cc;
                        comment.AppendChar((text->GetCharAt(i + 1, &cc), cc));
                    }
                    i += 2;
                }
                else {
                    comment.AppendChar(c);
                    i++;
                }
            }
        }
        else if (c == '<') {
            i++;

            while (i < cursor) {
                text->GetCharAt(i, &c);

                if (c == '>') {
                    i++;
                    break;
                }
                else {
                    address.AppendChar(c);
                    i++;
                }
            }
        }
        else if (c == ' ') {
            name.AppendChar('\0');
            i++;
        }
        else {
            name.AppendChar(c);
            i++;
        }
    }

    Crunch(name);
    Int32 count = 0;
    if (address.GetLength() > 0) {
        AutoPtr<IRfc822Token> rfc822Token;
        CRfc822Token::New(name.ToString(), address.ToString(), comment.ToString(), (IRfc822Token**)&rfc822Token);
        out->Add(rfc822Token.Get());
    }
    else if (name.GetLength() > 0) {
        AutoPtr<IRfc822Token> rfc822Token;
        CRfc822Token::New(String(NULL), name.ToString(), comment.ToString(), (IRfc822Token**)&rfc822Token);
        out->Add(rfc822Token.Get());
    }
    return NOERROR;
}

ECode CRfc822Tokenizer::Tokenize(
    /* [in] */ ICharSequence* text,
    /* [out, callee] */ ArrayOf<IRfc822Token*>** ret)
{
    VALIDATE_NOT_NULL(ret);
    AutoPtr<IObjectContainer> objContainer;
    CObjectContainer::New((IObjectContainer**)&objContainer);

    FAIL_RETURN(Tokenize(text, objContainer));
    Int32 objCount;
    objContainer->GetObjectCount(&objCount);
    AutoPtr< ArrayOf<IRfc822Token*> > ret_ = ArrayOf<IRfc822Token*>::Alloc(objCount);

    if (objContainer != NULL) {
        AutoPtr<IObjectEnumerator> it;
        objContainer->GetObjectEnumerator((IObjectEnumerator**)&it);
        Boolean succeeded;
        Int32 i = 0;
        while (it->MoveNext(&succeeded), succeeded) {
            AutoPtr<IRfc822Token> lm;
            it->Current((IInterface**)&lm);
            ret_->Set(i, lm.Get());
            i++;
        }
    }

    *ret = ret_.Get();
    ARRAYOF_ADDREF(*ret);
    return NOERROR;
}

void CRfc822Tokenizer::Crunch(
    /* [in] */ StringBuilder& sb)
{
    Int32 i = 0;
    Int32 len = sb.GetLength();
    while (i < len) {
        Char32 c;
        sb.GetCharAt(i, &c);

        if (c == '\0') {
            if (i == 0 || i == len - 1 ||
                    (sb.GetChar(i-1)) == ' ' ||
                    (sb.GetChar(i-1)) == '\0' ||
                    (sb.GetChar(i+1)) == ' ' ||
                    (sb.GetChar(i+1)) == '\0') {
                sb.DeleteChar(i);
                len--;
            }
            else {
                i++;
            }
        }
        else {
            i++;
        }
    }

    for (i = 0; i < len; i++) {
        if ((sb.GetChar(i)) == '\0') {
            sb.SetChar(i, ' ');
        }
    }
}

ECode CRfc822Tokenizer::FindTokenStart(
    /* [in] */ ICharSequence* text,
    /* [in] */ Int32 cursor,
    /* [out] */ Int32* ret)
{
    /*
     * It's hard to search backward, so search forward until
     * we reach the cursor.
     */
    VALIDATE_NOT_NULL(ret);
    VALIDATE_NOT_NULL(text);
    Int32 best = 0;
    Int32 i = 0;

    while (i < cursor) {
        FAIL_RETURN(FindTokenEnd(text, i, &i));

        if (i < cursor) {
            i++; // Skip terminating punctuation

            Char32 cc;
            while(i < cursor && (text->GetCharAt(i, &cc), cc) == ' '){
                i++;
            }

            if (i < cursor) {
                best = i;
            }
        }
    }

    *ret = best;
    return NOERROR;
}

ECode CRfc822Tokenizer::FindTokenEnd(
    /* [in] */ ICharSequence* text,
    /* [in] */ Int32 cursor,
    /* [out] */ Int32* ret)
{
    VALIDATE_NOT_NULL(ret);
    VALIDATE_NOT_NULL(text);
    Int32 len;
    text->GetLength(&len);
    Int32 i = cursor;

    while (i < len) {
        Char32 c;
        text->GetCharAt(i, &c);

        if (c == ',' || c == ';') {
            *ret = i;
            return NOERROR;
        }
        else if (c == '"') {
            i++;

            while (i < len) {
                text->GetCharAt(i, &c);

                if (c == '"') {
                    i++;
                    break;
                }
                else if (c == '\\' && i + 1 < len) {
                    i += 2;
                }
                else {
                    i++;
                }
            }
        }
        else if (c == '(') {
            Int32 level = 1;
            i++;

            while (i < len && level > 0) {
                text->GetCharAt(i, &c);

                if (c == ')') {
                    level--;
                    i++;
                }
                else if (c == '(') {
                    level++;
                    i++;
                }
                else if (c == '\\' && i + 1 < len) {
                    i += 2;
                }
                else {
                    i++;
                }
            }
        }
        else if (c == '<') {
            i++;
            while (i < len) {
                text->GetCharAt(i, &c);

                if (c == '>') {
                    i++;
                    break;
                }
                else {
                    i++;
                }
            }
        }
        else {
            i++;
        }
    }

    *ret = i;
    return NOERROR;
}

ECode CRfc822Tokenizer::TerminateToken(
    /* [in] */ ICharSequence* text,
    /* [out] */ ICharSequence** ret)
{
    VALIDATE_NOT_NULL(ret);
    *ret = NULL;
    VALIDATE_NOT_NULL(text);
    String strText;
    text->ToString(&strText);
    return CString::New(strText + String(", "), ret);
}

} // namespace Utility
} // namespace Text
} // namepsace Droid
} // namespace Elastos
