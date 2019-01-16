/*
 * Copyright (C) 2019  明心  <imleizhang@qq.com>
 * All rights reserved.
 * 
 * This program is an open-source software; and it is distributed in the hope 
 * that it will be useful, but WITHOUT ANY WARRANTY; without even the
 * implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
 * PURPOSE. 
 * This program is not a free software; so you can not redistribute it and/or 
 * modify it without my authorization. If you only need use it for personal
 * study purpose(no redistribution, and without any  commercial behavior), 
 * you should accept and follow the GNU AGPL v3 license, otherwise there
 * will be your's credit and legal risks.  And if you need use it for any 
 * commercial purpose, you should first get commercial authorization from
 * me, otherwise there will be your's credit and legal risks. 
 *
 */

#include <gObject.h>
#include <string>
#include <algorithm>
#include <stdio.h>

struct SenderPair
{
    SenderPair(GObject* _sender, SIGNAL_POINTER(void*) _signal )
        :sender(_sender), signal(_signal)
    { }

    bool operator==(const SenderPair &r ) const
    {
        return r.sender == sender && r.signal == signal;
    }

    GObject* sender;
    SIGNAL_POINTER(void*) signal;
};

class GObjectPrivate
{
public:
    GObjectPrivate ( GObject *p, const char* nm ) :m_parent ( p ), strName ( nm ) 
    {
        spLstInReceiver.clear();
        receiverLstInSender.clear();
    }

    ~GObjectPrivate()
    {
        spLstInReceiver.clear();
        receiverLstInSender.clear();
    }

    GObject *m_parent;
    string   strName;
    list<SenderPair>  spLstInReceiver;
    list<GObject*>  receiverLstInSender;
};

#define WARNING printf

GObject::GObject ( GObject *p,  const char *n )
    :m_priv ( new GObjectPrivate ( p, NULL==n?"":n ) )
{
}

GObject::~GObject()
{
    destructAsReceiver();

	EMIT_SIGNAL(T_pnrv, sigDestroyed);
    destructAsSender();
    
    delete m_priv;
}

void GObject::saveSenderPair(GObject* sender, SIGNAL_POINTER(void*) signal)
{
    SenderPair sp(sender, signal);
    m_priv->spLstInReceiver.push_back(sp);
}

void GObject::deleteSenderPair(GObject* sender, SIGNAL_POINTER(void*) signal)
{
    m_priv->spLstInReceiver.remove( SenderPair(sender, signal) );
}

class Receiver_Is
{
public:
    GObject* receiver;

    bool operator( ) ( Slot<void*> &obj1 )
    {
        return obj1.m_receiver == this->receiver;
    }

    bool operator( ) ( GObject* receiver )
    {
        return this->receiver == receiver;
    }

    Receiver_Is(GObject* r)
        :receiver(r)
    {

    }
};

int GObject::privConnect(GObject* sender, SIGNAL_POINTER(void*) signal, GObject* receiver, void* slot)
{
    if ( sender == 0 || receiver == 0 || signal == 0 || slot == 0 )
    {
        WARNING ( "can not connect %s::%p to %s::%p\n",
                  sender ? sender->name() : "(null)",
                  signal,
                  receiver ? receiver->name() : "(null)",
                  slot );
        return -1;
    }

    Slot<void*>  vslot(receiver, (void*)slot);
    SIGNAL_TYPE_ITERATOR(void*)  it = std::find(signal->begin(), signal->end(), vslot );
    if(it != signal->end() )
    {
        return 0;
    }

    signal->push_back( vslot );

    sender->saveReceiver(receiver);
    
    receiver->saveSenderPair( sender, signal );

    return 0;
}

void GObject::destructAsReceiver()
{
    list<SenderPair >::iterator it = m_priv->spLstInReceiver.begin();
    while(it != m_priv->spLstInReceiver.end() )
    {
        it->signal->remove_if( Receiver_Is(this) );
        it->sender->m_priv->receiverLstInSender.remove_if( Receiver_Is(this) );
        ++it;
    }
}

class Sender_Is
{
public:
    GObject* sender;

    bool operator( ) ( SenderPair &obj1 )
    {
        return obj1.sender == sender;
    }

    Sender_Is(GObject* s)
        :sender(s)
    {

    }
};

void GObject::destructAsSender()
{
    list<GObject*>::iterator it = m_priv->receiverLstInSender.begin();
    while(it != m_priv->receiverLstInSender.end() )
    {
        GObject* receiver = *it;
        receiver->m_priv->spLstInReceiver.remove_if( Sender_Is(this) );
        ++it;
    }
}

void GObject::saveReceiver(GObject *receiver)
{
    m_priv->receiverLstInSender.push_back( receiver );
}

void GObject::deleteReceiver(GObject *receiver)
{
    list<GObject*>::iterator it = std::find(m_priv->receiverLstInSender.begin(), m_priv->receiverLstInSender.end(), receiver );
    if(it == m_priv->receiverLstInSender.end() )
    {
        return ;
    }

    m_priv->receiverLstInSender.erase(it);
}

int GObject::privDisconnect(GObject* sender, SIGNAL_POINTER(void*) signal, GObject* receiver, void* slot)
{
    if ( sender == 0 || receiver == 0 || signal == 0 || slot == 0 )
    {
        WARNING ( "can not disconnect sender %s::%p from receiver %s::%p\n",
                  sender ? sender->name() : "(null)",
                  signal,
                  receiver ? receiver->name() : "(null)",
                  slot );
        return -1;
    }
    
    Slot<void*>  vslot(receiver, (void*)slot);
    SIGNAL_TYPE_ITERATOR(void*)  it = std::find(signal->begin(), signal->end(), vslot );
    if(it == signal->end() )
    {
        return 0;
    }

    signal->remove( vslot );

    sender->deleteReceiver(receiver);
    
    receiver->deleteSenderPair( sender, signal );

    return 0;
}


const char* GObject::name() const
{
    return  m_priv->strName.c_str();
}

GObject* GObject::parent() const
{
    return m_priv->m_parent;
}

GObject& GObject::operator= ( const GObject& /*src*/ )
{
    return *this;
}

GObject::GObject ( const GObject& /*src*/ )
    :m_priv ( new GObjectPrivate ( NULL, "" ) )
{
}
