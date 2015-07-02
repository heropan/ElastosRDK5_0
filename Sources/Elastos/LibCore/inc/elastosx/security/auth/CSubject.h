
#ifndef __CSUBJECT_H__
#define __CSUBJECT_H__

#include "_CSubject.h"
#include "AbstractSet.h"
#include "CAccessControlContext.h"

using Elastos::IO::IObjectInputStream;
using Elastos::IO::IObjectOutputStream;
using Elastos::Security::EIID_IPrivilegedAction;
using Elastos::Security::IAccessControlContext;
using Elastos::Security::IDomainCombiner;
using Elastos::Security::IPermission;
using Elastos::Security::IPrivilegedAction;
using Elastos::Security::IPrivilegedExceptionAction;
using Elastos::Security::CAccessControlContext;
using Elastos::Utility::ICollection;
using Elastos::Utility::IIterator;
using Elastos::Utility::ILinkedList;
using Elastos::Utility::ISet;
using Elastos::Utility::AbstractSet;
using Elastosx::Security::Auth::ISubjectDomainCombiner;

namespace Elastosx {
namespace Security {
namespace Auth {

CarClass(CSubject)
{
public:
    static CARAPI_(AutoPtr<IPermission>) InitStatics();

    static CARAPI DoAs(
        /* [in] */ ISubject *subject,
        /* [in] */ IPrivilegedAction *action,
        /* [out] */ IInterface **obj);

    static CARAPI DoAsPrivileged(
        /* [in] */ ISubject *subject,
        /* [in] */ IPrivilegedAction *action,
        /* [in] */ IAccessControlContext *context,
        /* [out] */ IInterface **obj);

    static CARAPI DoAs(
        /* [in] */ ISubject *subject,
        /* [in] */ IPrivilegedExceptionAction *action,
        /* [out] */ IInterface **obj);

    static CARAPI DoAsPrivileged(
        /* [in] */ ISubject *subject,
        /* [in] */ IPrivilegedExceptionAction *action,
        /* [in] */ IAccessControlContext *context,
        /* [out] */ IInterface **obj);

    static CARAPI GetSubject(
        /* [in] */ IAccessControlContext *context,
        /* [out] */ ISubject **subject);

    CARAPI Equals(
        /* [in] */ IInterface *obj,
        /* [out] */ Boolean *isEqual);

    CARAPI GetPrincipals(
        /* [out] */ ISet **principals);

    CARAPI GetPrincipals(
        /* [in] */ IInterface *pC,
        /* [out] */ ISet **principals);

    CARAPI GetPrivateCredentials(
        /* [out] */ ISet **credentials);

    CARAPI GetPrivateCredentials(
        /* [in] */ const ClassID& id,
        /* [out] */ ISet **credentials);

    CARAPI GetPublicCredentials(
        /* [out] */ ISet **credentials);

    CARAPI GetPublicCredentials(
        /* [in] */ const ClassID& id,
        /* [out] */ ISet **credentials);

    CARAPI GetHashCode(
        /* [out] */ Int32 *hashCode);

    CARAPI SetReadOnly();

    CARAPI IsReadOnly(
        /* [out] */ Boolean *isReadOnly);

    CARAPI ToString(
        /* [out] */ String *str);

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ Boolean readOnly,
        /* [in] */ ISet *subjPrincipals,
        /* [in] */ ISet *pubCredentials,
        /* [in] */ ISet *privCredentials);

private:
    class SecureSet
        : public AbstractSet
        , public ISet
        , public ElRefBase {
    public:
        CAR_INTERFACE_DECL()
        /*
         * verifies specified element, checks set state, and security permission
         * to modify set before adding new element
         */
        CARAPI Add(
            /* [in] */ IInterface *o,
            /* [out] */ Boolean *ret);

        CARAPI AddAll(
            /* [in] */ ICollection* collection,
            /* [out] */ Boolean* modified);

        CARAPI Clear();

        CARAPI Contains(
            /* [in] */ IInterface* object,
            /* [out] */ Boolean* result);

        CARAPI ContainsAll(
            /* [in] */ ICollection* collection,
            /* [out] */ Boolean* result);

        CARAPI Equals(
            /* [in] */ IInterface* object,
            /* [out] */ Boolean* value);

        CARAPI GetHashCode(
            /* [out] */ Int32* value);

        CARAPI IsEmpty(
            /* [out] */ Boolean* result);

        CARAPI Remove(
            /* [in] */ IInterface* object,
            /* [out] */ Boolean* result);

        CARAPI RemoveAll(
            /* [in] */ ICollection* collection,
            /* [out] */ Boolean* modified);

        CARAPI GetIterator(
            /* [out] */ IIterator **it);

        CARAPI RetainAll(
            /* [in] */ ICollection *c,
            /* [out] */ Boolean *ret);

        CARAPI GetSize(
            /* [out] */ Int32 *size);

        CARAPI ToArray(
            /* [out, callee] */ ArrayOf<IInterface*>** array);

        CARAPI ToArray(
            /* [in] */ ArrayOf<IInterface*>* contents,
            /* [out, callee] */ ArrayOf<IInterface*>** outArray);

        /**
         * return set with elements that are instances or subclasses of the
         * specified class
         */
         /*
            protected final <E> Set<E> get(final Class<E> c)
        */
        SecureSet(
            /* [in] */ IPermission *perm,
            /* [in] */ Handle32 host);

        SecureSet(
            /* [in] */ IPermission *perm,
            /* [in] */ ICollection *s,
            /* [in] */ Handle32 host);

        /**
         * return set with elements that are instances or subclasses of the
         * specified class
         */
        CARAPI Get(
            /* [in] */ const ClassID& id,
            /* [out] */ ISet **obj);
        /**
         * Represents iterator for subject's secure set
         */
        class SecureIterator
            : public IIterator
            , public ElRefBase
        {
        public:
            CAR_INTERFACE_DECL()
            CARAPI HasNext(
                /* [out] */ Boolean *hasNext);

            CARAPI Next(
                /* [out] */ IInterface **next);

            /**
             * checks set state, and security permission to modify set before
             * removing current element
             */
            Remove();
            SecureIterator(
                /* [in] */ IIterator *iterator,
                /* [in] */ Handle32 host);

        protected:
            AutoPtr<IIterator> mIterator;
            Handle32 mHost;
        };

        class SecureIteratorEx
            : public SecureIterator
        {
        public:
            SecureIterator(
                /* [in] */ IIterator *iterator,
                /* [in] */ Handle32 host)
                : SecureIterator(iterator, host)
            {}

            CARAPI Next(
                /* [out] */ IInterface **next)
            {
                return mIterator->Next(next);
            }
        };


        // verifies new set element
        CARAPI VerifyElement(
            /* [in] */ IInterface *o);

        CARAPI ReadObject(
            /* [in] */ IObjectInputStream *is);

        CARAPI WriteObject(
            /* [in] */ IObjectOutputStream *os);

    private:
        /**
         * Compatibility issue: see comments for setType variable
         */
        static const Int64 serialVersionUID;// = 7911754171111800359L;

        AutoPtr<ILinkedList> mElements;

        /*
         * Is used to define a set type for serialization.
         *
         * A type can be principal, priv. or pub. credential set. The spec.
         * doesn't clearly says that priv. and pub. credential sets can be
         * serialized and what classes they are. It is only possible to figure
         * out from writeObject method comments that priv. credential set is
         * serializable and it is an instance of SecureSet class. So pub.
         * credential was implemented by analogy
         *
         * Compatibility issue: the class follows its specified serial form.
         * Also according to the serialization spec. adding new field is a
         * compatible change. So is ok for principal set (because the default
         * value for integer is zero). But priv. or pub. credential set it is
         * not compatible because most probably other implementations resolve
         * this issue in other way
         */
        Int32 mSetType;

        // Defines principal set for serialization.
        static const Int32 SET_Principal = 0;

        // Defines private credential set for serialization.
        static const Int32 SET_PrivCred = 1;

        // Defines public credential set for serialization.
        static const Int32 SET_PubCred = 2;

        // permission required to modify set
        AutoPtr<IPermission> mPermission;

        Handle32 mHost;

    };

    class PrivilegedAction
        : public IPrivilegedAction
        , public ElRefBase
    {
    public:
        PrivilegedAction(
            /* [in] */ IAccessControlContext *context)
            : mAcc(context)
        {}

        PInterface Probe(
            /* [in] */ REIID riid)
        {
            if (riid == EIID_IInterface) {
                return (IInterface*)(IPrivilegedAction*)this;
            }
            else if (riid == EIID_IPrivilegedAction) {
                return (IPrivilegedAction*)this;
            }
            return NULL;
        }

        UInt32 AddRef()
        {
            return ElRefBase::AddRef();
        }

        UInt32 Release()
        {
            return ElRefBase::Release();
        }

        ECode GetInterfaceID(
            /* [in] */ IInterface* object,
            /* [out] */ InterfaceID* iid)
        {
            VALIDATE_NOT_NULL(iid)
            if (object == (IInterface*)(IPrivilegedAction*)this) {
                *iid = EIID_IPrivilegedAction;
            }
            else {
                return E_ILLEGAL_ARGUMENT_EXCEPTION;
            }
            return NOERROR;
        }

        ECode Run(
            /* [out] */ IInterface **dc)
        {
            AutoPtr<IDomainCombiner> tmpdc;
            ECode ec = mAcc->GetDomainCombiner((IDomainCombiner**)&tmpdc);
            *dc = tmpdc.Get();
            INTERFACE_ADDREF(*dc)
            return ec;
        }
    protected:
        AutoPtr<IAccessControlContext> mAcc;
    };

    class PrivilegedActionEx
        : public PrivilegedAction
    {
    public:
        PrivilegedAction(
            /* [in] */ IAccessControlContext *context,
            /* [in] */ ISubjectDomainCombiner *combiner)
            : PrivilegedAction(context)
            , mCombiner(combiner)
        {}

        ECode Run(
            /* [out] */ IInterface **dc)
        {
            AutoPtr<IAccessControlContext> acc;
            CAccessControlContext::New(mAcc, mCombiner, (IAccessControlContext**)&acc);
            *dc = acc.Get();
            INTERFACE_ADDREF(*dc)
            return NOERROR;
        }
    private:
        AutoPtr<ISubjectDomainCombiner> mCombiner;
    };

    static CARAPI DoAs_PrivilegedAction(
        /* [in] */ ISubject *subject,
        /* [in] */ IPrivilegedAction *action,
        /* [in] */ IAccessControlContext * const context,
        /* [out] */ IInterface **ret);

    static CARAPI DoAs_PrivilegedExceptionAction(
        /* [in] */ ISubject *subject,
        /* [in] */ IPrivilegedExceptionAction *action,
        /* [in] */ IAccessControlContext * const context,
        /* [out] */ IInterface **ret);

    CARAPI ReadObject(
        /* [in] */ IObjectInputStream *is);

    CARAPI WriteObject(
        /* [in] */ IObjectOutputStream *os);

    CARAPI CheckState();

private:
    static const Int64 serialVersionUID;

    static const AutoPtr<IPermission> _AS;

    static const AutoPtr<IPermission> _AS_PRIVILEGED;

    static const AutoPtr<IPermission> _SUBJECT;

    static const AutoPtr<IPermission> _PRINCIPALS;

    static const AutoPtr<IPermission> _PRIVATE_CREDENTIALS;

    static const AutoPtr<IPermission> _PUBLIC_CREDENTIALS;

    static const AutoPtr<IPermission> _READ_ONLY;

    const AutoPtr<ISet> mPrincipals;

    Boolean mReadOnly;

    // set of private credentials
    AutoPtr<SecureSet> mPrivateCredentials;

    // set of public credentials
    AutoPtr<SecureSet> mPublicCredentials;
};

}
}
}

#endif // __CSUBJECT_H__
