聚合
聚合是关联关系的一种特例,他体现的是整体与部分,拥有的关系,即has-a的关系.
此时整体与部分之间是可分离的,他们可以具有各自的生命周期,部分可以属于多个整体对象,也可以为多个整体对象共享,
比如计算机与CPU,公司与员工的关系等,表现在代码层面,和关联关系是一致的,只能从语义级别来区分,

组合
组合也是关联关系的一种特例,他体现的是一种contains-a的关系,这种关系比聚合更强,也称为强聚合,
他同样体现整体与部分间的关系,但此时整体与部分是不可分的,整体的生命周期结束也就意味着部分的生命周期结束,
比如你和你的大脑,
表现在代码层面,和关联关系是一致的,只能从语义级别来区分,

# 依赖和关联的区别
类与类之间由弱到强关系是: 
`没关系 < 依赖 < 关联 < 聚合 < 组合`

类和类之间八竿子打不着那就是没关系,这个没啥歧义.

**依赖(dependency)**  
可以简单的理解,就是一个类A使用到了另一个类B,而这种使用关系是具有偶然性的,,临时性的,非常弱的,但是B类的变化会影响到A,
比如某人要过河,需要借用一条船,此时人与船之间的关系就是依赖,
表现在代码层面,为**类B作为参数被类A在某个method方法中使用**.
用带虚线的箭头.

![依赖图例](http://images.cnitblog.com/blog/353089/201303/17003751-6e653965b858434d80664a2db6cbb64f.jpg)

**关联(association)**  
他体现的是两个类,或者类与接口之间语义级别的一种强依赖关系,
比如我和我的朋友,这种关系比依赖更强,
不存在依赖关系的偶然性,关系也不是临时性的,一般是长期性的,而且双方的关系一般是平等的,关联可以是单向,双向的,
表现在代码层面,为**被关联类B以类属性的形式出现在关联类A中**,也可能是关联类A引用了一个类型为被关联类B的全局变量,

![关联图例](http://images.cnitblog.com/blog/353089/201303/17004239-a59e7cac3e614d9da9129612b5cf91bc.jpg)

**依赖和关联区别**  
我用锤子修了一下桌子,我和锤子之间就是一种依赖.
我和我的同事就是一种关联.
依赖是一种弱关联,只要一个类用到另一个类,但是和另一个类的关系不是太明显的时候(可以说是"uses"了那个类),就可以把这种关系看成是依赖,依赖也可说是一种偶然的关系,而不是必然的关系.
关联是类之间的一种关系,例如老师教学生,老公和老婆这种关系是非常明显的.依赖是比较陌生,关联是我们已经认识熟悉了.

类图中的多重性(Multiplicity)
对于关联另一端的类的每个对象, 本端的类可能有多少个对象出现.
例如: 一个顾客(customer)可以拥有一个到多个的账户(account),但是一个账户只能由一个顾客所拥有.

![多重性实例](http://images.51cto.com/files/uploadimg/20081212/205524757.jpg)

Customer 有一个1, 表示, 对于关联另一端的每个对象, 这里也就是对于每一个Account, 只有一个Customer.
而Account 有一个1...*, 表示对于关联另一端的每个对象, 也即对于每个Customer, 可以有1..* 个Account.

# [UML 9种图](http://www.tuicool.com/articles/uaQzm2)
## 静态图与动态图
类图,对象图,用例图,部署图,构件图,为静态图 

状态图,顺序图,活动图,协作图为动态图 

## 定义
1. 用例图(user-case diagram)  
定义了系统的功能需求,它是从系统的外部看系统功能,并不描述系统内部对功能的具体实现.
2. 类图(class diagram)  
对静态结构的描述,用来定义系统中类和类之间的关系.
3. 对象图(object diagrams)  
表示类的对象实例.通常用来示例一个复杂的类图,通过对象图反映真正的实例是什么,它们之间可能具有什么样的关系,帮助对类的理解.
4. 状态图(statechart diagrams)  
类所描述事物的补充说明,类所有对象可能具有的状态,以及引起状态变化的事物.
5. 序列图(Sequence Diagrams)  
又叫顺序图,强调消息时间顺序的交互图,描述类以及类相互交互以完成期望行为的消息.
6. 协作图(collaboration diagrams)  
协作图只对相互间有交互作用的对象和这些对象间的关系建模,而忽略了其他对象和关联.
7. 活动图(activity diagram)  
反映一个连续的活动流,用于描述某个操作执行时的活动状况.
8. 构件图(component diagrams)  
主要用于描述各种软件构件之间的依赖关系.
9. 部署图(deployment diagrams)  
是用来显示系统中软件和硬件的物理架构.从部署图中,可以了解到软件和硬件组件之间的物理关系以及处理节点的组件分布情况.

## 九种图的应用阶段
- 需求阶段:用例图描述需求
- 分析阶段:类图描述静态结构
- 设计阶段:类图和包图对接口的应用
- 实现阶段:构件图,部署图

## PK赛
### 时序图 and 协作图
相同点:

1. 时序图和协作图都属于交互图 ,他们表示对象间的交互关系, 描述了一个交互,由一组对象和他们之间的关系组成,并且还包括在对象之间传递的消息,
2. 时序图和协作图是等价的
3. 两者都来自UML元模型的相同信息,因此他们的语义是等价的,他们可以从一种形式的图转换成另一种形式的图,而不丢失任何信息.

不同点:

- 协作图强调的是空间, 但时间顺序必须从序列号获得.
- 时序图强调的是时间, 但是没有明确的表达对象间的关系 .

### 状态图 and 活动图
相同点: 都属于行为图,都是描述对象的动态行为.

不同点:

1. 描述对象不同:
	- 状态图:描述对象状态及状态之间的转移,它主要表现该对象的状态.
	- 活动图:描述从活动到活动的控制流,它主要表现的是系统的动作, 跨过不同的对象从活动到活动的控制流
2. 使用场合不同:
	- 状态图:描述对象在其生命期中的行为状态变化.
	- 活动图:描述过程的流程变化.

### 对象图 and 类图
	- 相同点:对象图是类图的实例,几乎使用与类图完全相同的标识.
	- 不同点:对象图显示类的多个对象实例,而不是实例的类.由于对象存在生命周期,因此对象图只能在系统某一个时间段存在.

### 活动图 and 用例图
活动图是对用例图的一种细化.

### 状态图 and 类图
状态图是对类图的一种补充,帮助开发者完善某一类.

# 体系结构建模
## 组件
组件图(Component Diagram)描述了软件的各种组件和它们之间的依赖关系.

组件图中通常包含3种元素: 组件(Component),接口(Interface)和依赖(Dependency).
每个组件实现一些接口,并使用另一些接口.

组件是定义了良好接口的物理实现单元,是系统中可替换的物理部件.
一般情况下,组件表示将类,接口等逻辑元素打包而形成的物理模块.
一个组件包含它所实现的一个或多个逻辑类的相关信息,创建了一个从逻辑视图到组件视图的映射.

### 组件和包的区别
在工作中为了更好的对软件架构进行设计,对项目进行了从上到下的组件划分工作,同时对UML中包和组件的概念产生了模糊的理解,经过简单的搜索,整理其区别如下:

1. 可以用包来组织类(class),也可以用组件来组织类.
1. 可以用包来组织用例(use case),不可以用组件来组织用例.
1. 部置图(deployment diagram)中,节点(node)中可以放组件,但不可以放包.
1. 一个包不能画成两个实例(instance).一个组件可以画成两个实例.或者说,包只是类型(type),组件可以是实例也可以是类型.
1. 包可以作为开发视图(development view),用于管理.组件可作为物理视图(physical view),用于部署.但反之不然.

