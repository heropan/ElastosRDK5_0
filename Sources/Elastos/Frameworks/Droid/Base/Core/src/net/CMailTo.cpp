
#include "net/CMailTo.h"
#include "net/Uri.h"
#include <errno.h>
#include <elastos/core/StringUtils.h>
#include <Elastos.CoreLibrary.h>

using Elastos::Core::StringUtils;

namespace Elastos {
namespace Droid {
namespace Net {

ECode CCMailTo::IsMailTo(
    /* [in] */ const String& url,
    /* [out] */ Boolean* result)
{
    if (url != null && url.startsWith(MAILTO_SCHEME)) {
        result = TRUE;
        return NOERROR;
    }
    result = FALSE;
    return NOERROR;
}

ECode CCMailTo::Parse(
    /* [in] */ const String& url,
    /* [out] */ IMailTo** result)
{
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
                m->mHeaders.Insert(s1.toLowerCase(), s2);
            }
            else
            {
                m->mHeaders.Insert(s1.toLowerCase(), NULL);
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
}


ECode CCMailTo::GetTo(
    /* [out] */ String* result)
{
    HashMap<String, String>::Iterator iter = mHeaders.Find(TO);
    if (iter != mHeaders.End())
        result = iter->mSecond;

    return NOERROR;
}

ECode CCMailTo::GetCc(
    /* [out] */ String* result)
{
    HashMap<String, String>::Iterator iter = mHeaders.Find(CC);
    if (iter != mHeaders.End())
        result = iter->mSecond;

    return NOERROR;
}

ECode CCMailTo::GetSubject(
    /* [out] */ String* result)
{
    HashMap<String, String>::Iterator iter = mHeaders.Find(SUBJECT);
    if (iter != mHeaders.End())
        result = iter->mSecond;

    return NOERROR;
}

ECode CCMailTo::GetBody(
    /* [out] */ String* result)
{
    HashMap<String, String>::Iterator iter = mHeaders.Find(BODY);
    if (iter != mHeaders.End())
        result = iter->mSecond;

    return NOERROR;
}

ECode CCMailTo::ToString(
    /* [out] */ String* result)
{
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
}

ECode CCMailTo::GetHeaders(
    /* [out, callee] */ IObjectStringMap** rethashmap)
{
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
}

ECode CCMailTo::CCMailTo()
{
    return NOERROR;
}


} // namespace Net
} // namepsace Droid
} // namespace Elastos
