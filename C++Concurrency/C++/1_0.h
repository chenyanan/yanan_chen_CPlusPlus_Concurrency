//
//  1_0.h
//  C++Concurrency
//
//  Created by chenyanan on 2017/4/20.
//  Copyright © 2017年 chenyanan. All rights reserved.
//

#ifndef _1_0_H
#define _1_0_H

#include <iostream>
#include <thread>

//①②③④⑤⑥⑦⑧⑨⑩⑪⑫⑬⑭⑮⑯⑰⑱⑲⑳㉑㉒㉓㉔㉕㉖㉗㉘㉙㉚㉛㉜㉝㉞㉟㊱㊲㊳㊴㊵㊶㊷㊸㊹㊺㊻㊼㊽㊾㊿

//第1章,你好,C++的并发世界

//本章主要内容
//何谓并发和多线程
//应用程序为什么要使用并发和多线程
//C++的并发史
//一个简单的C++多线程程序

//令C++用户振奋的时刻到了,距初始的C++标准(1998年)发布13年后,C++标准委员会给语言本身,以及标准库,带来了一次重大的变革

//新C++标准(也被成为C++11或C++0x)在2011年发布,带来一系列的变革让C++变成更加简单和高效

//其中一个最重要的新特性就是对多线程的支持

//C++标准第一次承认多线程在语言中的存在,并在标准库中为多线程提供组件,这意味着使用C++编写与平台无关的多线程程序成为可能,也为可移植性提供了强有力的保证,于此同时,程序员们为提高应用的性能,对并发的关注也是与日俱增,特别在多线程编程方面

//本书是介绍如何使用C++11多线程来编写并发程序,及相关的语言特性和库工具(library facilities),本书以"解释并发和多线程的含义,为什么要使用并发"作为起始点,在对"什么情况下不使用并发"进行阐述之后,将对C++支持的并发方式进行概括,最后,以一个简单的C++并发实例技术这一章,资深的多线程开发人员可以跳过前面的小节,在后面的几个章节中,会有更多的例子,以便大家对库工具(library facilities)进行更加深入的了解,本书最后,将会给出所有多线程与并发相关的C++标准库工具的全面参考

//问题来了,何谓并发(concurrency)？何谓多线程(multithreading)?

//1.1 何谓并发

//最简单和最基本的并发,是指两个或更多独立的活动同时发生

//并发在生活中随处可见,我们可以一边走路一边说话,也可以两只手同时作不同的动作,还有我们每个人都过着相互独立的生活,当我在游泳的时候,你可以看球赛,等等

//1.1.1 计算机系统中的并发

//计算机领域的并发指的是在单个系统里同时执行多个独立的任务,而非顺序的进行一些活动,计算机领域里,并发不是一个新事物,很多年前,一台计算机就能通过多任务操作系统的切换功能,同时运行多个应用程序,高端多处理器服务器在很早就已经实现了真正的并行计算,那"老东西"上有哪些"新东西"能让它在计算机领域越来越流行呢？真正任务并行,而非一种错觉

//以前,大多数计算机只有一个处理器,具有单个处理单元(processing unit)或核心(core),如今还有很多这样的台式机,这种机器只能在某一时刻执行一个任务,不过它可以美妙进行多次任务切换,通过"这个任务做一会,再切换到别的任务,再做一会儿"的方式,让任务看起来是并行执行的,这种方式称为"任务切换(task switching)",如今,我们仍然将这样的系统厂公难为并发(concurrency):因为任务切换得太快,以至于无法感觉到任务载何时会被暂时挂起,而切换到另一个任务,任务切换会给用户和应用程序造成一种"并发的假象",因为这种假象,当应用在任务切换得环境下和真正并发环境下执行相比,行为还是有着微妙的不同,特别是对内存模型不正确的假设(详见第5章),在多线程环境中可能不会出现(详见第10章)

//多处理器计算机用于服务器和高性能计算已有多年,基于单芯多处理器(多核处理器)的台式机,也越来越大众化,无论拥有几个处理器,这些机器能够真正的并行多个任务,我们称其为"硬件并发(hardware concurrency)"

//图1.1显示了一个计算机处理恰好两个任务时的理想情景,每个任务被分为10个相等大小的块,在一个双核机器(具有两个处理核心)上,每个任务可以在各自的处理器核心上执行,在单核机器上做任务切换时,每个任务的块被分为10个相等的大小的块,在一个双核机器(具有两个处理器核心)上,每个任务可以在各自的处理核心上执行,在单核机器上做任务切换时,每个任务的块交织进行,但它们中间有一小段分割(图中所示灰色分割条的厚度大于双核机器的分隔条的厚度大于双核机器的分隔条),为了实现交织进行,系统每次从一个任务切换到另一个时都需要切换一次上下文,任务切换也有时间开销,进行上下文的切换时,操作系统必须为当前运行的任务保存CPU的状态和指令指针,并计算出要切换到哪个任务,并为即将切换到的任务重新加载处理器状态,然后,CPU可能要讲新任务的指令和数据内存载入到缓存中,这会阻止CPU执行任何指令,从而造成的更多的延迟

//有些处理器可以在一个核心上执行多个线程,但硬件并发在多处理器或多核系统上效果更加显著,硬件线程最重要的因素是数量,也就是硬件上可以并发运行多少独立的任务,即便是具有真正硬件并发的系统,也很容易拥有比硬件"可并行最大任务数"还要多的任务需要执行,所以任务切换在这些情况加仍然适用,例如,在一个典型的台式计算机上可能会有成百上千的任务在运行,即便是在计算机处于空闲时,还是会有后台任务在运行,正式任务切换使得这些后台任务可以运行,并使得你可以同时运行文字处理器,编译器,编辑器和web浏览器(或其他应用的组合),图1.2显示了四个任务在双核处理器上的任务切换,仍然是将任务整齐的划分为同等大小块的理想情况,实际上,许多阴虚会使得分割不均和调度不规则,部分因素将在第8章中讨论,那时我们再来看一看影响并行代码性能的阴虚

//无论应用程序在单核处理器,还是多核处理器上运行,也无论是任务切换还是真正的硬件并发,这里提到的技术,功能和类(本书所涉及的)都能使用的得到,如何使用并发,将很大程度上取决于可用的硬件并发,我么将在第8章中再次讨论这个问题,并具体研究C++代码并行设计的问题

//1.1.2 并发的途径

//试想当两个程序员在两个独立的办公室一起做一个软件项目,他们可用安静地工作,不互相干扰,并且他们人手一套参考手册,但是,他们沟通起来就有些困难,比起可用直接互相交谈,他们必须使用电话,电子邮件或到多方的办公室进行直接交流,并且,管理两个办公室需要有一定的经费支出,还需要购买多分参考手册

//假设,让开发人员同在一间办公室办公,他们可用自由的对某个引用程序设计进行讨论,也可以在纸或白板上轻易的绘制图标,对设计观点进行辅助性阐释,现在,你只需要管理一个办公室,只要有一套参考资料就够了,遗憾的是,开发人员可能难以集中注意力,并且还可能保存在资源共享的问题(比如,"参考手册哪去了?")

//以上两种方法,描绘了并发的两种基本途径,每个开发人员代表一个线程,每个办公室代表一个处理器,第一种途径是有多个单线程的进程,这就类似让每个开发人员拥有自己的办公室,而第二种途径是在单一进程里有多个线程,如同一个办公室里有两个开发人员,让我们咋一个应用程序中简单分析一下这两种途径

//1.多进程并发

//使用并发的第一种方法,是将应用程序分为多个独立的进程,它们在同一时刻运行,就像同时进行网页浏览和文字处理一样,如图1.3所示,独立的进程可以通过进程间常规的通信渠道传递信息(信号,套接字,文件,管道等等),不过,这种进程之间的通信不是设置复杂,就是速度慢,这是因为操作系统会在进程间提供了一定的保护措施,以避免一个进程去修改另一个进程的数据,还一个缺点是,运行多个进程需要的固定开销,需要时间启动进程,操作系统需要内部资源来管理进程,等等

//当然,以上的机制也不是一无是处,操作系统在进程间提供的附加保护操作和更高级别的通信机制,意味着可以更容易编写安全的并发代码,实际上,在类似于Erlang编程环境中,是将进程作为并发的基本构造块

//使用独立的进程,实现并发还有一个额外的优势,可以使用远程连接(可能需要联网)的方式,在不同的机器上运行独立的进程,虽然,这增加了通信成本,但在设计精良的系统上,这可能是一个提供并行可用性和性能的低成本方案

//2. 多线程并发

//并发的另一个途径,在单个进程中运行多个线程,线程是很轻量级的线程:每个线程相互独立运行,且线程可以在不同的指令序列中运行,但是,进程中的所有线程都共享地址空间,并且所有线程访问到大部分数据,全局变量仍然是全局的,指针,对象的引用或数据可以在线程之前传递,虽然,进程之间通常共享内存,但这种共享通常也是难以建立,且难以管理,因为,统一数据的内存地址在不同的进程中是不同的,图1.4展示了一个进程中的两个线程通过共享内存进行通信

//地址空间共享,以及缺少线程间数据的保护,使得操作系统的记录工作量减小,所以使用多线程相关的开销远远小于使用多个进程,不过,共享内存的灵活性是有代价的:如果数据要被多个线程访问,那么程序员那必须确保每个线程所访问到的数据是一致的(在本书第3、4、5和第8章中会涉及,线程间数据共性可能会遇到的问题,以及如何使用工具来避免这些问题),问题并非误解,只要在编写代码时适当地注意即可,这同样意味着需要对线程通信做大连的工作

//多个单线程/进程间的通信(包含启动)要比单一进程中的多线程间的通信(包括启动)的开销大,若不考虑共享内存可能会带来的问题,多线程将会成为主流语言(包括C++)更青睐的兵法途径,此外,C++标准并未对进程间通信提供任何原生支持,所以使用多进程的方式实现,这会依赖与平台相关的API,因此,本书只关注使用多线程的并发,并且在之后所提到"并发",均假设为多线程来实现

//了解并发后,让来看看为什么要使用并发

//1.2 为什么使用并发

//主要原因有两个:关注点分离(SOC)和性能,事实上,他们应该是使用并发的唯一原因,如果你观察的足够仔细,所有的因素都可以归结到其中的一个原因(或者可能是两个都有,当然,除了像"就因为我愿意"这样的原因之外)

//1.2.1 为了分离关注点

//编写软件时,分离关注点是个好主意,通过将相关的代码与无关的代码分离,可以使程序更容易理解和测试,从而减少出错的可能性,即使一些功能区域中的操作需要在同一时刻发生的情况下,依旧可以使用并发分离不同的功能区域,若不显示地使用并发,就得编写一个任务切换框架,或者在任务中主动地调用一段不相关的diamante

//考虑一个有用户界面的处理密集型引用,DVD播放程序,这样的引用程序,应具备这两种功能:一,不仅要从光盘中读出数据,还需要对图像和声音进行编码,之后吧编码出的信号输出只视频和音频硬件,从而实现DVD的无误播放,二,还需要接受来自用户的输入,当用户单击"暂停","返回菜单"或"退出"按键的时候执行对应的操作,当应用是单个线程时,引用需要在回放期间定期检查用户的输入,这就需要把"DVD播放"代码和"用户界面"代码放在一起,以便调用,如果使用多线程方式来分隔这些关注点,"用户界面"代码和"DVD播放"代码就不再需要放在一起:一个县城可以处理"用户界面"事件,另一个进行"DVD播放",他们之间会有交互,例如,用户点击"暂停",线程就可以直接进行任务交互

//这会给响应性带来一些错觉,因为用户界面线程通常可以立即响应用户的请求,在当请求传达给忙碌线程,这时的相应可以是简单地显示代表忙碌的光标或"请等待"字样的消息,类似地,独立的线程通常用来执行那些必须在后台持续运行的任务,例如,桌面搜索程序中监视文件系统变化的任务,因为他们之间的交互清晰可辨认,所以这种方式会使每个线程的逻辑变的更加简单

//在这种情况下,线程的数量不再依赖CPU中的可用内核的数量,因为对线程的划分是基于概念上的设计,而不是一种增加吞吐量的尝试

//1.2.2 为了性能

//多处理器系统已经存在了几十年,但直到最近,它们也只在超级计算机,大型机和大型服务器系统中才能看到,然而,芯片制造商越来越倾向于多核芯片的设计,即在单个芯片上集成2、4、16或更多的处理器,从而获取更好的性能,因此,多核台式计算机,多核嵌入式设备,越来越普遍,它们的计算能力的提高不是源自使单一任务运行的更快,而是并行运行多个任务,在过去,程序员曾坐看他们的程序随着处理器的更新换代而变得更快,无需他们这边做任何事情,但是现在,就像Herb Sutter所说的,"没有免费的午餐了",如果想要利用日益增长的计算你能力,那就必须设计多任务并发式软件,程序员必须留意这个,尤其是在那些迄今都忽略并发的人们,现在很有必要将其加入工具箱中了

//两种方式利用并发提高性能:第一,将一个单个任务分成几部分,且各自并行运行,从而降低总运行时间,这就是任务并行,虽然这听起来很直观,但它是一个相当复杂的过程,因为在各个部分之间可能存在着依赖,区别可能是在过程方面,一个县城执行算法的一部分,而另一个线程执行算法的另一部分,或是咋数据方便,每个线程在不同的数据部分上执行相同的操作(第二种方式),后一种方法被称为数据并行(data parallelism)

//易受这种并行影响的算法常被称为易并行(embarrassingly parallel),尽管你会受到易并行化代码影响,但这一与你来说是一件好事,我曾遇到过自然并行(naturally parallel)和便利并发(conveniently concurrent)的算法,容并行算法具有良好的可扩展特性,当可用硬件线程的数量增加时,算法的并行性也会随之增加,这种算法能很好的体现人多力量大,如果算法中有不易并行的部分,你可用吧算法划分成固定(不可扩展)数量的并行任务,第8章将会再来讨论,在线程之间划分任务的技巧

//第二种方法是使用可并行的方式,来解决更大的问题,与其同时处理一个文件,不如酌情处理2个、10个或20个,虽然,这是数据并行的一种应用(通过对多组数据同时执行相同的操作),但着重点不同,处理一个数据块仍然需要同样的时间,但是相同的时间内处理了更多的数据,当然,这种放啊也有限制,并非在所有情况下都是有益的,不过,这种方法所带来的吞吐量提升,可以让某些新功能成为可能,例如,可以并行处理图片的各个部分,就能提高视频的分辨率

//1.2.3 什么时候不使用并发

//知道何时不适用并发与知道何时使用它一样重要,基本上,不适用并发的唯一原因就是,收益比不上成本,使用并发的代码在很多情况下难以理解,因此编写和维护的多线程代码就会产生直接的脑力成本,同时额外的复杂性也可能引起跟多的错误,除非潜在的性能增益足够大或关注点分离地足够清晰,能抵消所需的额外的开发时间以及与维护多线程代码相关的额外成本(代码正确的前提下),否则,别用并发

//同样地,性能增益可能会小于预期,因为操作系统需要分配内核相关资源和堆栈空间,所以在启动线程时存在固有开销,然后才能把新线程加入调度器中,所有这一切都需要时间,如果在线程上的任务完成得很块,那么任务实际执行的时间要比启动线程的时间小很多,这就会导致应用程序的整体性能还不如直接使用"产生线程"的方式

//此外,线程是有限的资源,如果让太多的线程同时运行,则会消耗很多操作系统资源,从而使得操作系统整体上运行得更加缓慢,不仅如此,因为每个线程都需要一个独立的堆栈空间,所以运行太多的线程也会耗尽进程的可用内存或地址空间,对于一个肯用地址空间为4GB(32bit)的平坦构架的进程来说,这的确是个问题,如果每个线程都有一个1MB的堆栈(很多系统都会这样分配),那么4096个线程将会佣金所有地址空间,不会给代码、静态数据或者堆数据留有任何空间,即便64位(或者更大)的系统不存在这种直接的地址空间限制,但其他资源有限:如果你运行了太多的线程,最终也是会出问题的,尽管线程池(参见第9张)可以用来限制线程的数量,但这并不是灵丹妙药,它也有自己的问题

//当客户端/服务器(C/S)应用在服务器端为每一个链接启动一个独立的线程,对于少量的链接是可以正常工作的,但当同样的技术用于需要处理大量链接的高需求服务器时,也会因为线程太多而耗尽系统资源,在这种情景下,谨慎地使用线程池可以对性能产生优化(参见第9章)

//最后,运行越多的线程,操作系统就需要越多的上下文切换,每个上下文切换都需要耗费本可以花在有价值工作上的时间,所以在某些时候,增加一个额外的线程实际上会降低,而非提到应用程序的整体性能,为此,如果你试图得到系统的最佳性能,可以考虑使用硬件并发(或不用),并调整运行线程的数量

//为性能而使用并发就像所有其他优化策略一样,它拥有大幅度提高引用性能的潜力,但它也可能使代码复杂化,使其更难理解,并更容易出错,因此,只有引用中具有显著增添潜力IDE性能关键部分,才值得并发化,当然,如果性能收益的潜力仅次于设计清晰或关注点分离,可能也值得使用多线程设计

//假设你已经决定确实要在应用程序中使用并发,无论是为了性能,关注点分离,亦或是因为"多线程星期一"(multithreading Monday)(译者:可能是学习多线程的意思)

//问题又来了,对于C++程序员来说,多线程意味着什么?

//1.3 C++中使的并发和多线程

//通过多线程为C++并发提供标准化支持是件新鲜事,只有在C++11标准下,才能编写不依赖平台扩扎你的多线程代码,了解C++线程库中的众多规则,知晓其历史是很重要的

//1.3.1 C++多线程历史

//C++98(1998)标准不承认线程的存在,并且各种语言要素的操作效果都以顺序抽象机的形式编写,不仅如此,内存模型也没有正式定义,所以在C++98标准下,没办法在缺少编译器相关扩展的情况下编写多线程应用程序

//当然,编译器供应商可以自由地向语言添加扩展,添加C语言中流行的多线程API,POSIX标准中的C标准和Microsoft Windows API中的那些,这就使得很多C++编译器供应商通过各种平台相关扩展来支持多线程,这种编译器支持一般受限于只能使用平台相关的C语言API,并且该C++运行库(例如,异常处理机制的代码)能在多线程情况下正常工作,因为编译器和处理器的实际表现很不错了,所以在少数编译器供应商提供正式的多线程感知内存模型之前,程序员们已经编写了大量的C++多线程程序了

//由于不满足于使用平台相关的C语言API来处理多线程,C++程序员们希望使用的类库能提供面向对象的多线程工具,像MFC这样的应用框架,如同Boost和ACE这样的已经积累了多组类的通用C++类库,这些类封装了底层的平台相关API,并提供用来简化任务的高级多线程工具,各种类和库在细节方面差异很大,但在启动新线程的方面,总体构造却大同小异,一个为许多C++类和库共有的设计,同时也是为程序员提供很大便利的设计,也就是使用带锁的资源获取就是初始化(RAII,Resource Acquisition Is Initialization)的习惯,来确保当退出相关作用于时互斥元解锁

//编写多线程代码需要见识的变成基础,当前的很多C++编译器为多线程编程者提供了对应(平台相关)的API,当然,还有一些于平台无关的C++类库(例如:Boost和ACE),这因为如此,程序员可以通过这些API来实现多线程应用,不过,由于缺乏统一标准的支持,缺少统一的线程内存模型,进而导致一些问题,这些问题在跨硬件或跨平台相关的多线程引用上表现的由为明显

//1.3.2 新标准支持并发

//所有的这些随着C++11标准的发布而改变了,不仅有了一个全新的线程感知内存模型,C++标准库也扩展了:包含了用于管理线程(参见第2章),保护共享数据(参加第3章),线程间同步操作(参见第4章),以及低级院子操作(参见第5章)的各种类

//新C++线程库很大程度上,是基于上文提到的C++类库的经验积累,特别是,Boost线程库作为新类库的主要模型,很多类于Boost库中的相关类有着相同名称和结构,随着C++标注你的进步,Boost线程库也配合着C++标准在许多方面做出改变,因此之前使用Boost的用户将会发现自己非常熟悉C++11的线程库

//如本章起始提到的那样,支持并发仅仅是C++标注的变化之一,此外还有很多对于编程语言自身的改善,就是为了让程序员们的工作变得更加轻松,这些内容咋本书的讨论范围之外,但是其中的一些变化对于线程库本身及其使用方式缠身了很大的影响,附录A会对这些特性做一些介绍

//新的C++标准直接支持原子操作,允许程序员通过定义语义的方式编写高效代码,而无需了解与平台相关的汇编指令,这对于试图编写高效,可移植代码的程序员们来说是一个好消息,不仅编译器可以搞定具体平台,还可以编写优化器解释操作的语义,从而让程序整体得到更好的优化

//1.3.3 C++线程库的效率

//通常情况下,这是高性能计算开发者对C++的担忧之一,为了效率,C++类整合了一些底层工具,了解性能的极限后,了解香瓜能实现使用高级工具和直接低级工具的开销差,就显得十分重要了,这个开销差就是抽象惩罚(abstraction penalty)

//C++标准委员会在整体设计标注库时,特别是在设计标准线程库的时候,就已经注意到了这点,目标之一就是在提供相同的工具的情况下,直接使用底层API就完全得不到任何好处,因此,该类库在大部分主流平台上都能实现高效(带有非常低的抽象惩罚)

//C++标准委员会的另一个目标,为了达到终极性能,需要确保C++能提供足够多的底层工具,给那些要与硬件打交道的程序员,为了这个目的,伴随着新的内存模型,出现了一个综合的原子操作库,可用于直接控制单个位、字节、内部线程间同步,以及所有变化的可见性,这些原子类型和相应的操作现在可以在很多地方使用,而这些地方以前可能使用的是平台相关的汇编代码,使用了新的标准类型的操作diamante具有更好的可移植性,而且更容易维护

//C++标准库也提供了更高级别的抽象和工具,使得编写多线程代码更加简单,并且不易出错有时运用这些工具确实会带来性能开销,因为有额外的代码必须执行,但是,这种性能成本并不一定意味着更高的抽象惩罚,总体来看,这种性能开销并不比手工编写等效函数高,而且编译器可能会很好滴内联大部分额外代码

//某些情况加,高级工具会提供一些额外的功能,大部分情况下这都不是问题,因为你没有为你不使用的那部分买单,在罕见的情况下,这些未使用的功能会影响其他代码的性能,如果你很看重程序的性能,在绝大多数情况下,因额外增加的复杂性而出错的几率都远大于性能的小幅提升带来的收益,即便是有证据恩确实表明平静出现在C++标准库的工具中,也可能会归咎于低劣的应用设计,而非低劣的类库实现,例如,如果过多的线程竞争一个互斥元,将会很明显的影响想能,与其在互斥操作上耗费时间,不如重新设计应用,减少互斥元上的竞争来的划算,如何减少应用中的竞争,会在第8章中再次提及

//在非常罕见的情况下,当C++标准库没有提供所需的性能或行为时,就有必要使用平台相关的工具

//1.3.4 平台相关的工具

//虽然C++线程库为多线程和并发处理提供了较为全面的工具,但在某些平台上提供额外的工具,为了方便的访问那些工具的同时,又使用标准C++县城库,在C++线程库中提供一个native_handle()成员函数,允许通过使用平台相关API直接操作底层实现,酒其本质而言,任何使用native_handle()执行的操作是完全依赖于平台的,这超出了本书(同时也是标准C++库本身)的范围

//所以,使用平台相关的工具之前,明白标准库能够做什么很重要,那么通过一个例子来开始吧

//1.4 开始入门

//OK!现在你有一个能与C++11标准兼容的编译器,接下来呢？一个C++多线程程序是什么样子呢？其实,它看上去和其他C++程序差不多,通常是变量、类以及函数的组合,唯一的区别在于某些函数可以并发运行,所以需要确保共享数据在并发访问时是安全的,详见第3章,当然,为了并发地运行函数,必须使用特定的函数以及对象来管理各个线程

//1.4.1 你好,开始世界

//从一个经典的例子开始,一个打印"Hello World."的程序,一个非常简单的单线程中运行的Hello World程序如下所示,当我们谈到多线程时,它可以作为一个基准

void hello()
{
    std::cout << "thread " << std::this_thread::get_id() << " : " << "Hello World" << std::endl;
}

//这个程序所做的就是将"Hello World"写进标准输出流,让我们将它与下面清单所示的简单的"Hello,Concurrent World"程序做个比较,它起动了一个独立的线程来显示这个信息

//清单1.1 一个简单的Hello,Concurrent World程序:

#include <thread>   //①

void concurrent_hello()   //②
{
    std::cout << "thread " << std::this_thread::get_id() << " : " << "Hello Concurrent World" << std::endl;
}

void say_hello()
{
    hello();
}

void say_concurrent_hello()
{
    std::thread t(concurrent_hello);   //③
    t.join();   //④
}

//第一个区别是增加了#include <thread>①,标准C++库中对多线程支持的声明在新的头文件中,管理线程的函数和类在<thread>中声明,而保护共享数据的函数和类在其他头文件中声明

//其次,写信息的代码被移动到了一个独立的函数中②,因为每个线程够必须具有一个初始函数(initial function),新线程的执行在这里开始,对于应用程序来说,初试线程是main(),但是对于其他线程,可以在std::thread对象的构造函数中制定,在本例中,被命名为t③的std::thread对象拥有新函数hello()作为其初始函数

//下一个区别:与直接写入标准输出或是从main()调用hello()不同,该程序启动了一个全新的线程来实现,将线程数量一分为二,初试线程始于main(),而新线程始于hello()

//新的线程启动之后③,初始线程继续执行,如果它不等待新线程结束,它就讲自顾自地继续运行到main()结束,从而结束程序,有可能发生在新线程运行之前,这就是为什么在④这里调用join()的原因,详见第二章,这会导致调用线程(在main()中)等待与std::thread对象相关联的线程,即这个例子中的t

//如果这看起来仅仅为了将一条信息写入标准输出而做了大量的工作,确实如此,正如上文1.2.3节所描述的,一般李硕并不值得为了如此简单的任务而使用多线程,尤其在这期间初试线程并没做什么,本书后面的内容中,将通过实例来展示在那些情况下使用多线程可以获得明确的收益

//1.5 小结

//本章中,提及了并发与多线程的含义,以及在你的引用程序中为什么你会选择使用(或不使用)它,还提及了多线程在C++中的发展历程,从1998标准中完全缺乏支持,经历了各种平台相关的扩展,再到新的C++标准中具有合适的多线程支持,芯片制造商选择了以多核心的形式,使得更多任务可以同时执行的方式来增加处理能力,而不是增加单个核心的执行速度,在这个趋势下,C++多线程来的正是时候,他使得程序员们可以利用新的CPU,带来的更加强大的硬件并发

//使用1.4节中例子,展示C++标准库中的类和函数有多么的简单,C++中使用多线程并不复杂,复杂的是如何设计代码以实现其预期的行为

//尝试了1.4节的示例后,是时候看看更多实质性的内容了

//第2章中,我们将了解一下用于管理线程的类和函数

#endif /* _1_0_h */


