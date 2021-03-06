Unix network Programming note

[Best C/C++ Network Library](http://stackoverflow.com/questions/118945/best-c-c-network-library)  
Aggregated List of Libraries

- Boost.Asio is really good, though the documentation is scarce.
- ZeroMQ (C++)
- Berkeley Sockets
- libevent
- libcurl

[C/C++ Web Server Library?](http://stackoverflow.com/questions/175507/c-c-web-server-library)

# TCP
Note that TCP does not guarantee that the data will be received by the other endpoint, as this is impossible. It delivers data to the other endpoint if possible, and notifies the user (by giving up on retransmissions and breaking the connection) if it is not possible. Therefore, TCP cannot be described as a 100% reliable protocol; it provides **reliable delivery of data or reliable notification of failure.**

**socket中TCP的三次握手建立连接**  
![socket中TCP的三次握手建立连接详解](http://images.cnblogs.com/cnblogs_com/skynet/201012/201012122157467258.png)  
从图中可以看出:

1. 当客户端调用connect时,触发了连接请求,向服务器发送了SYN J包,这时connect进入阻塞状态.  
1. 服务器监听到连接请求,即收到SYN J包,调用accept函数接收请求向客户端发送SYN K ,ACK J+1,这时accept进入阻塞状态.  
1. 客户端收到服务器的SYN K ,ACK J+1之后,这时connect返回,并对SYN K进行确认,服务器收到ACK K+1时,accept返回,至此三次握手完毕,连接建立.

客户端的`connect`在三次握手的第二个次返回,而服务器端的`accept`在三次握手的第三次返回.

内核为任何一个给定的监听套接字维护两个队列:

1. **未完成连接队列(incomplete connection queue)**: 每个这样的SYN 分节对应其中一项:
已由某个客户发出并到达服务器,而服务器正在等待完成相应的TCP三路握手过程.这些套接字处于SYN_RCVD状态
2. **已完成连接队列(completed connection queue)**, 每个已完成TCP 三路握手过程的客户对应其中一项. 这些套接字处于established 状态

当来自客户的SYN到达时,TCP在未完成连接队列中创建一个新项.  
如果三路握手正常完成, 该项就从未完成连接队列移到已完成连接队列的末尾.  
当进程调用accept时,已完成连接队列中的队头项将返回给进程,或者如果该队列为空,那么进程将被投入睡眠,直到TCP 在该队列中放入一项才唤醒它

**TCP FLAG 标记**  
基于标记的TCP包匹配经常被用于过滤试图打开新连接的TCP数据包.  
TCP标记和他们的意义如下所列:

* F : FIN - 结束; 结束会话
* S : SYN - 同步; 表示开始会话请求
* R : RST - 复位;中断一个连接
* P : PUSH - 推送; 数据包立即发送
* A : ACK - 应答
* U : URG - 紧急
* E : ECE - 显式拥塞提醒回应
* W : CWR - 拥塞窗口减少
 
# UDP
We also say that UDP provides a connectionless service, as there need not be any long-term relationship between a UDP client and server. 
For example, a UDP client can create a socket and send a datagram to a given server and then immediately send another datagram on the same socket to a different server. 
Similarly, a UDP server can receive several datagrams on a single UDP socket, each from a different client.

![UDP客户/服务器程序所用的套接字函数](http://pic002.cnblogs.com/images/2012/367190/2012081121141279.jpg)  
如上图所示, 客户不与服务器建立连接, 而是只管使用`sendto`函数给服务器发送数据报, 其中必须指定目的地(即服务器)第地址作为参数. 
类似的, 服务器不接受来自客户的连接, 而是只管调用`recvfrom` 函数, 等待来自某个客户的数据到达.
recvfrom将接收到的数据与client 的地址一并返回, 因此服务器可以把响应发送给正确的客户.

写一个长度为0 的数据报是可行的. 在UDP情况下, 这会形成一个只包含一个IP首部和一个UDP首部而没有数据的IP数据报. 这也意味着对于UDP协议, recvfrom返回0 值是可接受的.
他并不像TCP套接字上read 返回0值那样表示对端已关闭连接. 既然UDP是无连接的, 因此也没有诸如关闭一个UDP连接之类的事情.

大多数TCP服务器是并发的, 而大多数UDP服务器是迭代的

## API
- `int inet_pton(int af, const char *src, void *dst)`: presentation to network, 将点分十进制 －> 二进制整数, dst 的实际类型为`struct in_addr`
- `const char *inet_ntop(int af, const void *src, char *dst, socklen_t cnt)`: cnt指缓存区dst的大小, 如果缓存区太小无法存储地址的值, 则返回一个空指针, 并将errno置为ENOSPC

# Signal
就是告知某个进程发生了某个事件的通知, 有时也称为软件中断.
信号通常是异步发生的, 也就是说进程预先不知道信号的准确发生时刻
 
在进程表的表项中有一个软中断信号域,该域中每一位对应一个信号,当有信号发送给进程时,对应位置位被设置.  
由此可以看出,进程对不同的信号可以同时保留,但对于同一个信号,进程并不知道在处理之前来过多少个, 也就是信号不排队(但是现在有排队的信号实现).

在一个信号处理函数运行期间, 正被递交的信号是阻塞的. 而且, 安装处理函数时在传递给sigaction函数的sa_mask 信号集中指定的任何额外信号也被阻塞.

A process can selectively block the receipt of certain signals. 
When a signal is blocked, it can be delivered, but the resulting pending signal will not be received until the process unblocks the signal.

一般有3 种方式进行操作

1. `signal(SIGINT ,SIG_IGN)`: `SIG_IGN` 代表忽略信号. SIGINT信号代表由`InterruptKey`产生,通常是`CTRL+C` 或者是`DELETE` .发送给所有ForeGround Group的进程.
2. `signal(SIGINT ,SIG_DFL)`: `SIG_DFL`代表执行系统默认操作,其实对于大多数信号的系统默认动作时终止该进程.这与不写此处理函数是一样的.
3.  自定义的信号或是信号处理函数

## 内核如何实现信号的捕捉
如果信号的处理动作是用户自定义函数, 在信号递达时就调用这个函数, 这称为捕捉信号. 由于信号处理函数的代码是在用户空间的, 处理过程比较复杂:

1. 用户程序注册了SIGQUIT信号的处理函数sighandler.
1. 当前正在执行main函数,这时发生中断或异常切换到内核态.
1. 在中断处理完毕后要返回用户态的`main`函数之前检查到有信号SIGQUIT递达.
1. 内核返回用户态后不是恢复main函数的上下文继续执行, 而是执行sighandler函数. sighandler和main函数使用不同的堆栈空间, 它们之间不存在调用和被调用的关系,是两个独立的控制流程.
1. sighandler函数返回后自动执行特殊的系统调用sigreturn再次进入内核态.
1. 如果没有新的信号要递达,这次再返回用户态就是恢复main函数的上下文继续执行了.

当捕捉到信号时, 不论进程的主控制流程当前执行到哪儿, 都会先跳到信号处理函数中执行, 从信号处理函数返回后再继续执行主控制流程.
信号处理函数是一个单独的控制流程, 因为它和主控制流程是异步的, 二者不存在调用和被调用的关系,并且使用不同的堆栈空间.
引入了信号处理函数使得一个进程具有多个控制流程, 如果这些控制流程访问相同的全局资源(全局变量, 硬件资源等), 就有可能出现冲突.

## Pause
    #include <unistd.h>
    int pause(void);

pause函数使调用进程挂起直到有信号递达.

- 如果信号的处理动作是终止进程,则进程终止,pause函数没有机会返回
- 如果信号的处理动作是忽略,则进程继续处于挂起状态,pause不返回
- 如果信号的处理动作是捕捉,则调用了信号处理函数之后pause返回-1,errno设置为EINTR.

所以pause只有出错的返回值(想想以前还学过什么函数只有出错返回值?).错误码EINTR表示"被信号中断".

    #include <unistd.h>
    #include <signal.h>
    #include <stdio.h>
    
    void sig_alrm(int signo) {
    	/* nothing to do */
    }
    
    unsigned int mysleep(unsigned int nsecs) {
    	struct sigaction newact, oldact;
    	unsigned int unslept;
    
    	newact.sa_handler = sig_alrm;
    	sigemptyset(&newact.sa_mask);
    	newact.sa_flags = 0;
    	sigaction(SIGALRM, &newact, &oldact);
    
    	alarm(nsecs);
    	pause();
    
    	unslept = alarm(0);
    	sigaction(SIGALRM, &oldact, NULL);
    
    	return unslept;
    }
    
    int main(void) {
    	while(1){
    		mysleep(2);
    		printf("Two seconds passed\n");
    	}
    	return 0;
    }

上述代码的运行流程:

1. `main`函数调用`mysleep`函数,后者调用`sigaction`注册了`SIGALRM`信号的处理函数`sig_alrm`.
1. 调用`alarm(nsecs)`设定闹钟.
1. 调用`pause`等待,内核切换到别的进程运行.
1. `nsecs`秒之后,闹钟超时,内核发`SIGALRM`给这个进程.
1. 从内核态返回这个进程的用户态之前处理未决信号,发现有`SIGALRM`信号,其处理函数是`sig_alrm`.
1. 切换到用户态执行`sig_alrm`函数,进入`sig_alrm`函数时`SIGALRM`信号被自动屏蔽,
	从`sig_alrm`函数返回时`SIGALRM`信号自动解除屏蔽.然后自动执行系统调用`sigreturn`再次进入内核,再返回用户态继续执行进程的主控制流程(`main`函数调用的`mysleep`函数).
1. `pause`函数返回-1,然后调用`alarm(0)`取消闹钟,调用`sigaction`恢复`SIGALRM`信号以前的处理动作.

现在重新审视上面"mysleep",设想这样的时序:

1. 注册SIGALRM信号的处理函数.
1. 调用`alarm(nsecs)`设定闹钟.
1. 内核调度优先级更高的进程取代当前进程执行,并且优先级更高的进程有很多个,每个都要执行很长时间
1. nsecs秒钟之后闹钟超时了,内核发送`SIGALRM`信号给这个进程,处于未决状态.
1. 优先级更高的进程执行完了,内核要调度回这个进程执行.`SIGALRM`信号递达,执行处理函数`sig_alrm`之后再次进入内核.
1. 返回这个进程的主控制流程,`alarm(nsecs)`返回,调用`pause()`挂起等待.
**可是SIGALRM信号已经处理完了,还等待什么呢**?

出现这个问题的根本原因是**系统运行的时序(Timing)**并不像我们写程序时所设想的那样.  
虽然alarm(nsecs)紧接着的下一行就是`pause()`,但是无法保证`pause()`一定会在调用`alarm(nsecs)`之后的nsecs秒之内被调用.  
由于异步事件在任何时候都有可能发生(这里的异步事件指出现更高优先级的进程),如果我们写程序时考虑不周密,就可能由于时序问题而导致错误,这叫做**竞态条件(Race Condition)**

使用`sigsuspend` 代替可以解决`pause` 的问题.

## API
函数signal 的正常函数原型: `void (*signal (int signo, void (* func)(int)))(int);`  
为了简化, 我们定义  
```C
typedef void Sigfunc(int);
Sigfunc *signal(int signo, Sigfunc *func); 
func 是指向信号处理函数的指针, 返回值也是指向信号处理函数的指针
```

僵死进程
```C
#include <sys/wait.h>
pid_t wait(int *status);
pid_t waitpid(pid_t pid, int *status, int options); if pid == -1: meaning wait for any child process.
```

Used to wait for **state changes** in a child of the calling process, and obtain information about the child whose state has changed.  
A state change is considered to be: the child terminated; the child was stopped by a signal; or the child was **resumed by a signal**.  
In the case of a terminated child, performing a wait allows the system to release the resources associated with the child;
if a wait is not performed, then the terminated child remains in a "zombie" state
If a child has already changed state, then these calls return immediately.  Otherwise they block until either a child changes state or a signal handler interrupts the call

The value of options is an OR of zero or more of the following constants:

- WNOHANG: return immediately if no child has exited, 最常用的选项
- WUNTRACED: also return if a child has stopped
- WCONTINUED: also return if a stopped child has been resumed by delivery of SIGCONT

# I/O 模型
在Unix 下的可用的5 中I/O 模型  

- 阻塞式I/O: 默认情况下, 所有套接字都是阻塞的.
- 非阻塞式I/O
- I/O 复用(select/poll/epoll)
- 信号驱动式I/O(SIGIO)
- 异步IO

![模型的比较](https://i.imgbox.com/OsCC0HMm.gif) 

## IO复用模型
I/O多路复用通过一种机制, 可以监视多个描述符, 一旦某个描述符就绪(一般是读就绪或者写就绪), 能够通知程序进行相应的读写操作.
但select, poll, epoll本质上都是同步I/O, 因为他们都需要在读写事件就绪后自己负责进行读写, 也就是说这个读写过程是阻塞的

[select, poll, epoll之间的区别总结](http://www.cnblogs.com/Anker/p/3265058.html)

### Select
```C
while true{
	select(streams[]);
	for i in streams[]{
		if i has data{
			read or write i;
		}
	}
}
```

描述符就绪条件

- 满足下面4 个条件中的任何一个时, 一个套接字准备好读
	1. todo:该套接字接受缓冲区中的数据字节数大于等于套接字接受缓冲区**低水位标记**的当前大小. 对这样的套接字执行读操作不会阻塞并将**返回一个大于0的值**
	2. 该连接的读半部关闭(也就是接受了FIN的TCP连接). **读不阻塞并返回0**(也就是EOF)(读已经关闭了, 读不了内容, 所以不用阻塞)
	3. 该套接字是一个监听套接字, 且已完成的连接数不为0. 对这样的套接字的accept通常不会阻塞
	4. 有一个套接字错误待处理. 不会阻塞并返回-1, 同时把errno 置为确切的错误条件
- 满足下面4 个条件中的任何一个时, 一个套接字准备好写
	1. todo该套接字发送缓冲区中的可用空间
	2. 该连接的写半部关闭. 对这样的套接字的写将返回SIGPIPE信号
	3. 使用非阻塞式connect 的套接字已建立连接, 或者connect 以失败告终
	4. 其上有一个套接字错误待处理. 不会阻塞并返回-1, 同时把errno 置为确切的错误条件

example:

1. 如果对端TCP发送一个RST(对端主机崩溃并重新启动), 那么该套接字变位可读, 并且read返回-1, 而errno中含有确切的错误代码.
2. 如果对端TCP发送数据, 那么该套接字变为可读, 并且read 返回一个大于0 的值(即读入数据的字节数).

使用select库的步骤:

1. 创建所关注的事件的描述符集合(`fd_set`),对于一个描述符,可以关注其上面的读(read), 写(write), 异常(exception)事件,所以通常,要创建三个fd_set, 如果不关注, 可以设为null
1. 然后调用FD_SET 对集合进程初始化, 然后用FD_SET 打开需要关注的事件
1. 调用select(),等待事件发生. 这里需要注意的一点是,select的阻塞与是否设置非阻塞I/O是没有关系的. select()的返回值指定了发生事件的fd个数.
	其中,最后一个参数timeout,可以设置select等待的时间.i)如果该值设置为0, select()在检查描述符后就立即返回;
	ii)如果该值为null, 那么select 会永远等待下去,一直到有一个描述符准备好IO; iii)其他值, select()会等待指定的时间,然后再返回.
1. 用`FD_ISSET`测试fd_set中的每一个fd ,检查是否有相应的事件发生,如果有,就进行处理.

API
```C
#include <sys/select.h>
int select(int nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, struct timeval *timeout); //the nfds 等于关注的fds中的最大值加1  
void FD_CLR(int fd, fd_set *set);
int FD_ISSET(int fd, fd_set *set);  //test
void FD_SET(int fd, fd_set *set);
void FD_ZERO(fd_set *set); //set the set empty
	
example: tcpcliser/tcpservselect01.c; elect/strcliselect02.c
```

select的几大缺点:

1. 每次调用select, 都需要把fd集合从用户态拷贝到内核态, 这个开销在fd很多时会很大
1. 同时每次调用select都需要在内核遍历传递进来的所有fd, 这个开销在fd很多时也很大
1. select支持的文件描述符数量太小了, 默认是1024
1. select 正常返回, 仅仅说明有I/O事件发生了, 但并不知道是那几个流, 我们只能无差别轮询所有流, 找出能读出数据, 或者写入数据的流, 对他们进行操作. 因此有O(n)的无差别轮询复杂度.

### poll
poll的实现和select非常相似,只是描述fd集合的方式不同,poll使用pollfd结构而不是select的fd_set结构,其他的都差不多.
poll与select的主要区别在与,select需要为读,写,异常事件分配创建一个描述符集合,最后轮询的时候,需要分别轮询这三个集合.
而poll只需要一个集合,在每个描述符对应的结构上分别设置读,写,异常事件,最后轮询的时候,可以同时检查三种事件.

### epoll
[我读过的最好的epoll讲解](https://my.oschina.net/dclink/blog/287198)

```C
while true{
	active_stream[] = epoll_wait(epollfd);
	for i in active_stream[]{
		read or write i;
	}
}
```
epoll提供了三个函数: `#include <sys/epoll.h>`

- `int epoll_create(int size)`: 创建一个epoll句柄, size 要大于 0
- `int epoll_ctl(int epfd, int op, int fd, struct epoll_event *event)`: 注册要监听的事件类型, op指定add, mod, del
- `int epoll_wait(int epfd, struct epoll_event *events, int maxevents, int timeout);` 等待事件的产生

针对select 的4个缺点

1. 对于第一个缺点, epoll的解决方案在epoll_ctl函数中, 每次注册新的事件到epoll句柄中时(在epoll_ctl中指定EPOLL_CTL_ADD), 会把所有的fd拷贝进内核, 而不是在epoll_wait的时候重复拷贝.
	epoll保证了每个fd在整个过程中只会拷贝一次.
1. 对于第二个缺点, epoll的解决方案不像select或poll一样每次都把current轮流加入fd对应的设备等待队列中, 而只在epoll_ctl时把current挂一遍(这一遍必不可少)并为每个fd指定一个回调函数,
	当设备就绪, 唤醒等待队列上的等待者时, 就会调用这个回调函数, 而这个回调函数会把就绪的fd加入一个就绪链表).
	epoll_wait的工作实际上就是在这个就绪链表中查看有没有就绪的fd(利用schedule_timeout()实现睡一会,判断一会的效果, 和select实现中的第7步是类似的).
1. 对于第三个缺点, epoll没有这个限制, 它所支持的FD上限是最大可以打开文件的数目, 这个数字一般远大于2048.
	举个例子,在1GB内存的机器上大约是10万左右,具体数目可以cat /proc/sys/fs/file-max察看,一般来说这个数目和系统内存关系很大.
1. 对于第四个缺点, epoll 使用回调函数的形式, 避免了全部轮询

Epoll的2种工作方式- Level Triggered(LT)和Edge Triggered(ET)

- LT(level triggered)是epoll缺省的工作方式,并且同时支持block和no-block socket.
	在这种做法中,内核告诉你一个文件描述符是否就绪了,然后你可以对这个就绪的fd进行IO操作.
	如果你不作任何操作,内核还是会继续通知你的,所以,这种模式编程出错误可能性要小一点.传统的select/poll都是这种模型的代表
- ET (edge-triggered)是高速工作方式,只支持no-block socket,它效率要比LT更高.
	ET与LT的区别在于,当一个新的事件到来时,ET模式下当然可以从epoll_wait调用中获取到这个事件,可是如果这次没有把这个事件对应的套接字缓冲区处理完,
	在这个套接字中没有新的事件再次到来时,在ET模式下是无法再次从epoll_wait调用中获取这个事件的.
	而LT模式正好相反,只要一个事件对应的套接字缓冲区还有数据,就总能从epoll_wait中获取这个事件.
	因此,LT模式下开发基于epoll的应用要简单些,不太容易出错.而在ET模式下事件发生时,如果没有彻底地将缓冲区数据处理完,则会导致缓冲区中的用户请求得不到响应.

# 高级IO函数
## 套接字超时
在涉及套接字的IO操作上设置超时的方法有以下3 种:

1. 调用`alarm`, 他在指定超时期满时产生`SIGALARM`信号.
1. 在`select`中阻塞等待IO(select 有内置的时间限制), 以此代替直接阻塞在read或write调用上
1. 使用较新的`SO_RECVTIMEO`和`SO_SNDTIMEO`套接字选项(但是并非所有实现都支持这两个套接字).

example: lib/connect_timeo.c, advio/dgclitimeo.c, lib/readable_timeo.c

## 高级轮询技术
Solaris 上名为`/dev/null`的特殊文件提供了一个可扩展的轮询大量描述符的方法.  
select 和 poll 存在的一个问题是, 每次调用它们都得传递待查询的文件描述符. 轮询设备能在调用之间维持状态, 因此轮询进程可以预先设置好待查询描述符的列表, 然后进入一个
循环等待事件发生, 每次循环回来时不必再次设置该列表.

### fcntl(file control) 函数
```C
#include <unistd.h>
#include <fcntl.h>
int fcntl(int fd, int cmd, ... /* arg */ );

使用fcntl开启非阻塞式IO的典型代码:
int flags = 0;
if((flags = fcntl(fd, F_GETFL, 0)) < 0){
	fprintf(stderr, "F_GETFL error");
}
flags |= O_NONBLOCK;
if(fcntl(fd, F_SETFL, flags) < 0){
	fprintf(stderr, "F_SETFL error");
}
```

### 可重入函数
若一个程序或子程序可以"安全的被并行执行(Parallel computing)",则称其为可重入(reentrant或re-entrant)的.

若一个函数是可重入的,则该函数:

1. 不能含有静态(全局)非常量数据.
1. 不能返回静态(全局)非常量数据的地址.
1. 只能处理由调用者提供的数据.
1. 不能依赖于单实例模式资源的锁.
1. 不能调用(call)不可重入的函数(有呼叫(call)到的函数需满足前述条件).

多"用户/对象/进程优先级"以及多进程,一般会使得对可重入代码的控制变得复杂.
同时, **IO代码通常不是可重入的**,因为他们依赖于像磁盘这样共享的,单独的(类似编程中的静态(Static),全域(Global))资源.

调用了malloc或free,因为malloc也是用**全局链表来管理堆**的  
调用了标准I/O库函数.标准I/O库的很多实现都以不可重入的方式使用**全局数据结构**.

#### errno 变量
errno 变量存在不可重入的问题, 这个整型变量历来每个进程各有一个副本. 但是同一个进程的各个线程共享一个errno 变量.

从close 系统调用返回时把错误代码存入errno到稍后由程序显示errno 的值之间存在一个小的时间窗口. 
期间同一个进程内的另一个执行线程(例如一个信号处理函数的某次调用) 可能改变了errno 的值.
就信号处理函数考虑这个问题, 可通过把信号处理函数编写成预先保存并事后恢复errno 的值加以避免, 代码如下:
```C
void sig_alarm(int signo){
	int errno_save;
	errno_save = errno; //save its value en entry
	if(write(...) != nbytes){
		fprintf(stderr, "write error, errno=%d\n,errno);
		errno = errno_save;
	}
}
```
这段代码中我们, 在信号处理函数中调用了fpritnf 这个标准IO 函数, 它引入了另外一个重入问题.

# 守护进程和inetd 超级服务器
要启动一个守护进程,可以采取以下的几种方式:

1. 在系统期间通过系统的初始化脚本启动守护进程.这些脚本通常在目录etc/rc.d 下, 通过它们所启动的守护进程具有超级用户的权限.系统的一些基本服务程序通常都是通过这种方式启动的.
2. 很多网络服务程序是由inetd 守护程序启动的. 它监听各种网络请求,如telnet,ftp 等,在请求到达时启动相应的服务器程序(telnet server, ftp server 等).
3. 由cron 定时启动的处理程序.这些程序在运行时实际上也是一个守护进程.
4. 由at 启动的处理程序.
5. 守护程序也可以从终端启动,通常这种方式只用于守护进程的测试,或者是重起因某种原因而停止的进程.
6. 在终端上用nohup 启动的进程.用这种方法可以把所有的程序都变为守护进程

## 守护进程启动实例
```
#include"unp.h"
#include<syslog.h>
 
#define MAXFD 64
extern int daemon_proc; /* defined in error.c */
 
int daemon_init(const char *pname, int facility){
	int i;
	pid_t pid;

	//若本程序从前台作为一个shell命令启动,当父进程终止时,shell会认为该命令已执行完毕.这样子进程就自动在后台运行
	//另外,子进程继承了父进程的进程组ID, 这就保证了子进程不是一个进程组的头进程, 这是接下去调用setsid的必要条件
	if ((pid = Fork()) < 0)
		return (-1); //error
	else if (pid)
		_exit(1);   /* parent terminates */

	/* child 1 continues... */

	// 当前进程变为新会话的会话头进程以及新进程组的进程组头进程, 从而不再有控制终端
	if (setsid() < 0)   /* become session leader */
		return (-1);

	// 这里必须忽略SIGHUP信号, 因为当会话头进程终止时(即首次fork 产生的子进程) 终止时, 
	// 其会话中的所有进程(即在此fork产生的子进程) 都收到SIGHUP信号    
	Signal(SIGHUP, SIG_IGN);

	// 再次调用fork的目的是确保本守护进程即使将来打开了一个终端设备, 也不会自动获得控制终端.
	// 因为当没有控制终端的会话头进程打开一个终端设备时, 该终端自动成为这个会话头进程的控制终端. 
	// 然后再次调用fork 之后, 我们确保新的子进程不再是一个会话头进程, 从而不能自动获得一个控制终端
	if ((pid = Fork()) < 0)
		return (-1);
	else if (pid)
		_exit(0);   /* child 1 terminates */

	/* child 2 continues... */

	daemon_proc = 1;/* for err_XXX() functions */

	// 使用fork创建的子进程继承了父进程的当前工作目录, 切换到根目录, 可以避免将来的文件系统卸载问题
	chdir("/"); /* change working directory */

	/* close off file descriptors */
	for (i = 0; i < MAXFD; i++)
		close(i);

	/* redirect stdin, stdout, and stderr to /dev/null */
	// 保证这些常用描述符是打开的, 针对他们的系统调用read 返回0(EOF)
	open("/dev/null", O_RDONLY);
	open("/dev/null", O_RDWR);
	open("/dev/null", O_RDWR);

	openlog(pname, LOG_PID, facility);

	return (0); /* success */
}
```	
example: inetd/daytimetcpserv2.c

进程组:是一个或多个进程的集合.
进程组有进程组ID来唯一标识.除了进程号(PID)之外,进程组ID也是一个进程的必备属性.
每个进程组都有一个组长进程,其组长进程的进程号等于进程组ID.且该**进程组ID不会因组长进程的退出而受到影响**.  

会话周期:会话期是一个或多个进程组的集合.通常,一个会话开始于用户登录,终止于用户退出,在此期间该用户运行的所有进程都属于这个会话期.

setsid函数用于创建一个新的会话,并担任该会话组的组长.调用setsid有下面的3个作用:

1. 让进程摆脱原会话的控制
1. 让进程摆脱原进程组的控制
1. 让进程摆脱原控制终端的控制

那么,在创建守护进程时为什么要调用setsid函数呢?
由于创建守护进程的第一步调用了fork函数来创建子进程,再将父进程退出.
由于在调用了fork函数时,子进程全盘拷贝了父进程的会话期,进程组,控制终端等,虽然父进程退出了,但会话期,进程组,控制终端等并没有改变,
因此,这还不是真正意义上的独立开来,而setsid函数能够使进程完全独立出来,从而摆脱其他进程的控制.

既然守护进程在没有控制终端的环境下运行, 他绝不会收到来自内核的`SIGHUP` 信号.
许多守护进程因此把这个吸纳后作为来自管理员的一个通知, 表示其配置文件爱你已经发生更改, 需要重新读取配置文件.  
守护进程同样绝不会收到来自内核的`SIGINT` 和`SIGWINCH` 信号, 因此这些信号也可以安全地用作系统管理员的通知手段, 指示守护进程应作出某种反应.

当用户需要外部停止守护进程运行时,往往会使用 kill命令停止该守护进程.所以,守护进程中需要编码来实现kill发出的signal信号处理,达到进程的正常退出

# 非阻塞式IO
套接字的默认状态是阻塞的. 这就意味着当发出一个不能立即完成的套接字调用时, 其进程将被投入睡眠, 等待相应操作完成.  
可能阻塞的套接字调用可分为以下四类

1. 输入操作: 包括`read, readv, recv, recvfrom, recvmsg` 共5 个函数.  
	- 如果某个进程对一个阻塞的TCP套接字(默认设置)调用这些输入函数, 而且**该套接字的接收缓冲区中没有数据可读**, 该进程将被投入睡眠, 直到有一些数据到达. 
	既然TCP 是字节流协议, 该进程的唤醒就是只要有一些数据到达, 可以使单个字节, 也可以是一个完整的TCP分节. 
	如果想等到某个固定数目的数据可读为止, 那么可以调用我们的`readn` 函数或者指定`MSG_WAITALL` 标志.  
	- 既然UDP是数据报协议, 如果一个阻塞的UDP套接字的接收缓冲区为空, 对它调用输入函数的进程将被投入睡眠, 直到有UDP数据报到达.  
	- 对于非阻塞的套接字, 如果输入操作不能被满足, 相应调用将立即返回一个`EWOULDBLOCK` 错误.
1. 输出操作: 包括`write, writev, send, sendto, sendmsg` 共5 个函数.  
	- 对于TCP, 内核将从应用进程的缓冲区到该套接字的发送缓冲区复制数据. 对于阻塞的套接字, 如果其发送发送缓冲区中没有空间, 进程将被投入睡眠, 直到有空间为止.
	- 对于一个非阻塞的TCP套接字, 如果其发送缓冲区中根本没有空间, 输出操作调用将立即返回一个`EWOULDBLOCK` 错误.
	- UDP套接字不存在真正的发送缓冲区. 内核只是复制应用进程数据并把它沿协议栈向下传送, 渐次冠以UDP首部和IP首部. 因此对一个阻塞的UDP套接字(默认设置), 输出函数调用将不会因与TCP套接字一样的原因二阻塞, 不过有可能会因为其他的原因而阻塞.
1. 接受外来连接, 即`accept`函数
	- 对一个阻塞的套接字调用accept 函数, 并且尚无新的连接到达, 调用进程将被投入睡眠
	- 对一个非阻塞的套接字调用accept函数, 并且尚无新的连接到达, 调用将立即返回一个`EWOULDBLOCK` 错误.
1. 发起外出连接, 即用于TCP的`connect` 函数
	- TCP的建立涉及一个三路握手过程, 而且`connect` 函数一直要等到客户收到对于自己的`SYN`的`ACK` 为止才返回. 这意味着TCP的每个connect 总会阻塞其调用进程至少一个到服务器的RTT时间.
	- 如果对一个非阻塞的TCP套接字调用connect, 并且连接不能立即建立, 那么连接的建立能照样发起(譬如发送出TCP三路握手的第一个分组), 不过会返回一个`EINPROGRESS`错误. 另请注意, 有些连接可以立即建立, 通常是由于服务器和客户同在一个主机, 因此对于一个非阻塞的connect, 我们也得预备connect 成功返回的情况发生.

# 信号驱动式IO
进程预先告知内核, 使得当某个描述符上发生某事时, 内核使用信号通知相关进程.

针对一个套接字使用信号驱动式IO(SIGIO)要求进程执行以下3个步骤:

1. 建立SIGIO信号的信号处理函数
1. 设置该套接字的属主, 通常使用`fcntl`的`F_SETOWN`命令设置
1. 开启该套接字的信号驱动式IO, 通常通过使用`fcntl` 的`F_SETFL`命令打开`O_ASYNC`标志完成

尽管很容易把一个套接字设置成以信号驱动式IO模式工作, 确定哪些条件导致内核条件递交给套接字属主的SIGIO信号却殊非易事. 这种判定取决于支撑协议.

- 对于UDP套接字, SIGIO信号在发生以下事件时产生
	- 数据报到达套接字
	- 套接字上发生异步错误
- 对于TCP套接字, 不幸的是, 信号驱动式IO对于TCP套接字近乎无用. 问题在于该信号产生得过于频繁, 并且它的出现并没有告诉我们发生了什么事件. 下列条件均导致对于一个TCP套接字产生SIGIO信号.
	- 监听套接字上某个连接请求已经完成
	- 某个断连请求已经发起
	- 某个断连请求已经关闭
	- 某个连接之半已经关闭
	- 数据到达套接字
	- 发生某个异步错误

举例来说, 如果一个进程即读自又写往一个TCP套接字, 那么当有新数据到达时或者当以前写出的数据得到确认时, SIGIO信号均会产生, 而且信号处理函数中无法区分这两种情况.  
如果SIGIO用于这种数据读写情形, 那么TCP套接字应该设置成非阻塞式, 以防`read`或`write`发生阻塞.  
我们应该考虑**只对监听套接字使用SIGIO**, 因为对于监听套接字产生SIGIO的唯一条件式某个新连接的完成

# 进程

    #include <sys/types.h>
    #include <unistd.h>
    #include <stdio.h>
    #include <stdlib.h>
    
    int main(void){
    	pid_t pid;
    	char *message;
    	int n;
    	pid = fork();
    	if (pid < 0) {
    		perror("fork failed");
    		exit(1);
    	}
    	if (pid == 0) {
    		message = "This is the child\n";
    		n = 6;
    	} else {
    		message = "This is the parent\n";
    		n = 3;
    	}
    	for(; n > 0; n--) {
    		printf(message);
    		sleep(1);
    	}
    	return 0;
    }
    
    output:
    $ ./a.out 
    This is the child
    This is the parent
    This is the child
    This is the parent
    This is the child
    This is the parent
    This is the child
    $ This is the child
    This is the child

# 线程 pthread
`fork` 是昂贵的. fork要把父进程的内存镜像复制到子进程, 并在子进程中复制所有描述符, 如此, 等等.  
子进程获取父进程数据空间,堆和栈的副本, 包括缓冲区

同一进程内的所有线程共享

1. 相同的全局内存(也就是全局变量)
1. 打开的文件(即描述符)
1. 信号处理函数和信号处置
1. 进程指令
1. 大多数数据
1. 当前工作目录
1. 用户ID和组ID

All threads share a common heap.

Each thread has a private stack, which it can quickly add and remove items from. 
This makes stack based memory fast, but if you use too much stack memory, as occurs in infinite recursion, you will get a stack overflow.

Since all threads share the same heap, access to the allocator/deallocator must be synchronized. 
There are various methods and libraries for avoiding allocator contention.

Some languages allow you to create private pools of memory, or individual heaps, which you can assign to a single thread.

不过每个线程拥有各自的

1. 线程ID
1. 寄存器集合, 包括程序计数器和栈指针
1. 栈(用于存放局部变量和返回地址)
1. errno. [ref](http://learn.akae.cn/media/ch35s02.html). 
	pthread库的函数都是通过返回值返回错误号,虽然每个线程也都有一个errno,但这是为了兼容其它函数接口而提供的,pthread库本身并不使用它. 
	所以errno 还是看成同一个进程的所有线程共享一个全局的errno.
1. 信号掩码
1. 优先级

一般情况下,线程终止后,其终止状态一直保留到其它线程调用`pthread_join`获取它的状态为止.  
但是线程也可以被置为`detach`状态,这样的线程一旦终止就**立刻回收它占用的所有资源**,而不保留终止状态.  
不能对一个已经处于`detach`状态的线程调用`pthread_join`,这样的调用将返回`EINVAL`

## 线程特定数据
也被称为线程私有数据,是一种存储和查找一个特定线程相关数据的机制.每个线程访问它自己独立的数据,而不用担心和其它线程的访问的同步.

线程特定数据看似很复杂,其实我们可以把它理解为就是一个索引和指针.key结构中存储的是索引,pthread结构中存储的是指针,指向线程中的私有数据,通常是malloc函数返回的指针.

## 互斥锁 Mutual Exclusive Lock
互斥锁变量(`pthread_mutex_t`类型)是静态分配的(全局变量或static变量), 我们就必须把他初始化为常值`PTHREAD_MUTEX_INITIALIZER`  
如果我们在共享内存区中分配一个互斥锁, 那么必须通过调用`pthread_mutex_init` 函数在运行时把它初始化.

**"挂起等待"和"唤醒等待线程"的操作如何实现**?  
每个Mutex有一个等待队列,一个线程要在Mutex上挂起等待,首先在把自己加入等待队列中,然后置线程状态为睡眠,然后调用调度器函数切换到别的线程.
一个线程要唤醒等待队列中的其它线程,只需从等待队列中取出一项,把它的状态从睡眠改为就绪,加入就绪队列,那么下次调度器函数执行时就有可能切换到被唤醒的线程.

## 条件变量 condition variable
互斥锁提供互斥机制, 条件变量提供信号机制

线程间的同步还有这样一种情况:线程A需要等某个条件成立才能继续往下执行,现在这个条件不成立,线程A就阻塞等待,而线程B在执行过程中使这个条件成立了,就唤醒线程A继续执行.

一个Condition Variable总是和一个Mutex搭配使用的.

一个线程可以调用`pthread_cond_wait`在一个Condition Variable上阻塞等待,这个函数做以下三步操作:

1. 释放Mutex
1. 阻塞等待
1. 当被唤醒时,重新获得Mutex并返回

## Semaphore
Mutex变量是非0即1的,可看作一种资源的可用数量,初始化时Mutex是1,表示有一个可用资源.
加锁时获得该资源,将Mutex减到0,表示不再有可用资源,解锁时释放该资源,将Mutex重新加到1,表示又有了一个可用资源.

信号量(Semaphore)和Mutex类似,表示可用资源的数量,和Mutex不同的是这个数量可以大于1.

本节介绍的是POSIX semaphore库函数,详见sem_overview(7),这种信号量不仅可用于同一进程的线程间同步,也可用于不同进程间的同步.

## 其它线程间同步机制
如果共享数据是只读的,那么各线程读到的数据应该总是一致的,不会出现访问冲突.
只要有一个线程可以改写数据,就必须考虑线程间同步的问题.由此引出了读者写者锁(Reader-Writer Lock)的概念.
Reader之间并不互斥,可以同时读共享数据,而Writer是独占的(exclusive),在Writer修改数据时其它Reader或Writer不能访问数据,可见Reader-Writer Lock比Mutex具有更好的并发性.

用挂起等待的方式解决访问冲突不见得是最好的办法,因为这样毕竟会影响系统的并发性,
在某些情况下解决访问冲突的问题可以尽量避免挂起某个线程,例如Linux内核的Seqlock,RCU(read-copy-update)等机制.

## pthread API
    #include <pthread.h>
    int pthread_create(pthread_t *restrict thread, const pthread_attr_t *restrict attr, 
						void *(*start_routine)(void*), void *restrict arg);
    返回值:成功返回0,失败返回正的错误号, Exxx
    类似于fork
    
    pthread_t pthread_self(void)
    获得当前线程的id
    类似于getpid
    
    void pthread_exit(void *value_ptr);
    pthread_exit或者return返回的指针所指向的内存单元必须是全局的或者是用malloc分配的,不能在线程函数的栈上分配,
	因为当其它线程得到这个返回指针时线程函数已经退出了.
    
    int pthread_join(pthread_t thread, void **value_ptr);
    返回值:成功返回0,失败返回错误号
    类似于waitpid
    
    int pthread_detach(pthread_t tid);
    返回值:成功返回0,失败返回错误号

    pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
    int pthread_mutex_destroy(pthread_mutex_t *mutex);
    int pthread_mutex_init(pthread_mutex_t *restrict mutex, const pthread_mutexattr_t *restrict attr);
	返回值:成功返回0,失败返回错误号

The `pthread_mutex_init()` function initializes a mutex with the specified attributes for use.  
If attr is specified as NULL, all attributes are set to the default mutex attributes for the newly created mutex.

Mutex initialization using the `PTHREAD_MUTEX_INITIALIZER` does not immediately initialize the mutex. 
Instead, on first use, the `pthread_mutex_lock()` or `pthread_mutex_trylock()` functions branch into a slow path and cause the initialization of the mutex.  
Because a mutex is not just a simple memory object and requires that some resources be allocated by the system, 
an attempt to call `pthread_mutex_destroy()` or `pthread_mutex_unlock()` on a mutex that was statically initialized using `PTHREAD_MUTEX_INITIALER` and 
was not yet locked causes an `EINVAL` error.

    int pthread_mutex_lock(pthread_mutex_t *mutex);
    int pthread_mutex_unlock(pthread_mutex_t *mutex);
    int pthread_mutex_trylock(pthread_mutex_t *mutex);
    如果一个线程既想获得锁,又不想挂起等待,可以调用pthread_mutex_trylock,如果Mutex已经被另一个线程获得,
	这个函数会失败返回EBUSY,而不会使线程挂起等待.还可以缓解死锁

    pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

    int pthread_cond_destroy(pthread_cond_t *cond);
    int pthread_cond_init(pthread_cond_t *restrict cond, const pthread_condattr_t *restrict attr);
    返回值:成功返回0,失败返回错误号.
    和Mutex的初始化和销毁类似,pthread_cond_init函数初始化一个Condition Variable,attr参数为NULL则表示缺省属性,
	pthread_cond_destroy函数销毁一个Condition Variable.
	如果Condition Variable是静态分配的,也可以用宏定义PTHEAD_COND_INITIALIZER初始化,相当于用pthread_cond_init初始化并且attr参数为NULL
    
    int pthread_cond_wait(pthread_cond_t *restrict cond, pthread_mutex_t *restrict mutex);
    int pthread_cond_signal(pthread_cond_t *cond);

    int pthread_cond_timedwait(pthread_cond_t *restrict cond, pthread_mutex_t *restrict mutex, 
								const struct timespec *restrict abstime);
    int pthread_cond_broadcast(pthread_cond_t *cond);

# Address
## IP
Convert IP addresses from a dots-and-number string to a struct in_addr and back
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>

    char *inet_ntoa(struct in_addr in);
    int inet_aton(const char *cp, struct in_addr *inp);
    in_addr_t inet_addr(const char *cp);

`inet_aton()`: **dots string to a number**. returns non-zero if the address is a valid one, and it returns zero if the address is invalid.  
`inet_ntoa()`: **number to a dots string**. returns the dots-and-numbers string **in a static buffer that is overwritten with each call to the function**.  
`inet_addr()` returns the address as an `in_addr_t` an `u_int32_t` value, or -1 if there is an error.

	struct sockaddr_in antelope;
	char *some_addr;

	inet_aton("10.0.0.1", &antelope.sin_addr); // store IP in antelope

	some_addr = inet_ntoa(antelope.sin_addr); // return the IP
	printf("%s\n", some_addr); // prints "10.0.0.1"

	// and this call is the same as the inet_aton() call, above:
	antelope.sin_addr.s_addr = inet_addr("10.0.0.1");
	
	 /* Internet address.  */
	typedef uint32_t in_addr_t;                                                 
	struct in_addr{
	    in_addr_t s_addr;
	};

Example of `inet_ntoa`

	struct in_addr addr1,addr2;
	ulong l1,l2;
	l1= inet_addr("192.168.0.74");
	l2 = inet_addr("211.100.21.179");
	memcpy(&addr1, &l1, 4);
	memcpy(&addr2, &l2, 4);
	printf("%s : %s\n", inet_ntoa(addr1), inet_ntoa(addr2)); //注意这一句的运行结果
	printf("%s\n", inet_ntoa(addr1));
	printf("%s\n", inet_ntoa(addr2));
	return 0;
	}
实际运行结果如下:  
192.168.0.74 : 192.168.0.74 //从这里可以看出,`printf`里的`inet_ntoa(addr2)`先于`inet_ntoa(addr1)`执行.  
192.168.0.74  
211.100.21.179  
`inet_ntoa`返回一个`char *`,而这个`char *`的空间是在`inet_ntoa`里面静态分配的,所以`inet_ntoa`后面的调用会覆盖上一次的调用.  
第一句printf的结果只能说明在printf里面的可变参数的求值是从右到左的,仅此而已.


**IP 的结构**
```    
struct ip{
    #if __BYTE_ORDER == __LITTLE_ENDIAN
    unsigned int ip_hl:4; /* header length */
    unsigned int ip_v:4; /* version */
    #endif
    #if __BYTE_ORDER == __BIG_ENDIAN
    unsigned int ip_v:4; /* version */
    unsigned int ip_hl:4; /* header length */
    #endif
    u_int8_t ip_tos; /* type of service */
    u_short ip_len; /* total length */
    u_short ip_id; /* identification */
    u_short ip_off; /* fragment offset field */
    #define IP_RF 0x8000 /* reserved fragment flag */
    #define IP_DF 0x4000 /* dont fragment flag */
    #define IP_MF 0x2000 /* more fragments flag */
    #define IP_OFFMASK 0x1fff /* mask for fragmenting bits */
    u_int8_t ip_ttl; /* time to live */
    u_int8_t ip_p; /* protocol */
    u_short ip_sum; /* checksum */
    struct in_addr ip_src, ip_dst; /* source and dest address */
}; 
```    

## Socket
    #include <netinet/in.h>
    struct sockaddr_in {
        short            sin_family;   // e.g. AF_INET
        unsigned short   sin_port;     // e.g. htons(3490)
        struct in_addr   sin_addr;     // see struct in_addr, below
        char             sin_zero[8];  // zero this if you want to
    };

    struct in_addr {
        unsigned long s_addr;  // load with inet_aton()
    };

`gethostbyname`和`gethostbyaddr`这两个函数仅仅支持IPv4,  
`getaddrinfo`函数能够处理名字到地址以及服务到端口这两种转换,返回的是一个`sockaddr`结构的链表而不是一个地址清单. 这些`sockaddr`结构随后可由套接口函数直接使用.如此一来,`getaddrinfo`函数把协议相关性安全隐藏在这个库函数内部.应用程序只要处理由getaddrinfo函数填写的套接口地址结构.该函数在 POSIX规范中定义了.

    #include<netdb.h>
    int getaddrinfo( const char *hostname, const char *service, 
					 const struct addrinfo *hints, struct addrinfo **result );
	返回值:0-成功,非0-出错
参数说明
hostname:一个主机名或者地址串(IPv4的点分十进制串或者IPv6的16进制串)  
service:服务名可以是十进制的端口号,也可以是已定义的服务名称,如ftp,http等  
hints:可以是一个空指针,也可以是一个指向某个addrinfo结构体的指针, 调用者在这个结构中填入关于期望返回的信息类型的暗示. 
	举例来说:指定的服务既可支持TCP也可支持UDP,所以调用者可以把	hints结构中的ai_socktype成员设置成SOCK_DGRAM使得返回的仅仅是适用于数据报套接口的信息.  
result:本函数通过result指针参数返回一个指向addrinfo结构体链表的指针.  

`getnameinfo` converts a socket address to a corresponding host and service, in a protocol-independent manner.  
It combines the functionality of `gethostbyaddr(3)` and `getservbyport(3)`, but unlike those functions, 
`getnameinfo()` is reentrant and allows programs to eliminate IPv4-versus-IPv6 dependencies.
the inverse is `getaddrinfo(3)`

