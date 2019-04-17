# 如何获取最新版本
这里早已不再更新，请到 http://giveda.com/ 下载最新开源代码。

# 授权协议
[![LICENSE](https://img.shields.io/badge/license-Anti%20996-blue.svg)](https://github.com/996icu/996.ICU/blob/master/LICENSE)   
作为用户的您需要认真阅读如下软件授权协议，知情您的权利与义务：  
*) 如果您需要将“Giveda®信号槽”用于个人私下的研究学习用途，您需要事先接受、并全程遵守AGPL v3许可证协议，否则将为自己带去信用与法律风险。  
*) 如果您需要将“Giveda®信号槽”用于任何商业用途，您需要事先从深圳技达®获得商业应用许可，否则将为自己带去信用与法律风险。  
*) 除非您主动联系giveda.com、并声明需要商业许可，否则意味着您接受并全程遵守AGPL v3协议。  
*)  **商业许可实行按年对公司收费。主动联系我们并付费的价格为3万CNY/年。被人举报的被动付费价格为30万CNY/年。**     
*)  **举报者可获得不低于成交额30%的现金奖励。**       
*) 收款账户：000235602055；户名：深圳市技达信息技术有限公司；收款银行：深圳农村商业银行。  
只有完全同意以上协议，您才可以使用本软件。
如果您不同意以上协议，不要使用本软件。

# 简介和概述  
为了解决Qt信号槽（需要依赖moc机制和moc工具）的缺陷，我创造了本软件。  
利用本软件，c++开发者可以在不依赖c++编译器之外的任何外部机制和工具的前提下，实现c++对象的解耦合（信号发射对象与信号接收对象之间的解耦合）。  
本软件支持c++98并向后兼容（支持c++98及其后续版本）。  
开发环境为ubuntu 14.04操作系统、c++编译器、STL等。  
本软件的目标用户是c++软件开发者。本软件为开发者提供了如下模块和功能：  
*) GObject类；信号发射者、信号接收者均需要继承此GObject类。  
*) DEFINE_SIGNAL/EMIT_SIGNAL等宏定义；信号发射者用此来定义信号、发射信号；  
*) GObject::connect函数；开发者使用本函数，将发射者的信号连接到接收者的槽函数。连接成功后，发射者发射信号，接收者的槽函数将会被调用。  
*) GObject::disconnect函数；开发者使用本函数，将发射者的信号与接收者的槽函数断开连接。断开连接后，发射者发射信号时，接收者的槽函数将不会被调用。  

开发者可以将本软件放到开发环境中去编译和运行，并配合《用户文档.doc》来学习本软件的使用方法。 

 **欢迎给作者捐赠，作者的联系电话：18676768949（可加微信）**  
  **捐赠者可获得技术支持。**    
