//
//  CacheCoherence.h
//  C++Concurrency
//
//  Created by chenyanan on 2017/8/23.
//  Copyright © 2017年 chenyanan. All rights reserved.
//

#ifndef CacheCoherence_h
#define CacheCoherence_h

//  http://  www.cnblogs.com/jiayy/p/3246133.html
//  
//  并行编程--内存模型之缓存一致性
//  
//  1. 定义
//  
//  缓存一致性Cache coherence简称CC, 缓存一致性协议是在共享缓存多处理器构架确保最终一致性最突出、最重要的机制, 这些协议在缓存线(cache-line)级别实现了对一致性的保证, 缓存线是从主内存中读取数据和想内存中写入数据的缓存单位(至少从一致性机制的角度看是这样的), 商用处理器上三个最退出最重要的缓存一致性MOESI, MESI, and MESIF的缩写都来自它们为缓存线定义的各种状态:Modified(已修改), Owned(被占用), Exclusive(独占的), Shared(共享的), Invalid(无效的), and Forward(转发的), 缓存一致性协议在对内存确保最终一致性的内存一致性机制的帮助下对这些状态进行管理
//  
//  Inter 奔腾: MESI协议
//  AMD opteron: MOESI协议
//  Inter i7: MESIF协议
//  
//  问题: 为什么要缓存CC？
//  
//  答案:
//  
//  从第二章的体系结构图可以看到, 一般每个核心都有一个私有的L1级和L2级Cache, 同一个物理CPU上的多个核心共享一个L3级缓存, 这样的设计是出于提高内存访问性能的考虑, 但是这样就有一个问题了, 每个核心之间的私有L1, L2级缓存之间是需要同步啊, 比如, 核心1上的线程A对一个共享变量global_counter进行了加1操作, 这个被写入的新值存到核心1的L1缓存里了, 此时另一个核心2上的线程B要读global_counter了, 但是核心2的L1缓存里的global_counter的值还是旧值, 最新被写入的值现在还在核心1上, 则就需要CPU有一个模块来保证, 同一个内存的数据在同一时刻对任何对其可见的核心看来, 数据是一致的, 由第二章缓存图知道, 这种专门的组件就是缓存控制器(Cbox,Bbox)
//  
//  2 CC协议
//  
//  2.1 MESI
//  
//  详细了解参考: Cache一致性协议之MESI: http://  blog.csdn.net/muxiqingyang/article/details/6615199
//  
//  2.2 MOESI
//  
//  MOESI是MESI的一种扩展
//  
//  下面是基于MOESI的一个例子, 展示的是共享缓存多处理器中共享读写的声明周期
//  
//  [http://  www.oschina.net/translate/nonblocking-algorithms-and-scalable-multicore-programing]
//  
//  2.3 MESIF
//  
//  Inter提出了另外一种MESI协议的变种, 即MESIF协议, 该协议与MOESI协议有较大的不同, 也远比MOESI协议复杂, 该协议由Inter的QPI(QuickPath Interconnect)技术引入, 其主要目的是解决"基于点到点的全互联处理器系统"的Cache共相一致性问题, 而不是"基于共享总线的处理器系统"的Cache共享一致性问题
//  
//  在基于点到点互连的NUMA(Non-Uniform Memory Architecture)处理器系统中, 包含东哥子处理器系统, 这些字处理器系统由多个CPU组成, 如果这个处理器系统需要进行全机Cache共享一致性, 该处理器系统也被称为ccNUMA(Cache Cohenrent NUMA)处理器系统, MESIF系统主要解决ccNUMA处理器结构的Cache共享一致性问题, 这种结构通常使用目录表, 而不使用总线监听处理Cache的共享一致性
//  
//  关于MESIF, 可以参阅陈怀林的"浅谈Inter QPI的MESIF协议和Home,source Snoop:弯曲评论"
//  
//  http://  www.360doc.com/content/10/1207/13/158286_75798413.shtml
//  
//  3 伪共享
//  
//  3.1 定义
//  
//  从上一节可以知道, 缓存一致性协议操作的最小对象的缓存行, 缓存行内数据的修改、写入内存、写入其他缓存等操作都会改变其状态, 这样, 在共享缓存多核构架里, 数据结构如果组织不好, 就非常容易出现多个核线程反复修改同一条缓存行的数据导致缓存行状态频繁变化从而导致严重性能问题, 这就是伪共享现象
//  
//  下图就是一个伪共享的例子, core1上运行的线程想修改变量x, core2上运行的线程想修改变量y, 但x和y刚好在一个缓存行上, 每个线程都要去竞争缓存行的所有权来更新变量, 如果核心1获得了所有权, 缓存子系统将会使核心2中对应的缓存行失败, 当核心2获得了所有权然后执行更新操作, 核心1就要使自己对应的缓存行为失效, 这会来来回回的经过L3缓存, 大大影响了性能, 如果互相竞争的核心位于不同的插槽, 就要额外横跨插槽连接, 问题可能更加严重
//  
//  3.2 解决
//  
//  与缓存行导致性能问题的严重相比, 对这个问题的解决方案显得非常简单, 这就是缓存行填充, 通过填充缓存行, 是的某个核心线程频繁操作的数据独享缓存行, 这样就不会出现伪共享问题了, 下面是一个例子
//   
//  #define N_THR 8
//   
//  struct counter
//  {
//      unsigned long long value;
//  };
//   
//  static volatile struct counter counters[N_THR];
//   
//  void* thread(void* unused)
//  {
//      while ()
//      {
//          counters[UNIQUE_THREAD_ID].value++;
//      }
//      
//      return NULL;
//  }
//   
//  32位机long long是8字节, 这样一个缓存行64字节可以存8个counter, 这样最差的情况下同时会有8个线程争夺同一个缓存行的操作权, 性能会非常低, 解决方式非常简单, 如下图所示, 每个counter变量增加一个填充变量pad, 使得一个counter变量行号是一个缓存行大小, 这样数组counters每个元素占用一个缓存行, 所有县城独占自己的缓存行, 避免了伪共享问题
//   
//  struct counter
//  {
//      unsigned long long value;
//      char pad[64 - sizeof(unsigned long long)];
//  };


#endif /* CacheCoherence_h */
