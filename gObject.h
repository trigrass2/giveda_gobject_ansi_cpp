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

#ifndef GOBJECT_H
#define GOBJECT_H

#include <string>
#include <list>

using namespace std;

class GObjectPrivate;
class GObject;

#define slots

#define SIGNAL_TYPE(SlotFuncType)  list<Slot<SlotFuncType> >
#define SIGNAL_POINTER(SlotFuncType)  SIGNAL_TYPE(SlotFuncType)*
#define SIGNAL_TYPE_ITERATOR(SlotFuncType)  list<Slot<SlotFuncType> >::iterator

typedef void (*T_pnrv)( GObject* );
typedef void (*T_pirv)( GObject*p, int );

/**
 * @def 定义一个函数类型名称为SlotFuncTypeName，函数名称为signalName的信号。
 *
 * SlotFuncTypeName 信号函数的类型名称，比如T_pnrv
 * signalName 信号名称
 */
#define DEFINE_SIGNAL(SlotFuncTypeName, signalName) \
public: SIGNAL_POINTER(SlotFuncTypeName) signalName() { return &m_##signalName; } \
private: SIGNAL_TYPE(SlotFuncTypeName) m_##signalName;

/**
 * @def 定义一个函数类型定义为SlotFuncType，函数类型名称为SlotFuncTypeName，函数名称为signalName的信号。
 *
 * SlotFuncType  信号函数的类型定义
 * SlotFuncTypeName 信号函数的类型名称，比如T_pnrv
 * signalName 信号名称
 */
#define DEFINE_SIGNAL_EX(SlotFuncType, SlotFuncTypeName, signalName) \
public: typedef SlotFuncType; \
public: SIGNAL_POINTER(SlotFuncTypeName) signalName() { return &m_##signalName; } \
private: SIGNAL_TYPE(SlotFuncTypeName) m_##signalName;

/**
 * @def 发射一个函数类型名称为SlotFuncTypeName，函数名称为signalName的信号。可以传递参数给该信号。
 *
 * SlotFuncTypeName 信号函数的类型名称，比如T_pnrv
 * signalName 信号名称
 * ...  代表要传递给该信号的参数
 */
#define EMIT_SIGNAL_EX(SlotFuncTypeName, signalName, ...)   \
{ \
    for(SIGNAL_TYPE(SlotFuncTypeName)::iterator it = m_##signalName.begin(); it != m_##signalName.end(); ++it) \
    { \
        (*(it->m_slot))(it->m_receiver, __VA_ARGS__); \
    } \
}

/**
 * @def 发射一个函数类型名称为SlotFuncTypeName，函数名称为signalName的信号。不可以传递参数给该信号。
 *
 * SlotFuncTypeName 信号函数的类型名称，比如T_pnrv
 * signalName 信号名称
 * ...  代表要传递给该信号的参数
 */
#define EMIT_SIGNAL(SlotFuncTypeName, signalName) \
{ \
    for(SIGNAL_TYPE(SlotFuncTypeName)::iterator it = m_##signalName.begin(); it != m_##signalName.end(); ++it) \
    { \
        (*(it->m_slot))(it->m_receiver); \
    } \
}

template<class SlotFuncType>
struct Slot
{
    Slot( GObject* receiver, SlotFuncType slot)
    :m_receiver(receiver),
    m_slot(slot)
    {
    }
    
    bool operator==(const Slot& other)
    {
        return other.m_receiver == m_receiver && other.m_slot == m_slot;
    }

    GObject* m_receiver;
    SlotFuncType m_slot;
};


class  GObject
{    
private:
    GObjectPrivate *m_priv;

public:
    explicit GObject ( GObject *parent=NULL,  const char *name=NULL );
    GObject( const GObject & src);
    GObject & operator=(const GObject & src);
    virtual ~GObject();

	/**
	 * @brief 将信号连接到槽。\n
	 * SlotFuncType是槽函数的函数类型名称。
	 *
	 * @param sender 指向发射者的指针
	 * @param signal 指向信号的指针。
	 * @param receiver 指向接收者的指针
	 * @param slot 指向槽函数的指针
	 *
	 * @return 0代表成功；非0代表失败
	 */
    template<class SlotFuncType>
    static int  connect(GObject* sender, SIGNAL_POINTER(SlotFuncType) signal, GObject* receiver, SlotFuncType slot);

    /**
	 * @brief 将信号和槽断开连接。\n
	 * SlotFuncType是槽函数的函数类型名称。
	 *
	 * @param sender 指向发射者的指针
	 * @param signal 指向信号的指针。
	 * @param receiver 指向接收者的指针
	 * @param slot 指向槽函数的指针
	 *
	 * @return 0代表成功；非0代表失败
	 */
    template<class SlotFuncType>
    static int  disconnect(GObject* sender, SIGNAL_POINTER(SlotFuncType) signal, GObject* receiver, SlotFuncType slot);

    const char *name() const;
    GObject *parent() const;
    

private:
    static int  privConnect(GObject* sender, SIGNAL_POINTER(void*) signal, GObject* receiver, void* slot);
    static int  privDisconnect(GObject* sender, SIGNAL_POINTER(void*) signal, GObject* receiver, void* slot);
    void saveSenderPair(GObject* sender, SIGNAL_POINTER(void*) signal);
    void deleteSenderPair(GObject* sender, SIGNAL_POINTER(void*) signal);
    void destructAsReceiver();
    void destructAsSender();
    void saveReceiver(GObject* receiver);
    void deleteReceiver(GObject* receiver);
};

template<class SlotFuncType>
int   GObject::connect(GObject* sender, SIGNAL_POINTER(SlotFuncType) signal, GObject* receiver, SlotFuncType slot)
{
    int ret = privConnect(sender, reinterpret_cast<SIGNAL_POINTER(void*)>(signal), receiver, (void*)slot);
    return ret;
}

template<class SlotFuncType>
int   GObject::disconnect(GObject* sender, SIGNAL_POINTER(SlotFuncType) signal, GObject* receiver, SlotFuncType slot)
{
    int ret = privDisconnect(sender, reinterpret_cast<SIGNAL_POINTER(void*)>(signal), receiver, (void*)slot);
    return ret;
}

#endif // GOBJECT_H
