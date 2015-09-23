
#include "elastos/droid/net/MailTo.h"

using Elastos::Core::StringUtils;

namespace Elastos {
namespace Droid {
namespace Net {

CAR_INTERFACE_IMPL(MailTo, Object, IMailTo)

const String MailTo::TO = String("to");
const String MailTo::BODY = String("body");
const String MailTo::CC = String("cc");
const String MailTo::SUBJECT = String("subject");

ECode MailTo::IsMailTo(
    /* [in] */ const String& url,
    /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translated before. Need check.
    if (url != null && url.startsWith(MAILTO_SCHEME)) {
        result = TRUE;
        return NOERROR;
    }
    result = FALSE;
    return NOERROR;
#endif
}

ECode MailTo::Parse(
    /* [in] */ const String& url,
    /* [out] */ IMailTo** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translated before. Need check.
    VALIDATE_NOT_NULL(result);
    *result = NULL;

    if (url == NULL) {
        return E_NULL_POINTER_EXCEPTION;
    }
    Boolean bval;
    IsMailTo(url, &bval);
    if (!bval) {
        Logger::E(TAG, "Not a mailto scheme");
        return E_RUNTIME_EXCEPTION;
    }

    // Strip the scheme as the Uri parser can't cope with it.
    String noScheme = url.Substring(MAILTO_SCHEME.GetLength());
    AutoPtr<IUri> email;
    Uri::Parse(noScheme, (IUri**)&email);
    AutoPtr<IMailTo> m = new MailTo();

    String query;
    email->GetQuery(&query);


    if (query != NULL ) {
        AutoPtr<ArrayOf<String> > queries;
        ec = StringUtils::Split(query,String("&"), (ArrayOf<String>**)&queries);

        Int32 tempsize = queries->GetLength();
        Int32 i;
        String q;
        for (i=0;i<tempsize;i++)
        {
            q=(*queries)[i];
            AutoPtr<ArrayOf<String> > nameval;
            ec = StringUtils::Split(q,String("="), (ArrayOf<String>**)&nameval);
             if (nameval->GetLength() == 0) {
                continue;
            }

            // insert the headers with the name in lowercase so that
            // we can easily find common headers
            String s1;
            Uri::Decode(nameval[0],&s1);
            if (nameval->GetLength() > 1)
            {
                String s2;
                Uri::Decode(nameval[1], &s2);
                m->mHeaders.Insert(s1.toLowerCase(ILocale::ROOT), s2);
            }
            else
            {
                m->mHeaders.Insert(s1.toLowerCase(ILocale::ROOT), NULL);
            }
        }
    }

    // Address can be specified in both the headers and just after the
    // mailto line. Join the two together.
    String address;
    email->GetPath(&address);

    if (address != NULL) {
        String addr;
        m->GetTo(&addr);

        if (addr != null) {
            address += ", " + addr;
        }
        m->mHeaders.Insert(TO, address);
    }

    *result = m;
    REFCOUNT_ADD(*result);
    return NOERROR;
#endif
}


ECode MailTo::GetTo(
    /* [out] */ String* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translated before. Need check.
    HashMap<String, String>::Iterator iter = mHeaders.Find(TO);
    if (iter != mHeaders.End())
        result = iter->mSecond;

    return NOERROR;
#endif
}

ECode MailTo::GetCc(
    /* [out] */ String* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translated before. Need check.
    HashMap<String, String>::Iterator iter = mHeaders.Find(CC);
    if (iter != mHeaders.End())
        result = iter->mSecond;

    return NOERROR;
#endif
}

ECode MailTo::GetSubject(
    /* [out] */ String* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translated before. Need check.
    HashMap<String, String>::Iterator iter = mHeaders.Find(SUBJECT);
    if (iter != mHeaders.End())
        result = iter->mSecond;

    return NOERROR;
#endif
}

ECode MailTo::GetBody(
    /* [out] */ String* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translated before. Need check.
    HashMap<String, String>::Iterator iter = mHeaders.Find(BODY);
    if (iter != mHeaders.End())
        result = iter->mSecond;

    return NOERROR;
#endif
}

ECode MailTo::ToString(
    /* [out] */ String* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translated before. Need check.
    StringBuilder sb(MAILTO_SCHEME);
    sb.AppendChar('?');

    HashMap<String,String>:: Iterator it = mHeaders.Begin();


    //for (Map.Entry<String,String> header : mHeaders.entrySet()) {
    for (;it!=mHeaders.End();++it)
    {
        String temp1, temp2;
        Uri::encode(it->mFirst, &temp1);
        sb.Append(temp1);
        sb.AppendChar('=');

        Uri::encode(it->mSecond, &temp2);
        sb.Append(temp2);
        sb.AppendChar('&');
    }

    *result = sb.ToString();
    return NOERROR;
#endif
}

ECode MailTo::GetHeaders(
    /* [out] */ IMap** rethashmap)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translated before. Need check.
    VALIDATE_NOT_NULL(rethashmap);
    CObjectStringMap::New(rethashmap);

    String tempkey, tempvalue;
    HashMap<String,String>:: Iterator it = mHeaders.Begin();
    for (; it != mHeaders.End(); ++it)
    {
        tempkey = it->mFirst;
        tempvalue = it->mSecond;

        AutoPtr<ICharSequence> tempcharsequence;
        CString::New(tempvalue, (ICharSequence**)&tempcharsequence);
        rethashmap->Put(tempkey,tempcharsequence);
    }
    return NOERROR;
#endif
}

MailTo::MailTo()
{
#if 0 // TODO: Translated before. Need check.
    return NOERROR;
#endif
}


} // namespace Net
} // namepsace Droid
} // namespace Elastos
