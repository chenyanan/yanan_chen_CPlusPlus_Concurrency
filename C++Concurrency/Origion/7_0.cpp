//
//  7_0.cpp
//  123
//
//  Created by chenyanan on 2017/5/10.
//  Copyright © 2017年 chenyanan. All rights reserved.
//

#include <iostream>
#include <thread>
#include <mutex>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-function"

//第7章 设计无锁的并发数据结构

//本章主要内容

//为无需使用锁的并发而设计的数据结构的实现
//在无所数据结构中管理内存的技术
//有助于编写无锁数据结构的简单准则

//上一章中，我们分析了为实现并发性设计数据结构时需要考虑的一般方面，考虑了这种设计确保安全的准则，然后，我们验证了几种常见的数据结构，并且分析了使用互斥元和锁来保护共享数据的实现的例子，在前面的几个例子中，使用一个互斥元来保护整个数据结构，在后面的几个例子中，使用多个互斥元来保护数据结构的多个小部分，并且在访问数据结构时允许了更大界别的并发

//互斥元时保证多个线程可以安全访问数据结构，而不会遇到竞争条件和破坏不变量的有效机制，在探讨使用它们的代码的行为时也相对较简单，代码要么让保护数据的互斥元锁定，要么就不这样，然而，这也并不全然那么好，第3章中，看到了锁的不当使用会如何导致死锁，并且在基于锁的队列和查找表的例子中，可以看出锁的粒度是如何影响真正并发的潜能，如果能设计出不使用锁就能实现安全并发存取的数据结构，就有可能避免这些问题，这种数据结构被称为无锁数据结构

//在本章中，我们将考虑如何将第5张中提到的原子操作的内存顺序特性应用到无锁数据结构的构造中，设计这种数据结构时需要特别小心，因为很难做得正确，并且导致设计是被的条件可能很少发生，首先，我们来了解数据结构无锁的含义，然后，在分析一些实例之前，我们会介绍使用它们的原因，并得出一些通用准则

#pragma clang diagnostic pop
