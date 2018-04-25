//
//  5_7.cpp
//  123
//
//  Created by chenyanan on 2017/5/4.
//  Copyright © 2017年 chenyanan. All rights reserved.
//

#include <iostream>
#include <thread>
#include <mutex>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-function"

//2. 非顺序一致的内存顺序

//一旦你走出美好的顺序一致的世界，事情开始变得复杂起来，可能要面对的一个最大问题是事情不再有单一的全局顺序的事实，这意味着，不同的线程可能看到相同的操作的不同方面，以及你所拥有的不同线程操作一前一后整齐交错的所有心里模型都必须扔到一边，你不仅得考虑事情真正的并行发生，而且线程不必和事件的顺序一致，为了编写(或者甚至只是理解)任何使用非默认的memory_order_seq_cst内存顺序的代码，让你的大脑思考这个问题绝对是至关重要的，这不仅仅意味着编译器能够重新排列指令，即使线程正在运行完全相同的代码，由于其他线程中的操作没有明确的顺序约束，它们可能与事件的顺序不一致，因为不同的CPU缓存和内部缓冲区可能为相同的内存保存了不同的值，它是如此重要以至于我要再说一遍:线程不必和事件的顺序一致

//你不仅要将基于交错操作的心里模型扔到一遍，还得将基于编译器或处理器重拍指令的思想的心理模型也扔掉，在没有其他的顺序约束时，唯一的要求是所有的线程对每个独立变量的修改顺序达成一致，不同变量上的操作可以以不同的顺序出现在不同的线程中，前提是所能看到的值与所有附加的顺序约束时一致的

//通过完全跳出顺序一致的世界，并未所有操作使用memory_order_relaxed，就是最好的展示，一旦你掌握了，就可以回过头来获得-释放顺序，它让你选择性地在操作之间引入顺序关系，多会一些理性

#pragma clang diagnostic pop
