//
//  7_10.cpp
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

//7.4 小结

//紧接着第6章中描述的基于锁的数据结构，这一章描述了多种使用栈或队列的无锁数据结构的简单实现，你必须注意你的原子操作的内存顺序，确保没有数据竞争并且每个线程看到的数据结构是一致的，你也注意到无锁数据结构中的内存管理比基于锁的数据结构中的内存管理变得更难，并且通过一些方法来处理它，你也注意到如何通过帮助你所等待的线程完成它的操作，从而避免创造等待循环

//设计无锁数据结构是一个艰难的任务，并且很容易产生错误，但是在某些情况下，这种数据结构有很好的可扩展性，希望通过本章的例子和准则，你可以设计你自己的无锁数据结构，实现它或者发现别的人写的数据结构中的错误

//如果多个线程共享数据，那么就需要考虑使用什么数据结构以及如何在线程间同步此数据，通过设计并发数据结构，可以将它封装在数据结构中，这样剩下来的代码就可以集中在如何操作此数据结构上而不是数据同步上，在第8章中，当我们从并发数据结构转移到并发代码上，你就可以看到它所起的作用了，并行算法使用多线程来提供效率，当算法需要多线程共享数据的时候，选择使用何种并发数据结构就很重要了

#pragma clang diagnostic pop
