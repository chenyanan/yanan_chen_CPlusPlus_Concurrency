//
//  8_0.cpp
//  123
//
//  Created by chenyanan on 2017/5/15.
//  Copyright © 2017年 chenyanan. All rights reserved.
//

#include <iostream>
#include <thread>
#include <mutex>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-function"

//第8章 设计并发代码

//本章主要内容

//在线程间划分数据的技术
//影响并发代码性能的因素
//性能因素如何影响数据结构的设计
//多线程代码中的异常安全
//可扩展性
//几个并行算法实现的示例

//前面的章节主要是讨论新的C++11工具箱里用来写并行代码的工具，在第6章和第7章中，我们观察了如何使用这些工具来设计多个线程可以并发存取的安全的基础数据结构，作为木匠，为了制作柜橱或者桌子，不仅仅需要知道如何铰链或者接缝处，同样，需要设计并行代码而不仅仅是设计和使用基础数据结构，你需要了解更广泛的背景，这样就可以构造进行有用的工作的更大的结构，我将使用一些C++标准库算法的多线程实现作为例子，但是同样的原则适用于应用的所有方面

//正如所有编程项目一样，仔细考虑并行代码是很重要的，尽管如此，使用多线程代码比使用顺序代码需要考虑更多的因素，你不仅苏姚考虑常见的因素，例如封装，耦合和内聚(在很多软件设计书中有详细的描述)，而且需要考虑共享哪些数据，如何同步那些数据的存取，那个线程需要等待哪些别的线程完成特定操作，等等

//本章，我们将致力于这些问题，从高层次考虑使用多少个线程，各个线程执行哪些代码，以及这些是如何影响代码的透明度，到低层次考虑如何构造共享数据来获得最佳性能

//让我们从在线程间划分工作的技术开始

#pragma clang diagnostic pop
