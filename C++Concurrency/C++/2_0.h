//
//  2_0.h
//  C++Concurrency
//
//  Created by chenyanan on 2017/4/20.
//  Copyright © 2017年 chenyanan. All rights reserved.
//

#ifndef _2_0_H
#define _2_0_H

#include <iostream>
#include <thread>
#include <string>
#include <memory>
#include <vector>
#include <numeric>
#include <iterator>
#include "assert.h"

//第2章 线程管理

//本章主要内容
//启动新线程
//等待线程与分离线程
//线程唯一标识符

//好的,看来你已经决定使用多线程了,先做点什么呢？启动线程、结束线程,还是如何监管线程？在C++标准库中只需要管理std::thread关联的线程,无需把注意力放在其他方面,不过,标准库太灵活,所以管理起来不会太容易

//本章将从基本开始,启动一个线程,等待这个线程结束,或放在后台运行,再看看怎么给已经启动的线程函数传递参数,以及怎么将一个线程的所有权从当前std::thread对象移交给另一个,最后,再来确定线程数,以及识别特殊线程

//2.1 线程管理的基础

//每个程序至少有一个线程,执行main()函数的线程是原始线程,其余线程有其各自的入口函数,线程与原始线程(以main()为入口函数的线程)同时运行,如同main()函数执行完全会退出一样,当线程执行完入口函数后,线程也会退出,在为另一个线程创建了一个std::thread对象后,需要等待这个线程结束,不过,线程需要先进行启动,下面就来启动线程

//2.1.1 启动线程

//第1章中,线程在std::thread对象创建(为线程制定任务)时启动,最简单的情况下,任务也会很简单,通常是无参数无返回(void-returning)的函数,这种函数在其所属线程上运行,知道函数执行完毕,线程也就结束了,在一些极端情况下,线程运行时,任务中的函数对象需要通过某种通讯机制进行参数的传递,或者执行一系列独立操作,可以通过通讯机制传递信号,让线程停止,线程要做什么,以及什么时候启动,其实都无关紧要,总之,使用C++线程库启动线程,可以归结为结构std::thread对象:

void do_some_work()
{
    std::cout << "void do_some_work()" << std::endl;
}

std::thread my_thread_1(do_some_work);

//为了让编译器识别std::thread类,这个简单的例子也要包含<thread>头文件,如同大多数C++标准库一样,std::thread可以用可调用(callable)类型构造,将带有函数调用符类型的实例传入std::thread类中,替换默认的构造函数

void do_something()
{
    std::cout << "void do_something()" << std::endl;
}

void do_something_else()
{
    std::cout << "void do_something_else()" << std::endl;
}

class background_task
{
public:
    void operator()() const
    {
        do_something();
        do_something_else();
    }
};

background_task f;
std::thread my_thread_2(f);

//代码中,提供的函数对象会复制到新线程的存储空间当中,函数对象的执行和调用都在线程的内存空间中进行,函数对象的副本应与原始函数对象保持一致,否则得到的结果会与我们的期望不同

//有件事需要注意,当把函数对象传入到线程构造函数中时,需要避免"最令人头痛的语法解析"(C++'s most vexing parse,中文简介),如果你传递了一个临时变量,而不是一个命名的变量,C++编译器会将其解析为函数声明,而不是类型对象的定义

//例如：

std::thread not_thread(background_task());

//这里相当于声明了一个名为not_thread的函数,这个函数带有一个参数(函数指针指向没有参数并返回background_taskd对象的函数),返回一个std::thread对象的函数,而非启动了一个线程

//使用在前面明明函数对象的方式,或使用多组括号①,或使用新同意的初始化语法②,可以避免这个问题

//如下所示:

std::thread my_thread_3((background_task()));   //①
std::thread my_thread_4{background_task()};   //②

//使用lambda表达式也能避免这个问题,lambda表达式是C++11的一个新特性,它允许使用一个可以捕获局部变量的局部函数(可以避免传递参数,参见2.2节),想要具体的了解lambda表达式,可以阅读附录A的A.5节,之前的例子可以改写为lambda表达式的类型:

std::thread my_thread_5([]{
    do_something();
    do_something_else();
});

//启动了线程,你需要明确是要等待线程结束(加入式,参见2.1.2节),还是让其自主运行(分离式,参见2.1.3节),如果std::thread对象销毁之前还米有做出决定,程序就会终止(std::thread的析构函数会调用std::terminate()),因此,即便是有异常存在,也需要确保线程能够正确的加入(joined)或分离(detached),2.1.3节中,会介绍对应的方法来处理这两种情况,需要注意的是,必须在std::thread对象销毁之前做出决定,加入或分离线程之前,如果线程就已经结束,想再去分离它,线程可能会在std::thread对象销毁之后继续运行下去

//如果不等待线程,就必须保证线程结束之前,可访问的数据得有效性,这不是一个新问题,单线程代码中,对象销毁之后再去访问,也会产生未定义行为,不过,线程的生命周期增加了这个问题发生的几率

//这种情况很可能发生在线程还没结束,函数已经退出的时候,这时线程函数还持有函数局部变量的指针或引用,下面的清单中就展示了这样一种情况

//清单2.1 函数已经结束,线程依旧访问局部变量

void do_something_with_int(int i)
{
    std::cout << "i = " << i << std::endl;
}

struct func
{
    int& i;
    func(int& i_) : i(i_) {}
    void operator()()
    {
        for (unsigned j = 0; j < 100; ++j)
            do_something_with_int(i);   //①潜在访问隐患:悬空引用
    }
};

void thread_detach_use_some_local_object()
{
    int some_local_state = 0;
    func local_object(some_local_state);
    std::thread my_thread(local_object);
    my_thread.detach();   //②不等待线程结束
}                         //③新线程可能还在运行

//这个例子中,已经决定了不等待线程结束(使用了detach()②),所以当thread_detach_use_some_local_object()函数执行完成时③,新线程中的函数可能还在运行,如果线程还在运行,他就会去调用do_something_with_int()①,这时就会访问已经销毁的变量,如同一个单线程程序,允许在函数完成后继续持有局部变量的指针或引用,当然,这从来就不是一个好主意,这种情况发生时,错误并不明显,会使多线程更容易出错

//处理这种情况的常规方法:使线程函数的功能齐全,将数据复制到线程中,而非复制到共享数据中,如果使用一个可调用的对象作为线程函数,这个对象就会复制到线程中,而后原始对象就会立即销毁,但对于对象中包含的指针和引用还需谨慎,例如清单2.1所示,使用一个能访问全局变量的函数去创建线程是一个糟糕的注意(除非十分确定线程会在函数完成前结束),此外,可以通过join()的方式来确保线程在函数完成前结束

//2.1.2 等待线程完成

//如果需要等待线程,相关的std::thread实例需要使用join(),清单2.1中,将my_thread.detach()替换为my_thread.join(),就可以确保局部变量在线程完成后,才被销毁,在这种情况下,因为原始线程再起生命周期中并没有做什么事,使得用一个独立的线程去执行函数变得收益甚微,但在实际编程中,原始线程要么有自己的工作要做,要么会启动多个子线程来做一些有用的工作,并等待这些线程结束

//join()是简单粗暴的等待线程完成或不等待,当你需要对等待中的线程有更灵活的控制时,比如,看一下某个线程是否结束,或者只等待一段时间(超过时间就判定为超时),想要做到这些,你需要使用其他机制来完成,比如条件变量和期待(futures),相关的讨论将会在第4章继续,调用join()的行为,还清理了线程相关的存储部分,这样std::thread对象将不再与已经完成的线程有任何关联,这意味着,只能对一个线程使用一次join(),一旦已经使用过join(),std::thread对象就不能再次加入了,当对其使用joinable()时,将返回否(false)

//2.1.3 特殊情况下的等待

//如前所述,需要对一个还未销毁的std::thread对象使用join()或detach(),如果想要分离一个线程,可以在线程启动后,直接使用detach()进行分离,如果打算等待对应线程,字需要细心挑选调用join()的位置,当在线程运行之后产生异常,在join()调用之前跑出,就意味着这次调用湖北跳过

//为了避免应用程序在引发异常的时候被终止,你需要在这种情况决定要做什么,一般来说,如果你打算在非异常的情况下调用join(),你还需要在存在异常时调用join(),以避免意外的生命周期问题,清单2.2展示了这样的简单代码

//清单2.2 等待线程完成

void do_something_in_current_thread()
{
    std::cout << "void do_something_in_current_thread()" << std::endl;
}

struct func;   //定义在清单2.1中
void thread_join_in_catch_block_when_throw_before_default_join_call()
{
    int some_local_state = 0;
    func local_object(some_local_state);
    std::thread t(local_object);
    try
    {
        do_something_in_current_thread();
    }
    catch(...)
    {
        t.join();   //①
        throw;
    }
    t.join();   //②
}

//清单2.2中的代码使用了try/catch块,以确保访问局部状态的线程在函数退出前结束,无论函数是正常退出②还是异常①中断,使用try/catch块很啰嗦,而且很容易将作用域弄乱,所以并不是一个理想的方案,如果确保线程必须在函数退出前完成是很重要的,无论是因为它具有对其他局部变量的引用还是任何其他原因,那么确保这是所有可能的退出路径的情况是很重要的,无论正常还是异常,并且希望提供一个这样做的简单明了的机制

//这样做的方法之一是使用标准的资源获取即初始化(RAII)惯用语法,并提供一个类,在它的析构函数中进行join(),正如清单2.3的代码,看看它是如何简化函数use_thread_guard()的

//清单2.3 使用RAII等待线程完成

class thread_guard
{
    std::thread& t;
public:
    explicit thread_guard(std::thread& t_) : t(t_) {}
    ~thread_guard()
    {
        if (t.joinable())   //①
            t.join();   //②
    }
    thread_guard(const thread_guard&) = delete;   //③
    thread_guard& operator=(const thread_guard&) = delete;
};

void use_thread_guard()
{
    int some_local_state = 0;
    func local_object(some_local_state);
    std::thread t(local_object);
    thread_guard g(t);
    do_something_in_current_thread();
}   //④

//当线程执行到④处时,局部对象就要被逆序销毁了,因此,thread_guard对象g是第一个被销毁的,这时线程在析构函数中被加入②到原始线程中,即使do_something_in_current_thread抛出一个异常,这个销毁依旧会发生

//在thread_guard的析构函数的测试中,首先判断线程是否已经加入①,如果没有会调用join()②进行加入,这很重要,因为join()只能对给定的对象调用一次,所以对给已加入的线程再次进行加入操作时,将会导致错误

//拷贝构造函数和拷贝赋值操作被标记为=delete③,是为了不让编译器自动生成他们,直接对一个对象进行拷贝或赋值是危险的,因为这可能会弄丢已经加入的线程,通过删除声明,任何尝试给thread_guard对象赋值的操作都会引发一个编译错误,想要了解删除函数的更多知识,请参阅附录A的A.2节

//如果不想等待线程结束,可以分离(detaching)线程,从而避免异常安全(exception-safety)问题,不过,这就打破了线程与std::thread对象的联系,即使线程仍然在后台运行着,分离操作也能确保std::terminate()在std::thread对象销毁才被调用

//2.1.4 后台运行线程

//使用detach()会让线程在后台运行,这就意味着主线程不能与之产生直接交互,也就是说,不会等待这个线程结束,如果线程分离,那么就不可能有std::thread对象能引用它,分离的线程的确在后台运行,所以分离线程不能被加入,不过C++运行库保证,当线程退出时,相关资源的能欧正确回收,后台线程的归属和控制C++运行库都会处理

//通常称分离线程为守护线程(daemon threads),UNIX中守护线程是指,且没有任何用户接口,并在后台运行的线程,这种线程的特点就是长时间运行,线程的生命周期可能会从某一个应用起始到结束,可能会在后台监视文件系统,还会有可能对缓存进行清理,亦或对数据结构进行优化,另一方面,分离线程的另一方面只能确定线程什么时候结束,"发后即忘"(fire and forget)的任务就使用到变成的这种方式

//如2.1.2节所示,调用std::thread成员函数detach()来分离一个线程,之后,相应的std::thread对象就与实际执行的线程无关了,并且这个线程也无法进行加入:

void background_work()
{
    std::cout << "void background_work()" << std::endl;
}

void do_background_work()
{
    std::thread t(background_work);
    t.detach();
    assert(!t.joinable());
}

//为了从std::thread对象中分离线程(前提是有可进行分离的线程):不能对没有执行线程的std::thread对象使用detach(),也是join()的使用条件,并且要用同样的方式进行检查,当std::thread对象使用t.joinable()返回的是true,就可以使用t.detach()

//试想如何能让一个文字处理应用同时编辑多个文档,无论是用户界面,还是在内部应用内部进行,都有很多的解决方法,虽然,这些窗口看起来是完全独立的,每个窗口都有自己独立的菜单选项,但他们却运行在同一个应用实例中,一种内部处理方式是,让每个文档处理窗口都拥有自己的线程,每个线程运行童颜的代码,并隔离不同窗口处理的数据,如此这般,打开一个文档就要启动一个新线程,因为是对独立的文档进行操作,所以没有必要等待其他线程完成,因此,这里就可以让文档处理窗口运行在分离的线程上

//夏下面代码简要的展示了这种方法:

//清单2.4使用分离线程去处理其他文档

enum user_command_type
{
    open_new_document,
    other_command
};

struct user_command
{
    user_command_type _type;
    user_command(user_command_type type_) : _type(type_) {}
};

void open_document_and_display_gui(const std::string& filename)
{
    std::cout << "void open_document_and_display_gui(const std::string& filename) : filename = "
    << filename << std::endl;
    std::cout << "thread_id = " << std::this_thread::get_id() << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));
}

bool done_editing()
{
    static int count = 0;
    static std::mutex m;
    std::lock_guard<std::mutex> g(m);
    bool status = count++ < 4 ? false : true;
    std::cout << (status ? "edit done" : "edit not yet")<< std::endl;
    std::cout << "thread_id = " << std::this_thread::get_id() << std::endl;
    return status;
}

user_command get_user_input()
{
    static int count = 0;
    user_command cmd = count++ < 4 ? user_command(open_new_document) : user_command(other_command);
    std::cout << "user_command get_user_input() : input = " <<
    (cmd._type == open_new_document ? "open_new_document" : "other_command") << std::endl;
    return cmd;
}

std::string get_filename_from_user()
{
    static const char* files[] = {"file_one","file_two","file_three","file_four","file_five"};
    static int index = 0;
    return files[index++];
}

void process_user_input(const user_command& cmd_)
{
    std::cout << "void process_user_input(const user_command& cmd_) : cmd_ = " <<
    (cmd_._type == open_new_document ? "open_new_document" : "other_command")<< std::endl;
}

void edit_document(const std::string& filename)
{
    std::cout << "void edit_document(const std::string& filename) : filename = " << filename << std::endl;
    
    open_document_and_display_gui(filename);
    
    while(!done_editing())
    {
        user_command cmd = get_user_input();
        if (cmd._type == open_new_document)
        {
            const std::string new_name = get_filename_from_user();
            std::thread t(edit_document, new_name);   //①
            t.detach();   //②
        }
        else
        {
            process_user_input(cmd);
        }
    }
}

//如果用户选择打开一个新的文档,他会提示其有文档要打开,启动新线程来打开文档①,然后分离它②,因为新的线程与当前线程作着同样的操作,只是文件不同,你可以用新选定的文件名作为参数,重用同一个函数(edit_document)

//这个例子还展示了一个案例,它有助于传递参数给用来启动线程的函数:并非仅仅将函数传递给std::thread构造函数①,你还可以传递文件名参数,虽然也有其他机制能够做到这一点,例如使用具有成员数据的函数对象取代普通的带有参数的函数,但线程提供了一个简单方法来实现之

//2.2 向线程函数传递参数

//清单2.4中,向std::thread构造函数中的可调用对象,或函数传递一个参数很简单,需要注意的是,默认参数要拷贝到线程独立内存中,即使参数是引用的形式,也可以在线程中进行访问,再来看一个例子

void f_0(int i, const std::string& s)
{
    std::cout << "void f_0(int i, std::string const& s)" << std::endl;
}

std::thread t_f_0(f_0, 3, "hello");

//这里创建一个新的与t_f_0相关联的执行线程,称为f(3,"hello"),注意即使f接受std::string作为第二个参数,字符串字面值仅在新线程的上下文中才会作为const char *传送,并转换为std::string尤其重要的是当提供的参数是一个自动变量的指针时,如下所示

void oops(int some_param)
{
    char buffer[1024];   //①
    sprintf(buffer, "%i", some_param);
    std::thread t(f_0, 3, buffer);   //②
    t.detach();
}

//在这种情况下,正是局部变量buffer①的指针被传递给新线程②,还有一个重要的时机,即函数oops会在缓冲在新线程上被转换为std::string之前退出,从而导致未定义的行为,解决之道是在将缓冲传递给std::thread的构造函数之前转换为std::string

void not_oops(int some_param)
{
    char buffer[1024];
    sprintf(buffer, "%i", some_param);
    std::thread t(f_0, 3, std::string(buffer));
    t.detach();
}

//在这种情况下,问题就出在你依赖从缓冲的指针得到函数所期望的std::string对象的隐式转换,因为std::thread构造函数原样复制了所提供的值,并未转换为期望的参数类型

//也有可能得到相反的情况,对象被复制,而你想要的是引用,这可能发生在当线程正在更新一个通过引用传递来的数据结构时,例如,

typedef int widget_id;
typedef int widget_data;

void display_status()
{
    std::cout << "void display_status()" << std::endl;
}

void process_widget_data(const widget_data& data_)
{
    std::cout << "void process_widget_data(const widget_data& data)" << std::endl;
}

void update_data_for_widget(widget_id w, widget_data& data)   //①
{
    std::cout << "update_data_for_widget(widget_id w, widget_data& data)" << std::endl;
}

void oops_again(widget_id w)
{
    /*
     
    widget_data data;
    std::thread t(update_data_for_widget, w, data);   //②
    display_status();
    t.join();
    process_widget_data(data);   //③
     
     */
}

//尽管update_data_for_widget①希望通过引用传递第二个参数,std::thread的构造函数②却并不知道,它无视函数所期望的类型,并且盲目地复制了所提供的值,当它调用update_data_for_widget()时,它最后将传递data在内部的副本的引用而非对data自身的引用,于是,当线程完成时,随着所提供参数的内部副本的销毁,这些改动都将被舍弃,将会传递一个未改变的data③,而非正确更新的版本给process_widget_data,对于熟悉std::bind的人来说,解决方案也是显而易见的,你需要用std::ref来保证确实需要被引用的参数,在这种情况下,如果你将对线程的调用改为std::thread t(update_data_for_widget, w, std::ref(data));那么update_data_for_widget将被正确地传入data引用,而非data副本(copy)的引用

void not_oops_again(widget_id w)
{
    widget_data data;
    std::thread t(update_data_for_widget, w, std::ref(data));   //②
    display_status();
    t.join();
    process_widget_data(data);   //③
}

//如果你熟悉std::bind那么参数传递语义就不足为奇,因为std::thread构造函数和std::bind的操作都是依据相同的机制定义的,这意味着,例如,你可以传递一个成员函数的指针作为函数,前提是提供一个合适的对象指针作为第一个参数

class X
{
public:
    void do_lengthy_work()
    {
        std::cout << "void X::do_lengthy_work()" << std::endl;
    }
    
    void do_lengthy_work_with_int(int)
    {
        std::cout << "void X::do_lengthy_work_with_int(int)" << std::endl;
    }
};

X my_x;
std::thread t_0(&X::do_lengthy_work, &my_x);   //①

//这段代码中,新线程将my_x.do_lengthy_work()作为线程函数,my_x的地址①作为指针对象提供给函数,也可以为成员函数提供参数,std::thread构造函数的第三个参数就是成员函数的第一个参数,依次类推(代码如下,译者自加)

int num(0);
std::thread t_1(&X::do_lengthy_work_with_int, &my_x, num);

//有趣的是,提供的参数可以"移动"(move), 但不能"拷贝"(copy),"移动"是指,原始对象中的数据转移给另一对象,而转移的这些数据就不再在原始对象中保存了(译者:比较像在文本编辑时"剪切"操作),std::unique_ptr就是这样一种类型(译者:C++11中的智能指针),这种类型为动态分配的对象提供内存自动管理机制(译者:类似垃圾回收机制),同一时间内,只允许一个std::unique_ptr实现指向一个给定对象,并且当这个实现销毁时,指向的对象也将被删除,移动构造函数(move constructor)和移动赋值操作符(move assignment operator)允许一个对象在多个std::unique_ptr实现中传递(有关"移动"的更多内容,请参考附录A的A.1.1节),使用"移动"转移原对象后,就会留下一个空指针(NULL),移动操作可以将对象转换成可接受的类型,例如:函数参数或函数返回的类型,当原对象是一个临时变量时,自动进行移动操作,当原对象是一个命名变量,那么转移的时候就需要使用std::move()进行显示移动,下面的代码展示了std::move的用法,展示了std::move是如何转移一个动态对象到一个线程中去的

struct big_object
{
    void prepare_data(int i)
    {
        std::cout << "void big_object::prepare_data(int i) : i = " << i << std::endl;
    }
};

void process_big_object(std::unique_ptr<big_object>)
{
    std::cout << "void process_big_object(std::unique_ptr<big_object>)" << std::endl;
}

void use_unique_ptr_as_parameter()
{
    std::unique_ptr<big_object> p = std::make_unique<big_object>();
    p->prepare_data(42);
    std::thread t(process_big_object, std::move(p));
    t.join();
}

//在std::thread的构造函数中指定std::move(p),big_object对象的所有权就被首先转移到新构建线程的内部存储中,之后传递给process_big_object函数

//标准线程库中std::unique_ptr和std::thread在所属权上有相似语义的类型,虽然,std::thread实例不会如std::unique_ptr去占有一个动态对象所有权,但是他会占用一个部分资源的所有权,每个实例都管理一个执行线程,std::thread所有权可以在多个实例中互相转移,因为这些实例是可移动的(movable)且不可复制(aren't copyalbe),在同一时间点,就能保证只关联一个执行线程,同时,也允许程序员能在不同的对象之间转移所有权

//2.3 转移线程所有权

//假设你想要编写一个函数,它创建一个在后台运行的线程,但是向调用函数回传新线程的所有权,而非等待其完成,又或者你想要反过来,创建一个线程,并将所有权传递给要等待他完成的函数,在任意一种情况下,你都需要将所有权从一个地方转移到另一个地方

//这里就是std::thread支持移动的由来,正如在上一节所描述的,在C++标准库里许多拥有资源的类型,如std::ifstream和std::unique_ptr是可移动的(moveable),而非可复制的(copyable),并且std::thread就是其中之一,这意味着一个特定执行线程的所有权可以在std::thread实例之间移动,如同接下来的例子,该示例展示了创建两个执行线程,以及在三个std::thread实例t1、t2、t3之间对那些线程的所有权进行转移

void some_function()
{
    std::cout << "void some_function()" << std::endl;
}

void some_other_function()
{
    std::cout << "void some_other_function()" << std::endl;
}

void some_thread_move_operations()
{
    std::thread t1(some_function);   //①
    std::thread t2 = std::move(t1);   //②
    t1 = std::thread(some_other_function);   //③
    std::thread t3;   //④
    t3 = std::move(t2);   //⑤
    t1.join();   /* 创建的线程不调用join()或者detach(),离开作用域时,会调用std::terminate()导致程序终止 */
    t3.join();   /* 创建的线程不调用join()或者detach(),离开作用域时,会调用std::terminate()导致程序终止 */
//    t1 = std::move(t3);   //⑥   赋值操作将使程序崩溃
}

//当显示使用std::move()创建t2后②,t1的所有权就转移给了t2,之后t1和执行线程已经没有关联了,执行some_funciton的函数现在与t2关联

//然后,与一个临时std::thread对象相关的线程启动了③,为什么不显示调用std::move()转移所有权呢？因为,所有者是一个临时对象,移动操作将会隐式的调用

//t3使用默认构造方式创建④,与任何执行现场都没有关联,调用std::move()将与t2关联线程的所有权移到t3中⑤,显示的调用std::move(),是因为t2是一个命名对象,移动操作⑤完成后,t1与执行some_other_function的线程相关联,t2与任何线程都无关联,t3与执行some_function的线程相关联

//最后一次移动⑥将运行some_function的线程的所有权回给t1,但是在这种情况下t1已经有了一个相关联的线程(运行着some_other_function),所以会调用std::terminate()来终止程序,这样做是为了与std::thread的析构函数保持一致,你在第2.1.1节曾看到,你必须在析构前显示地等待线程完成或是分离,这同样适用于赋值:你不能仅仅通过向管理一个线程的std::thread赋值一个新的值来"舍弃"一个线程

//std::thread支持移动意味着所有权可以很容易从一个函数中被转移出,如清单2.5所示,

//清单2.5函数返回std::thread对象

//特别的,如果腰包所有权转移到函数外,它只能以值的形式返回std::thread,如下所示

void move_thread()
{
    std::cout << "void move_thread()" << std::endl;
}

std::thread return_temporary_thread()
{
    return std::thread(move_thread);   /* 创建的线程不调用join()或者detach(),离开作用域时,会调用std::terminate()导致程序终止 */
    
/* 返回值优化（Return Value Optimization,简称RVO）,是这么一种优化机制：当函数需要返回一个对象的时候,如果自己创建一个临时对象用户返回,那么这个临时对象会消耗一个构造函数（Constructor）的调用、一个复制构造函数的调用（Copy Constructor）以及一个析构函数（Destructor）的调用的代价。而如果稍微做一点优化,就可以将成本降低到一个构造函数的代价,使用方法是在return语句中调用构造函数或者计算性构造函数,另外必须定义拷贝构造函数来“打开”RVO */
}

void move_thread_with_int(int)
{
    std::cout << "void move_thread_with_int(int)" << std::endl;
}

std::thread return_local_thread()
{
    std::thread t(move_thread_with_int, 42);   /* 创建的线程不调用join()或者detach(),离开作用域时,会调用std::terminate()导致程序终止 */
    return t;
    
/* 由于std::thread没有拷贝构造操作,推测此处专门针对std::thread做了RVO处理,在返回点用t的数据构造了一个新的std::thread对象,虽然返回过程只有一个构造函数的开销,但是函数内临时对象的构造开销不可避免 */
}

//同样地,如果要把所有权转移到函数中,它只能以值的形式接受std::thread实例作为其中一个参数,如下所示

void function_use_thread_as_parameter(std::thread t_)
{
    std::cout << "void function_use_thread_as_parameter(std::thread t)" << std::endl;
    t_.join();
}

void function_call_function_use_thread_as_parameter()
{
    function_use_thread_as_parameter(std::thread(some_function));   //临时对象,隐式调用std::move,传参过程调用std::thread移动构造函数
    std::thread t(some_function);
    function_use_thread_as_parameter(std::move(t));   //本地对象,显示调用std::move,传参过程调用std::thread移动构造函数
}

//std::thread支持移动的好处之一,就是你可以建立在清单2.3中thread_guard类的基础上,同时使它实际上获得线程的所有权,这可以避免thread_guard对象在引用它的线程结束后继续存在所造成的不良影响,同时也意味着一旦所有权转移到了该对象,那么其他对象都不可以结合或分离该线程,因为这主要是为了确保在推出一个作用与之前线程够已经完成,我吧这个类成为scoped_thread,其实现如清单2.6所示,同时附带一个简单的实例

//清单2.6 scoped_thread和示例的用法

class scoped_thread
{
    std::thread t;
public:
    explicit scoped_thread(std::thread t_) : t(std::move(t_))   //①
    {
        if (!t.joinable())   //②
            throw std::logic_error("No thread");
    }
    /* 传参过程调用了一次std::thread的移动构造函数创建了t_, 类对象初始化过程又一次调用了std::thread的移动构造函数创建了t */
    
    ~scoped_thread()
    {
        t.join();   //③
    }
    
    scoped_thread(const scoped_thread &) = delete;
    scoped_thread& operator=(const scoped_thread &) = delete;
};

struct func;

void function_use_scoped_thread()
{
    int some_local_state;
    scoped_thread t1{std::thread{func{some_local_state}}};   //④
    std::thread t{func{some_local_state}};
    scoped_thread t2{std::move(t)};
    do_something_in_current_thread();   //⑤
}

//这个例子与清单2.3类似,但是新线程被直接传递到scoped_thread④,而不是为它创建一个单独的命名变量,当初线程到达function_use_scoped_thread结尾的时,scoped_thread对象被销毁,然后结合③提供给构造函数①的线程,使用禽蛋2.3中的thread_guard类,析构函数必须检查线程是不是仍然可结合,你可以在构造函数中②来做,如果不是则引发异常

//std::thread对移动的支持同样考虑了std::thread对象的容器,如果那些容器是移动感知的(如更新后的std::vector<>)这意味着你可以编写像清单2.7中的代码,生成一批新线程,然后等爱他们完成

//清单2.7 生成一批线程并等待它们完成

void do_work(unsigned thread_id)
{
    std::cout << "void do_work(unsigned thread_id)" << std::endl;
}

void produce_multiple_threads()
{
    std::vector<std::thread> threads;
    for (unsigned i = 0; i < 20; ++i)
        threads.push_back(std::thread(do_work, i));   //生成线程,用emplace_back也可以
    std::for_each(threads.begin(), threads.end(), std::mem_fn(&std::thread::join));   //轮流在每个线程上调用join()
}

//如果线程是被用来细分某种算法的工作,这往往正是所需的,在返回调用者之前,所有线程必须全都完成,当然,清单2.7的简单结构意味着由线程所做的工作是自包含的,同时它们操作的记过纯粹是共享数据的副作用,如果f()向调用者返回一个依赖于这些线程的操作结果的值,那么正如所写的这样,该返回值就得通过检查线程终止后的共享数据来决定,在线程间转移操作结果的替代方案将在第4章中讨论

//将std::thread对象放到std::vector中是线程迈向自动管理的一步,与其为那些线程创建独立的变量并直接与之结合,不如将他们视为群组,你可以进一步创建在运行时确定的动态数量的线程,更进一步地利用这一步,而不是如清单2.7中的那样创建固定的数量

//2.4 运行时决定线程数量

//C++标准库中对此有所帮助的特性是std::thread::hardware_concurrency(),这个函数返回一个对于给定程序执行时能够真正并发运行的线程数量的指示,例如,在多核系统上它可能是CPU核心的数量,它仅仅是一个提示,如果该信息不可用则函数可能会返回0,但它对于在线程间分隔任务是一个有用的指南

//清单2.8展示了std::accumulate的一个简单的并行版本实现,它在线程之间划分所做的工作,使得每个线程具有最小数目的元素以避免过多线程的开销,请注意,该实现假定所有的操作都不引发异常,即便异常可能会发生,例如,std::thread构造函数如果不能启动一个新的执行线程那么它将引发异常,在这样的算法中处理异常超出了这个简单示例的范围,将放在第8章中阐述

//清单2.8 std::accumulate的简单的并行版本

template<typename Iterator, typename T>
struct accumulate_block
{
    void operator()(Iterator first, Iterator last, T& result)
    {
        result = std::accumulate(first, last, result);
    }
};

template<typename Iterator, typename T>
T parallel_accumulate(Iterator first, Iterator last, T init)
{
    unsigned long const length = std::distance(first, last);
    
    if (!length)   //①
        return init;
    
    unsigned long const min_per_thread = 25;
    unsigned long const max_threads = (length + min_per_thread - 1) / min_per_thread;   //②

    unsigned long const hardware_threads = std::thread::hardware_concurrency();
    unsigned long const num_threads = std::min(hardware_threads != 0 ? hardware_threads : 2, max_threads);   //③
    
    unsigned long const block_size = length / num_threads;    //④
    
    std::vector<T> results(num_threads);
    std::vector<std::thread> threads(num_threads - 1);   //⑤
    
    Iterator block_start = first;
    for (unsigned long i = 0; i < (num_threads - 1); ++i)
    {
        Iterator block_end = block_start;
        std::advance(block_end, block_size);   //给迭代器增加偏移量
        threads[i] = std::thread(accumulate_block<Iterator,T>(), block_start, block_end, std::ref(results[i]));   //⑦
        block_start = block_end;   //⑧
    }
    
    accumulate_block<Iterator,T>()(block_start, last, results[num_threads - 1]);   //⑨
    std::for_each(threads.begin(), threads.end(), std::mem_fn(&std::thread::join));   //⑩
    
    return std::accumulate(results.begin(), results.end(), init);   //⑪
}

//虽然是一个相当长的函数,但它实际上是很直观的,如果输入范围为空①,只返回初始值init,否则,此范围内至少有一个元素,于是你将要处理的元素数量除以最小的块大小,以获取线程的最大数量②,这是为了避免当范围中只有五个值时,在一个32核的机器上创建32个线程

//要运行的线程数是你计算出的最大值和硬件线程数量③的较小值,你不会想要运行比硬件所能支持的更多线程(超额订阅,oversubscription),因为上下文切换将意味着更多的线程会降低性能,如果对std::thread::hardwareconcurrency()的调用返回0,你只需简单替换上你所选择的数量,在这个例子中我选择了2,你不会想要运行过多的线程,因为在单核的机器上这会使事情变慢,但同样地你也不希望运行的过少,因为那样的话,你就会错过可用的并发

//每个待处理的线程的条目数量是范围的长度除以线程的数量④,如果你担心数量不能整除,没必要,稍后再来处理

//既然你知道有多少个线程,你可以为中间结果创建一个std::vector<T>,同时为线程创建一个std::vector<std::thread>⑤,请注意,你需要启动比num_threads少一个的线程,因为已经有一个了

//启动线程是个简单的循环,递进block_end迭代器到当前块的结尾⑥,并启动一个新的线程来累计此块的结果⑦,下一个块的开始时这一个的结束⑧

//当你启动了所有的线程后,这个线程就可以处理最后的块⑨,这就是你处理所有未被整除的地方,你知道最后一块的结尾只能是last,无论在那个块里有多少元素,一旦累计出最后一个块的结果,你可以等待所有使用std::for_each生成的线程⑩,如清单2.7中所示,接着通过最后调用std::accumulate将结果累加起来⑪

//在你离开这个例子前,值得指出的是在类型T的加法运算符不满足结合律的地方(如float何double),这个parallel_accumulate的结果可能会跟std::accumulate的有所出入,这是将范围分组成块导致的,此外,对迭代器的需求要更严格一些,他们必须至少是前向迭代器(forward iterators),然而std::accumulate可以和单通输入迭代器(input iterators)一起工作,同时T必须是可默认构造的(default constructible)以使得你能够创建results向量,这些需求的各种变化是并行算法很常见的,就其本质而言,他们以某种方式的不同是为了使其并行,并且在结果和需求上产生影响,并行算法会在第8章中进行更深入的阐述,另外值得一提的是,因为你不能直接从一个线程中返回值,所以你必须将相关项的引用传入results向量中,从线程中返回结果的替代方法,会在第4章中通过使用future来实现

//在这种情况下,每个线程所需的所有信息在线程开始时传入,包括存储其计算结果的位置,实际情况并非总是如此,有时,作为进程的一部分有必要能够以某种方式标识线程,你可以传入一个标识数,如同在清单2.7中i的值,但是如果粗腰此标识符的函数在调用栈中深达数个层次,并且可能从任意线程中被调用,那样做就很不方便,当我们设计C++线程库时就遇见了这方面的需求,所以每个线程都有一个唯一的标识符

//2.5 识别线程

//线程标识符是std::thread::id类型的,并且有两种获取方式,其一,线程的标识符可以通过从与之相关联的std::thread对象中通过调用get_id()成员函数来获得,如果std::thread对象没有相关联的执行线程,对get_id()的调用返回一个默认构造的std::thread::id对象,表示"没有线程",另外,当前线程的标识符,可以通过std::this_thread::get_id()来获得,这也是定义在<thread>头文件中的

//std::thread::id类型的对象可以自由地复制和比较,否则,他们作为标识符就没什么大用处,如果两个std::thread::id类型的对象相等,则他们代表着同一个线程,或两者都具有"没有线程"的值,如果两个对象不相等,则他们代表着不同的线程,或其中一个代表着线程,而另一个具有"没有线程"的值

//线程库不限制你检查线程的标识符是否相同,std::thread::id类型的对象提供了一套完整的比较运算符,提供了所有不同值的总排序,这就允许他们在关系型容器中被用作主键,或是被排序,或者任何作为程序员的你认为合适的方式进行比较,比较运算符为std::thread::id所有不相等的值提供了一个总的排序,所以它们表现为你直觉上期望的那样:如果a<b且b<c,那么a<c,等等,标准库还提供了std::hash<std::thread::id>使得std::thread::id类型的值可在新的无需关系型容器中作为主键来用

//std::thread::id的实例常被用来检查一个线程是否需要执行某些操作,例如,如果线程像在清单2.8中那样的被用来分配工作,启动了其他线程的初始线程在需要做的工作可能会在算法中略有不同,在这种情况下,它可以在启动其他线程之前存储std::this_thread::get_id()的结果,然后算法的核心部分(这对所有线程都是公共的)可以对照所存储的值来检查自己的线程ID

void do_master_thread_work()
{
    std::cout << "void do_master_thread_work()" << std::endl;
}

void do_common_work()
{
    std::cout << "void do_common_work()" << std::endl;
}

std::thread::id master_thread;
void thread_id_equal()
{
    if (std::this_thread::get_id() == master_thread)
    {
        do_master_thread_work();
    }
    do_common_work();
}

//另外,当前线程的std::thread::id可以作为操作的一部分而存储在数据结构中,以后在相同数据结构上的操作可以对照执行此操作的线程ID来检查所存储的ID,来确定哪些操作是允许的/需要的

//类似地,线程ID可以指定的数据需要与一个线程进行关联,并且诸如线程局部存储这样的替代机制不适用的地方,用作关系型容器的主键,例如这样一个容器,它可以被控制线程用来存储关于它控制下的每个线程的信息,或是在线程之间传递信息

//这种想法就是,在大多数情况下,std::thread::id是以作为线程的通用标识符,只有当标识符具有与其相关联的语义(比如所谓数组的索引)时,才有必要用替代方案,你甚至可以将一个std::thread::id实例写到诸如std::cout这样的输出流中

void print_thread_id()
{
    std::cout << std::this_thread::get_id() << std::endl;
}

//你得到的确切的输出,严格的取决于实现,标准给定的为以保证是,比较结果相等的线程ID应该产生相同的输出,而那些比较结果不相等的萤爱给出不同的输出,因此,这主要是对调试和日志有用,但数值是没有语义的,所以也没有更多的可说的了

//2.6 总结

//本章讨论了C++标准库中基本的线程管理方式:启动线程,等待结束和不等待结束(因为需要他们运行在后台),并了解应该如何在线程启动前,向线程函数中传递参数,如何转移线程的所有权,如何使用线程组来分割任务,最后,讨论了使用线程标识来确定关联数据,以及特殊线程的特殊解决方案,虽然,现在已经可以纯粹的依赖线程,使用独立的数据,做独立的任务(如清单2.8),但在某些情况下,线程确实需要有共享数据,第3章会讨论共享数据和线程的直接关系,第4章会讨论(有/没有)共享数据情况下的线程同步操作

#endif /* _2_0_h */

