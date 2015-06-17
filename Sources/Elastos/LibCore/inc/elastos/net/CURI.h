
#ifndef __ELASTOS_NET_CURI_H__
#define __ELASTOS_NET_CURI_H__

#include "_Elastos_Net_CURI.h"
#include "Object.h"
#include "Character.h"
#include "UriCodec.h"

using Elastos::Core::Character;
using Elastos::IO::ISerializable;
using Elastos::Core::IComparable;
using Libcore::Net::UriCodec;

namespace Elastos {
namespace Net {

CarClass(CURI)
    , public Object
    , public IURI
    , public ISerializable
    , public IComparable
{
public:
    CAR_INTERFACE_DECL()

    CAR_OBJECT_DECL()

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
        /* [in] */ IInterface* uri,
        /* [out] */ Int32* result);

    static CARAPI Create(
        /* [in] */ const String& uri,
        /* [out] */ IURI** obj);

    CARAPI Equals(
        /* [in] */ IInterface* o,
        /* [out] */ Boolean* result);

    CARAPI GetScheme(
        /* [out] */ String* scheme);

    CARAPI GetSchemeSpecificPart(
        /* [out] */ String* schemeSpecific);

    CARAPI GetRawSchemeSpecificPart(
        /* [out] */ String* schemeSpecific);

    CARAPI GetAuthority(
        /* [out] */ String* authority);

    CARAPI GetRawAuthority(
        /* [out] */ String* authority);

    CARAPI GetUserInfo(
        /* [out] */ String* userInfo);

    CARAPI GetRawUserInfo(
        /* [out] */ String* userInfo);

    CARAPI GetHost(
        /* [out] */ String* host);

    CARAPI GetPort(
        /* [out] */ Int32* port);

    /** @hide */
    CARAPI GetEffectivePort(
        /* [out] */ Int32* port);

    static CARAPI_(Int32) GetEffectivePort(
        /* [in] */ const String& scheme,
        /* [in] */ Int32 specifiedPort);

    CARAPI GetPath(
        /* [out] */ String* path);

    CARAPI GetRawPath(
        /* [out] */ String* path);


    CARAPI GetQuery(
        /* [out] */ String* query);

    CARAPI GetRawQuery(
        /* [out] */ String* query);

    CARAPI GetFragment(
        /* [out] */ String* fragment);

    CARAPI GetRawFragment(
        /* [out] */ String* fragment);

    CARAPI GetHashCode(
        /* [out] */ Int32* hash);

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

    CARAPI ToString(
        /* [out] */ String* s);

    CARAPI ToURL(
        /* [out] */ IURL** url);

private:
    /**
     * Breaks uri into its component parts. This first splits URI into scheme,
     * scheme-specific part and fragment:
     *   [scheme:][scheme-specific part][#fragment]
     *
     * Then it breaks the scheme-specific part into authority, path and query:
     *   [//authority][path][?query]
     *
     * Finally it delegates to parseAuthority to break the authority into user
     * info, host and port:
     *   [user-info@][host][:port]
     */
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

    CARAPI_(AutoPtr<IURI>) Duplicate();

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
            /* [in] */ Char32 c)
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
            /* [in] */ Char32 c)
        {
            return c < 127;
        }
    };

public:
    static const String UNRESERVED;
    static const String PUNCTUATION;
    static const AutoPtr<UriCodec> USER_INFO_ENCODER;
    static const AutoPtr<UriCodec> PATH_ENCODER;
    static const AutoPtr<UriCodec> AUTHORITY_ENCODER;
    static const AutoPtr<UriCodec> FILE_AND_QUERY_ENCODER;
    static const AutoPtr<UriCodec> ALL_LEGAL_ENCODER;
    static const AutoPtr<UriCodec> ASCII_ONLY;

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

};

} // namespace Net
} // namespace Elastos

#endif //__ELASTOS_NET_CURI_H__
