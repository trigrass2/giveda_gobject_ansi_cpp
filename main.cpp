/*
 * Copyright (C) 2019  明心  <imleizhang@qq.com>
 * All rights reserved.
 *
 * This program is an open-source software; and it is distributed in the hope
 * that it will be useful, but WITHOUT ANY WARRANTY; without even the
 * implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 * PURPOSE.
 * This program is not a free software; so you can not redistribute it(include
 * binary form and source code form) without my authorization. And if you
 * need use it for any commercial purpose, you should first get commercial
 * authorization from me, otherwise there will be your's legal&credit risks.
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
 * 通常情况下，一个文件应当只包含一个类，应当由一个名为sender.cpp/sender.h的文件来放置Sender类。
 * 请看main函数中的测试代码，测试代码解释了为什么Sender需要继承于GObject。
 * 
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
 * 通常情况下，一个文件应当只包含一个类，应当由一个名为receiver.cpp/receiver.h的文件来放置Receiver类。
 * 请看main函数中的测试代码，测试代码解释了为什么Receiver需要继承于GObject。
 * 
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

/**
 * @brief 通常情况下，main函数应当放置在一个单独文件比如名为main.cpp。
 * main.cpp需要include(依赖)Sender.h和Receiver.h。
 * sender.cpp/sender.h不需要include(依赖)receiver.h。
 * receiver.cpp/receiver.h不需要include(依赖)Sender.h。
 */
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
    
    //请看如下这段代码，这里解释了为什么Sender和Receiver需要继承于GObject。
    //Receiver对象被销毁后，Sender对象与Receiver对象之间的依赖会自动被解除。
    //事实上，如果你根本不在意Receiver被销毁后的情形，那么你当然可以不用继承于GObject。
    GObject::connect(s, s->onClick(), r, Receiver::slotOnClick);
    GObject::connect(s, s->notify(), r, Receiver::slotOnNotify);
    delete r;
    printf("connect again, and delete receiver\n");
    s->sendMsg();
    
    delete s;
    
    return 0;
}
