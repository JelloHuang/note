强数据类型 `/usr/include/sys/types.h`  
`u_int64_t, int64_t, u_int32_t, int32_t, u_int16_t, int16_t, u_int8_t, int8_t ...`

指针数组: `int *ptr_array[10]`, 每个元组都是指针, 共10个元素.  
数组指针: `int (\*)array_ptr[10]`, 指向一个10个元素数组的指针.

[运算符](http://www.math.pku.edu.cn/teachers/qiuzy/c/operator.htm)

| 运算符                           | 解释                                 | 结合方式 |
| -------------------------------- | ---------------------------------- | ---- |
| () [] -> .                       | 括号(函数等),数组,两种结构成员访问                | 由左向右 |
| ! ~ ++ -- + - * & (类型) sizeof  | 否定,按位否定,增量,减量,正负号, 间接,取地址,类型转换,求大小 | 由右向左 |
| * / %                            | 乘,除,取模                             | 由左向右 |
| + -                              | 加,减                                | 由左向右 |
| << >>                            | 左移,右移                              | 由左向右 |
| < <= >= >                        | 小于,小于等于,大于等于,大于                    | 由左向右 |
| == !=                            | 等于,不等于                             | 由左向右 |
| &                                | 按位与                                | 由左向右 |
| ^                                | 按位异或(对应位相同的,结果为 0,不相同为 1) | 由左向右 |
| \|                                | 按位或                                | 由左向右 |
| &&                               | 逻辑与                                | 由左向右 |
| ||                               | 逻辑或                                | 由左向右 |
| ? :                              | 条件                                 | 由右向左 |
| = += -= *= /= &= ^= |= <<= >>=   | 各种赋值                               | 由右向左 |
| ,                                | 逗号(顺序)                             | 由左向右 |

**sizeof**  
Technically speaking, the C standard only guarantees that `sizeof(char) == 1`, and the rest is up to the implementation.  
In practice, pointers will be size 2 on a 16-bit system (if you can find one), 4 on a 32-bit system, and 8 on a 64-bit system, 
but there is nothing to be gained in relying on a given size

# String
`int snprintf(char *str, size_t size, const char *format, ...);`  
The functions `snprintf()` and `vsnprintf()`  write  **at most size bytes (including the trailing null byte ('\0'))** to str.

If I stick a `'\0'` into the middle of the allocated memory, does `free()` still work properly?  
Yes.

## char
	int main(){
		char b = 'a';
		printf("%lu,", sizeof ('a'));
		printf("%lu", sizeof (b));
		return 0;
	}

	OUTPUT: 4,1
character literals are of type int and not char in C.  
So `sizeof('a') == sizeof (int)`.  
Note that in C++, a character literal is of type char and so `sizeof('a') == sizeof(char)`

## [字符数组与字符指针](http://blog.csdn.net/qiumm/article/details/5657120)
	char *str = "This is a pointer to a string.";
字符指针指向的是一个字符串常量的首地址,即指向字符串的首地址.
	
	char string[ ]="This is an array of char."
string是字符数组,它存放了一个字符串, 编译会自动在string 末尾添加`\0`.

str是一个变量,可以改变str, 使它指向不同的字符串,但不能改变str所指的字符串常量.  
string是一个数组,可以改变数组中保存的内容(但是数组的名字string本身, 它是一个常量, 也就是说string is not assignable), 但是必须一个一个字符的操作, 而不能进行整体操作.  
错误的赋值操作: `string = "This is a NEW string.";`

**"abc"是常量吗?答案是有时是,有时不是.**  

不是常量的情况:"abc"作为字符数组初始值的时候就不是,如
	
	char str[] = "abc";
因为定义的是一个字符数组,所以就相当于定义了一些空间来存放"abc",而又因为字符数组就是把字符一个一个地存放的,
所以编译器把这个语句解析为 `char str[3] = {'a','b','c'};`, 然后补零, 所以最终结果是 `char str[4] = {'a','b','c','/0'};`  
做一下扩展,如果`char str[] = "abc";`, 是在函数内部写的话,那么这里的`"abc/0"`, 因为不是常量,所以应该被放在栈上.
 
是常量的情况: 把"abc"赋给一个字符指针变量时,如
 	
 	char* ptr = "abc";
因为定义的是一个普通指针,并**没有定义空间来存放"abc"**,所以编译器得帮我们找地方来放"abc",
显然,把这里的"abc"当成常量并把它放到程序的常量区是编译器最合适的选择.
所以尽管ptr的类型不是`const char *`,并且`ptr[0] = 'x'`;也能编译通过,但是执行`ptr[0] = 'x';`就会发生运行时异常,因为这个语句试图去修改程序常量区中的东西.  
但是建议的写法应该是`const char* ptr = "abc";`, 这样如果后面写`ptr[0] = 'x'`, 的话编译器就不会让它编译通过,也就避免了上面说的运行时异常.  
又扩展一下,如果`char* ptr = "abc";`, 写在函数体内,那么虽然这里的`"abc/0"`被
放在常量区中,但是ptr本身只是一个普通的指针变量,所以ptr是被放在栈上的, 只不过是它所指向的东西被放在常量区罢了.  

1. 数组的类型是由该数组所存放的东西的类型以及数组本身的大小决定的.  
如`char s1[3]`和`char s2[4]`,s1的类型就是`char[3]`,s2的类型就是`char[4]`  
也就是说尽管s1和s2都是字符数组,但两者的类型却是不同的
1. 字符串常量的类型可以理解为相应字符常量数组的类型,如"abcdef"的类型就可以看成是`const char[7]`
1. 对于函数参数列表中的以数组类型书写的形式参数,编译器把其解释为普通的指针类型,如对于
 	
 		void func(char sa[100],int ia[20],char *p)
则sa的类型为`char*`,ia的类型为`int*`,p的类型为`char*`, 所以我们看到的`string.h` 中的函数的参数都是 `char *` 或者`const char *`类型的, 而不是 `char []`.

`strcpy(ptr2, ptr1)` is equivalent to `while(\*ptr2++ = *ptr1++)`
where as strdup is equivalent to

	ptr2 = malloc(strlen(ptr1)+1);
	strcpy(ptr2,ptr1);
**So if you want the string which you have copied to be used in another function (as it is created in heap section) you can use `strdup`, else strcpy is enough.**

The functions `strcpy` and `strncpy` are part of the C standard library and **operate on existing memory**.   
By constrast, `strdup` is a Posix function, and it performs **dynamic memory allocation** for you. It returns a pointer to newly allocated memory into which it has copied the string. But you are now responsible for this memory and must eventually free it.

	char *strdup(const char *s);
	char *strndup(const char *s, size_t n);

- `strchr` 查找字符串
- `strcasecmp, strncasecmp` compare two strings ignoring case
- `strsep, settok` extract token from string
- `atoi, atol, atoll, atof, strtol, strtoul, strtoll, strtoull` 字符串转换为数字 

# IO

printf使用的格式字符如下

- d:以十进制输出整型值
- o:以八进制输出整型值
- x:以十六进制输出整型值
- u:以无符号形式输出整型值
- c:输出一个字符
- s:输出一个字符串
- f:输出一个浮点数
- e:以科学表示法输出浮点数
- g:输出%f与%e中占用位数较短的一个

- `fopen` opens the file whose name is the string pointed to by path and associates a stream with it.  
`FILE *fopen(const char *path, const char *mode);`
- `fdopen` function associates a stream with the existing file descriptor fd  
`FILE *fdopen(int fd, const char *mode);`
- `setbuf, setbuffer, setlinebuf, setvbuf` stream buffering operations in `<stdio.h>`

	#include <unistd.h>
    int dup(int oldfd);
	int dup2(int oldfd, int newfd);
dup() uses  the lowest-numbered unused descriptor for the new descriptor.  
dup2() makes newfd be the copy of oldfd, closing newfd first if necessary  
On success, these system calls return the new descriptor.  On error, -1 is returned, and errno is set appropriately.

# apr
install
```
./configure --prefix=/usr/local/apr --enable-debug --enable-profile --enable-threads --enable-other-child
```
安装完之后, 进入到test 目录, 进行测试, 看时候安装成功

[apr hash](http://apr.apache.org/docs/apr/1.4/group__apr__hash.html)

# [GLib](https://developer.gnome.org/glib/unstable/)
GLib提供了多种高级的数据结构,如内存块,双向和单向链表,哈希表,动态字符串以及字符串工具(例如词法分析器,字符串切分等),动态数组,平衡二叉树,n-叉树,键值存储,关系和元组etc.
Glib 还实现了线程相关的函数,多线程编程以及相关的工具,例如原始变量访问,互斥锁,异步队列,安全内存池,消息传递和日志,钩子函数和计时器,同时消息传递还包含了字节序转换和IO channel

GLib由五个库组成:

- GObject – 对象系统,包括类型系统 GType
- Glib
- GModule
- GThread
- GIO

compile options:

	[eric@human glib]$ pkg-config --cflags --libs glib-2.0
	-I/usr/include/glib-2.0 -I/usr/lib64/glib-2.0/include  -lglib-2.0

## [Basic Types](https://developer.gnome.org/glib/stable/glib-Basic-Types.html)
gboolean: A standard boolean type. Variables of this type should only contain the value TRUE or FALSE

	typedef gint   gboolean;
	#define FALSE (0)
	#define TRUE (!FALSE)

gpointer: An untyped pointer. gpointer looks better and is easier to use than void*.
gconstpointer: An untyped pointer to constant data. The data pointed to should not be changed.
	
	typedef void* gpointer;
	typedef const void *gconstpointer;

gchar: Corresponds to the standard C char type.
guchar: Corresponds to the standard C unsigned char type.

	typedef char   gchar;
	typedef unsigned char   guchar;

gint: Corresponds to the standard C int type. Values of this type can range from G_MININT to G_MAXINT.
guint: Corresponds to the standard C unsigned int type. Values of this type can range from 0 to G_MAXUINT.

	typedef int    gint;
	typedef unsigned int    guint;

gint8, guint8, gint16, guint16, gint32, guint32, gint64, guint64

## [Gstring](https://developer.gnome.org/glib/unstable/glib-Strings.html)
	struct GString {
	  gchar  *str;
	  gsize len;
	  gsize allocated_len;
	};

	GString * g_string_new(const gchar *init);
	GString * g_string_new_len(const gchar *init, gssize len);

	GString * g_string_append(GString *string, const gchar *val);
	GString * g_string_append_c(GString *string, gchar c);

	GString * g_string_prepend(GString *string, const gchar *val);
	GString * g_string_prepend_c(GString *string, gchar c);

	GString * g_string_insert(GString *string, gssize pos, const gchar *val);
	GString * g_string_insert_c(GString *string, gssize pos, gchar c);

	GString * g_string_overwrite(GString *string, gsize pos, const gchar *val);
	GString * g_string_overwrite_len(GString *string, gsize pos, const gchar *val, gssize len);

	GString * g_string_erase(GString *string, gssize pos, gssize len);

	GString * g_string_truncate(GString *string, gsize len); Cuts off the end of the GString, leaving the first len bytes.

	gchar * g_string_free(GString *string, gboolean free_segment);
	Frees the memory allocated for the GString. 
	If free_segment is TRUE it also frees the character data. 
	If it is FALSE, the caller gains ownership of the buffer and must free it after use with g_free().

## [Hash Tables](https://developer.gnome.org/glib/stable/glib-Hash-Tables.html)

## [GSList glib single linked list](https://developer.gnome.org/glib/unstable/glib-Singly-Linked-Lists.html)
```
struct GSList {
  gpointer data;
  GSList *next;
};
typedef void* gpointer;
```
[GSList demo](../../demo/c/glib_slist.c)  
glib slist 的循环,最后好还是采用glib本身提供的函数, 而不要自己写, 
因为我们在使用slist的时候,可以会将append和prepend混合着使用, 所以自己写循环函数会比较麻烦.

## Related
Other widget toolkits provide low-level functions and implementations of data structures as well, e.g.:

- STL – C++ alternative to GObject
- Boost – provides some functionality for C++, such as threading primitives, similar to what GLib does for C.[6]
- wxBase – non-GUI functions of the wxWidgets library
- The Apache Portable Runtime and Apple Core Foundation have a large functional overlap with GLib, 
and provide many similar OS-portable threading, network and data structure implementations in C.

# Time
```
#include <time.h>
time_t time(time_t *t);
```
returns the time since the Epoch (00:00:00 UTC, January 1, 1970), measured in seconds  
If t is non-NULL, the return value is also stored in the memory pointed to by t.

```
#include <time.h>
#ifndef _TM_DEFINED
struct tm {
	int tm_sec; /* 秒–取值区间为[0,59] */
	int tm_min; /* 分 - 取值区间为[0,59] */
	int tm_hour; /* 时 - 取值区间为[0,23] */
	int tm_mday; /* 一个月中的日期 - 取值区间为[1,31] */
	int tm_mon; /* 月份(从一月开始,0代表一月) - 取值区间为[0,11] */
	int tm_year; /* 年份,其值从1900开始 */
	int tm_wday; /* 星期–取值区间为[0,6],其中0代表星期天,1代表星期一,以此类推 */
	int tm_yday; /* 从每年的1月1日开始的天数–取值区间为[0,365],其中0代表1月1日,1代表1月2日,以此类推 */
	int tm_isdst; /* 夏令时标识符,实行夏令时的时候,tm_isdst为正.不实行夏令时的进候,tm_isdst为0,不了解情况时,tm_isdst()为负.*/
	long int tm_gmtoff; /*指定了日期变更线东面时区中UTC东部时区正秒数或UTC西部时区的负秒数*/
	const char *tm_zone; /*当前时区的名字(与环境变量TZ有关)*/
};
#define _TM_DEFINED
#endif
```
gmtime()和localtime()可以将time()获得的日历时间time_t结构体转换成tm结构体.  
其中gmtime()函数是将日历时间转化为世界标准时间(即格林尼治时间0,并返回一个tm结构体来保存这个时间,
而localtime()函数是将日历时间转化为本地时间.

[conversion demo](../../demo/c/time.c) 里面还有将时间转化为mysql 的格式的

The struct timeval structure represents an elapsed time. 
```
#include <sys/time.h>
struct timeval{
	// the number of whole seconds of elapsed time since the Epoch((00:00:00 UTC, January 1, 1970)
   	long int tv_sec; 
	// microseconds 微秒, 百万分之一秒
   	long int tv_usec; 
}

int gettimeofday(struct timeval *tv, struct timezone *tz);
```

`time_t` just stores seconds, so  
`time_t time = (time_t)ut_tv.tv_sec;`  
Should work, but since you are just looking for a difference, there is always the magic of subtraction.

# Process
**`_exit`**  
The function `_exit()` terminates the calling process "immediately".  Any open file descriptors belonging to the process are closed; any children
of the process are inherited by process 1, init, and the process parent is sent a SIGCHLD signal.  
The value status is returned to the parent process as the process exit status, and can be collected using one of the wait(2) family of  calls
The  function `_exit()` is like exit(3), **but does not call any functions registered with `atexit(3)` or `on_exit(3)`**.  
Whether it flushes standard I/O buffers and removes temporary files created with tmpfile(3) is implementation-dependent.  
On the other hand, `_exit()` does close open file descriptors, and this may cause an unknown delay, waiting for pending output to finish.  
If the delay is undesired, it may be useful to call functions like `tcflush(3)` before calling `_exit()`.  
Whether any pending I/O is canceled, and which pending I/O may be canceled upon `_exit()`, is implementation-dependent.

# 关键字
## const
const最经常的用法

1. 为了防止传递的函数参数不被修改,在调用函数的形参中用const关键字.

2. `const int *p = &a;` //p是指向**int常量**的指针,因此,不可以通过给指针赋值来改变数组中的数据,例如:

		*p = 10;       /*错误*/
		*(p + 2) = 1;  /*错误*/

3. `int * const p = a;` //p是**指针常量**,我们知道,指针是指向了一个数组的首地址,那么,它的**位置就不可以改变了**.
但是你现在应该和第一个表达式比较了,现在的数组并不是常量数组,所以数组的数据是可以改变的,而指针这时候它是不可以移动的,指向数组第一个数据,

4. `const int * const p = a;` //假如前面两种表达式的本质你理解了,这种表达式你来理解根本没有问题,const现在有两个,
而且一个const的位置是第一种情况的位置,第二个const是第二种情况的位置,所以这表达式的功能就是前两种情况的作用总合.这里不多说

5. **const并不会阻止参数的修改**  
之所以把这作为一点来谈,就是因为有一些朋友可能会以为在函数参数中用了const就一定不可以改变参数,这实际上是错误的理解,因为,它并不阻止参数的修改,
下面举个简单的例子来阐述一下,

		void ChangeStr(const char *String){
			char *Source = (char *)String;
			while (*Source){
				*Source = toupper(*Source);
				Source++;
			}
		}
		int main(void){
			char str[] = "The C programme";
			Change(str);
			printf(str);
			return 0;
		}
上面的程序把字符串中的每个字符都转换成大写字母了.因为`*String`把地址给了`*Source`,而 `*Source`的值的改变编译器并不干涉,可能有的编译器会发出警告之类.
上面的程序只是为了说明const并不会阻止参数的修改,如果象上面程序那样,个人感觉没什么意义,只会让人容易混乱而已.

### demo
	#include <stdio.h>
	#include <string.h>
	#include <stdlib.h>
	
	void print(int *p, int n){
		while(n){
			printf("%d, ", *p);
			p++;
			n--;
		}
	}
	int main(){
		int a =10;
		int b = 5;
		int c[]={2,3};
		print(c,2); // output 2, 3
	
	// 	p points to a constant integer, so error, even the new value is the same as 10.
		const int *p =&a;
		p = &b; // right
	// 	*p = 10; // error
		
	// 	p1 is a constant pointer, so p can not be changed
		int *const p1 =&a;
		*p1 = 1 ; // right, we have not changed p1, but changed *p1
	// 	p1 = &b; // error
	
		int *const p2 = c;
		*p2 = 1;
		*(p2 + 1) = 4; // right, we did not change p2
		print(c,2); // output 1, 4
	// 	p2 ++; // error
	
	// 	p3 is a constant pointer, and it points to a constant integer
		const int * const p3 = &a;
	// 	p3 = &b; // error
	// 	*p3 = 1; // error
		
	// 	although a is a constant integer, we can still change its value
		p = &a;
		int *p4 = (int *)p;
		*p4 = 1;
		printf("%d, %d\n", a, *p); // output: 1, 1
		printf("over\n");
		return 0;
	}

## const faq
**C (Linux): warning: assignment discards qualifiers from pointer target type**  
As the compiler states, you are discarding a qualifier (const in this case) from a pointer upon assignment.  
This happens when you assign a `char *` the value of a `const char *`.

[Why do most C developers use define instead of const?](http://stackoverflow.com/questions/4024318/why-do-most-c-developers-use-define-instead-of-const)  
There is a very solid reason for this: **const in C does not mean something is constant. It just means a variable is read-only.**  
In places where the compiler requires a true constant (such as for array sizes for non-VLA arrays), using a const variable, such as fieldWidth is just not possible.

## extern
extern可以置于变量或者函数前,以标示变量或者函数的定义在别的文件中,提示编译器遇到此变量和函数时在其他模块中寻找其定义

当它与"C"一起连用时,如: `extern "C" void fun(int a, int b)`;  
则告诉编译器在编译fun这个函数名时按着C的规则去翻译相应的函数名而不是C++的,C++的规则在翻译这个函数名时会把fun这个名字变得面目全非.

在一个源文件里定义了一个数组:`char a[6];`
声明为`extern char a[]`, 而不是`extern char *a`, 两者的类型不一样

例子分析如下,如果`a[] = "abcd"`, 则外部变量`a=0x61626364` (abcd的ASCII码值), 显然a指向的空间(0x61626364)没有意义,易出现非法内存访问.
这提示我们,在使用extern时候要严格对应声明时的格式
extern用在变量声明中常常有这样一个作用,你在`*.c`文件中声明了一个全局的变量,这个全局的变量如果要被引用,就放在`*.h`中并用extern来声明

常常见extern放在函数的前面成为函数声明的一部分,那么,C语言的关键字extern在函数的声明中起什么作用?  
如果函数的声明中带有关键字extern,仅仅是暗示这个函数可能在别的源文件里定义,没有其它作用.即下述两个函数声明没有明显的区别: `extern int f();` 和`int f();`

C++中const修饰的全局常量据有跟static相同的特性, 即它们只能作用于本编译模块中, 但是const可以与extern连用来声明该常量可以作用于其他编译模块中, 如
`extern const char str[];`  
然后在原文件中别忘了定义: `const char str[] = "123456";`  
所以当const单独使用时它就与static相同,而当与extern一起合作的时候,它的特性就跟extern的一样了!

## static
static 表示静态的变量,分配内存的时候, 存储在静态区,不存储在栈上面.

static 作用范围是内部连接的关系, 和extern有点相反, 只允许对象本身用它. 

具体差别首先,static与extern是一对"水火不容"的家伙,也就是说extern和static不能同时修饰一个变量.  
其次,static修饰的全局变量声明与定义同时进行,也就是说当你在头文件中使用static声明了全局变量后,它也同时被定义了  
最后,static修饰全局变量的作用域只能是本身的编译单元,也就是说它的"全局"只对本编译单元有效,其他编译单元则看不到它,如:

    (1) test1.h:
    #ifndef TEST1H
    #define TEST1H
    static char str[] = "123456"; 
    void fun1();
    #endif
    (2) test1.cpp:
    #include "test1.h"
    void fun1()  {   cout <<  str<<  endl;}
     (3) test2.cpp
    #include "test1.h"
    void fun2()  {   cout < str<<  endl;}
以上两个编译单元可以连接成功, 当你打开test1.obj时,你可以在它里面找到字符串"123456",同时你也可以在test2.obj中找到它们,
它们之所以可以连接成功而没有报重复定义的错误是因为虽然它们有相同的内容,但是存储的物理地址并不一样,就像是两个不同变量赋了相同的值一样,
而这两个变量分别作用于它们各自的编译单元.   
也许你比较较真,自己偷偷的跟踪调试上面的代码,结果你发现两个编译单元(test1,test2)的str的内存地址相同,于是你下结论static修饰的变量也可以作用于其他模块,  
但是我要告诉你m那是你的编译器在欺骗你, 大多数编译器都对代码都有优化功能m以达到生成的目标程序更节省内存, 执行效率更高, 当编译器在连接各个编译单元的时候,
它会把相同内容的内存只拷贝一份,比如上面的"123456", 位于两个编译单元中的变量都是同样的内容,那么在连接的时候它在内存中就只会存在一份了,  
如果你把上面的代码改成下面的样子,你马上就可以拆穿编译器的谎言:

    (1) test1.cpp:
    #include "test1.h"
    void fun1(){
        str[0] = ''a'';
        cout < str<<  endl;}
    (2) test2.cpp
    #include "test1.h"
    void fun2()  {  cout <<  str <<endl;}
    (3) void main()     {
        fun1(); // a23456
        fun2(); // 123456
    }
这个时候你在跟踪代码时,就会发现两个编译单元中的str地址并不相同,因为你在一处修改了它,
所以编译器被强行的恢复内存的原貌,在内存中存在了两份拷贝给两个模块中的变量使用.

正是因为static有以上的特性,所以**一般定义static全局变量时,都把它放在原文件中**而不是头文件,这样就不会给其他模块造成不必要的信息污染

## define  
Function macro definitions accept two special operators (`#` and `##`) in the replacement sequence:
If the operator # is used before a parameter is used in the replacement sequence, that parameter is replaced by a **string literal** (as if it were enclosed between double quotes)

    #define str(x) #x
    cout << str(test);
	This would be translated into:
	cout << "test";

The operator `##` concatenates two arguments **leaving no blank spaces** between them:

    #define glue(a,b) a ## b
    glue(c,out) << "test";
    This would also be translated into:
    cout << "test";

"\","#","#@"和"##"

在用`#define` 定义时  
斜杠("\")是用来续行的  
"#"用来把参数转换成字符串,是给参数加上双引号  
"##"则用来连接前后两个参数,把它们变成一个  
"#@"是给参数加上单引号.

下面的例子会使您很容易理解.
	
	#define Conn(x,y) x##y
	#define ToChar(a) #@a 
	#define ToString(x) #x
	 int n = Conn(123,456);   结果就是n=123456;
	 char* str = Conn("asdf", "adf")结果就是 str = "asdfadf";
	 char a = ToChar(1);结果就是a='1';
	 char* str = ToString(123132);就成了str="123132";

`realloc`: 重新分配内存, 但是原来的内容保留下来

# Linux
## [Example of Parsing Arguments with getopt](http://www.gnu.org/software/libc/manual/html_node/Getopt.html)
Normally, `getopt` is called in a loop. When `getopt` returns `-1`, indicating no more options are present, the loop terminates.  
A **switch statement** is used to dispatch on the return value from getopt. In typical use, each case just sets a variable that is used later in the program.  
A second loop is used to process the remaining non-option arguments.

optstring中后面的**冒号表示需要接变量**,并存于optarg中.如果接二个冒号,则表示变量可有可无.

扫描模式:

1. 如果第一个字符为加号: 停止于发现第一个非选项
2. 如果第一个字符为减号:全部扫描,此为默认模式.

碰到'--',则忽略扫描模式,必须停止扫描.

`getopt`返回值:  

1. 成功,返回option,  
1. 结束,返回-1,  
1. 发现不认识的option,返回?号  
1. 碰到option后面缺参数,则返回?号. 或返回:号(optstring中第一个字符(如果有加减号,在加减号后面)为冒号时返回,即为:或+:或-:).

`optarg` 存数据  
`optopt` 存储出错的option(如缺参数),或者不认识的option  
`optind`, 表示下一次运行getopt时将读取数组第optind个

[parse options demo](../../demo/c/parse_options.c)

**chroot**  
[Is chroot a security feature?](https://securityblog.redhat.com/2013/03/27/is-chroot-a-security-feature/)  
chroot: an operation that changes the apparent root directory for the current running process and its children.  
This call changes an ingredient in the pathname resolution process and does nothing else.  
This call does not close open file descriptors, and such file descriptors may allow access to files outside the chroot tree.  
A program that is run in such a modified environment cannot name (and therefore normally not access) files outside the designated directory tree.  
The modified environment is called a "chroot jail".  
This can be useful for: Testing and development and Privilege separation 

This call does not change the current working directory, so that after the call '.' can be outside the tree rooted at '/'.  In particular, the superuser can escape from a "chroot jail" by doing:  
`mkdir foo; chroot foo; cd ..`  
So chrooted programs should relinquish root privileges as soon as practical after chrooting

# Linux 下的动态与静态库
使用locate命令找到该库的被默认安装的路径,使用方法是先用updatedb更新一下locate命令的索引库,然后"locate libnet.so"查找就可以了

Linux 中的应用程序以以下两种方式之一链接到外部函数:

- 要么在构建时与静态库( lib*.a)静态地链接,并且将库代码包含在该应用程序的可执行文件里
- 要么在运行时与共享库( lib*.so(Shared Object))动态地链接.通过动态链接装入器,将动态库映射进应用程序的可执行内存中.
在启动应用程序之前,动态链接装入器将所需的共享目标库映射到应用程序的内存,或者使用系统共享的目标并为应用程序解析所需的外部引用.
然后应用程序就可以运行了


`LIBRARY_PATH` is used by gcc before compilation to search for directories containing libraries that need to be linked to your program.

`LD_LIBRARY_PATH` is used by your program to search for directories containing the libraries after it has been successfully compiled and linked.

EDIT: As pointed below, your libraries can be static or shared. 
If it is static then the code is copied over into your program and you do not need to search for the library after your program is compiled and linked. 
If your library is shared then it needs to be dynamically linked to your program and that is when `LD_LIBRARY_PATH` comes into play.

需要打印函数调用堆栈时, 都可以使用 `-rdynamic` 来进行, 经过测试都是可以的

## 动态链接库
[添加搜索路径方法步骤](http://blog.sciencenet.cn/blog-402211-745740.html)

可以查看程序**执行时**调用动态库的过程:

	# ldd hello
运行结果:

	[pin@localhost 20090505]$ ldd hello
	linux-gate.so.1 => (0x00110000)
	libmyhello.so => /usr/lib/libmyhello.so (0x00111000)
	libc.so.6 => /lib/libc.so.6 (0x00859000)
	/lib/ld-linux.so.2 (0x0083a000)

`ld.so.cache` 里面的路径是程序在运行时搜索的路径

修改`/etc/ld.so.conf`,添加路径.  
在CentOS 6.3下我看到这个文件实际上是包含了`/etc/ld.so.conf.d/`这个目录下的所有`.conf`文件,因此我们可以在这个路径下面创建一个新的文件,其中写上诸如`/usr/local/lib` 等路径,保存退出.  
切记一定要主动执行命令:`ldconfig`,它会更新记录了系统中有哪些so文件的缓存文件(`/etc /ld.so.cache`)

ldconfig几个需要注意的地方

1. 往/lib和/usr/lib里面加东西,是不用修改/etc/ld.so.conf的,但是完了之后要调一下ldconfig,不然这个library会找不到
2. `/usr/local/lib` 和 `/usr/local/lib64` 居然不在标准路径之列
3. 如果想在这两个目录以外放lib,但是又不想在/etc/ld.so.conf中加东西(或者是没有权限加东西).那也可以,就是export一个全局变 量LD_LIBRARY_PATH,然后运行程序的时候就会去这个目录中找library.一般来讲这只是一种临时的解决方案,在没有权限或临时需要的时 候使用.
4. ldconfig做的这些东西都与运行程序时有关,跟编译时一点关系都没有.编译的时候还是该加-L就得加,不要混淆了.
5. 总之,就是不管做了什么关于library的变动后,最好都ldconfig一下,不然会出现一些意想不到的结果.不会花太多的时间,但是会省很多的事.

### [Demo](http://www.cppblog.com/deane/archive/2014/05/23/165216.html)
hello.h

	#ifndef HELLO_H_
	#define HELLO_H_
	void hello(const char* name);
	#endif

hello.c

	#include <stdio.h>
	void hello(const char* name){
		printf("Hello %s!\n", name);
	}

main.c

	#include "hello.h"
	int main(){
		hello("everyone");
		return 0;
	}

**创建动态链接库**  

	gcc -c -fPIC hello.c  // 这个里面的 -fPIC 也是必须的, 否则进行下面的一个步骤会出错
	gcc -shared -fPIC -o libmyhello.so hello.o
"PIC"命令行标记告诉GCC产生的代码不要包含对函数和变量具体内存位置的引用,这是因为现在还无法知道使用该消息代码的应用程序会将它连接到哪一段内存地址空间.  
表示编译为位置独立的代码,不用此选项的话编译后的代码是位置相关的所以动态载入时是通过代码拷贝的方式来满足不同进程的需要,而不能达到真正代码段共享的目的

**使用**  
	
	gcc -c main.c
	gcc -o hello main.o -L. -lmyhello
	./hello

但是出错

	./hello: error while loading shared libraries: libmyhello.so: cannot open shared object file: No such file or directory
原因: libmyhello.so 不再系统的动态链接库搜索目录中.  
需要把他拷贝到/usr/lib或者其他搜索目录中, 之后运行 `ldconfig`.  
对于这个测试, 可以将这个环境变量设置成当前目录:

	export LD_LIBRARY_PATH=$(pwd)
	./hello
我们可以查看hello 依赖了哪些动态链接库:

	[eric@iis computer]$ ldd hello
    linux-vdso.so.1 =>  (0x00007fffdad30000)
    libmyhello.so => /home/eric/git/note/latex/computer/libmyhello.so (0x00007f497a5e6000)
    libc.so.6 => /lib64/libc.so.6 (0x000000382ec00000)
    /lib64/ld-linux-x86-64.so.2 (0x000000382e400000)

## 静态链接库
静态函数库实际上就是简单的一个普通的目标文件的集合,一般来说习惯用".a"作为文件的后缀.
可以用ar这个程序来产生静态函数库文件.Ar 是archiver的缩写.  
静态函数库现在已经不在像以前用得那么多了,主要是共享函数库与之相比较有很多的优势的原因.慢慢地,大家都喜欢使用共享函数库了.
不过,在一些场所静态函数库仍然在使用,一来是保持一些与以前某些程序的兼容,二来它描述起来也比较简单.

### Demo
假设有下面几个文件:String.h, Strlen.c, Strlnen.c

String.h

	#ifndef _STRING_H_
	#define _STRING_H_
	int Strlen(char *pStr);
	int Strnlen(char *pStr, unsigned long ulMaxLen);
	#endif

Strlen.c

	#include<stdio.h>
	#include<assert.h>
	int Strlen(char *pStr){
	    unsigned long ulLength;
	    assert(NULL != pStr);
	    ulLength = 0;
	    while(*pStr++){
	        ulLength++;
	    }
	    return ulLength;
	}

Strlnen.c

	#include<stdio.h>
	#include<assert.h>
	// 函数StrNlen的实现,获取给定字符串的长度,如果输入字符串的长度大于指定的最大长度,则返回最大长度,否者返回字符串的实际长度
	int Strnlen(char *pStr, unsigned long ulMaxLen){
	    unsigned long ulLength;
	    assert(NULL != pStr);
	    if(ulMaxLen <= 0){
	        printf("Wrong Max Length!\n");
	        return -1;
	    }
	    ulLength = 0;
	    while(\*pStr++ && ulLength < ulMaxLen){
	        ulLength++;
	    }
	    return ulLength;
	}

**生成静态链接库**:
利用GCC生成对应目标文件:

	#>gcc –c Strlen.c Strnlen.c
用ar创建一个名字为libstr.a的库文件,并把Strlen.o 和Strnlen.o的内容插入到对应的库文件中.

	#>ar –rc libstr.a Strlen.o Strnlen.o
命令执行成功以后,对应的静态库libstr.a已经成功生成.

rc option of ar

- r Insert the files member... into archive (with replacement). This operation differs from q in that any previously existing members are
deleted if their names match those being added
- c Create the archive


**静态库的使用**:
假设有下面的文件要使用对应的的静态库:
编译生成对应的目标文件:

	#>gcc -c -I/home/hcj/dir_of_String.h main.c 
其中`-I/home/hcj/xxxxxxxx`指定对应的头文件的路径.
生成可执行文件:

	#>gcc -o main main.o libstr.a 
	#>./main
上面的命令要求libstr.a 在当前目录中.

**如果libstr不再当前目录**  
gcc会在静态库名前加上前缀lib,然后追加扩展名.a得到的静态库文件名来查找静态库文件,
因此,我们在写需要连接的库时,只写名字就可以,如libstr.a的库,只写:-lstr

	#>gcc -o main -Ldir_of_libstr.a main.o -lstr
	#>./main

main.c

	// Stirng static library test
	#include "String.h"   //静态库对应函数的头文件, 当然声明也可以写在main.c文件中, 就不需要引用String.h
	int main(int argc, char* argv[]){
	    char str[] = {"hello world"};
	    unsigned long ulLength = 0;
	    printf("The string is : %s\n", str);
	    ulLength = Strlen(str);
	    printf("The string length is : %d(use Strlen)\n", ulLength);
	    ulLength = StrNlen(str, 10);
	    printf("The string length is : %d(use StrNlen)\n", ulLength);
    	return 0;
	}

## 顺序
我们回过头看看,发现使用静态库和使用动态库编译成目标程序使用的gcc命令完全一样,
那当静态库和动态库同名时,gcc命令会使用哪个库文件呢?

	# ls
	hello.c hello.h hello.o libmyhello.a libmyhello.so main.c
通过上述最后一条ls命令,可以发现静态库文件libmyhello.a和动态库文件libmyhello.so都已经生成,并都在当前目录中
	
	# gcc -o hello main.c -L. -lmyhello
	# ./hello
	./hello: error while loading shared libraries: libmyhello.so: cannot open shared object file: No such file or directory
从程序hello运行的结果中很容易知道,当**静态库和动态库同名时, gcc命令将优先使用动态库**.

# 高级
## volatile 
volatile 影响编译器编译的结果, volatile 变量是随时可能发生变化的,每次使用时都需要去内存里重新读取它的值,与volatile变量有关的运算,不要进行编译优化,以免出错.
(VC++ 在产生release版可执行码时会进行编译优化,加volatile关键字的变量有关的运算,将不进行编译优化).
```
volatile int i=10;
int j = i;
...
int k = i;
```
volatile 告诉编译器i是随时可能发生变化的,每次使用它的时候必须从i的地址中读取,因而编译器生成的可执行码会重新从i的地址读取数据放在k中.  
而优化做法是,由于编译器发现两次从i读数据的代码之间的代码没有对i进行过操作,它会自动把上次读的数据放在k中.而不是重新从i里面读.
这样以来,如果i是一个寄存器变量或者表示一个端口数据就容易出错,所以说volatile可以保证对特殊地址的稳定访问,不会出错.

建议使用volatile变量的场所

1. 并行设备的硬件寄存器
2. 一个中断服务子程序中会访问到的非自动变量(全局变量)
3. 多线程应用中被几个任务共享的变量
 
## sig_atomic_t
sig_atomic_t: 当把变量声明为该类型是,则会保证该变量在使用或赋值时, 无论是在32位还是64位的机器上都能保证操作是原子的, 它会根据机器的类型自动适应.

通常情况下,int类型的变量通常是原子访问的,也可以认为 sig_atomic_t就是int类型的数据,因为对这些变量要求一条指令完成,所以sig_atomic_t不可能是结构体,只会是数字类型.
在linux里这样定义:
```
typedef int __sig_atomic_t;
```
另外gnu c的文档也说比int短的类型通常也是具有原子性的,例如short类型.
同时,指针(地址)类型也一定是原子性的. 该类型在所有gnu c库支持的系统和支持posix的系统中都有定义.

# GCC
## Useful GCC flags for C
Those marked * sometimes give too many spurious warnings, so I use them on as-needed basis.

- Wextra, -Wall: essential.
- Wfloat-equal: useful because usually testing floating-point numbers for equality is bad.
- Wundef: warn if an uninitialized identifier is evaluated in an #if directive.
- Wshadow: warn whenever a local variable shadows another local variable, parameter or global variable or whenever a built-in function is shadowed.
- Wpointer-arith: warn if anything depends upon the size of a function or of void.
- Wcast-align: warn whenever a pointer is cast such that the required alignment of the target is increased. For example, warn if a char * is cast to an int * on machines where integers can only be accessed at two- or four-byte boundaries.
- Wstrict-prototypes: warn if a function is declared or defined without specifying the argument types.
- Wstrict-overflow=5: warns about cases where the compiler optimizes based on the assumption that signed overflow does not occur. (The value 5 may be too strict, see the manual page.)
- Wwrite-strings: give string constants the type const char[length] so that copying the address of one into a non-const char * pointer will get a warning.
- Waggregate-return: warn if any functions that return structures or unions are defined or called.
- Wcast-qual: warn whenever a pointer is cast to remove a type qualifier from the target type*.
- Wswitch-default: warn whenever a switch statement does not have a default case*.
- Wswitch-enum: warn whenever a switch statement has an index of enumerated type and lacks a case for one or more of the named codes of that enumeration*.
- Wconversion: warn for implicit conversions that may alter a value*.
- Wunreachable-code: warn if the compiler detects that code will never be executed*.

# User related
- `getpwnam()` function returns a pointer to a structure containing the broken-out fields of the record in the **password database** (e.g., the local password file /etc/passwd, NIS, and LDAP) that matches the username name.
- `getpwuid()` function returns a pointer to a structure containing the broken-out fields of the record in the password database that matches the user ID uid.
- `chroot` change root directory

# Object orientation of C
[Object-Orientation in C](http://stackoverflow.com/questions/415452/object-orientation-in-c)

I would advise against preprocessor (ab)use to try and make C syntax more like that of another more object-oriented language. At the most basic level, you just use plain structs as objects and pass them around by pointers:

	struct monkey{
	    float age;
	    bool is_male;
	    int happiness;
	};

	void monkey_dance(struct monkey *monkey){
	    /* do a little dance */
	}
To get things like inheritance and polymorphism, you have to work a little harder. You can do manual inheritance by having the first member of a structure be an instance of the superclass, and then you can cast around pointers to base and derived classes freely:

	struct base{
	    /* base class members */
	};

	struct derived{
	    struct base super;
	    /* derived class members */
	};

struct derived d;
struct base *base_ptr = (struct base *)&d;  // upcast
struct derived derived_ptr = (struct derived *)base_ptr;  // downcast
To get polymorphism (i.e. virtual functions), you use function pointers, and optionally function pointer tables, also known as virtual tables or vtables:

	struct base;
	struct base_vtable{
	    void (*dance)(struct base *);
	    void (*jump)(struct base *, int how_high);
	};

	struct base{
	    struct base_vtable *vtable;
	    /* base members */
	};

	void base_dance(struct base *b){
	    b->vtable->dance(b);
	}

	void base_jump(struct base *b, int how_high){
	    b->vtable->jump(b, how_high);
	}

	struct derived1{
	    struct base super;
	    /* derived1 members */
	};

	void derived1_dance(struct derived1 *d){
	    /* implementation of derived1's dance function */
	}

	void derived1_jump(struct derived1 *d, int how_high){
	    /* implementation of derived 1's jump function */
	}

	/* global vtable for derived1 */
	struct base_vtable derived1_vtable ={
	    &derived1_dance, /* you might get a warning here about incompatible pointer types */
	    &derived1_jump   /* you can ignore it, or perform a cast to get rid of it */
	};

	void derived1_init(struct derived1 *d){
	    d->super.vtable = &derived1_vtable;
	    /* init base members d->super.foo */
	    /* init derived1 members d->foo */
	}

	struct derived2{
	    struct base super;
	    /* derived2 members */
	};

	void derived2_dance(struct derived2 *d){
	    /* implementation of derived2's dance function */
	}

	void derived2_jump(struct derived2 *d, int how_high){
	    /* implementation of derived2's jump function */
	}

	struct base_vtable derived2_vtable ={
	   &derived2_dance,
	   &derived2_jump
	};

	void derived2_init(struct derived2 *d){
	    d->super.vtable = &derived2_vtable;
	    /* init base members d->super.foo */
	    /* init derived1 members d->foo */
	}

	int main(void){
	    /* OK!  We are done with our declarations, now we can finally do some
	       polymorphism in C */
	    struct derived1 d1;
	    derived1_init(&d1);
	
	    struct derived2 d2;
	    derived2_init(&d2);
	
	    struct base *b1_ptr = (struct base *)&d1;
	    struct base *b2_ptr = (struct base *)&d2;
	
	    base_dance(b1_ptr);  /* calls derived1_dance */
	    base_dance(b2_ptr);  /* calls derived2_dance */
	
	    base_jump(b1_ptr, 42);  /* calls derived1_jump */
	    base_jump(b2_ptr, 42);  /* calls derived2_jump */
	
	    return 0;
	}

And that is how you do polymorphism in C. It ai not pretty, but it does the job. 
There are some sticky issues involving pointer casts between base and derived classes, which are safe as long as 
**the base class is the first member of the derived class**. 
Multiple inheritance is much harder - in that case, in order to case between base classes other than the first, 
you need to manually adjust your pointers based on the proper offsets, which is really tricky and error-prone.

Another (tricky) thing you can do is change the dynamic type of an object at runtime! You just reassign it a new vtable pointer. 
You can even selectively change some of the virtual functions while keeping others, creating new hybrid types. 
Just be careful to create a new vtable instead of modifying the global vtable, otherwise you will accidentally affect all objects of a given type.
