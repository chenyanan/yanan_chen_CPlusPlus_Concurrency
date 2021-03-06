//
//  6_6.cpp
//  123
//
//  Created by chenyanan on 2017/5/9.
//  Copyright © 2017年 chenyanan. All rights reserved.
//

#include <iostream>
#include <thread>
#include <mutex>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-function"

//6.3 设计更复杂的基于锁的数据结构

//栈和队列是很简单的，它们的接口及其有限，并且紧紧关注特定的目的，并非所有的数据结构都是那么简单的，大部分数据结构支持各种操作，原则上，这可能导致更多的并发机会，但因为需要考虑多种访问模式，使得保护数据的任务变得更加困难，当为并发访问设计数据结构时，能够执行的各种操作的精确特性是很重要的

//为了研究所涉及到的问题，我们先来看看查找表的设计

//6.3.1 编写一个使用锁的线程安全超找表

//超找表或字典将一种类型(键类型)的值与另外一种相同或不同类型(映射类型)的值联系起来，一般来说，这种数据结构的目的是使代码可以用一个给定的键值来查询相关的数据，在C++标准库中，是通过使用关联容器来实现这种功能的，例如，std::map<>，std::multimap<>，std::unordered_map<>以及std::unordered_multimap<>

//查找表的使用模式与栈和队列都不同，栈和队列上的每个操作都会在一定程度上对它有所修改，清单3.13中的简单DNS缓存就是这种情形的一个例子，与std::map<>相比，它的接口极大地简化了，如你在栈和队列中看到的，当多个线程并发访问数据接口时，标准容器的接口并不合适，因为在接口设计中存在固有的竞争条件，所以它们需要被削减修订

//从并发的角度来说，std::map<>接口的最大问题就是迭代器，尽管当别的线程访问(以及修改)容器时，拥有一个能够安全访问容器的迭代器也是有可能的，但这很棘手，正确把握迭代器要求你去处理以下的问题，例如另一个线程正在删除迭代器引用的元素，这很麻烦，作为线程安全查找表要砍掉的第一个接口，你应跳过迭代器，std::map<>(以及标准库中其他的关联容器)的接口在很大程度上基于迭代器，所以讲它们踢到一边并重新开始设计接口可能很是值得的

//查找表只有一些的基本操作

//添加新的键值对
//改变与给定的键相关联的值
//删除键及其关联的值
//获得与给定键相关联的值，如果又的话

//还有一些容器范围的操作也是有用的，例如检查容器是否为空，键的完整列表的快照，或是键值对的完整集合的快照

//如果坚持简单的线程安全准则，例如不返回引用，以及在每个成员函数上都有一个互斥元，那么这些操作都是安全的，它们要么出现在其他线程的某个修改之前，要么在之后，最有可能产生竞争条件的，是在添加一个新的键值对的时候，如果两个线程添加一个新值，只有一个线程会胜出，第二个会因此失败，一种可能的方法将添加和改变操作整合进单个成员函数中，就像你为清单3.13中的DNS缓存所做的那样

//从接口的进度来看，有趣一点是获取相关联值时的"如果有"的部分，一种选择是当键不存在的情况下，允许用户提供一个"默认的"结果来返回

//mapped_type get_value(const key_type& key, mapped_type default_value);

//在这种情况下，如果没有显示提供default_value，可以使用mapped_type的默认构造函数实例，这也可以扩展返回一个std::pair<mapped_type, bool>类型的实例，而不只是mapped_type的实例，这里的bool指示值是否存在，另一种选择就是，返回一个引用该值得智能指针，如果指针的值为NULL，就是没有返回值

//如上所述，一旦决定了接口，那么(假设没有接口竞争条件)可以通过在每个成员函数中使用一个互斥元和一个简单锁来保护下层的数据结构，以保证线程安全，然而，这回浪费通过独立的函数来读取数据结构并修改它所提供的并发可能性，一种方法是使用一个支持多个读线程或者一个写线程的互斥元，例如清单3.12中使用boost::shared_mutex，尽管这种方法可以提供并发访问的可能性，但是每次只有一个线程能够修改数据结构，理想情况下，你会想要做得更好一些

#pragma clang diagnostic pop
