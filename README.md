# htmlCpp

基于 C++ 实现的 html 解析库

## 环境

C++

camake

## 主要调用

### 静态函数

```c++
/// <summary>
/// 根据字符串内容生成节点列表
/// </summary>
/// <param name="std_c_w_string">指向字符串的指针</param>
/// <param name="end_index">结束下标</param>
/// <param name="start_index">开始下标，最终下标</param>
/// <returns>解析后的列表</returns>
static HtmlDoc HtmlTools::HtmlDoc::parse( const std::shared_ptr< std::wstring > std_c_w_string, size_t &end_index, size_t &start_index );


```

### 成员函数

```c++
/// <summary>
/// 本对象关联的节点解析兄弟关系
/// </summary>
/// <returns>解析完成的节点列表</returns>
Vector_HtmlNodeSPtr_Shared HtmlTools::HtmlDoc::analysisBrotherNode( );

```

```c++
/// <summary>
/// 遍历获取节点<br/>
/// 节点传递到 callFun 调用参数当中，当需要存储时，返回 true。直到结束
/// </summary>
/// <param name="callFun">校验函数</param>
/// <returns>返回列表</returns>
Vector_HtmlNodeSPtr_Shared HtmlTools::HtmlDoc::getNodes( const std::function< bool( const HtmlNode_Shared &node ) > &callFun );
```



## 库

### cylHtmlTools::XPath

实现 xptah路径查找

#### 首要路径

../

​	上级目录，获取父级节点的所有友邻节点

./ 

​	当前目录，获取当前节点的所有友邻节点

any

​	任意节点，将会搜索任意名称为 any 节点名称的节点

/ 

​	根节点，返回根部节点

#### 案例

//body

​	根节点下的 body 节点

./body

​	当前节点中搜索所有友邻节点，并且返回 bdoy 节点

../body

​	父级节点当中搜索 body 节点

div

​	所有名为 div 的节点

div[@class='hd']

​	所有属性为 class=‘hd’ 的 div 节点

div[@class='hd cf']/div

所有拥有属性为 class=‘hd’ 的 div 父节点的 div 节点

​	



#### 测试调用

testHtmlCpp