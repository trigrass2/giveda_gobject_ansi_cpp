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
#include <stdio.h>

/**
 * @mainpage 基于ANSI c++实现的信号和槽
 * @author 明心
 * @version 1.0.0
 * @date 2018-12-27
 */

/**
 * @class Sender
 * @brief Sender类负责定义信号；并负责在需要时，发射信号。
 */
class Sender : public GObject
{
    /**
     * @brief 定义一个函数类型为T_pnrv，函数名称为onClick的信号。
     *
     * @param T_pnrv 信号函数的类型
     * @param onClick 信号名称
     */
    DEFINE_SIGNAL(T_pnrv, onClick)

    /**
     * @brief 定义一个函数类型为notifyType、函数名称为notify的信号。 
     *
     * @param void (*notifyType)( GObject*p, string ) 完整的函数类型定义
     * @param notifyType 完整函数类型定义中的类型名称
     * @param notify 信号名称
     */
    DEFINE_SIGNAL_EX(void (*notifyType)( GObject*p, string ), notifyType,  notify)
public:
    Sender()
    :GObject()
    {
        
    }
    
    /**
     * @brief 发射onClick信号、和notify信号。
     */
    void sendMsg()
    {
        EMIT_SIGNAL(T_pnrv, onClick);

		string param="giveda.com";
        EMIT_SIGNAL_EX(notifyType, notify, param);
    }
};

/**
 * @class Receiver
 * @brief Receiver类负责接收信号；并进行业务处理。
 */
class Receiver : public GObject
{
public:
    Receiver()
    :GObject()
    {
        
    }
    
public slots:
    /**
     * @brief 用于接收Sender发射的onClick信号的槽函数。
     *
     * @param p 指向Receiver的指针。
     */
    static void slotOnClick(GObject* p)
    {
        Receiver *r = (Receiver*)p;
        r->doSth1();
    }
        
    /**
     * @brief 用于接收Sender发射的notify信号的槽函数。
     *
     * @param p 指向Receiver的指针。
     * @param para Sender发射notify信号时，附带的参数。
     */
    static void slotOnNotify(GObject* p, string para)
    {
        Receiver *r = (Receiver*)p;
        r->doSth2(para);
    }
private:
    void doSth1()
    {
        printf("I've got sender's onClick signal\n");
    }

    void doSth2(string para)
    {
        printf("I've got sender's notify signal, content is [%s]\n", para.c_str() );
    }
};

int main(int argc, char** argv)
{
    Sender* s = new Sender;
    printf("before connect\n");
    s->sendMsg();
    
    Receiver *r = new Receiver;
    GObject::connect(s, s->onClick(), r, Receiver::slotOnClick);
    GObject::connect(s, s->notify(), r, Receiver::slotOnNotify);
    printf("after connect\n");
    s->sendMsg();
    
    GObject::disconnect(s, s->onClick(), r, Receiver::slotOnClick);
    GObject::disconnect(s, s->notify(), r, Receiver::slotOnNotify);
    printf("after disconnect\n");
    s->sendMsg();
    
    GObject::connect(s, s->onClick(), r, Receiver::slotOnClick);
    GObject::connect(s, s->notify(), r, Receiver::slotOnNotify);
    delete r;
    printf("connect again, and delete receiver\n");
    s->sendMsg();
    
    delete s;
    
    return 0;
}
