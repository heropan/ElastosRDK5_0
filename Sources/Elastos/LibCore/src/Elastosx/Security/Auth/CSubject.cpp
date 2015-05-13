
#include "CSubject.h"
#include "cmdef.h"
#include "CAuthPermission.h"
#include "CAccessController.h"
#include "CAccessControlContext.h"
#include "CSubjectDomainCombiner.h"
#include "StringBuilder.h"
#include "CLinkedList.h"

using Elastos::Core::StringBuilder;
using Elastos::Security::CAccessController;
using Elastos::Security::CAccessControlContext;
using Elastos::Security::IAccessController;
using Elastos::Security::IProtectionDomain;
using Elastos::Utility::EIID_IIterator;
using Elastos::Utility::EIID_ISet;
using Elastos::Utility::CLinkedList;
using Elastosx::Security::Auth::CSubjectDomainCombiner;

namespace Elastosx {
namespace Security {
namespace Auth {

const Int64 CSubject::serialVersionUID = -8308522755600156056L;

const Int64 CSubject::SecureSet::serialVersionUID = 7911754171111800359L;

const Int64 serialVersionUID = -8308522755600156056L;

const AutoPtr<IPermission> CSubject::_AS_PRIVILEGED;

const AutoPtr<IPermission> CSubject::_SUBJECT;

const AutoPtr<IPermission> CSubject::_PRINCIPALS;

const AutoPtr<IPermission> CSubject::_PRIVATE_CREDENTIALS;

const AutoPtr<IPermission> CSubject::_PUBLIC_CREDENTIALS;

const AutoPtr<IPermission> CSubject::_READ_ONLY;

const AutoPtr<IPermission> CSubject::_AS = CSubject::InitStatics();

AutoPtr<IPermission> CSubject::InitStatics()
{
    AutoPtr<CAuthPermission> obj;
    CAuthPermission::NewByFriend(String("doAs"), (CAuthPermission**)&obj);
    AutoPtr<IPermission> as = (IPermission*)obj.Get();

    obj = NULL;
    CAuthPermission::NewByFriend(String("doAsPrivileged"), (CAuthPermission**)&obj);
    _AS_PRIVILEGED = (IPermission*)obj.Get();

    obj = NULL;
    CAuthPermission::NewByFriend(String("getSubject"), (CAuthPermission**)&obj);
    _SUBJECT = (IPermission*)obj.Get();

    obj = NULL;
    CAuthPermission::NewByFriend(String("modifyPrincipals"), (CAuthPermission**)&obj);
    _PRINCIPALS = (IPermission*)obj.Get();

    obj = NULL;
    CAuthPermission::NewByFriend(String("modifyPrivateCredentials"), (CAuthPermission**)&obj);
    _PRIVATE_CREDENTIALS = (IPermission*)obj.Get();

    obj = NULL;
    CAuthPermission::NewByFriend(String("modifyPublicCredentials"), (CAuthPermission**)&obj);
    _PUBLIC_CREDENTIALS = (IPermission*)obj.Get();

    obj = NULL;
    CAuthPermission::NewByFriend(String("setReadOnly"), (CAuthPermission**)&obj);
    _READ_ONLY = (IPermission*)obj.Get();

    return as;
}

ECode CSubject::DoAs(
    /* [in] */ ISubject *subject,
    /* [in] */ IPrivilegedAction *action,
    /* [out] */ IInterface **obj)
{
    AutoPtr<IAccessController> ac;
    AutoPtr<IAccessControlContext> acc;
    CAccessController::AcquireSingleton((IAccessController**)&ac);
    ac->GetContext((IAccessControlContext**)&acc);
    return DoAs_PrivilegedAction(subject, action, acc, obj);
}

ECode CSubject::DoAsPrivileged(
    /* [in] */ ISubject *subject,
    /* [in] */ IPrivilegedAction *action,
    /* [in] */ IAccessControlContext *context,
    /* [out] */ IInterface **obj)
{
    if (context == NULL) {
        AutoPtr<ArrayOf<IProtectionDomain*> > context = ArrayOf<IProtectionDomain*>::Alloc(0);
        AutoPtr<IAccessControlContext> acc;
        CAccessControlContext::New(context, (IAccessControlContext**)&acc);
        return DoAs_PrivilegedAction(subject, action, acc, obj);
    }
    return DoAs_PrivilegedAction(subject, action, context, obj);
}

ECode CSubject::DoAsEx(
    /* [in] */ ISubject *subject,
    /* [in] */ IPrivilegedExceptionAction *action,
    /* [out] */ IInterface **obj)
{
    AutoPtr<IAccessController> ac;
    AutoPtr<IAccessControlContext> acc;
    CAccessController::AcquireSingleton((IAccessController**)&ac);
    ac->GetContext((IAccessControlContext**)&acc);
    return DoAs_PrivilegedExceptionAction(subject, action, acc, obj);
}

ECode CSubject::DoAsPrivilegedEx(
    /* [in] */ ISubject *subject,
    /* [in] */ IPrivilegedExceptionAction *action,
    /* [in] */ IAccessControlContext *context,
    /* [out] */ IInterface **obj)
{
    if (context == NULL) {
        AutoPtr<ArrayOf<IProtectionDomain*> > context = ArrayOf<IProtectionDomain*>::Alloc(0);
        AutoPtr<IAccessControlContext> acc;
        CAccessControlContext::New(context, (IAccessControlContext**)&acc);
        return DoAs_PrivilegedExceptionAction(subject, action, acc, obj);
    }
    return DoAs_PrivilegedExceptionAction(subject, action, context, obj);
}

ECode CSubject::GetSubject(
    /* [in] */ IAccessControlContext *context,
    /* [out] */ ISubject **subject)
{
    if (!context) {
        return E_NULL_POINTER_EXCEPTION;
    }
    AutoPtr<IPrivilegedAction> action = new PrivilegedAction(context);
    AutoPtr<IAccessController> ac;
    CAccessController::AcquireSingleton((IAccessController**)&ac);
    AutoPtr<IInterface> cmb;
    ac->DoPrivileged(action, (IInterface**)&cmb);
    AutoPtr<IDomainCombiner> combiner = IDomainCombiner::Probe(cmb);
    if (combiner == NULL || !ISubjectDomainCombiner::Probe(combiner.Get())) {
        return NOERROR;
    }
    return ISubjectDomainCombiner::Probe(combiner.Get())->GetSubject(subject);
}

ECode CSubject::Equals(
    /* [in] */ IInterface *obj,
    /* [out] */ Boolean *isEqual)
{
    VALIDATE_NOT_NULL(isEqual)
    *isEqual = FALSE;

    if (obj == NULL) {
        return NOERROR;
    }

    ISubject* thatObj = (ISubject*)obj->Probe(EIID_ISubject);
    if (thatObj == NULL) {
        return NOERROR;
    }

    if (THIS_PROBE(ISubject) == thatObj) {
        *isEqual = TRUE;
        return NOERROR;
    }

    CSubject* that = (CSubject*)thatObj;
    Boolean equals1, equals2, equals3;
    mPrincipals->Equals(that->mPrincipals, &equals1);
    mPublicCredentials->Equals(that->mPublicCredentials, &equals2);
    mPrivateCredentials->Equals(that->mPrivateCredentials, &equals3);
    if (equals1 && equals2 && equals3) {
        *isEqual = TRUE;
        return NOERROR;
    }
    *isEqual = FALSE;
    return NOERROR;
}

ECode CSubject::GetPrincipals(
    /* [out] */ ISet **principals)
{
    VALIDATE_NOT_NULL(principals)
    *principals = mPrincipals;
    return NOERROR;
}

ECode CSubject::GetPrincipalsEx(
    /* [in] */ IInterface *c,
    /* [out] */ ISet **principals)
{
    ClassID clsId;
    IObject::Probe(c)->GetClassID(&clsId);
    return ((SecureSet*)mPrincipals.Get())->Get(clsId, principals);
}

ECode CSubject::GetPrivateCredentials(
    /* [out] */ ISet **credentials)
{
    VALIDATE_NOT_NULL(credentials)
    *credentials = mPrivateCredentials;
    INTERFACE_ADDREF(*credentials)
    return NOERROR;
}

ECode CSubject::GetPrivateCredentialsEx(
    /* [in] */ const ClassID& id,
    /* [out] */ ISet **credentials)
{
    /*
    if (c == null) {
        throw new NullPointerException("c == null");
    }

    AbstractSet<E> s = new AbstractSet<E>() {
        private LinkedList<E> elements = new LinkedList<E>();

        @Override
        public boolean add(E o) {
            if (!c.isAssignableFrom(o.getClass())) {
                throw new IllegalArgumentException("Invalid type: " + o.getClass());
            }

            if (elements.contains(o)) {
                return false;
            }
            elements.add(o);
            return true;
        }

        @Override
        public Iterator<E> iterator() {
            return elements.iterator();
        }

        @Override
        public boolean retainAll(Collection<?> c) {

            if (c == null) {
                throw new NullPointerException("c == null");
            }
            return super.retainAll(c);
        }

        @Override
        public int size() {
            return elements.size();
        }
    };

    // FIXME must have permissions for requested priv. credentials
    for (SST o : this) {
        if (c.isAssignableFrom(o.getClass())) {
            s.add(c.cast(o));
        }
    }
    return s;
    */
    return E_NOT_IMPLEMENTED;
}

ECode CSubject::GetPublicCredentials(
    /* [out] */ ISet **credentials)
{
    VALIDATE_NOT_NULL(credentials)
    *credentials = mPublicCredentials;
    INTERFACE_ADDREF(*credentials)
    return NOERROR;
}

ECode CSubject::GetPublicCredentialsEx(
    /* [in] */ const ClassID& id,
    /* [out] */ ISet **credentials)
{
    return mPublicCredentials->Get(id, credentials);
}

ECode CSubject::GetHashCode(
    /* [out] */ Int32 *hashCode)
{
    VALIDATE_NOT_NULL(hashCode)
    Int32 hc;
    mPrincipals->GetHashCode(hashCode);
    mPrivateCredentials->GetHashCode(&hc);
    *hashCode += hc;
    mPublicCredentials->GetHashCode(&hc);
    *hashCode += hc;
    return NOERROR;
}

ECode CSubject::SetReadOnly()
{
    mReadOnly = TRUE;
    return NOERROR;
}

ECode CSubject::IsReadOnly(
    /* [out] */ Boolean *isReadOnly)
{
    VALIDATE_NOT_NULL(isReadOnly)
    *isReadOnly = mReadOnly;
    return NOERROR;
}

ECode CSubject::ToString(
    /* [out] */ String *str)
{
    VALIDATE_NOT_NULL(str)
    StringBuilder buf("Subject:\n");
    AutoPtr<IIterator> it;
    mPrincipals->GetIterator((IIterator**)&it);
    Boolean hasNext;
    while(it->HasNext(&hasNext), hasNext) {
        buf.AppendCStr("\tPrincipal: ");
        AutoPtr<IInterface> elem;
        it->Next((IInterface**)&elem);
        buf.AppendObject(elem);
        buf.AppendChar('\n');
    }

    it = NULL;
    mPublicCredentials->GetIterator((IIterator**)&it);
    while(it->HasNext(&hasNext), hasNext) {
        buf.AppendCStr("\tPublic Credential: ");
        AutoPtr<IInterface> elem;
        it->Next((IInterface**)&elem);
        buf.AppendObject(elem);
        buf.AppendChar('\n');
    }

    Int32 offset;
    buf.GetLength(&offset);
    offset -= 1;
    it = NULL;
    mPrivateCredentials->GetIterator((IIterator**)&it);
    while(it->HasNext(&hasNext), hasNext) {
        buf.AppendCStr("\tPrivate Credential: ");
        AutoPtr<IInterface> elem;
        it->Next((IInterface**)&elem);
        buf.AppendObject(elem);
        buf.AppendChar('\n');
    }
    buf.ToString(str);
}

ECode CSubject::constructor()
{
    mPrincipals = new SecureSet(_PRINCIPALS, (Handle32)this);
    mPublicCredentials = new SecureSet(_PUBLIC_CREDENTIALS, (Handle32)this);
    mPrivateCredentials = new SecureSet(_PRIVATE_CREDENTIALS, (Handle32)this);
    mReadOnly = FALSE;
    return NOERROR;
}

ECode CSubject::constructor(
    /* [in] */ Boolean readOnly,
    /* [in] */ ISet *subjPrincipals,
    /* [in] */ ISet *pubCredentials,
    /* [in] */ ISet *privCredentials)
{
    if (subjPrincipals == NULL) {
        //throw new NullPointerException("subjPrincipals == null");
        return E_NULL_POINTER_EXCEPTION;
    }
    else if (pubCredentials == NULL) {
        //throw new NullPointerException("pubCredentials == null");
        return E_NULL_POINTER_EXCEPTION;
    }
    else if (privCredentials == NULL) {
        return E_NULL_POINTER_EXCEPTION;
    }

    mPrincipals = new SecureSet(_PRINCIPALS, subjPrincipals, (Handle32)this);
    mPublicCredentials = new SecureSet(_PUBLIC_CREDENTIALS, pubCredentials, (Handle32)this);
    mPrivateCredentials = new SecureSet(_PRIVATE_CREDENTIALS, privCredentials, (Handle32)this);
    mReadOnly = readOnly;
    return NOERROR;
}

ECode CSubject::DoAs_PrivilegedAction(
    /* [in] */ ISubject *subject,
    /* [in] */ IPrivilegedAction *action,
    /* [in] */ IAccessControlContext * const context,
    /* [out] */ IInterface **ret)
{
    VALIDATE_NOT_NULL(ret)
    AutoPtr<IAccessControlContext> newContext;
    AutoPtr<ISubjectDomainCombiner> combiner;
    if (subject == NULL) {
        // performance optimization
        // if subject is null there is nothing to combine
    }
    else {
        CSubjectDomainCombiner::New(subject, (ISubjectDomainCombiner**)&combiner);
    }

    AutoPtr<IPrivilegedAction> dccAction = new PrivilegedActionEx(context, combiner);
    AutoPtr<IAccessController> ac;
    CAccessController::AcquireSingleton((IAccessController**)&ac);
    AutoPtr<IInterface> priv;
    ac->DoPrivileged(dccAction, (IInterface**)&priv);
    newContext = IAccessControlContext::Probe(priv);
    return ac->DoPrivilegedEx(action, newContext, ret);
}

ECode CSubject::DoAs_PrivilegedExceptionAction(
    /* [in] */ ISubject *subject,
    /* [in] */ IPrivilegedExceptionAction *action,
    /* [in] */ IAccessControlContext * const context,
    /* [out] */ IInterface **ret)
{
    VALIDATE_NOT_NULL(ret)
    AutoPtr<IAccessControlContext> newContext;
    AutoPtr<ISubjectDomainCombiner> combiner;
    if (subject == NULL) {
        // performance optimization
        // if subject is null there is nothing to combine
    }
    else {
        CSubjectDomainCombiner::New(subject, (ISubjectDomainCombiner**)&combiner);
    }

    AutoPtr<IPrivilegedAction> dccAction = new PrivilegedActionEx(context, combiner);
    AutoPtr<IAccessController> ac;
    CAccessController::AcquireSingleton((IAccessController**)&ac);
    AutoPtr<IInterface> priv;
    ac->DoPrivileged(dccAction, (IInterface**)&priv);
    newContext = IAccessControlContext::Probe(priv);
    return ac->DoPrivilegedEx3(action, newContext, ret);
}

ECode CSubject::CheckState()
{
    if (mReadOnly) {
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    return NOERROR;
}

CAR_INTERFACE_IMPL(CSubject::SecureSet, ISet)

/*
 * verifies specified element, checks set state, and security permission
 * to modify set before adding new element
 */
ECode CSubject::SecureSet::Add(
    /* [in] */ IInterface *o,
    /* [out] */ Boolean *ret)
{
    VALIDATE_NOT_NULL(ret)
    VerifyElement(o);
    ((CSubject*)mHost)->CheckState();
    Boolean isContained;
    if ((mElements->Contains(o, &isContained), !isContained)) {
        mElements->Add(o, &isContained);
        *ret = TRUE;
        return NOERROR;
    }
    *ret = FALSE;
    return NOERROR;
}

ECode CSubject::SecureSet::AddAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* modified)
{
    return AbstractSet::AddAll(collection, modified);
}

ECode CSubject::SecureSet::Clear()
{
    return AbstractSet::Clear();
}

ECode CSubject::SecureSet::Contains(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* result)
{
    return AbstractSet::Contains(object, result);
}

ECode CSubject::SecureSet::ContainsAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* result)
{
    return AbstractSet::ContainsAll(collection, result);
}

ECode CSubject::SecureSet::Equals(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* value)
{
    return AbstractSet::Equals(object, value);
}

ECode CSubject::SecureSet::GetHashCode(
    /* [out] */ Int32* value)
{
    return AbstractSet::GetHashCode(value);
}

ECode CSubject::SecureSet::IsEmpty(
    /* [out] */ Boolean* result)
{
    return AbstractSet::IsEmpty(result);
}

ECode CSubject::SecureSet::Remove(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* result)
{
    return AbstractSet::Remove(object, result);
}

ECode CSubject::SecureSet::RemoveAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* modified)
{
    return AbstractSet::RemoveAll(collection, modified);
}

ECode CSubject::SecureSet::GetIterator(
    /* [out] */ IIterator **it)
{
    if (mPermission == _PRIVATE_CREDENTIALS) {
        /*
        * private credential set requires iterator with additional
        * security check (PrivateCredentialPermission)
        */
        AutoPtr<IIterator> iter;
        mElements->GetIterator((IIterator**)&iter);
        *it = new SecureIteratorEx(iter, mHost);
        INTERFACE_ADDREF(*it)
        return NOERROR;
    }
    AutoPtr<IIterator> iter;
    mElements->GetIterator((IIterator**)&iter);
    *it = new SecureIterator(iter, mHost);
    INTERFACE_ADDREF(*it)
    return NOERROR;
}

ECode CSubject::SecureSet::RetainAll(
    /* [in] */ ICollection *c,
    /* [out] */ Boolean *ret)
{
    VALIDATE_NOT_NULL(ret)
    if (c == NULL) {
        return E_NULL_POINTER_EXCEPTION;
    }
    return AbstractSet::RetainAll(c, ret);
}

ECode CSubject::SecureSet::GetSize(
    /* [out] */ Int32 *size)
{
    return mElements->GetSize(size);
}

ECode CSubject::SecureSet::ToArray(
    /* [out, callee] */ ArrayOf<IInterface*>** array)
{
    return AbstractSet::ToArray(array);
}

ECode CSubject::SecureSet::ToArrayEx(
    /* [in] */ ArrayOf<IInterface*>* contents,
    /* [out, callee] */ ArrayOf<IInterface*>** outArray)
{
    return AbstractSet::ToArrayEx(contents, outArray);
}

CSubject::SecureSet::SecureSet(
    /* [in] */ IPermission *perm,
    /* [in] */ Handle32 host)
    : mPermission(perm)
    , mHost(host)
{
    CLinkedList::New((ILinkedList**)&mElements);
}
CSubject::SecureSet::SecureSet(
    /* [in] */ IPermission *perm,
    /* [in] */ ICollection *s,
    /* [in] */ Handle32 host)
    : mPermission(perm)
    , mHost(host)
{
    /*
    // Subject's constructor receives a Set, we can trusts if a set is from bootclasspath,
    // and not to check whether it contains duplicates or not
    boolean trust = s.getClass().getClassLoader() == null;

    for (SST o : s) {
        verifyElement(o);
        if (trust || !elements.contains(o)) {
            elements.add(o);
        }
    }
    */
}

ECode CSubject::SecureSet::Get(
    /* [in] */ const ClassID& id,
    /* [out] */ ISet **obj)
{
    /*
        if (c == null) {
            throw new NullPointerException("c == null");
        }

        AbstractSet<E> s = new AbstractSet<E>() {
            private LinkedList<E> elements = new LinkedList<E>();

            @Override
            public boolean add(E o) {
                if (!c.isAssignableFrom(o.getClass())) {
                    throw new IllegalArgumentException("Invalid type: " + o.getClass());
                }

                if (elements.contains(o)) {
                    return false;
                }
                elements.add(o);
                return true;
            }

            @Override
            public Iterator<E> iterator() {
                return elements.iterator();
            }

            @Override
            public boolean retainAll(Collection<?> c) {

                if (c == null) {
                    throw new NullPointerException("c == null");
                }
                return super.retainAll(c);
            }

            @Override
            public int size() {
                return elements.size();
            }
        };

        // FIXME must have permissions for requested priv. credentials
        for (SST o : this) {
            if (c.isAssignableFrom(o.getClass())) {
                s.add(c.cast(o));
            }
        }
        return s;
    */
    return E_NOT_IMPLEMENTED;
}

ECode CSubject::SecureSet::VerifyElement(
    /* [in] */ IInterface *o)
{
    if (o == NULL) {
        return E_NULL_POINTER_EXCEPTION;
    }
    /*
    if (permission == _PRINCIPALS && !(Principal.class.isAssignableFrom(o.getClass()))) {
        throw new IllegalArgumentException("Element is not instance of java.security.Principal");
    }
    */
    return E_NOT_IMPLEMENTED;
}

ECode CSubject::SecureSet::ReadObject(
    /* [in] */ IObjectInputStream *is)
{
    is->DefaultReadObject();
    switch(mSetType) {
        case SET_Principal:
            mPermission = _PRINCIPALS;
            break;
        case SET_PrivCred:
            mPermission = _PRIVATE_CREDENTIALS;
            break;
        case SET_PubCred:
            mPermission = _PUBLIC_CREDENTIALS;
            break;
        default:
            return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    AutoPtr<IIterator> it;
    mElements->GetIterator((IIterator**)&it);
    for(Boolean hasNext = FALSE; (it->HasNext(&hasNext), hasNext);) {
        AutoPtr<IInterface> elem;
        it->Next((IInterface**)&elem);
        VerifyElement(elem);
    }
    return NOERROR;
}

ECode CSubject::SecureSet::WriteObject(
    /* [in] */ IObjectOutputStream *os)
{
    if (mPermission == _PRIVATE_CREDENTIALS) {
        mSetType = SET_PrivCred;
    } else if (mPermission == _PRINCIPALS) {
        mSetType = SET_Principal;
    } else {
        mSetType = SET_PubCred;
    }

    return os->DefaultWriteObject();
}

CAR_INTERFACE_IMPL(CSubject::SecureSet::SecureIterator, IIterator)

ECode CSubject::SecureSet::SecureIterator::HasNext(
    /* [out] */ Boolean *hasNext)
{
    return mIterator->HasNext(hasNext);
}

ECode CSubject::SecureSet::SecureIterator::Next(
    /* [out] */ IInterface **next)
{
    return mIterator->Next(next);
}

/**
 * checks set state, and security permission to modify set before
 * removing current element
 */
ECode CSubject::SecureSet::SecureIterator::Remove()
{
    ((CSubject*)mHost)->CheckState();
    return mIterator->Remove();
}

CSubject::SecureSet::SecureIterator::SecureIterator(
    /* [in] */ IIterator *iterator,
    /* [in] */ Handle32 host)
    : mIterator(iterator)
    , mHost(host)
{}

}
}
}

