事件驱动状态机系统
=======================

description：
------------
这是一个简单的事件驱动状态机系统，完全由事件驱动，fsm支持入口事件和出口事件。
支持多个状态机调度，支持定时器，支持电源管理。机械化的编程，非常适合用在单片机上面。

2015-4-5

zrpeng

history:
--------
版本	说明

0.01    2015-6-6 在stm32上面运行正常，支持低功耗__wfe模式,运行正常，仿照CC2540
		的osal层写的，增加了有限状态机跳转操作。增加双向链表操作。
		增加了定时器删除和重复定时器。
		调用的api就这4个，这就足以驱动fsm了
		

