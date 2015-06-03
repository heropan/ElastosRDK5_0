
#ifndef __CURI_H__
#define __CURI_H__

#include "_CURI.h"

#include <elastos/Character.h>
#include "UriCodec.h"

using Elastos::Core::Character;


namespace Elastos {
namespace Net {

CarClass(CURI)
{
public:
    CURI();

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ const String& uri);

    CARAPI constructor(
        /* [in] */ const String& scheme,
        /* [in] */ const String& ssp,
        /* [in] */ const String& frag);

    CARAPI constructor(
        /* [in] */ const String& scheme,
        /* [in] */ const String& userInfo,
        /* [in] */ const String& host,
        /* [in] */ Int32 port,
        /* [in] */ const String& path,
        /* [in] */ const String& query,
        /* [in] */ const String& fragment);

    CARAPI constructor(
        /* [in] */ const String& scheme,
        /* [in] */ const String& host,
        /* [in] */ const String& path,
        /* [in] */ const String& fragment);

    CARAPI constructor(
        /* [in] */ const String& scheme,
        /* [in] */ const String& authority,
        /* [in] */ const String& path,
        /* [in] */ const String& query,
        /* [in] */ const String& fragment);

    CARAPI CompareTo(
        /* [in] */ IURI* uri,
        /* [out] */ Int32* result);

    static CARAPI Create(
        /* [in] */ const String& uri,
        /* [out] */ IURI** obj);

    CARAPI GetAuthority(
        /* [out] */ String* authority);

    CARAPI GetFragment(
        /* [out] */ String* fragment);

    CARAPI GetHost(
        /* [out] */ String* host);

    CARAPI GetPath(
        /* [out] */ String* path);

    CARAPI GetPort(
        /* [out] */ Int32* port);

    /** @hide */
    CARAPI GetEffectivePort(
        /* [out] */ Int32* port);

    static CARAPI_(Int32) GetEffectivePort(
        /* [in] */ const String& scheme,
        /* [in] */ Int32 specifiedPort);

    CARAPI GetQuery(
        /* [out] */ String* query);

    CARAPI GetRawAuthority(
        /* [out] */ String* authority);

    CARAPI GetRawFragment(
        /* [out] */ String* fragment);

    CARAPI GetRawPath(
        /* [out] */ String* path);

    CARAPI GetRawQuery(
        /* [out] */ String* query);

    CARAPI GetRawSchemeSpecificPart(
        /* [out] */ String* schemeSpecific);

    CARAPI GetRawUserInfo(
        /* [out] */ String* userInfo);

    CARAPI GetScheme(
        /* [out] */ String* scheme);

    CARAPI GetSchemeSpecificPart(
        /* [out] */ String* schemeSpecific);

    CARAPI GetUserInfo(
        /* [out] */ String* userInfo);

    CARAPI IsAbsolute(
        /* [out] */ Boolean* isAbsolute);

    CARAPI IsOpaque(
        /* [out] */ Boolean* isOpaque);

    CARAPI Normalize(
        /* [out] */ IURI** uri);

    CARAPI ParseServerAuthority(
        /* [out] */ IURI** uri);

    CARAPI Relativize(
        /* [in] */ IURI* relative,
        /* [out] */ IURI** uri);

    CARAPI Resolve(
        /* [in] */ IURI* relative,
        /* [out] */ IURI** uri);

    CARAPI Resolve(
        /* [in] */ const String& relative,
        /* [out] */ IURI** uri);

    CARAPI ToASCIIString(
        /* [out] */ String* str);

    CARAPI GetHashCode(
        /* [out] */ Int32* hash);

    CARAPI Equals(
        /* [in] */ IInterface* o,
        /* [out] */ Boolean* result);

    CARAPI ToString(
        /* [out] */ String* s);

    CARAPI ToURL(
        /* [out] */ IURL** url);

private:
    CARAPI ParseURI(
        /* [in] */ const String& uri,
        /* [in] */ Boolean forceServer);

    CARAPI ValidateScheme(
        /* [in] */ const String& uri,
        /* [in] */ Int32 end,
        /* [out] */ String* result);

    CARAPI ParseAuthority(
        /* [in] */ Boolean forceServer);

    CARAPI ValidateUserInfo(
        /* [in] */ const String& uri,
        /* [in] */ const String& userInfo,
        /* [in] */ Int32 index);

    CARAPI IsValidHost(
        /* [in] */ Boolean forceServer,
        /* [in] */ const String& host,
        /* [out] */ Boolean* isValid);

    CARAPI_(Boolean) IsValidDomainName(
        /* [in] */ const String& host);

    CARAPI_(String) QuoteComponent(
        /* [in] */ const String& component,
        /* [in] */ const String& legalSet);

    CARAPI_(AutoPtr<CURI>) Duplicate();

    CARAPI_(String) ConvertHexToLowerCase(
        /* [in] */ const String& s);

    CARAPI_(Boolean) EscapedEquals(
        /* [in] */ const String& first,
        /* [in] */ const String& second);

    CARAPI_(String) Normalize(
        /* [in] */ const String& path,
        /* [in] */ Boolean discardRelativePrefix);

    CARAPI_(void) SetSchemeSpecificPart();

    CARAPI Decode(
        /* [in] */ const String& s,
        /* [out] */ String* decodeS);

    CARAPI_(String) GetHashString();


//    void readObject(ObjectInputStream in);
//    void writeObject(ObjectOutputStream out);

public:
    static const String UNRESERVED;
    static const String PUNCTUATION;
    static const UriCodec& USER_INFO_ENCODER;
    static const UriCodec& PATH_ENCODER;
    static const UriCodec& AUTHORITY_ENCODER;
    static const UriCodec& FILE_AND_QUERY_ENCODER;
    static const UriCodec& ALL_LEGAL_ENCODER;

private:
    class PartEncoder : public UriCodec
    {
    private:
        String mExtraLegalCharacters;

    public:
        PartEncoder(
            /* [in] */ const String& extraLegalCharacters)
        {
            mExtraLegalCharacters = extraLegalCharacters;
        }
    protected:
        Boolean IsRetained(
            /* [in] */ char c) const
        {
            return CURI::UNRESERVED.IndexOf(c) != -1
                    || PUNCTUATION.IndexOf(c) != -1
                    || mExtraLegalCharacters.IndexOf(c) != -1
                    || (c > 127 && !Character::IsSpaceChar(c) && !Character::IsISOControl(c));
        }
    };

    class ASCIIEncoder : public UriCodec
    {
    protected:
        Boolean IsRetained(
            /* [in] */ char c) const
        {
            return c < 127;
        }
    };
private:
    String mString;
    String mScheme;
    String mSchemeSpecificPart;
    String mAuthority;
    String mUserInfo;
    String mHost;
    Int32 mPort;
    String mPath;
    String mQuery;
    String mFragment;
    Boolean mOpaque;
    Boolean mAbsolute;
    Boolean mServerAuthority;
    Int32 mHash;

    static const UriCodec& ASCII_ONLY;
};

} // namespace Net
} // namespace Elastos

#endif //__CURI_H__
