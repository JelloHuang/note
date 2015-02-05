# R中绘图系统的介绍
ggplot2:elegant graphics for data analysis 一书是ggplot软件包2的系统介绍,包括其语法结构的详细介绍以及绘图实例,也包括了plyr,reshape等包的强大的数据处理能力,是数据分析和统计图形的优秀著作.

R的基础图形系统基本上是一个"纸笔模型",即:一块画布摆在面前,你可以在这里画几个点,在那里画几条线,指哪儿画哪儿.

后来lattice包的出现稍微改善了这种情况,你可以说,我要画散点图或直方图,并且按照某个分类变量给图中的元素上色,
此时数据才在画图中扮演了一定的中心角色,我们不用去想具体这个点要用什么颜色(颜色会根据变量自动生成).
然而,lattice继承了R语言的一个糟糕特征,就是参数设置铺天盖地,足以让人窒息,光是一份xyplot()函数的帮助文档,恐怕就够我们消磨一天时间了,
更重要的是,lattice仍然面向特定的统计图形,像基础图形系统一样,有直方图,箱线图,条形图等等,它没有一套可以让数据分析者说话的语法.

那么数据分析者是怎样说话的呢?他们从来不会说这条线用#FE09BE颜色,那个点用三角形状,他们只会说,把图中的线用数据中的职业类型变量上色,或图中点的形状对应性别变量.
有时候他们画了一幅散点图,但马上他们发现这幅图太拥挤,最好是能具体看一下里面不同收入阶层的特征,所以他们会说,把这幅图拆成七幅小图,每幅图对应一个收入阶层.
然后发现散点图的趋势不明显,最好加上回归直线,看看回归模型反映的趋势是什么,或者发现图中离群点太多,最好做一下对数变换,减少大数值对图形的主导性.

从始至终,数据分析者都在数据层面上思考问题,而不是拿着水彩笔和调色板在那里一笔一划作图,而计算机程序员则倾向于画点画线.
Leland Wilkinson的著作在理论上改善了这种状况,他提出了一套图形语法,让我们在考虑如何构建一幅图形的时候不再陷在具体的图形元素里面,而是把图形拆分为一些互相独立并且可以自由组合的成分.
这套语法提出来之后他自己也做了一套软件,但显然这套软件没有被广泛采用,幸运的是,Hadley Wickham在R语言中把这套想法巧妙地实现了.

为了说明这种语法的想法,我们考虑图形中的一个成分:坐标系.常见的坐标系有两种:笛卡尔坐标系和极坐标系.
在语法中,它们属于一个成分,可自由拆卸替换.笛卡尔坐标系下的条形图实际上可以对应极坐标系下的饼图,因为条形图的高可以对应饼图的角度,本质上没什么区别.
因此在ggplot2中,从一幅条形图过渡到饼图,只需要加极少量的代码,把坐标系换一下就可以了.
如果我们用纸笔模型,则可以想象,这完全是不同的两幅图,一幅图里面要画的是矩形,另一幅图要画扇形.

## ggplot2 绘图概述
一张统计图形就是从数据到几何对象(geometric object, 缩写为geom, 包括点, 线, 条形等)的图形属性(aesthetic attributes, 缩写为aes, 
包括颜色, 形状, 大小等)的一个映射.
此外, 图形中还可能包含数据的统计变换(statistical transformation, 缩写为stats),
最后绘制在某个特性的坐标系(coordinate system, 缩写为coord)中, 
而分面(facet, 指将绘图窗口划分为若干个子窗口)则可以用来生成数据不同子集的图形.
总而言之, 一个统计图形就是由上述这些独立的图形部件所组成的.

- 最基础的部分是想要可视化的**数据(data)**, 以及一系列将数据中的变量对应到图形属性的**映射(mapping)**
- **几何对象(geom)**: 代表你在图中实际看到的图形元素, 如点, 线, 多边形等
- **统计变换(stats)**: 是对数据进行的某种汇总. 例如将数据分组计数以创建直方图, 或将一个二维的关系用线性模型来进行解释
- **标度(scale)**: 将数据的取值映射到图形控件, 例如用颜色, 大小或形状来表示不同的取值. 展现标度的常见做法是绘制图例和坐标轴---他们实际上是图形到数据的一个映射, 使读者可以从图中读取原始的数据.
- **坐标系(coord)**: 描述了数据是如何映射到图形所在的平面的, 它同时提供了看图所需的坐标轴和网格线. 通常使用笛卡尔坐标系, 但也用极坐标系和地图投影
- **分面(facet)**: 描述了如何将数据分解为各个子集, 记忆如何对子集作图并联合进行展示. 分面也叫做条件作图或网格作图

绘图有两种方式:

1. 一种是一步到位, 即利用qplot
1. 另一种是逐层叠加式, 即利用ggplot()函数和图层叠加逐步作图.

当我们得到一个图形对象时, 可以对它进行如下处理.

- 用print() 将其打印到屏幕上. 在交互操作时, print 会自动被调用, 但是在循环或函数里, 我们需要手动输入print
- 用ggsave()将其保存在磁盘上  
`ggsave("plot.png", width=5, height=5)`
- summary() 简单查看其结构
- save() 把图像的缓存副本保存到磁盘; 这样可以保存图像的完整副本(包括图形中的数据), 可以调用load()来重现该图  
`save(p, file = "plot.rdata")`

# qplot
```
qplot(x, y, data=, color=, shape=, size=, alpha=, geom=, method=, formula=, facets=, 
		xlim=, ylim= xlab=, ylab=, main=, sub=)
```
**x, y**  
Specifies the variables placed on the horizontal and vertical axis. 
For univariate plots (for example, histograms), omit y

**data**  
Specifies a data frame

**color, shape, size, fill**  
Associates the levels of variable with symbol color, shape, or size.   
For line plots, color associates levels of a variable with line color.   
shape 必须是离散的, color 可以是连续的, 也可以是离散的, size 适合连续量
除了可以自动指定颜色外, 也可以手动指定, eg: `color = I("red"), size = I(2)`
For density and box plots, fill associates fill colors with a variable(用color来填充)., 例如
[ggplotdensity.png](http://www.statmethods.net/advgraphs/images/ggplotdensity.png)  
Legends are drawn automatically.

**alpha**  
transparency for overlapping elements expressed as a fraction between 0 (complete transparency) and 1 (complete opacity)

**facets**  
Creates a trellis(格子) graph by specifying conditioning variables.   
Its value is expressed as rowvar ~ colvar. 
To create trellis graphs based on a single conditioning variable, use rowvar~. or .~colvar)

**geom**  
Specifies the geometric objects that define the graph type.   
The geom option is expressed as a character vector with one or more entries.   
geom values include:

- "point"(散点图), 
- "smooth"(拟合曲线), 
- "path" 和 "line"(数据点之间绘制连线): line 只能创建从左到右的连线, 而path图可以使任意的方向
- "histogram"(直方图)
- "freqpoly"(频率多边形)
- "density"(密度曲线)
- "bar"(条形图)
- "boxplot"(箱线图), 
- "jitter"(扰动点图)

`geom=c('point','smooth')`: 将多个几何对象组成一个向量传递给geom, 几何对象会按照指定的顺序进行堆叠

**main, sub**  
Character vectors specifying the title and subtitle

If the text argument to one of the text-drawing functions (text, mtext, axis, legend) in R is an expression, 
the argument is interpreted as a mathematical expression and the output will be formatted according to TeX-like rules. 
Expressions can also be used for titles, subtitles and x- and y-axis labels (but not for axis labels on persp plots).

`?plotmath`

**method, formula**  
在图中添加平滑曲线

**xlab, ylab**  
Character vectors specifying horizontal and vertical axis labels

**xlim,ylim**  
Two-element numeric vectors giving the minimum and maximum values for the horizontal and vertical axes, respectively  
eg: `xlim = c(0,15)`

**log**  
一个字符向量, 说明哪一个坐标轴应该取对数.
例如, `log = "x"` 表示对 x 轴取对数, `log = "xy"` 表示对x轴和y轴都取对数

[ggplot2 demo](../demo/r/ggplot2.r)

[ggplot2 tutoraial](https://github.com/echen/ggplot2-tutorial/blob/master/README.md)

[Graphics with ggplot2](http://www.statmethods.net/advgraphs/ggplot2.html)

## 平滑曲线
If geom="smooth", a **loess fit line** and **confidence limits** are added by default.   
se = FALSE 可以去掉 confidence limits  
曲线的平滑程度由span参数控制, 0(很不平滑) 到 1(很平滑)  
当n较小时, loess是默认选项, 但是loess复杂度是O(n^2), 对于大数据不适用.
When the number of observations is greater than 1,000, a more efficient smoothing algorithm is employed. 

Methods include "lm" for regression, "gam" for generalized additive models, and "rlm" for robust regression. 
The formula parameter gives the form of the fit.   
For example, to add simple linear regression lines, you would specify `geom="smooth", method="lm", formula=y~x`. 
Changing the formula to `y~poly(x,2)` would produce a quadratic fit.   
Note that the formula uses the letters x and y, not the names of the variables.   
For `method="gam"`, be sure to load the mgcv package. For `method="rml"`, load the MASS package.

```
library(ggplot2)
head(diamonds)
##   carat       cut color clarity depth table price    x    y    z
## 1  0.23     Ideal     E     SI2  61.5    55   326 3.95 3.98 2.43
## 2  0.21   Premium     E     SI1  59.8    61   326 3.89 3.84 2.31
## 3  0.23      Good     E     VS1  56.9    65   327 4.05 4.07 2.31
## 4  0.29   Premium     I     VS2  62.4    58   334 4.20 4.23 2.63
## 5  0.31      Good     J     SI2  63.3    58   335 4.34 4.35 2.75
## 6  0.24 Very Good     J    VVS2  62.8    57   336 3.94 3.96 2.48

nrow(diamonds)
## [1] 53940

set.seed(1410)
dsmall <- diamonds[sample(nrow(diamonds),100),]

qplot(carat, price, data = dsmall, geom=c('point','smooth'))
qplot(carat, price, data = dsmall, geom=c('point','smooth'),se=FALSE)

qplot(carat, price, data = dsmall, geom=c('point','smooth'),se=FALSE)

library(mgcv)
qplot(carat, price, data = dsmall, geom=c('point','smooth'), method = 'gam', formula = y ~ s(x))
qplot(carat, price, data = dsmall, geom=c('point','smooth'), method = 'gam', formula = y ~ s(x, bs = "cs"))
## method = 'gam', formula = y ~ s(x, bs = "cs") 是数据量超过1000时默认的选项
```

## 箱线图与扰动点图
一个分类变量和一个或多个连续变量, 想知道连续变量会如何随着分类变量的变化而变化

箱线图  
用了5个数字对分布进行概括
```
qplot(color, price/carat, data=diamonds, geom = "boxplot")
```
![boxplot](http://i.imgbox.com/kWos4O2E.png)

扰动点图  
```
qplot(color, price/carat, data=diamonds, geom = "jitter")
```
![jitter](http://i.imgbox.com/GLkp5p6R.png)

扰动图可以像散点图那样对点的属性(大小, 颜色, 形状等)进行设置

```
qplot(color, price/carat, data=diamonds, geom = "jitter", alpha=I(0.1))
```
![jitter alpha](http://i.imgbox.com/QajYxjdO.png)


## 直方图与密度曲线图
直方图  
binwidth: 设定组距来调整平滑度  
breaks:切分位置也可以通过breaks 显示的设定  
在直方图中, 当组距较大时, 图形能反应数据的总体特征; 当组距较小时, 能能显示出更多的细节.

绘制直方图和密度曲线对平滑程度进行实验非常重要.

## 条形图

```
qplot(color, data = diamonds, geom = "bar")
nrow(diamonds[diamonds$color == "J",])
## [1] 2808
nrow(diamonds[diamonds$color == "F",])
## [1] 9542
```
![bar](http://i.imgbox.com/Ks90NwGZ.png)

使用weight, 对连续变量进行分组求和, 这里是将carat 对color分组求和
```
qplot(color, data = diamonds, geom = "bar", weight= carat)
sum(diamonds$carat[diamonds$color == "H"])
## [1] 7571.58
```
![bar weight](http://i.imgbox.com/miTsdwvy.png)

## 时间序列中的线条图和路径图

- 线条图: 将点从左到右进行连接
- 路线图: 按照点在数据集中的顺序对其进行连接

线条图的x轴一般是时间, 它展示了单个变量随时间变化的情况.  
路径图则展示了两个变量随时间联动的情况, 时间反映在点的顺序上

```
qplot(date, unemploy / pop, data = economics, geom = "line")
```

可以将两个时间序列花在同一张图中, 又尽管我们可以用散点图来表示失业率和失业时间长度之间的关系, 但是我们并不能看出变量随时间变化的情况.
对此, 解决的办法是将临近时点的散点连接起来, 形成一张**路径图**
```
year <- function(x){as.POSIXlt(x)$year + 1900}
qplot(unemploy / pop, uempmed, data = economics, geom = c("path"), color = year(date))
```

## 分面
将数据分隔为若干个子集, 然后创建一个图形的矩阵, 将每一个子集绘制到图形矩阵的窗格中.
所有图形子集采用相同的图形类型, 并进行了一定的设计, 使得他们之间更方面的进行比较.

`row_var ~ col_var`: 行变量 列变量  
如果想指定一行或一列, 可以使用 . 作为占位符, 
例如 `row_var ~ .` 表示一个单列多行的图形矩阵
例如 `. ~ col_var` 表示一个单行多列的图形矩阵

```
qplot(carat, data = diamonds, facets = color ~ ., geom = "histogram", binwidth = 0.1, xlim = c(0,3))
```
![facet](http://i.imgbox.com/Cdcs8zUF.png)

`.. density ..` 是一个新的语法, 将密度而不是频数映射到y 轴
```
qplot(carat,..density.., data = diamonds, facets = color ~ ., geom = "histogram", binwidth = 0.1, xlim = c(0,3))
```
![facet density](http://i.imgbox.com/XneB5Y95.png)


## 将多图绘制到一页
ggplot2在生成一页多图方面,有个facet,分面的命令,可以自动根据分组,每组对应一幅图出来.
但是,如果是不同类型的图,想拼到一起,就比较麻烦了.
在这里,Hiu给大家提供两种方法.

第一种,使用grid包的viewport功能.
```
library(ggplot2)
#generate two ggplot2 objects.
a <- qplot(TMPRSS4, ECADHERIN, data=spear)
b <- qplot(TMPRSS4, ECADHERIN, data=spear, geom="jitter")

library(grid)
#generate a new page, or erase the current one.
grid.newpage()
#maintain a Viewport tree, which describe the regions and coordinate systems.
pushViewport(viewport(layout=grid.layout(1,2)))
#print them into the Viewport, or the page.
print(a, vp=viewport(layout.pos.row=1,layout.pos.col=1))
print(b, vp=viewport(layout.pos.row=1,layout.pos.col=2))
```

第二种,使用一个新包,叫做gridExtra.
```
library(gridExtra)
a <- qplot(TMPRSS4, ECADHERIN, data=spear)
b <- qplot(TMPRSS4, ECADHERIN, data=spear, geom="jitter")
grid.arrange(a,b,ncol=2)
```
于是,我们就得到了二合一的图片.
当然你也可以按照你的意愿随意控制图的位置.

Customizing ggplot2 Graphs
```
library(ggplot2)
p <- qplot(hp, mpg, data=mtcars, shape=am, color=am, facets=gear~cyl, 
	main="Scatterplots of MPG vs. Horsepower", xlab="Horsepower", ylab="Miles per Gallon")

# White background and black grid lines
p <- p + theme_bw()

# Large brown bold italics labels and legend placed at top of plot
p <- p + theme(axis.title=element_text(face="bold.italic", size="12", color="brown"), legend.position="top")
```

# 用图层构建图像
qplot()的局限性在于它只能用一个数据集合一组图形属性映射, 解决这个问题的办法就是利用图层.
每个图层可以有自己的数据集合图形属性映射, 附加的数据元素可通过图层添加到图形中.

一个图层的由5 个部分组成:

1. 数据, 必须是一个data frame, 在绘图结束后可以被修改
1. 一组图形属性映射
1. 几何对象
1. 统计变换
1. 位置调整, 通过调整元素位置来避免图形重合

## 创建绘图对象
参数**映射**的设定方法与前面讲过的qplot()非常相似, 只需要将图形属性和变量名放到函数`aes()`里面即可. 例如
```
## 设定一组默认参数, x 为 carat, y 为price, color 为 cut
p <- ggplot(diamonds, aes(carat, price, color = cut))
```
这个图形在加上图层之前无法显示, 因此现在什么也看不到

## 图层
最简易的图层莫过于指设定一个几何对象
```
p <- p + layer(geom = "point")
```
现在可以显示, 得到一个散点图

该图层使用了默认的数据集合图形属性映射, 并且只使用了另外两个可选参数的默认值: 统计变换和位置调整.

图层的完整形式
```
layer(geom, geom_params, stat, stat_params, data, mapping, position)
```

组距为2, 红色的直方图
```
p <- p + layer(
geom = "bar",
geom_params = list(fill = "red"),
stat = "bin",
stat_params = list(binwidth = 2)
)
p
```

`geom_histogram(binwidth = 2, fill = "red")` 生成与上面完全一样的图层.  
这就是快捷函数, 因为**每一个几何对象都对应着一个默认的统计变换和位置参数, 而每一个统计变换都对应着一个默认的几何对象参数**, 所以对于一个图层, 
我们只需要设定stat或geom 参数即可.

# 工具箱
## 基本图形类型
数据准备
```
df <- data.frame(x = c(3,1,5), y = c(2,4,6), label = c("a", "b", "c"))
df
##   x y label
## 1 3 2     a
## 2 1 4     b
## 3 5 6     c
p <- ggplot(df, aes(x,y)) + xlab(NULL) + ylab(NULL)
```
散点图
```
p + geom_point() + labs(title = "geom point")
```

条形图
```
p + geom_bar(stat="identity") + labs(title = "geom_bar(stat=\"identity\")")
```
![bar identity](http://i.imgbox.com/K7asxCBB.png)

```
p + geom_bar() + labs(title = "geom_bar()")
Error : Mapping a variable to y and also using stat="bin".
  With stat="bin", it will attempt to set the y value to the count of cases in each group.
  This can result in unexpected behavior and will not be allowed in a future version of ggplot2.
  If you want y to represent counts of cases, use stat="bin" and don't map a variable to y.
  If you want y to represent values in the data, use stat="identity".
  See ?geom_bar for examples. (Defunct; last used in version 0.9.2)
q <- ggplot(df, aes(x)) + xlab(NULL) + ylab(NULL)
q + geom_bar(stat="bin") + labs(title = "geom_bar(stat=\"bin\")")
```
![bar bin](http://i.imgbox.com/TgdZDMiu.png)

线条图
```
p + geom_line() + labs(title = "geom_line")
```
从图中可以明显的看出线条是由图中的点从左到右进行连接的
![line](http://i.imgbox.com/7pdUnV34.png)

路径图
```
p + geom_path() + labs(title = "geom_path")
```
![path](http://i.imgbox.com/3PIbjRQw.png)
从图中可以明显的看出路径是按照点在df 中出现的顺序进行连接的

面积图
```
p + geom_area() + labs(title = "geom_area")
```
![area](http://i.imgbox.com/zvZNiVIy.png)

含标签的散点图  
可以通过图形属性angle 控制文本的旋转
```
p + geom_text(aes(label = label)) + labs(title = "geom_text")
```
![text](http://i.imgbox.com/t9QdlbNq.png)

多边形图  
绘制填充后的路径
```
p + geom_polygon() + labs(title = "geom_polygon")
```
![polygon](http://i.imgbox.com/LGxs2xwr.png)

瓦片图  
用来绘制色深图(image plot)或水平图(level plot).
```
p + geom_tile() + labs(title = "geom_tile")
```
![tile](http://i.imgbox.com/NFiHgXve.png)

## 展示数据分布
基本的数据: `depth_dist <- ggplot(diamonds, aes(depth))`
加上直方图几何对象: `depth_dist + geom_histogram()`[figure](http://imgbox.com/n8oJgh17)  
设置组距: `depth_dist + geom_histogram(binwidth = 0.1)`[figure](http://imgbox.com/8LmIEbI5)  
放大细节: `depth_dist + geom_histogram(binwidth = 0.1) + scale_x_continuous(limits = c(55,70))`[figure](http://imgbox.com/0k1c4LIv)  
设置标题: `depth_dist + geom_histogram(binwidth = 0.1) + scale_x_continuous(limits = c(55,70)) + labs(title = "histogram of depth with binwidth = 0.1")`[figure](http://imgbox.com/bXEJHgP3)  
应用分面: `depth_dist + geom_histogram(binwidth = 0.1) + scale_x_continuous(limits = c(55,70)) + labs(title = "histogram of depth with binwidth = 0.1") + facet_grid(cut ~ .)`[figure](http://imgbox.com/awV1nNWM)  

## 处理遮盖