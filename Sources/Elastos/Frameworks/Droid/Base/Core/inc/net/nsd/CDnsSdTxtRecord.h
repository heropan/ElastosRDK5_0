#ifndef __ELASTOS_DROID_NET_NSD_CDNSSDTXTRECORD_H__
#define __ELASTOS_DROID_NET_NSD_CDNSSDTXTRECORD_H__

#include "_Elastos_Droid_Net_Nsd_CDnsSdTxtRecord.h"

namespace Elastos {
namespace Droid {
namespace Net {
namespace Nsd {

/**
 * This class handles TXT record data for DNS based service discovery as specified at
 * http://tools.ietf.org/html/draft-cheshire-dnsext-dns-sd-11
 *
 * DNS-SD specifies that a TXT record corresponding to an SRV record consist of
 * a packed array of bytes, each preceded by a length byte. Each string
 * is an attribute-value pair.
 *
 * The DnsSdTxtRecord object stores the entire TXT data as a single byte array, traversing it
 * as need be to implement its various methods.
 * @hide
 *
 */
CarClass(CDnsSdTxtRecord)
{
public:
    CDnsSdTxtRecord();

    ~CDnsSdTxtRecord();

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ ArrayOf<Byte>* data);

    CARAPI constructor(
        /* [in] */ IDnsSdTxtRecord* src);

    /**
     * Set a key/value pair. Setting an existing key will replace its value.
     * @param key Must be ascii with no '='
     * @param value matching value to key
     */
    CARAPI Set(
        /* [in] */ const String& key,
        /* [in] */ const String& value);

    /**
     * Get a value for a key
     *
     * @param key
     * @return The value associated with the key
     */
    CARAPI Get(
        /* [in] */ const String& key,
        /* [out] */ String* str);

    /** Remove a key/value pair. If found, returns the index or -1 if not found */
    CARAPI Remove(
        /* [in] */ const String& key,
        /* [out] */ Int32* val);

    /** Return the count of keys */
    CARAPI KeyCount(
        /* [out] */ Int32* val);

    /** Return true if key is present, false if not. */
    CARAPI Contains(
        /* [in] */ const String& key,
        /* [out] */ Boolean* val);

    /* Gets the size in bytes */
    CARAPI Size(
        /* [out] */ Int32* val);

    /* Gets the raw data in bytes */
    CARAPI GetRawData(
        /* [out, callee] */ ArrayOf<Byte>** data);

    CARAPI Equals(
        /* [in] */ IInterface* o,
        /* [out] */ Boolean* val);

    CARAPI Equals(
        /* [in] */ IDnsSdTxtRecord* o,
        /* [out] */ Boolean* val);

    CARAPI GetHashCode(
        /* [out] */ Int32* val);

    CARAPI WriteToParcel(
        /* [in] */ IParcel* dest);

    CARAPI ReadFromParcel(
        /* [in] */ IParcel* source);

    /**
     * Return a string representation.
     * Example : {key1=value1},{key2=value2}..
     *
     * For a key say like "key3" with null value
     * {key1=value1},{key2=value2}{key3}
     */
    CARAPI ToString(
        /* [out] */ String* info);

private:
    CARAPI_(void) Insert(
        /* [in] */ ArrayOf<Byte>* keyBytes,
        /* [in] */ ArrayOf<Byte>* value,
        /* [in] */ Int32 index);

    /** Return a key in the TXT record by zero-based index. Returns null if index exceeds the total number of keys. */
    CARAPI_(String) GetKey(
        /* [in] */ Int32 index);

    /**
     * Look up a key in the TXT record by zero-based index and return its value.
     * Returns null if index exceeds the total number of keys.
     * Returns null if the key is present with no value.
     */
    CARAPI GetValue(
        /* [in] */ Int32 index,
        /* [out, callee] */ ArrayOf<Byte>** val);

    CARAPI_(String) GetValueAsString(
        /* [in] */ Int32 index);

    CARAPI GetValue(
        /* [in] */ const String& forKey,
        /* [out, callee] */ ArrayOf<Byte>** val);

public:
    static const Byte mSeperator;

    AutoPtr<ArrayOf<Byte> > mData;
};

}
}
}
}

#endif // __ELASTOS_DROID_NET_NSD_CDNSSDTXTRECORD_H__
