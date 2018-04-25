//
//  3_0.h
//  C++Concurrency
//
//  Created by chenyanan on 2017/4/20.
//  Copyright © 2017年 chenyanan. All rights reserved.
//

#ifndef _3_0_H
#define _3_0_H

#include <list>
#include <stack>
#include <deque>
#include <mutex>
#include <string>
#include <thread>
#include <algorithm>
#include <iostream>
#include "assert.h"
#include "hierarchical_mutex.h"

//第3章 线程间共享数据

//本章主要内容

//共享数据带来的问题
//使用互斥量保护数据
//数据保护的替代方案

//上一章中,我们已经对线程管理有所了解了,现在让我们来看一下"共享数据的那些事"

//想象一下,你和你的朋友合租一个公寓,公寓中只有一个厨房和一个卫生间,当你的朋友在卫生间时,你就会不能使用了(除非你们特别好,好到可以在同时使用一个房间),这个问题也会出现在厨房,假如,厨房里有一个组合式烤箱,当在烤香肠的时候,也在做蛋糕,就可能得到我们不想要的食物(香肠味的蛋糕),此外,在公共空间做一件事做到一半时,发现某些需要的东西被别人借走,或是当离开的一段时间内有些东西被变动了地方,这都会令我们不爽

//同样的问题,也困扰着线程,当线程在访问共享数据的时候,必须定一些规矩,用来限定线程可访问的数据位,还有,一个线程更新了共享数据,需要对其他线程进行通知,从易用性的角度,同一进程中的多个线程进行数据共享,有利有弊,错误的共享数据使用是产生并发bug的一个主要原因,并且后果要比香肠味的蛋糕更加严重

//本章就以在C++中进行安全的数据共享为主题,避免上述及其他潜在的问题的发生的同时,将共享数据的有时发挥到最大

//3.1 共享数据带来的问题

//当涉及到共享数据时,问题很可能是因为共享数据修改所导致,如果共享数据是只读的,那么只读操作不会影响到数据,更不会涉及对数据的修改,所以所有线程都会获得同样的数据,但是,当一个或多个线程要修改共享数据时,就会产生很多麻烦,这种情况下,就必须小心谨慎,才能确保一切所有线程都能工作正常

//不变量(invariants)的概念对程序员们编写的程序会有一定的帮助,对于特殊结构体的描述,比如,"变量包含列表中的项数",不变量通常会在一次更新中被破坏,特别是比较复杂的数据结构,或者一次更细你就要搞懂很大的数据结构

//双链表中每个节点都有一个指针指向列表中的下一个节点,还有一个指针指向前一个节点,其中不变量就是节点A中指向"下一个"节点B的指针,还有前向指针,为了从列表中删除一个节点,其两边节点的的指针都需要更新,当其中一个更新完成时,不变量就被破坏了,直到另一个边也完成更新,在两边都完成更新后,不变量就又稳定了

//从一个列表中删除一个及节点的步骤如下(如图3.1)

//1. 找到要删除的节点N
//2. 更新前一个节点指向N的指针,让这个指针指向N的下一个节点
//3. 更新后一个节点指向N的指针,让这个指针指向N的前一个及诶单
//4. 删除节点N

//图3.1 从一个双链表中删除一个节点

//图中b和c在相同的方向上指向和原来已经不一致了,这就破坏了不变量

//线程间潜在问题就是修改共享数据,致使不变量遭到破坏,当不做些事来确保在这个过程中不会有其他线程进行访问的话,可能就有线程访问到刚刚删除一遍的节点,这样的话,线程就读取到要删除节点的数据(因为只有一边的连接被修改,如图3.1(b)),所以不变量就被破坏,破坏不变量的后果是多样的,当其他线程按从左往右的顺序来访问列表时,它将跳过被删除的节点,在一方面,如有第二个线程阐释删除图中右边的节点,那么可能会让数据结构产生永久性的损坏,是程序崩溃,无论结果如何,都是并行代码常见错误:条件竞争(race condition)

//3.1.1 条件竞争

//假设你去电影院买电影票,如果去的是一家大电影院,有很多收银台,很多人就可以在同一事件买电影票,当另一个收银台也在卖你想看的这场电影的电影票,那么你的座位选择范围就取决于在之前已预订的座位,当只有少量的座位剩下,这就意味着,这可能是一场抢票比赛,看谁能抢到最后一张票,这就是一个条件竞争的例子:你的座位(或者你的电影票)都取决于两种购买方式的相对顺序

//并发中竞争条件的形成,取决于一个以上线程的相对执行顺序,每个线程都抢着完成自己的任务,大多数情况下,即使改变执行顺序,也是良性竞争,其结果可以接受,例如,有两个线程同时向一个处理队列中添加任务,因为系统提供的不变量保持不变,所以谁先谁后都不会有什么影响,当不变量遭到破坏时,才会产生条件竞争,比如双向链表的例子,并发中对数据的条件竞争通常表示为"恶性"(problematic)条件竞争,我们队不产生问题的良性条件竞争不感兴趣,C++标准中也定义了数据竞争(data race)这个术语,一种特殊的条件竞争:并发的去修改一个独立对象(参见5.1.2节),数据竞争是(可怕的)未定义行为(undefine behavior)的起因

//恶性条件竞争通常发生于完成对多于一个的数据块的修改时,例如,对两个链接指针的修改(如图3.1),因为操作要访问两个独立的数据块,独立的指令将会对数据块进行修改,并且其中一个线程可能正在进行时,另一个线程就对数据块进行了访问,因为出现的概率太低,条件竞争很难查找,也很难浮现,如CPU指令连续修改完成后,即使数据结构可以让其他并发线程访问,问题再次复现的概率也在增加,这样的问题只可能会出现在负载比较大的情况下,条件竞争通常是时间敏感的,所以程序以调试模式运行时,他们常会完全消失,因而调试模式会影响程序的执行时间(即使影响不多)

//当你以写多线程程序为主,条件竞争就会成为你的梦魇,编写软件时,我们会使用大量复杂的操作,用来避免恶性条件竞争

//3.1.2 避免恶性条件竞争

//这里提供一些方法来解决恶性条件竞争,最简单的办法就是对数据结构采用某种保护机制,确保只有进行修改的线程才能看到不变量被破坏时的中间状态,从其他访问线程的角度来看,修改不是已经完成了,就是还没开始,C++标准库提供很多类似的机制,下面会逐一介绍

//另一个选择是对数据结构和不变量的设计进行修改,修改完的结构必须能完成一系列不可分割的变化,也就是保证每个不变量保持稳定的状态,这就是所谓的无锁编程(lock-free-programming),不过,这种方式很难得到正确的结果,如果到这个级别,无论是内存模上的细微差异,还是线程访问数据的能力,都会让工作变得复杂,内存模型将在第5章讨论,无锁编程将在第7章讨论

//另一种处理条件竞争的方式是,使用事务(transacting)的方式去处理数据结构的更新(这里的"处理"就如同对数据库进行更新一样),所需的一些数据和读取都存储在事务日志中,然后将之前的操作合为一步,再进行提交,当数据结构被另一个线程修改后,或处理已经重启的情况下,提交就会无法进行,这称为"软件事务内存"(software transactional memory)(STM),研究理论中,这是一个很热门的研究领域,这个概念将不会在本书中再进行介绍,因为在C++中没有对STM进行直接支持,但是,基本思想会在后面提及

//保护共享数据结构的最基本的方式,是使用C++标准库提供的互斥量(mutex)

//3.2 使用互斥量保护共享数据

//当程序中有共享数据,肯定不想让其陷入条件竞争,或是不变量被破坏,那么,将所有访问共享数据结构的代码都标记为互斥岂不是更好？这样任何一个线程在执行这些代码时,其他任何线程试图访问共享数据结构,就必须等待那一段代码执行结束,于是,一个线程就不可能会看到被破坏的不变量,除非它本身就是修改共享数据的线程

//当访问共享数据前,使用互斥量将相关数据锁住,当访问结束后,再将数据解锁,线程库需要保证,当一个线程使用特定互斥量锁住共享数据时,其他的线程想要访问锁住的数据,都必须等到之前那个线程对数据进行解锁后,才能进行访问,这就保证了所有线程能看到共享数据,而不破坏不变量

//互斥量是C++中一种最通用的数据保护机制,但它不是"银弹",精心组织的代码来保护正确的数据(见3.2.2节),并在接口内部避免竞争条件(见3.2.3节)是非常虫咬的,但互斥量自身也有问题,也会造成死锁(见3.2.4节),或是对数据保护的太多(或太少)(见3.2.8节)

//3.2.1 C++中使用互斥量

//C++中通过实例化std::mutex创建互斥量,通过调用成员函数lock()进行上锁,unlock()进行解锁,不过,不推荐实践中直接去调用成员函数,因为调用成员函数就意味着,必须记住在每个函数出口都要去调用unlock(),也包括异常的情况,C++标准库为互斥量提供了一个RAII语法的模板类std::lock_guard,其会在构造的时候提供已锁的互斥量,并在析构函数的时候进行解锁,从而保证了一个已锁的互斥量总是会被正确的解锁,下面的程序清单中,展示了如何在多线程程序中,使用std::mutex构造的std::lock_guard实例,对一个列表进行访问保护,std::mutex和std::lock_guard都在<mutex>头文件中声明

//清单3.1使用互斥量保护列表

std::list<int> some_list;   //①
std::mutex some_mutex;   //②

void add_to_list(int new_value)
{
    std::lock_guard<std::mutex> guard(some_mutex);   //③
    some_list.push_back(new_value);
}

bool list_contains(int value_to_find)
{
    std::lock_guard<std::mutex> guard(some_mutex);   //④
    return std::find(some_list.begin(), some_list.end(), value_to_find) != some_list.end();
}

//清单3.1中有一个全局变量①,这个全局变量被一个全局的互斥量保护②,add_to_list()③和list_contains()④函数中使用std::lock_guard<std::mutex>,使得这两个函数中对数据的访问是互斥的:list_contains()不可能看到正在被add_to_list()修改的列表

//虽然某些情况下,使用全局变量没问题,但在大多数情况下,互斥量通常会与保护的数据放在同一个类中,而不是定义成全局变量,这是面向对象设计的准则,将其放在一个类中,就可让他们联系在一起,也可以对类的功能进行封装,并进行数据保护,在这种情况下,函数add_to_list和list_contains可以作为这个类的成员函数,互斥量和要保护的数据,在类中都需要定义为private成员,这会让访问数据的代码变的清晰,并且容易看出在什么时候对互斥量上锁,当所有成员函数都会在调用时对数据上锁,结束时对数据解锁,那么就保证了数据访问时不变量不会被破坏

//当然,也不总是那么理想,聪明的你一定注意到了,当其中一个成员函数返回的是保护数据的指针或引用,会破坏对数据的保护,具有访问能力的指针或引用可以访问(并可能修改)被保护的数据,而不会被互斥锁限制,互斥量保护的数据需要对接口的设计相当严谨,要确保互斥量能锁住任何对保护数据的访问,并且不留后门

//3.2.2 精心组织代码来保护共享数据

//使用互斥量来保护数据,并不是仅仅在每一个成员函数中都加入一个std::lock_guard对象那么简单, 一个迷失的指针或引用,将会让这种保护形同虚设,不过,检查迷失指针或引用是很容易的,只要没有成员函数通过返回值或者输出参数的形式向其调用者返回指向受保护数据的指针或引用,数据就是安全的,如果你还想往祖坟上刨,就没这么简单了,在确保成员函数不会传出指针或引用的同时,检查成员函数是否通过指针或引用的方式来调用也是很重要的(尤其是这个操作不在你的控制下时),函数可能没在互斥量保护的区域内,存储着指针或者引用,这样就很危险,更危险的是,将保护数据作为一个运行时参数,如同下面清单中所示那样

//清单3.2 无意中传递了保护数据的引用

class some_data
{
    int a;
    std::string b;
public:
    void do_something() { std::cout << "some_data::do_something()" << std::endl; }
};

class data_wrapper
{
private:
    some_data data;
    std::mutex m;
public:
    template<typename Function>
    void process_data(Function func)
    {
        std::lock_guard<std::mutex> l(m);
        func(data);                           //①传递"受保护的"数据到用户提供的函数
    }
};

some_data* unprotected;

void malicious_function(some_data& protected_data)
{
    unprotected = &protected_data;
}

data_wrapper x;

void pass_protected_data_to_function()
{
    x.process_data(malicious_function);      //②传入一个恶意函数
    unprotected->do_something();             //③对受保护的数据进行未受保护的访问
}

//例子中process_data看起来没有任何问题,std::lock_guard对数据做了很好的保护,但调用用户提供的函数func①,就意味着foo能够绕过保护机制将函数malicious_function传递进去②,在没有锁定互斥量的情况下调用do_something()

//从根本上说,这个代码的问题在于它没有完成你所设置的内容,标记所有访问该数据结构的代码为互斥的(mutuallyexclusive),在这个例子中,忽略了foo()中调用unprotected->do_something()的代码,不幸的是,这部分问题不是C++线程库所能帮助你的,而这取决于作为程序员的你,去锁定正确的互斥元来保护你的数据,想想好的一面,你有了一个可遵循的准则,它会在这些情况下帮助你,不要将对受保护数据的指针和引用传递到锁的范围之外,无论是通过从函数中返回它们、将其存放在外部可见的内存中,还是作为参数传递给用户提供的函数

//虽然这是在试图使用互斥元来保护共享数据时常犯的所务,但这绝非唯一可能的隐患,在下一节中你会看到,可能仍然会有竞争条件,即便当数据被互斥元保护着

//3.2.3 发现接口中固有的竞争条件

//仅仅因为使用了互斥元或其他几只来保护共享数据,未必会免于竞争条件,你仍然需要确定保护了适当的数据,再次考虑双向链表的例子,为了让线程安全地删除节点,你需要确保已组织对三个节点的并发访问,要删除的节点及其两边的节点,如果你分别保护访问每个节点的指针,就不会比未使用互斥元的代码更好,因为竞争条件仍会发生,需要保护的不是个别步骤中的个别节点,而是整个删除操作中的整个数据结构,这种情况下最简单的解决办法,就是用单个互斥元保护整个列表,如清单3.1中所示

//仅仅因为在列表上的个别操作是安全的,你还没有摆脱困境,你仍然会遇到竞争条件,即便是一个非常简单的接口,考虑像std::stack容器适配器这样的堆栈数据结构,如清单3.3中所示,除了构造函数和swap(),对std::stack你只有五件事情可以做,可以push()一个新元素入栈,pop()一个元素出栈,读top()元素,检查它是否empty()以及读取元素数量,堆栈的size(),如果更改top()使得它返回一个副本,而不是引用(这样你就遵照了3.2.2节的标准),同时用互斥元保护内部数据,该接口依然固有地受制于竞争条件,这个问题对基于互斥元的实现并不是独一无二的,他是一个接口问题,因此对于无锁实现仍然会发生竞争条件

//清单3.3 std::stack容器的实现

template<typename T, typename Container = std::deque<T>>
class stack {
public:
    explicit stack(const Container&);
    explicit stack(Container&& = Container());
    template <class Alloc> explicit stack(const Alloc&);
    template <class Alloc> stack(const Container&, const Alloc&);
    template <class Alloc> stack(Container&&, const Alloc&);
    template <class Alloc> stack(stack&&, const Alloc&);
    
    bool empty() const;
    size_t size() const;
    T& top();
    const T& top() const;
    void push(const T&);
    void push(T&&);
    void pop();
    void swap(stack&&);
};

template<typename T, typename Container>
stack<T,Container>::stack(const Container&) {}

template<typename T, typename Container>
stack<T,Container>::stack(Container&&) {}

template<typename T, typename Container>
template<typename Alloc>
stack<T,Container>::stack(const Alloc&) {};

template<typename T, typename Container>
template<typename Alloc>
stack<T,Container>::stack(const Container&, const Alloc&) {}

template<typename T, typename Container>
template<typename Alloc>
stack<T,Container>::stack(Container&&, const Alloc&) {}

template<typename T, typename Container>
template<typename Alloc>
stack<T,Container>::stack(stack&&, const Alloc&) {}

template<typename T, typename Container>
bool stack<T,Container>::empty() const { return true; }

template<typename T, typename Container>
size_t stack<T,Container>::size() const { return 0; }

template<typename T, typename Container>
T& stack<T,Container>::top() { static T t = T(); return t; }

template<typename T, typename Container>
const T& stack<T,Container>::top() const { static T t = T(); return t; }

template<typename T, typename Container>
void stack<T,Container>::push(const T&) {}

template<typename T, typename Container>
void stack<T,Container>::push(T&&) {}

template<typename T, typename Container>
void stack<T,Container>::pop() {}

template<typename T, typename Container>
void swap(stack<T,Container>&&) {}

//这里的问题是empty()的结果和size()不可靠,虽然他们可能在被调用时是正确的,一旦他们返回,在调用了empty()或size()的线程可以使用该信息之前,其他线程可以自由地访问堆栈,并且可能push()新元素入栈或pop()已有的元素出栈

//特别的,如果该stack实例是非共享的,如果栈非空,检查empty()并调用top()访问顶部元素是安全的,如下所示

void do_something(int value)
{
    std::cout << "void do_something(int value)" << std::endl;
}

void stack_not_shared_empty_top()
{
    stack<int> s;
    if (!s.empty())   //①
    {
        const int value = s.top();   //②
        s.pop();   //③
        do_something(value);
    }
}

/*

if (!s.empty())
                            s.pop();
const int value = s.top();
s.pop()
do_something(value);
 
*/

//它仅仅在单线程代码中是安全的,预计为:在空堆栈上调用top()是未定义的行为,对于共享的stack对象,这个调用序列不再安全,因为在调用empty()①和top()②之间可能有来自另一个线程的pop()调用,删除最后一个元素,因此,这是一个典型的竞争条件,为了保护栈的内容而在内部使用互斥元,却并未能将其阻止,这就是接口的影响

//怎么解决呢？发生这个问题是接口设计的后果,所以解决办法就是改变接口,然而,这仍然回避了问题,要做出什么样的改变,在最简单的情况下,你只要声明top()在调用时如果栈中没有元素则引发异常,虽然这直接解决了问题,但它是编程变得更麻烦,因为现在你得能捕捉异常,即使对empty()的调用返回false,这基本上使得empty()的调用变得纯粹多余

//如果你仔细看看前面的代码片段,还有另一个可能的竞争条件,但这一次是在调用top()②和调用pop()③之间,考虑运行着前面代码片段的两个线程,他们都引用着同一个stack对象s,这并非罕见的情形,当为了性能而使用线程时,有数个线程在不同的数据上运行相同的代码是很常见的,并且一个共享的stack对象非常适合用来在它们之间分隔工作,假设一开始栈有两个元素,那么你不用担心在任一线程上的empty()和top()之间的竞争,只需要考虑可能的执行模式

/*
 
 if(!s.empty())
 if(!s.empty())
 const int value = s.top()
 const int value = s.top()
 s.pop()
 do_something(value)         s.pop()
 do_something(value);
 
*/

//如你所见,如果这些是仅有的在运行的线程,在两次调用top()修改该栈之间没有任何东西,所以这两个线程将看到相同的值,不仅如此,在pop()的两次调用之间没有对top()调用,因此,栈上的两个值其中一个还没有读取就被丢弃了,而另一个被处理了两次,这是另一种竞争条件,远比empty()/top()竞争的未定义行为更为糟糕,从来没有任何明显的错误发生,同时错误造成的后果可能和诱因差距甚远,尽管他们明显取决于do_something()到底做什么

//这要求对接口进行更加激进的改变,在互斥元的保护下结合对top()和pop()两者的调用,Tom Cargill指出,如果栈上对象的拷贝构造函数能够引发异常,结合调用可能会导致问题,从Herb Sutter的异常安全的观点来看,这个问题被处理得较为全面,但潜在的竞争条件为这一结合带来了新的东西

//对于那些尚未意识到这个问题的人,考虑一下stack<vector<int>>,现在,vector是一个动态大小的容器,所以当你复制vector时,为了复制其内容,库就必须从堆中分配更多的内存,如果系统负载过重,或有明显的资源约束,此次内存分配就可能失败,于是vector的拷贝构造函数可能引发std::bad_alloc异常,如果vector中含有大量的元素的话则尤其可能,如果pop()函数被定义为返回出栈值,并且从栈中删除它,就会有潜在的问题,仅在栈被修改后,出栈值才返回给调用者,但复制数据以返回给调用者的过程可能会引发异常,如果发生这种情况,刚从栈中出栈的数据会丢失,它已经从栈中被删除了,但该复制却没有成功,std::stack接口的设计者笼统地将操作一分为二,获取顶部的元素top(),然后将其从栈中删除pop(),以至于你无法安全的复制数据,它将留在栈上,如果问题是堆内存不足,也许用用程序可以释放一些内存,然后再试一次

//不幸的是,这种划分正式你在消除竞争条件中试图去避免的！值得庆幸的是,还有替代方案,但他们并非无代价的

//选项1.传入引用
//第一个选项是把你希望接受出栈值的变量的引用,作为参数传递给对pop()的调用

/*
 
 std::vector<int> result;
 some_stack.pop(result)
 
 */

//这在很多情况下都适用,但它有个明显的缺点,要求调用代码在调用之前先构造一个该栈值类型的实例,以便将其作为目标传入,对于某些类型而言这是行不通的,因为构造一个实例在时间和资源方面是非常昂贵的,对于其他类型,这并不总是可能的,因为构造函数需要参数,而在代码的这个位置不一定可用,最后,它要求所存储的类型是可赋值的,这是一个重要的限制,许多用户定义的类型不支持赋值,尽管他们可能支持移动构造函数,或者甚至是拷贝构造函数



//选项2.要求不引发异常的拷贝构造函数或移动构造函数
//对于有返回值的pop()而言只有一个异常安全问题,就是以值进行的返回可能引发异常,许多类型具有不引发异常的拷贝构造函数,并且在C++标准中有了新的右值引用的支持,越来越多的类型将不会引发异常的移动构造函数,即便他们的拷贝构造函数会如此,一个有效的选择,就是把对线程安全堆栈的使用,限制在能够安全的通过值来返回且不引发异常的类型之内,虽然这样安全了,但并不理想,尽管你可以在编译时使用std::is_nothrow_copy_constructible和std::is_nothrow_move_constructible类型特征,来检测一个不引发异常的拷贝或移动构造函数的存在,但这却很受限制,相比于具有不能引发异常的拷贝或移动构造函数的类型,有更多的用户定义类型具有能够引发异常的拷贝构造函数且没有移动构造函数(尽管这会随着人们习惯了C++11中对右值引用的支持而改变),如果这种类型不能被存储在你的线程安全堆栈中,是不幸的



//选项3.返回指向出栈项的指针
//第三个选择是返回一个指向出栈项的指针,而非通过值来返回该项,其优点是指针可以被自由地复制而不会引发异常,这样你就避免了Cargill的异常问题,其缺点是,返回一个指针时需要一种手段来管理分配给对象的内存,对于像整数这样简单的类型,这种内存管理的成本可能会超过仅通过值来返回该类型,对于任何使用此选项的接口,std::shared_ptr会是指针类型的一个好的选择,它不仅避免了内存泄漏,因为一旦最后一个指针被销毁则该对象也会被销毁,并且库可以完全控制内存分配方案且不必使用new和delete,对于优化用途来说这是很重要的,要求用new分别分配堆栈中的内一个对象,会比原来非线程安全的版本带来大得多的开销



//选项4.同时提供选项1以及2或3
//灵活性永远不应被排除在外,特别是通用的代码中,如果你选择选项2或3,那么同时提供选项1也是相对容易的,这也是你的代码的用户提供了选择的能力,为了很小的额外成本,哪个选项对他们是最合适的



//5. 一个线程安全堆栈的示范意义

//清单3.4展示了在接口中没有竞争条件的栈的类定义,实现了选项1和3,pop()有来个两个重载,一个接受存储该值的位置的引用,另一个返回std::shared_ptr<>,它具有一个简单的接口,只有两个函数,push()和pop()

//通过削减接口,你考虑了最大的安全性,甚至对整个堆栈的操作都受到限制,栈本身不能被赋值,因为赋值运算符被删除①(参见附录A中A.2节),而且也没有swap()函数,然而,它可以被复制,假设栈的元素可以被复制,如果栈是空的,pop()函数引发一个empty_stack异常,所以即使在调用empty()后栈被修改,一切仍然将正常工作,正如选项3的描述中提到的,如果需要,std::shared_ptr的使用允许栈来处理内存分配的问题,同时避免对new和delete的过多调用,五个堆栈操作现在变成三个,push(),pop(),empty(),甚至empty()都是多余的,接口的简化可以更好的控制数据,你可以确保互斥元为了操作的整体而被锁定,清单3.5展示了一个简单的实现,一个围绕std::stack<>的封装器

//清单3.5 一个线程安全的栈的详细类定义

struct empty_stack : std::exception
{
    const char* what() const throw()
    {
        return "this is an empry stack";
    }
};

template<typename T>
class threadsafe_stack
{
private:
    std::stack<T> data;
    mutable std::mutex m;
public:
    threadsafe_stack() {}
    threadsafe_stack(const threadsafe_stack& other)
    {
        std::lock_guard<std::mutex> lock(other.m);
        data = other.data;   //①在构造函数中执行复制
    }
    
    threadsafe_stack& operator=(const threadsafe_stack&) = delete;
    
    void push(T new_value)
    {
        std::lock_guard<std::mutex> lock(m);
        data.push(new_value);
    }
    
    std::shared_ptr<T> pop()
    {
        std::lock_guard<std::mutex> lock(m);
        if (data.empty())
            throw empty_stack();   //在试着出栈值的时候检查是否为空
        const std::shared_ptr<T> res(std::make_shared<T>(data.top()));   //在修改栈之前分配返回值
        data.pop();
        return res;
    }
    
    void pop(T& value)
    {
        std::lock_guard<std::mutex> lock(m);
        if (data.empty())
            throw empty_stack();   //在试着出栈值的时候检查是否为空
        value = data.top();
        data.pop();
    }
    
    bool empty() const
    {
        std::lock_guard<std::mutex> lock(m);
        return data.empty();
    }
};

void some_operation_on_threadsafe_stack()
{
    threadsafe_stack<int> threadsafe_stack_of_int;
    threadsafe_stack_of_int.push(1);
    threadsafe_stack_of_int.push(2);
    threadsafe_stack_of_int.push(3);
    threadsafe_stack_of_int.push(4);
    threadsafe_stack_of_int.push(5);
    
    int a = -1;
    threadsafe_stack_of_int.pop(a);
    std::cout << "a = " << a << std::endl;
    
    auto b = threadsafe_stack_of_int.pop();
    std::cout << "b = " << *b << std::endl;
}

//这个栈的实现实际上是可复制的,源对象中的拷贝构造函数锁定互斥元,然后复制内部栈,你在构造函数体中进行复制①而不是成员初始化列表,以确保互斥元被整个副本持有

//top()和pop()的讨论表明,接口中有问题的竞争条件基本上因为锁定的粒度过小而引起,保护没有覆盖期望操作的整体,互斥元的问题也可以由锁定的粒度过大因此,极端情况是单个的全局互斥元保护所有共享的数据,在一个有大量共享数据的系统中,这可能会消除并发的所有性能又是,因为线程被限制为每次只能运行一个,即便是在他们访问数据的不同部分的时候,被设计为处理多处理器系统的Linux内核的第一个版本,使用了单个全局内核锁,虽然这也能工作,但却意味着一个双处理器系统通常比两个单处理器系统的性能更差,四个处理器系统的性能远远没有四个单核处理器系统的性能好,有太多对内核的竞争,因此在更多处理器上运行的线程无法进行有效的工作,Linux内核的后续版本已经转移到一个更细粒度的锁定方案,因而四个处理器的系统性能更接近理想的单处理器系统的四倍,因为竞争少得多

//细粒度锁定方案的一个问题,就是有时为了保护操作中的所有数据,需要不止一个互斥元,如前所述,有时要做的正确的事情是增加被互斥元所覆盖的数据粒度,以使得只需要一个互斥元被锁定,然而,这有时是不可取的,例如互斥元保护着一个类的各个势力,在这种情况下,在下个级别进行锁定,将意味着要么将锁丢给用户,要么就让单个互斥元保护该类的所有实例,这些都不理想

//如果对于一个给定的操作你最终需要锁定两个或更多的互斥元,还有另一个潜在的问题潜伏在侧:死锁,这几乎是竞争条件的反面,两个线程不是在竞争称为第一,而是每一个都在等待另外一个,因而都不会有任何进展

//3.2.4 死锁:问题和解决方案

//试想一下,你有一个由两个部分组成的玩具,并且你需要两个部分一起玩,例如玩具鼓和鼓棒,现在假设你有两个小孩,他们两人都喜欢玩它,如果其中一人同时得到鼓和鼓棒,那这个孩子就可以高兴的玩鼓,知道厌烦,如果另一个孩子想要玩,就得等,不管这让她多不爽,现在想象一下,鼓和鼓棒被分别埋在玩具箱里,你的孩子同时都决定玩他们,于是他们去翻玩具箱,其中一个发现了鼓,而另外一个发现了鼓棒,现在他们被困住了,除非一个人让另一个人玩,不然每个人都回来着他已有的东西,并要求另一个人将另一部分给自己,否则就都玩不成

//现在想象一下,你没有抢玩具的孩子,但却有争夺互斥元的线程,一对线程中的每一个都需要同时锁定两个互斥元来执行一些操作,并且每个线程都拥有了一个互斥元,同时等待另外一个,线程都无法继续,应为每个线程都在等待另一个释放其互斥元,这种情景称为死锁,它是在需要锁定两个或更多互斥元以执行操作时的最大问题

//DEATH LOCK SAMPLE
std::mutex M1;
std::mutex M2;

void l1() {
    M1.lock();
    std::this_thread::sleep_for(std::chrono::seconds(10));
    M2.lock();
    M2.unlock();
    M1.unlock();
}

void l2() {
    M2.lock();
    std::this_thread::sleep_for(std::chrono::seconds(10));
    M1.lock();
    M1.unlock();
    M2.unlock();
}

void death_lock() {
    std::thread t1(l1);
    t1.detach();
    
    std::thread t2(l2);
    t2.detach();
}
//t1 LOCK M1,
//t2 LOCK M2 WHILE t1 IS STILL IN BED
//t1 WAIT M2
//t2 WAIT M1
//DEATH LOCK

//为了避免死锁,常见的建议是始终使用相同的顺序锁定这两个互斥元,如果你总是在互斥元B之前锁定互斥元A,那么你永远不会死锁,有时候这是很直观的,因为互斥元服务于不同的目的,但其他时候却并不那么简单,比如当互斥元分别保护相同类的各个实例时,例如,考虑同一个类的两个实例之间的数据交换操作,为了确保数据被正确的交换,而不受并发修改的影响,两个实例上的互斥元都必须被锁定,然而,如果选择了一个固定的顺序(例如,作为第一个参数提供的实例的互斥元,然后是作为第二个参数所提供的实例的互斥元),可能适得其反,它表示两个线程尝试通过交换参数,而在相同的两个实例之间交换数据,你将产生死锁

namespace death_lock_on_swap
{
    class some_class
    {
    private:
        std::mutex m;
        std::string data;
        friend void swap(some_class& lhs, some_class& rhs);
    };
    
    void swap(some_class& lhs, some_class& rhs)
    {
        using std::swap;
        std::lock_guard<std::mutex> g1(lhs.m);
        std::this_thread::sleep_for(std::chrono::seconds(10));
        std::lock_guard<std::mutex> g2(rhs.m);
        swap(lhs.data, rhs.data);
    }
};

void death_lock_again()
{
    death_lock_on_swap::some_class c1;
    death_lock_on_swap::some_class c2;
    
    std::thread t1(death_lock_on_swap::swap, std::ref(c1), std::ref(c2));
    std::thread t2(death_lock_on_swap::swap, std::ref(c2), std::ref(c1));
    
    if (t1.joinable())
        t1.detach();
    
    if (t2.joinable())
        t2.detach();
    
    //t1 LOCK c1.m,
    //t2 LOCK c2.m WHILE t1 IS STILL IN BED
    //t1 WAIT c2.m
    //t2 WAIT c1.m
    //DEATH LOCK
}

void what_is_death_lock()
{
    std::cout << "look up, so that you can understand what is death lock" << std::endl;
}

//幸运的是,C++标准库中的std::lock可以解决这一问题,std::lock函数可以同时锁定两个或更多的互斥元,而没有死锁的风险,以下的代码展示了如何使用它来完成简单的交换操作

struct some_big_object
{
    std::string name;
};

void swap(some_big_object& lhs, some_big_object& rhs)
{
    using std::swap;
    swap(lhs.name, rhs.name);
}

class S
{
private:
    some_big_object some_detail;
    std::mutex m;
public:
    S(const some_big_object& sd) : some_detail(sd) {}
    friend void swap(S& lhs, S& rhs)
    {
        if (&lhs == &rhs)
            return;
        std::lock(lhs.m, rhs.m);   //①
        std::lock_guard<std::mutex> lock_a(lhs.m, std::adopt_lock);   //②
        std::lock_guard<std::mutex> lock_b(rhs.m, std::adopt_lock);   //③
        swap(lhs.some_detail, rhs.some_detail);
    }
};

void swap_object_with_lock_guard()
{
    some_big_object object_one;
    object_one.name = "one";
    
    some_big_object object_two;
    object_two.name = "two";
    
    S s1(object_one);
    S s2(object_two);
    
    swap(s1, s2);
}

//首先,检查参数以确保他们是不同的实例,因为试图在你已经锁定了的std::mutex上再次锁定,是未定义的行为(允许同一线程多重锁定的互斥元类型为std::recursive_mutex),然后调用std::lock()锁定这两个互斥元①,同时构造两个std::lock_guard的实例②③,每个实例对应一个互斥元,额外提供一个参数std::adopt_lock给互斥元,告知std::lock_guard对象该互斥元已被锁定,并且他们只应沿用互斥元上已有锁的所有权,而不是试图在构造函数中锁定互斥元

//这就确保了通常在受保护的操作可能引发异常的情况下,函数退出时正确的解锁互斥元,这也考虑到了简单返回,此外值得一提的是,在对std::lock的调用中锁定lhs.m抑或是rhs.m都可能引发异常,在这种情况下,该异常被传播出std::lock如果std::lock已经成功地在一个互斥元上施加了锁,当它试图在另一个互斥元上获取锁的时候,就会引发异常,前一个互斥元将会自动释放,std::lock提供了关于锁定给定的互斥元的全或无的语义

//尽管std::lock能够帮助你在需要同时获得两个或更多锁的情况下避免死锁,但是如果要分别获取锁,就没有用了,在这种情况下,你必须依靠你作为开发人员的戒律,以确保不会得到死锁,这谈何容易,死锁是在编写多线程代码时遇到的最令人头痛的问题之一,而且往往无法预测,大部分时间内一切都工作正常,然而,有一些相对简单的规则可以帮助你写出无死锁的代码

//3.2.5 避免死锁的进一步指南

//死锁并不仅仅产生于锁定,虽然这是最常见的诱因,你可以通过两个线程来制造死锁,不用锁定,只需令每个线程在std::thread对象上为另一个线程调用join(),在这种情况下,两个线程都无法取得进展,因为正等着另一个线程完成,就像孩子们争夺他们的玩具,这种简单的循环可以发生在任何地方,一个线程等待另一个线程执行一些动作而另一个线程同时又在等待第一个线程,而且这不仅限于两个线程,三个或更多线程的循环也会导致死锁,避免死锁的准则全都可以归结为一个思路,如果又另外一个线程有可能在等待你,那你就别等他,这个独特的准则为识别和消除别的线程等待你的可能性提供了方法

//1.避免嵌套锁
//第一个思路是最简单的,如果你已经持有一个锁,就别再获取锁,如果你坚持这个准则,光凭使用锁是不可能导致死锁的,因为每个线程仅持有一个锁,你仍然会从其他事情(像是线程相互等待)中得到死锁,但是互斥元锁定可能死锁最常见的诱因,如果需要获取多个锁,为了避免死锁,就以std::lock的单个动作来实行

//2.在持有锁时,避免调用用户提供的代码
//这是前面一条准则的简单后续,因为代码是用户提供的,你不知道它会做什么,它可能做包括获取锁在内的任何事情,如果你在持有锁时调用用户提供的代码,并且这段代码获取一个锁,你就违反了避免嵌套锁的准则,可能导致死锁,有时候这是无法避免的,如果你在编写泛型代码,如3.2.3节中的堆栈,在参数类型上的每一个操作都是用户提供的代码,在这种情况下,你需要新的准则

//3.以固定顺序获取锁
//如果你绝对需要获取两个或更多的锁,并且不能以std::lock的单个操作取得,次优的做法是在每个线程中以相同的顺序获取他们,我在3.2.4接种曾谈及此点,是作为在获取两个互斥元时避免死锁的方法,关键是要以一种在线程间相一致的方法来定义其顺序,在某些情况下,这是相对简单的,例如看一看3.2.3节中的堆栈,互斥元在每个栈实例的内部,但对于储存在栈中的数据项的操作,则需要调用用户提供的代码,然而,你可以添加约束,对于存储在栈中的数据项的操作,都不应对栈本身进行任何操作,这样就增加了栈的使用者的负担,但是将数据存储在一个容器中来访问该容器是很罕见的,并且一旦发生就会十分明显,因此这并不是一个很难承受的负担

//在别的情况下,可能就不那么直观,就像在3.2.4节中你所看到的交换操作那样,至少在这种情况下,你可以同时锁定这些互斥元,但并不总是可能的,如果你回顾一下3.1节中链表的例子,你会看到一种保护链表的可能性,就是让每个节点都有一个互斥元,然后为了访问这个链表,线程必须获取他们感兴趣的每个节点上的锁,要删除的结点以及它两边的结点,因为他们全都要以某种方式进行修改,同样地为了遍历链表,线程在获取序列中下一个结点上的锁的时候,必须保持当前节点上的锁,以确保指向下一结点的指针在此期间不被修改,一旦获取到下一个结点上的锁,就可以释放前面结点上的锁,因为它已经没用了

//这种逐节向上的锁定方式允许多线程访问链表,前提是每个线程访问不同的节点,然而为了避免死锁,必须始终以相同的顺序锁定结点,如果两个线程试图用逐节锁定的方式以相反的顺序便利链表,它们就会在链表中间产生相互死锁,如果结点A和B在链表中相邻,一个方向上的线程会试图保持锁定结点A,并尝试获取结点B上的锁,而另一个方向上的线程会保持锁定结点B,并且尝试获得结点A上的锁,死锁的典型情况

//同样的,当删除位于结点A和C之间的结点B时,如果该线程在获取结点A和C上的锁之前获取B上的锁,他就有可能与遍历链表的线程产生死锁,这样的线程会试图首先锁定A和C(取决于遍历的方向),但是它接下来会发现无法获得结点B上的锁,因为正在进行删除操作的线程持有了结点B上的锁,并试图获得结点A和C上的锁

//在这里防止死锁的一个办法是定义遍历的顺序,让线程必须始终在锁定B之前锁定A,在锁定C之前锁定B,该方法以禁止反向便利为代价来消除产生死锁的可能,对于其他数据结构,常常会建立类似的约定

//4.使用层次锁
//虽然这实际上是定义锁定顺序的一个特例,但锁层次能够提供一种方法,来检查在运行时是否遵循了约定,其思路是将应用程序分层,并且确认所有能够在任意给定的次鞥及上被锁定的互斥元,当代码试图锁定一个互斥元时,如果它在较低层已经持有锁定,那么就不允许它锁定该互斥元,通过给每一个互斥元分配层号,并记录下每个线程都锁定了哪些互斥元,你就可以在运行时进行检查了,清单3.7列出了两个线程使用层次互斥元的例子

//清单3.7 使用锁层次来避免死锁

hierarchical_mutex high_level_mutex(10000);   //①
hierarchical_mutex low_level_mutex(5000);   //②

int do_low_level_stuff() { return 0; }

int low_level_func()
{
    std::lock_guard<hierarchical_mutex> lk(low_level_mutex);   //③
    return do_low_level_stuff();
}

void high_level_stuff(int some_param) {}

void high_level_func()
{
    std::lock_guard<hierarchical_mutex> lk(high_level_mutex);   //④
    high_level_stuff(low_level_func());   //⑤
}

void thread_a()   //⑥
{
    high_level_func();
}

hierarchical_mutex other_mutex(100);   //⑦
void do_other_stuff() {}

void other_func()
{
    high_level_func();   //⑧
    do_other_stuff();
}

void thread_b()   //⑨
{
    std::lock_guard<hierarchical_mutex> lk(other_mutex);   //⑩
    other_func();
}

void some_operation_with_hierarchicalmutext()
{
    thread_a();
    thread_b();
}

//thread_a()⑥遵守了规则,所以它运行良好,另一方面,thread_b()⑨无视了规则,因此将在运行时失败,thread_a()调用high_level_func(),它锁定了high_level_mutex④(具有层次值10000)并接着使用这个锁定了的互斥元调用low_level_func()⑤,以获得high_level_stuff()的参数,low_level_func()接着锁定了low_level_mutex③,但是没有关系,因为该互斥元具有较低的层次值5000②

//在另一方面thread_b()却不妥,刚开始,它锁定了other_mutex⑩,它具有的层次值仅为100⑦,这意味着它应该是保护着超低级别的数据,当other_func()调用high_level_func()⑧时,就会违反层次,high_level_func()试图获取值为10000的high_level_mutex,大大超过100的当前层次值,因此hierarchical_mutex可能通过引发异常或终止程序来报错,层次互斥元之间的死锁是不可能出现的,因为互斥元本身实行了锁定顺序,这还意味着如果两个锁在层次中处于相同级别,你就不能同时持有它们,因此逐节锁定的方案要求链条中的每个互斥元具有比前一个互斥元更低的层次值,在某些情况下,这可能是不切实际的

//这个例子也展现了另外一点,带有用户定义的互斥元类型的std::lock_guard<>模板的使用,hierarchical_mutex不是标准的一部分,但易于编写,清单3.8中展示了一个简单的实现,即便它是个用户定义的类型,但是可以用于std::lock_guard<>这是因为它实现了满足互斥元概念所需要的三个成员函数:lock(),unlock(),try_lock(),你还没有见过直接使用try_lock(),但是它相当简单的,如果互斥元上的锁已被另一个线程持有,则返回false,而非一直等到调用线程可以获取该互斥元上的锁,try_lock()也可以在std::lock()内部,作为避免死锁算法的一部分来使用

//清单3.8 简单的分层次互斥元

//5.将这些设计准则扩展到锁之外
//正如我在本节开始时提到的,死锁不只是出现于锁定中,它可以发生在任何可以导致循环等待的同步结构中,因此,扩展上面所述的准则来涵盖那些情况也是值得的,举个例子,正如你应该尽量避免获取嵌套锁那样,在持有锁时等待第一个线程是坏主意,因为该长城可能需要获取这个锁以继续运行,类似地,如果你正要等待一个线程完成,指定线程层次结构可能也是值得的,这样线程就只需要等待底层次上的线程,一个简单的做到这一点的方法,就是确保你的线程在启动他们的同一个函数中被结合,就像3.1.2节和3.3节中所描述的那样

//一旦你设计了代码来避免死锁,std::lock()和std::lock_guard涵盖了大多数简单锁定的情况,但有时却需要更大的灵活性,在那种情况下,标准库提供了std::unique_lock模板,在与std::lock_guard类似,std::unique_lock是在互斥元类型上进行参数化的类模板,并且它提供了与std::lock_guard相同的RAII风格锁管理,但是更加灵活

//3.2.6 用std::unqiue_lock灵活锁定

//通过松弛不变量,std::unique_lock比std::lock_guard提供了更多的灵活性,一个std::unique_lock实例并不总是拥有与之相关联的互斥元,首先就像你可以把std::adopt_lock作为第二参数传递给构造函数,以便让锁对象来管理互斥元上的锁那样,你也可以把std::unique_lock对象本身传递给std::lock()来获取,使用std::unique_lock和std::defer_lock,而不是std::lock_guard和std::adopt_lock能够很容易地将清单3.6写成清单3.9中所示的那样,这段代码具有相同的行数,并且本质上是等效的,除了一个小问题,std::unique_lock占用更多空间并且使用起来比std::lock_guard略慢,允许std::unique_lock实例不拥有互斥元的灵活性是有代价的,这条信息必须被存储,并且必须被更新

//清单3.9 在交换操作中使用std::lock()和std::unique_lock

class SS
{
private:
    some_big_object some_detail;
    std::mutex m;
public:
    SS(const some_big_object& sd) : some_detail(sd) {}
    friend void swap(SS& lhs, SS& rhs)
    {
        if (&lhs == &rhs)
            return;
        std::unique_lock<std::mutex> lock_a(lhs.m, std::defer_lock);   //①std::defer_lock保留互斥元为未锁定
        std::unique_lock<std::mutex> lock_b(lhs.m, std::defer_lock);   //①std::defer_lock保留互斥元为未锁定
        std::lock(lock_a, lock_b);   //②
        swap(lhs.some_detail, rhs.some_detail);
    }
};

void swap_object_with_unique_lock()
{
    some_big_object object_one;
    object_one.name = "one";
    
    some_big_object object_two;
    object_two.name = "two";
    
    SS s1(object_one);
    SS s2(object_two);
    
    swap(s1, s2);
}

//在清单3.9中,std::unique_lock对象能够被传递给std::lock()②,因为std::unique_lock提供了lock(),try_lock()和unlock()三个成员函数,他们会转发给底层互斥元上同名的成员函数去做实际的工作,并且只是更新在std::unique_lock实例内部的一个标识,来标识该实例当前是否拥有此互斥元,为了确保unlock()在析构函数中被正确调用,这个标识是必须的,如果该实例确已拥有此互斥元,则析构函数必须调用unlock(),并且如果该实例并未拥有此互斥元,则析构函数决不能调用unlock(),可以通过调用owns_lock()成员函数来查询这个标识

//如你所想,这个标识必须被存储在某个地方,因此std::unique_lock对象的大小通常大于std::lock_guard对象,并且相比于std::lock_guard使用std::unique_lock的时候,会有些许性能损失,因为需要对标识进行相应的更新或检查,如果std::lock_guard足以满足要求,我会建议优先使用它,也就是说,还有一些使用std::unique_lock更适合于手头任务的情况,因为你需要利用额外的灵活性,一个例子就是延迟锁定,正如你已经看到的,另一种情况是锁的所有权需要从一个作用域转移到另一个作用域

//3.2.7 在作用域之间转移锁的所有权

//因为std::unique_lock实例并没有拥有与其相关的互斥元,所以通过四处移动实例,互斥元的所有权可以在实例之间进行转移,在某些情况下这种转移是自动的,比如从函数中返回一个实例,而在其他情况下,你必须通过调用std::move()来显示实现,从根本上说,这取决于源是否为左值,实变量或对实变量的引用,或者是右值,某种临时变量,如果源为右值,则左右全转移是自动的,而对于左值,多有权转移必须显示的完成,以避免从变量中意外地转移了所有权,std::unique_lock就是可移动但不可复制的类型的例子,关于移动语义的详情,可参阅附录A中A.1.1节

//一种可能的用法,是允许函数锁定一个互斥元,并将此锁的所有权转移给调用者,于是调用者接下来可以在同一个锁的保护下执行额外的操作,下面的代码片段展示了这样的例子:函数get_lock()锁定了互斥元,然后在将锁返回给调用者之前准备数据

void prepare_data()
{
    std::cout << "void prepare_data()" << std::endl;
}

void do_somestuff()
{
    std::cout << "void do_somestuff()" << std::endl;
}

std::unique_lock<std::mutex> get_lock()
{
    extern std::mutex some_mutex;
    std::unique_lock<std::mutex> lk(some_mutex);
    prepare_data();
    return lk;   //①
}

void process_data()
{
    std::unique_lock<std::mutex> lk(get_lock());   //②
    do_somestuff();
}

//因为lk是在函数内声明的自动变量,它可以被直接返回①而无需调用std::move(),编译器负责调用移动构造函数,process_data()函数可以直接将所有权转移到它自己的std::unique_lock实例②,并且对do_something()的调用能够依赖被正确准备了的数据,而无需另一个线程再次期间去修改数据

//通常使用这种模式,是在待锁定的互斥元依赖于程序的当前状态,或者依赖于传递给返回std::unqiue_lock对象的函数的参数的地方,这种用法之一,unique_lock对象的函数的参数的方法,这种用法之一,就是并不直接返回锁,但是使用一个网关类的数据成员,以确保正确锁定了对受保护的数据的访问,这种情况下,所有对该数据的访问都通过这个网关类,当你想要访问数据时,就获取这个网关类的实例(通过调用类似于前面例子中的get_lock()函数),它会获取锁,然后,你可以通过网关对象的成员函数来访问数据,在完成之后,销毁网关对象,从而释放锁,并允许其他线程访问受保护的数据,这样的网关对象很可能是可移动的(因此它可以从函数返回),在这种情况下,锁对象的数据成员也需要是可移动的

//std::unique_lock的灵活性同样允许实例在被销毁之前撤回他们的锁,你可以使用unlock()尘缘函数来实现,就像对于互斥元那样,std::unique_lock支持与互斥元一样的用来锁定和解锁的基本成员函数集合,这是为了让它可以用于通用函数,比如std::lock在std::unique_lock实例被销毁之前释放锁的能力,意味着你可以有选择的在特定的代码分支释放锁,如果很显然不再需要这个锁,这对于应用程序的性能可能很重要,持有锁的时间比所需时间更长,会导致性能下降,因为其他等待该锁的线程,被阻止运行超过了所需的时间

//3.2.8 锁定在恰当的粒度

//锁粒度是我在之前曾提到过的,在3.2.3节中:锁粒度是一个文字术语,用来描述由单个锁所保护的数据量,细粒度锁保护着少量的数据,粗粒度锁保护着大量的数据,选择一个足够粗的锁粒度,来确保所需的数据都被保护是很重要的,不仅如此,同样重要的是,确保只在真正需要锁的操作中持有锁,我们都知道,带着满满一车杂货在超市排队结账,只因为正在结账的人突然意识到自己忘了一些小红莓酱,然后就跑去找,而让大家都等着,或者收银员已经准备好收钱,顾客才开始在自己的手提包里翻找钱包,是很令人抓狂的,如果每个人去结账时都拿到了他们想要的,并准备好了适当的支付方式,一切都更容易进行

//这同样适用于线程,如果很多个线程正等待着同一个资源(收银台的收银员),然后,如果任意线程持有锁的时间比所需时间长,就会增加等待所花费的总时间(不要等到你已经到了收银台才开始寻找小红莓酱),如果可能,仅在实际访问共享数据的时候锁定互斥元,尝试在锁的外面做任意的数据处理,特别地,在持有锁时,不要做任何确实很耗时的活动,比如文件I/O,文件I/O通常比从内存中读取或写入相同大小的数据量要慢上数百倍(如果不是数千倍),因此,除非这个锁是真的想保护对文件的访问,否则在持有锁时进行I/O会不必要的延迟其他线程(因为它们在等待获取锁时会阻塞),潜在地消除了使用多线程带来的性能提升

//std::unique_lock在这种情况下运作良好,因为能够在代码不再需要访问共享数据时调用unlock(),然后在代码中有需要访问时再次调用lock()

std::mutex the_mutex;

typedef int result_type;

int data_to_process;
int some_class_data_to_process;
int get_next_data_chunk() { return 0; }
int process(int data_to_process) { return 0; }
void write_result(int data_to_process, result_type result) { std::cout << "function write_result: data_to_process = " << data_to_process << ", result = " << result << std::endl; }

void get_and_process_data()
{
    std::unique_lock<std::mutex> my_lock(the_mutex);
    some_class_data_to_process = get_next_data_chunk();
    my_lock.unlock();   //①在对process的调用中不需要锁定互斥元
    result_type result = process(data_to_process);
    my_lock.lock();   //②重新锁定互斥元以回写结果
    write_result(data_to_process, result);
}

//在调用process()过程中不需要锁定互斥元,所以手动地将其在调用前解锁①,并在之后再次锁定②

//希望这是显而易见的,如果你让一个互斥锁保护整个数据结构,不仅可能会有更多的对锁的竞争,锁被持有的时间也可能会减少,更多的操作步骤会需要在同一个互斥元上的锁,所以所必须被持有更长的时间,这种成本上的双重打击,也是尽可能走向细粒度锁定的双重激励

//如这个例子所示,锁定在恰当的粒度不仅关乎锁定的数据量,这也是关系到锁会被持有多长时间,以及在持有锁时执行哪些操作,一般情况下,只有应该以执行要求的擦偶作所需的最小可能时间而去持有锁,这也意味着耗时的操作,比如获取另一个锁(即便你知道它不会死锁)或是等待I/O完成,都不应该在持有锁的时候去做,除非绝对必要。

//在清单3.6和清单3.9中,需要锁定两个互斥锁的操作是交换操作,这显然是需要并发访问两个对象,假设取而代之,你试图去比较仅为普通int的简单数据成员,这会有区别吗？int可以轻易被复制,所以你可以很容易地为每个待比较的对象复制器数据,同时只用持有该对象的锁,然后比较已复制数值,这意味着你在每个互斥元上持有锁的时间最短,并且你也没有在持有一个锁的时候去锁定另外一个,清单3.10展示了这样一个类Y,以及相等比较运算符的示例实现

//清单3.10 在比较运算符中每次锁定一个互斥元

class Y {
private:
    int some_detail;
    mutable std::mutex m;
    
    int get_detail() const
    {
        std::lock_guard<std::mutex> lock_a(m);   //①
        return some_detail;
    }
public:
    Y(int sd) : some_detail(sd) {}
    friend bool operator==(const Y& lhs, const Y& rhs)
    {
        if (&lhs == &rhs)
            return true;
        const int lhs_value = lhs.get_detail();   //②
        const int rhs_value = rhs.get_detail();   //③
        return lhs_value == rhs_value;   //④
    }
};

std::mutex y_lock;

void race_condition_come_if_operation_unprotected_by_a_lock()
{
    std::lock_guard<std::mutex> g(y_lock);
    Y y1(1);
    Y y2(2);
    std::cout << "y1" << (y1 == y2 ? " = " : " != ") << "y2" << std::endl;
}

//在这种情况下,比较运算符首先通过调用get_detail()成员函数获取要进行比较的值②和③,此函数在获取值得同时用一个锁来保护它①,比较运算符接着比较获取到的值④,但是请注意,这同样会减少锁定的时间,而且每次只持有一个锁(从而消除了死锁的可能性),与同时持有两个锁相比,这巧妙地改变了操作语义,在清单3.10中,如果运算符返回true,意味着lhs.some_detail在一个时间点的值与rhs.some_detail在另一个时间点的值相等,这两个值能够在两次读取之中以任何方式改变,例如,这两个值可能在②和③之间进行了交换,从而使这个比较变得毫无意义,这个相等比较可能返回true来表示值是相等的,即使这两个值在某个瞬间从未真正地相等的,即使这两个值在某个瞬间从未真正地相等过,因此,当进行这样的改变时小心注意是很重要的,操作的语义不能以有问题的方式而被改变,如果你不能在操作的整个持续时间中持有所需的锁,你就把自己暴露在竞争条件中

//有时,根本就没有一个合适IDE粒度级别,因为并非所有的数据结构的访问都要求同样级别的保护,因为并非所有的对数据结构的访问都要求同样级别的保护,在这种情况下,使用替代机制来代替普通的std::mutex可能才是恰当的

//3.3 用于共享数据保护的替代工具

//虽然互斥元是最通用的机制,但提到保护共享数据时,他们并不是唯一的选择,还有别的替代品,可以在特定情况下提供更恰当的保护

//一个特别极端(但却相当常见)的情况,就是共享数据只在初始化才需要并发访问的保护,但在那之后却不需要显示同步,这可能使因为数据是已经创建就是只读的,所以就不存在可能的同步问题,或者是因为必要的保护作为数据上操作的一部分被隐式地执行,在任何一种情况中,在数据被初始化之后锁定互斥元,纯粹是为了保护初始化,这是不必要的,并且对性能会产生的不必要的打击,为了这个原因,C++标准提供了一种机制,纯粹为了在初始化过程中保护共享数据

//3.3.1 在初始化时保护共享数据
//假设你有一个构造起来非常昂贵的共享资源,只有当实际需要时你才会要这样做,也许,它会打开一个数据库连接或分配大量的内存,像这样的延迟初始化在单线程中是很常见的,每个请求资源的操作首先检查它是否已经初始化,如果没有就在使用之前初始化之

class some_resource
{
public:
    void do_something() { std::cout << "some_resource::do_something()" << std::endl; }
};

std::shared_ptr<some_resource> resource_ptr;

void init_or_get_resource()
{
    if (!resource_ptr)
        resource_ptr.reset(new some_resource);
    resource_ptr->do_something();   //①
}

//如果共享资源本身对于并发访问是安全的,当将其转换为多线程代码时唯一需要保护的部分就是初始化①,但是像清单3.11中这样的朴素的转换,会引起使用该资源的线程产生不必要的序列化,这是因为每个线程都必须等待互斥元,以检查资源是否已经被初始化

//清单3.11 使用互斥元进行线程安全的延迟初始化

std::mutex resource_mutex;

void init_or_get_resource_with_lock()
{
    std::unique_lock<std::mutex> lk(resource_mutex);   //所有的线程在这里被序列化
    if (!resource_ptr)
        resource_ptr.reset(new some_resource);   //只有初始化需要被保护
    lk.unlock();
    resource_ptr->do_something();
}

//这段代码是很常见的,不必要的序列化问题已足够大,以至于许多人都试图想出一个更好的方法来实现,包括臭名昭著的二次检查锁定(Double-CheckedLocking)模式,在不获取锁①(在下面的代码中)的情况下首次读取指针,并仅当次指针为NULL时获得该锁,一旦已经获取了锁,该指针要被再次检查②(这就是二次检查的部分),以防止在首次检查和这个线程获取锁之间,另一个线程就已经完成了初始化

void undefined_behaviour_with_double_checked_locking()
{
    if (!resource_ptr)   //①
    {
        std::lock_guard<std::mutex> lk(resource_mutex);
        if (!resource_ptr)   //②
            resource_ptr.reset(new some_resource);   //③
        resource_ptr->do_something();   //④
    }
}

//不幸的是,这种模式因某个原因而臭名昭著,它有可能产生恶劣的竞争条件,因为在锁外部的读取①与锁内部由另一线程完成的写入不同步③,这就因此创建了一个竞争条件,不仅涵盖了指针本身,还涵盖了指向的对象,就算一个线程看到另一个线程写入的指针,它也可能无法看到新创建的some_resource实例,从而导致do_something()④的调用在不正确的值上运行,这是一个竞争条件的例子,该类型的竞争条件被C++标准定义为数据竞争,因此被定义为未定义行为,因此这是肯定要避免的,内存模型的详细讨论参见第五章,包括了什么构成数据竞争

//C++标准委员会也发现这是一个重要的场景,所以C++标准库提供了std::once_flag和std::call_once来处理这种情况,与其锁定互斥元并显示地检查指针,还不如每个线程都可以使用std::call_once,到std::call_once返回时,指针将会被某个线程初始化(以完全同步的方式),这样就安全了,使用std::call_once比现实使用互斥元通常会有更低的开销,特别是初始化已经完成的时候,所以在std::call_once符合所要求的功能时应优先使用之,下面的例子展示了与清单3.11相同的操作,改写为使用std::call_once,在这种情况下,通过调用函数来完成初始化,但是通过一个担忧函数调用操作符的类实例也可以很容易地完成初始化,与标准库中接受函数或者断言作为参数的大部分函数类似,std::call_once可以与任意函数或可调用对象合作

std::once_flag resource_flag;   //①

void init_resource()
{
    resource_ptr.reset(new some_resource);
}

void init_resource_with_call_one_and_once_flag()
{
    std::call_once(resource_flag, init_resource);
    resource_ptr->do_something();
}

//在这个例子中,std::once_flag①和被初始化的数据都是命名空间作用域的对象,但是std::call_once()可以很容易地用于类成员的延迟初始化,如清单3.12所示

//清单3.12 使用std::call_once的线程安全的类成员延迟初始化

struct data_packet {};
struct connection_info {};
struct connection_handle
{
    void send_data(const data_packet& packet) {}
    data_packet receive_data() { return data_packet(); }
};
struct connection_manager
{
    static connection_handle open(const connection_info& connection_info) { return connection_handle(); }
};

class D
{
private:
    connection_info connection_details;
    connection_handle connection;
    std::once_flag connection_init_flag;
    
    void open_connection()
    {
        connection = connection_manager::open(connection_details);
    }
    
public:
    D(const connection_info& connection_details_) : connection_details(connection_details_) {}
    
    void send_data(const data_packet& data)   //①
    {
        std::call_once(connection_init_flag, &D::open_connection, this);   //②
        connection.send_data(data);
    }
    
    data_packet receive_data()   //③
    {
        std::call_once(connection_init_flag, &D::open_connection, this);   //②
        return connection.receive_data();
    }
};

void some_example_on_init_resource_with_call_once()
{
    D d{connection_info()};
}

//在这个例子中,初始化由首次调用send_data()①或是由首次调用receive_data()③来完成,使用成员函数open_connection()来初始化数据,同样需要将this指针传入函数,和标准库中其他接受可调用对象的函数一样,比如std::thread和std::bind()的构造函数,这是通过传递一个额外的参数给std::call_once()来完成的②

//值得注意的是,像std::mutex、std::once_flag的实例是不能被复制或移动的,所以如果想要这样把它们作为类成员来使用,就必须显示定义这些你所需要的特殊成员函数

//一个在初始化过程中可能会有竞争条件的场景,是将局部变量声明为static的,这种变量的初始化,被定义为在时间控制首次经过其声明时发生,对于多个调用该函数的线程,这意味着可能会有针对定义"首次"的竞争条件,在许多C++11之前的编译器上,这个竞争条件在实践中是有问题的,因为多个线程可能都认为他们是第一个,并试图去初始化该变量,又或者线程可能都认为它们是第一个,并试图去初始化该变量,又或者线程可能会在初始化已在另一个线程上启动但尚未完成之时试图使用它,在C++11中,这个问题得到了解决,初始化被定义为只发生在一个线程上,并且其他线程不可以继续直到初始化完成,所以竞争条件仅仅在于哪个线程会执行初始化,而不会有更多别的问题,对于需要单一全局实例的场合,这可以用作std::call_once的替代品

class my_class {};

my_class& get_my_class_instance()
{
    static my_class instance;   //①初始化保证线程是安全的
    return instance;
}

//多个线程可以继续安全地调用get_my_class_instance()①,而不必担心初始化时的竞争条件

//保护仅用于初始化的数据是更普遍的场景下的一个特例,那些很少更新的数据结构,对于大多数时间而言,这样的数据结构是只读的,因而可以毫无顾忌地被多个线程同时读取,但是数据结构偶尔可能需要更新,这里我们所需要的是一种承认这一事实的保护机制

//3.3.2 保护很少更新的数据结构

//假设有一个用于存储DNS条目缓存的表,它用来将域名解析为相应的IP地址,通常,一个给定的DNS条目将在很长一段时间里保持不变,在许多情况下,DNS条目会保持数年不变,虽然随着用户访问不同的网站,新的条目可能会被不时地添加到表中,但这一数据却将在其整个生命中基本保持不变,定期检查缓存条目的有效性是很重要的,但是只有在细节已有实际改变的时候才会需要更新

//虽然更新是罕见的,但他们仍然会发生,并且如果这个缓存可以从很多个线程访问,他就需要在更新过程中进行适当的保护,以确保所有线程在读取缓存时都不会看到损坏的数据结构

//在缺乏完全预期用法并且为并发更新与读取专门设计(例如在第六章和第七章的那些)的专用数据结构的情况下,这种更新要求线程在进行更新时独占访问数据结构,直到它完成了操作,一旦更新完成,该数据结构对于多线程并发访问又是安全的了,使用std::mutex来保护数据结构就因而显得过于悲观,因为这会在数据结构没有进行修改时消除并发读取数据结构的可能,我们需要的是另一种互斥元,这种新的互斥元通常称为读写互斥元,因为它考虑到了两种不同的用法,由单个"写线程"独占访问或共享,由多个"读"线程并发访问

//新的C++标准库并没有直接提供这样的互斥元,尽管已向标准委员会提议,由于这个建议未被接纳,本节中的例子使用由Boost库提供的实现,它是基于这个建议的,在第8章中你会看到,使用这样的互斥元并不是万能药,性能依赖于处理器的数量以及读线程和更新线程的相对工作负载,因此,分析代码在目标系统上的性能是很重要的,以确保额外的复杂度会有实际的收益

//你可以使用boost::shared_mute的实例来实现同步,而不是使用std::mutex的实例,对于更新操作,std::lock_guard<boost::shared_mutex>和std::unique_lock<boost::shared_mutex>可用于锁定,以取代相应的std::mutex特化,这确保了独占访问,就像std::mutex那样,那些不需要更新数据结构的线程能够转而使用boost::shared_lock<boost::shared_mutex>来获得共享访问,这与std::unique_lock用起来是相同的,除了多个线程在同一时间,同一boost::share_mutex上可能会具有共享锁,唯一限制是,如果任意一个线程拥有一个共享锁,试图获取独占锁的线程会被阻塞,直到其他线程全都撤回它们的锁,同样地,如果任意一个线程具有独占锁,其他线程都不能获取共享锁或独占锁,直到第一个线程撤回了它的锁

//清单3.13展示了一个简单的如前面所描述的DNS缓存,使用std::map来保存缓存数据,用boost::share_mutex进行保护

//清单3.13 使用boost::shared_mutex保护数据结构

#include <map>
#include <mutex>
#include <string>

/*

#include <boost/thread/shared_mutex.hpp>

class dns_entry;

class dns_cache
{
    std::map<std::string, dns_entry> entries;
    mutable boost::shared_mutex entry_mutex;
public:
    dns_entry find_entry(const std::string& domain) const
    {
        boost::shared_lock<boost::shared_mutex> lk(entry_mutex);   //①
        const std::map<std::string,dns_entry>::const_iterator it = entries.find(domain);
        return (it == entries.end()) ? dns_entry() : it->second;
    }
    
    void update_or_add_entry(const std::string& domain, const dns_entry& dns_details)
    {
        std::lock_guard<boost::shared_mutex> lk(entry_mutex);   //②
        entries[domain] = dns_details;
    }
}
 
*/

//在清单3.13中,find_entry()使用一个boost::shared_lock<>实例来保护它,以供共享,只读的访问①,多个线程因而可以毫无问题地同时调用find_entry(),另一方面,update_or_add_entry()使用一个std::lock_guard<>实例,在表被更新时提供独占访问②,不仅在调用update_or_add_entry()中其他线程被阻止进行更新,调用find_entry()的线程也会被阻塞

//3.3.3 递归锁

//在使用std::mutex的情况下,一个线程试图锁定其已经拥有的互斥元是错误的,并且试图这么做将导致未定义行为,然而,在某些情况下,线程多次重新获取同一个互斥元却无需实现释放它是可取的,为了这个目的,C++标准库提供了std::recursive_mutex,它就像std::mutex一样,区别在于你可以在同一个线程中的单个实例上获取多个锁,在互斥元能够被另一个线程锁定之前,你必须释放所有的锁,因此如果你调用lock()三次,你必须也调用unlock()三次,正确使用std::lock_guard<std::recursive_mutex>和std::unique_lock<std::recursive_mutex>将会为你处理

//大多数时间,如果你觉得需要一个递归互斥元,你可能反而需要改变你的设计,递归互斥元常用在一个类被设计成多线程并发访问的情况中,因此它具有一个互斥元来保护成员数据,每个公共成员函数锁定互斥元,进行工作,然后解锁互斥元,然而,有时一个公共成员函数调用另一个函数作为其操作的一部分是可取的,在这种情况下,第二个成员函数也将尝试锁定该互斥元,从而导致未定义行为,粗制滥造的解决方案,就是将互斥元改为递归互斥元,这将允许在第二个成员函数中对互斥元的锁定成功进行,并且函数继续

//然而,这样的用法是不推荐的,因为它可能导致草率的想法和糟糕的设计,特别地,类的不变量在锁被持有时通常是损坏的,这意味着第二个成员函数需要工作,即便在被调用时使用的是损坏的不变量,通常最好是提取一个新的私有成员函数,该函数是从这两个成员函数中调用的,它不锁定互斥元(它认为互斥元已经被锁定),然后,你可以仔细想想在什么情况下可以调用这个新函数以及在那些情况下数据的状态

//在本章中,我讨论了在线程之间共享数据时,有问题的竞争条件如何成为灾难,以及怎样使用std::mutex和精心设计接口以避免它们,你看到了互斥元不是万能药,也有它们自己的以死锁出现的问题,尽管C++标准库以std::lock()的形式提供了工具来帮助避免死锁,然后你看到了一些进一步的技术来避免死锁,接着简要看了一下锁的所有权转让,以围绕着为锁选择恰当的粒度的问题,最后,我介绍了为特定场景提供的替代的数据保护工具,例如std::call_once()和boost::shared_mutex

//然而,还有一件事我没有提到,就是等待来自其他线程的输入,我们的线程安全栈在栈为空的情况下只是引发异常,因此如果一个线程需要等待另一个线程来将一个值压入栈中(毕竟,这是线程安全栈的主要用途之一),它将不得不反复尝试弹出值,如果引发异常则重试,这会消耗宝贵的处理时间进行检查,而没有实际取得任何进展,的确,不断地检查可能会通过阻止系统中其他线程的运行而阻碍进度,我们需要的是以某种方法让一个线程等待另一个线程完成任务,而无需耗费CPU时间,第四章构建在已经讨论过的用于保护共享数据的工具上,介绍了C++中用于线程间同步操作的各种机制,第六章展示了如何使用它们来构建更大的可复用的数据结构

//3.4 小节

//在本章中,我讨论了在线程之间共享数据时,有问题的竞争条件如何成为灾难,以及怎样使用std::mutex和精心设计接口以避免它们,你看到了互斥元不是万能药,也有它们自己的以死锁形式出现的问题,尽管C++标准库以std::lock()的形式提供了工具来帮助避免死锁,然后,你看到了一些进一步的技术来避免死锁,接着简要看了一下所的所有权的转让,以围绕着为锁选择恰当的粒度的问题,最后,我介绍了为特定场景提供的替代的数据保护工具,例如std::call_once()和boost::shared_mutex

//然而,还有一件事我没有提到,就是等待来自其他线程的输入,我们的线程安全栈在栈为空的情况下只是引发异常,因此如果一个线程需要等待另一个线程来将一个值压入栈中(毕竟,这是线程安全栈的主要用途之一),它将不得不反复尝试弹出值,如果引发异常则重试,这回消耗宝贵的处理时间来进行检查,而没有实际取得任何进展,的确,不断地检查可能会通过阻止系统中其他现场的运行而阻碍进度,我么你需要的是以某种方法让一个线程等待另一个线程完成任务,而无需耗费CPU时间,第4章构建在已经讨论过的用于保护共享数据的工具上,介绍了C++中用于线程间同步操作的各种机制,第6章展示了如何使用它们来构建更大的可复用的数据结构

#endif /* _3_0_h */

