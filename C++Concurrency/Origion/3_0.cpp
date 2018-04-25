//
//  3_0.cpp
//  123
//
//  Created by chenyanan on 2017/4/20.
//  Copyright © 2017年 chenyanan. All rights reserved.
//

#include <iostream>
#include <thread>
#include "assert.h"

//第3章 线程间共享数据

//本章主要内容

//共享数据带来的问题
//使用互斥量保护数据
//数据保护的替代方案

//上一章中，我们已经对线程管理有所了解了，现在让我们来看一下"共享数据的那些事"

//想象一下，你和你的朋友合租一个公寓，公寓中只有一个厨房和一个卫生间，当你的朋友在卫生间时，你就会不能使用了(除非你们特别好，好到可以在同时使用一个房间)，这个问题也会出现在厨房，假如，厨房里有一个组合式烤箱，当在烤香肠的时候，也在做蛋糕，就可能得到我们不想要的食物(香肠味的蛋糕)，此外，在公共空间做一件事做到一半时，发现某些需要的东西被别人借走，或是当离开的一段时间内有些东西被变动了地方，这都会令我们不爽

//同样的问题，也困扰着线程，当线程在访问共享数据的时候，必须定一些规矩，用来限定线程可访问的数据位，还有，一个线程更新了共享数据，需要对其他线程进行通知，从易用性的角度，同一进程中的多个线程进行数据共享，有利有弊，错误的共享数据使用是产生并发bug的一个主要原因，并且后果要比香肠味的蛋糕更加严重

//本章就以在C++中进行安全的数据共享为主题，避免上述及其他潜在的问题的发生的同时，将共享数据的有时发挥到最大

//3.1 共享数据带来的问题

//当涉及到共享数据时，问题很可能是因为共享数据修改所导致，如果共享数据是只读的，那么只读操作不会影响到数据，更不会涉及对数据的修改，所以所有线程都会获得同样的数据，但是，当一个或多个线程要修改共享数据时，就会产生很多麻烦，这种情况下，就必须小心谨慎，才能确保一切所有线程都能工作正常

//不变量(invariants)的概念对程序员们编写的程序会有一定的帮助，对于特殊结构体的描述，比如，"变量包含列表中的项数"，不变量通常会在一次更新中被破坏，特别是比较复杂的数据结构，或者一次更细你就要搞懂很大的数据结构

//双链表中每个节点都有一个指针指向列表中的下一个节点，还有一个指针指向前一个节点，其中不变量就是节点A中指向"下一个"节点B的指针，还有前向指针，为了从列表中删除一个节点，其两边节点的的指针都需要更新，当其中一个更新完成时，不变量就被破坏了，知道另一个边也完成更新，在两边都完成更新后，不变量就又稳定了

//从一个列表中删除一个及节点的步骤如下(如图3.1)

//1. 找到要删除的节点N
//2. 更新前一个节点指向N的指针，让这个指针指向N的下一个节点
//3. 更新后一个节点指向N的指针，让这个指针指向N的前一个及诶单
//4. 删除节点N

//图3.1 从一个双链表中删除一个节点

//图中b和c在相同的方向上指向和原来已经不一致了，这就破坏了不变量

//线程间潜在问题就是修改共享数据，致使不变量遭到破坏，当不做些事来确保在这个过程中不会有其他线程进行访问的话，可能就有线程访问到刚刚删除一遍的节点，这样的话，线程就读取到要删除节点的数据(因为只有一边的连接被修改，如图3.1(b))，所以不变量就被破坏，破坏不变量的后果是多样的，当其他线程按从左往右的顺序来访问列表时，它将跳过被删除的节点，在一方面，如有第二个线程阐释删除图中右边的节点，那么可能会让数据结构产生永久性的损坏，是程序崩溃，无论结果如何，都是并行代码常见错误:条件竞争(race condition)

//3.1.1 条件竞争

//假设你去电影院买电影票，如果去的是一家大电影院，有很多收银台，很多人就可以在同一事件买电影票，当另一个收银台也在卖你想看的这场电影的电影票，那么你的座位选择范围就取决于在之前已预订的座位，当只有少量的座位剩下，这就意味着，这可能是一场抢票比赛，看谁能抢到最后一张票，这就是一个条件竞争的例子:你的座位(或者你的电影票)都取决于两种购买方式的相对顺序

//并发中竞争条件的形成，取决于一个以上线程的相对执行顺序，每个线程都抢着完成自己的任务，大多数情况下，即使改变执行顺序，也是良性竞争，其结果可以接受，例如，有两个线程同时向一个处理队列中添加任务，因为系统提供的不变量保持不变，所以谁先谁后都不会有什么影响，当不变量遭到破坏时，才会产生条件竞争，比如双向链表的例子，并发中对数据的条件竞争通常表示为"恶性"(problematic)条件竞争，我们队不产生问题的良性条件竞争不感兴趣，C++标准中也定义了数据竞争(data race)这个术语，一种特殊的条件竞争:并发的去修改一个独立对象(参见5.1.2节)，数据竞争是(可怕的)未定义行为(undefine behavior)的起因

//恶性条件竞争通常发生于完成对多于一个的数据块的修改时，例如，对两个链接指针的修改(如图3.1)，因为操作要访问两个独立的数据块，独立的指令将会对数据块进行修改，并且其中一个线程可能正在进行时，另一个线程就对数据块进行了访问，因为出现的概率太低，条件竞争很难查找，也很难浮现，如CPU指令连续修改完成后，即使数据结构可以让其他并发线程访问，问题再次复现的概率也在增加，这样的问题只可能会出现在负载比较大的情况下，条件竞争通常是时间敏感的，所以程序以调试模式运行时，他们常会完全消失，因而调试模式会影响程序的执行时间(即使影响不多)

//当你以写多线程程序为主，条件竞争就会成为你的梦魇，编写软件时，我们会使用大量复杂的操作，用来避免恶性条件竞争

//3.1.2 避免恶性条件竞争

//这里提供一些方法来解决恶性条件竞争，最简单的办法就是对数据结构采用某种保护机制，确保只有进行修改的线程才能看到不变量被破坏时的中间状态，从其他访问线程的角度来看，修改不是已经完成了，就是还没开始，C++标准库提供很多类似的机制，下面会逐一介绍

//另一个选择是对数据结构和不变量的设计进行修改，修改完的结构必须能完成一系列不可分割的变化，也就是保证每个不变量保持稳定的状态，这就是所谓的无锁编程(lock-free-programming)，不过，这种方式很难得到正确的结果，如果到这个级别，无论是内存模上的细微差异，还是线程访问数据的能力，都会让工作变得复杂，内存模型将在第5章讨论，无锁编程将在第7章讨论

//另一种处理条件竞争的方式是，使用事务(transacting)的方式去处理数据结构的更新(这里的"处理"就如同对数据库进行更新一样)，所需的一些数据和读取都存储在事务日志中，然后将之前的操作合为异步，再进行提交，当数据结构被另一个线程修改后，或处理已经重启的情况下，提交就会无法进行，这称为"软件事务内存"(software transactional memory)(STM)，研究理论中，这是一个很热门的研究领域，这个概念将不会在本书中再进行介绍，因为在C++中没有对STM进行直接支持，但是，基本思想会在后面提及

//保护共享数据结构的最基本的方式，是使用C++标准库提供的互斥量(mutex)

//3.2 使用互斥量保护共享数据

//当程序中有共享数据，肯定不想让其陷入条件竞争，或是不变量被破坏，那么，将所有访问共享数据结构的代码都标记为互斥岂不是更好？这样任何一个线程在执行这些代码时，其他任何线程试图访问共享数据结构，就必须等待那一段代码执行结束，于是，一个线程就不可能会看到被破坏的不变量，除非它本身就是修改共享数据的线程

//当访问共享数据前，使用互斥量将相关数据锁住，当访问结束后，再将数据解锁，线程库需要保证，当一个线程使用特定互斥量锁住共享数据时，其他的线程想要访问锁住的数据，都必须等到之前那个线程对数据进行解锁后，才能进行访问，这就保证了所有线程能看到共享数据，而不破坏不变量

//互斥量是C++中一种最通用的数据保护机制，但它不是"银弹"，精心组织的代码来保护正确的数据(见3.2.2节)，并在接口内部避免竞争条件(见3.2.3节)是非常虫咬的，但互斥量自身也有问题，也会造成死锁(见3.2.4节)，或是对数据保护的太多(或太少)(见3.2.8节)

//3.2.1 C++中使用互斥量

//C++中通过实例化std::mutex创建互斥量，通过调用成员函数lock()进行上锁，unlock()进行解锁，不过，不推荐实践中直接去调用成员函数，因为调用成员函数就意味着，必须记住在每个函数出口都要去调用unlock()，也包括异常的情况，C++标准库为互斥量提供了一个RAII语法的模板类std::lock_guard，其会在构造的时候提供已锁的互斥量，并在析构函数的时候进行解锁，从而保证了一个已锁的互斥量总是会被正确的解锁，下面的程序清单中，展示了如何在多线程程序中，使用std::mutex构造的std::lock_guard实例，对一个列表进行访问保护，std::mutex和std::lock_guard都在<mutex>头文件中声明
