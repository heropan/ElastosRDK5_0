
#ifndef __CPARSEEXCEPTION_H__
#define __CPARSEEXCEPTION_H__



namespace Elastos {
namespace Droid {
namespace Net {

CarClass(CParseException), public CRuntimeException
{
public:
    String response;
    /**
     * Thrown when parsing a URL fails.
    */
    // See non-public class {@link WebAddress}.
    CARAPI ParseException(
        /* [in] */ const String& in_response);
};

} //namespace Net
} //namespace Droid
} //namespace Elastos

#endif // __CPARSEEXCEPTION_H__
