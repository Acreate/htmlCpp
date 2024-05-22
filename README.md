# htmlCpp

基于 C++ / cpp  实现的 html 解析库

加入 xpath查找（类 xpath）

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

​	上级目录

./ 

​	当前目录

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



# 测试调用

## testHtmlCpp

#### void testXAttribute( const cylHtmlTools::HtmlString &test_paremt_name, const cylHtmlTools::HtmlString &test_paremt_value )

```c++
/// <summary>
/// 测试 XDirAttribute
/// </summary>
/// <param name="test_paremt_name">XDirAttribute 名称</param>
/// <param name="test_paremt_value">XDirAttribute 值</param>
void testXAttribute( const cylHtmlTools::HtmlString &test_paremt_name, const cylHtmlTools::HtmlString &test_paremt_value );
```

```c++
void testXAttribute( const cylHtmlTools::HtmlString &test_paremt ) {
	testXAttribute( test_paremt, test_paremt );
}
```

```c++
	testXAttribute( LR"(@class="23 31" 123 " 3 11 ")" );
	testXAttribute( LR"(@acd="23 31" 123 " 3 11 ")" );
```

##### 显示

```
===============
找到名称: "class"
找到值: "23"
找到值: "31"
找到值: "123"
找到值: "3"
找到值: "11"
===============
===============
找到名称: "acd"
找到值: "23 31"
找到值: "123"
找到值: " 3 11 "
===============
```



#### void testXAttributeIsIncludeOther( const cylHtmlTools::HtmlString &test_paremt_name, const cylHtmlTools::HtmlString &test_paremt_value, const std::vector< cylHtmlTools::HtmlString > &value );

```c++
/// <summary>
/// 测试属性对象是否包含指定属性值
/// </summary>
/// <param name="test_paremt_name">用于构建 xdir 对象的名称参数</param>
/// <param name="test_paremt_value">用于构建 xdir 对象的值参数</param>
/// <param name="value">用于匹配被包含的值列表</param>
void testXAttributeIsIncludeOther( const cylHtmlTools::HtmlString &test_paremt_name, const cylHtmlTools::HtmlString &test_paremt_value, const std::vector< cylHtmlTools::HtmlString > &value );
```

```c++
void testXAttributeIsIncludeOther( const cylHtmlTools::HtmlString &test_paremt, const std::vector< cylHtmlTools::HtmlString > &value ) {
	testXAttributeIsIncludeOther( test_paremt, test_paremt, value );
}
```

```c++
	testXAttributeIsIncludeOther( LR"(@acd="23 31" 123 " 3 11 ")", { LR"(23)" } );
	testXAttributeIsIncludeOther( LR"(@acd="23 31" 123 " 3 11 ")", { LR"(123)" } );
	testXAttributeIsOtherInclude( LR"(@acd="23 31" 123 " 3 11 ")",
		{ LR"(23 31)"
			, LR"(123)"
			, LR"( 3 11 )"
			, LR"(23)"
			, LR"(777)" }
	);
	testXAttributeIsOtherInclude( LR"(@acd="23 31" 123 " 3 11 ")",
		{ LR"(23 31)"
			, LR"( 3 11 )"
			, LR"(23)"
			, LR"(777)" }
	);
```

##### 显示

```
=============== 测试 XDirAttribute 包含其他值列表
        找到名称: "acd"
                找到值: "23 31"
                找到值: "123"
                找到值: " 3 11 "
        输出值列表 : "23"
        查找值列表 : 不包含
===============
=============== 测试 XDirAttribute 包含其他值列表
        找到名称: "acd"
                找到值: "23 31"
                找到值: "123"
                找到值: " 3 11 "
        输出值列表 : "123"
        查找值列表 : 包含
===============
=============== 测试其他包含 XDirAttribute 值列表
        找到名称: "acd"
        找到值: "23 31"
        找到值: "123"
        找到值: " 3 11 "
        输出值列表 : "23 31", "123", " 3 11 ", "23", "777"
        查找值列表 : 包含
===============
=============== 测试其他包含 XDirAttribute 值列表
        找到名称: "acd"
        找到值: "23 31"
        找到值: "123"
        找到值: " 3 11 "
        输出值列表 : "23 31", " 3 11 ", "23", "777"
        查找值列表 : 不包含
===============
```



#### void testXDir( const cylHtmlTools::HtmlString &test_paremt ) 

```c++
/// <summary>
/// 测试 XDir 对象
/// </summary>
/// <param name="test_paremt">生成 XDir 对象的参数</param>
void testXDir( const cylHtmlTools::HtmlString &test_paremt ) ;
```

```c++
testXDir( LR"(div[@"id"="sitebox sd" @class="cf ds"])" );
```

##### 显示

```
===============
获得路径名称:"div"
        获得属性名称:"id"
                获得属性值:"sitebox"
                获得属性值:"sd"
        获得属性名称:"class"
                获得属性值:"cf"
                获得属性值:"ds"

===============
```



#### void testXPath( const cylHtmlTools::HtmlString &test_paremt )

```c++
/// <summary>
/// 测试 XPath 对象
/// </summary>
/// <param name="test_paremt"> xpath 对象的参数</param>
void testXPath( const cylHtmlTools::HtmlString &test_paremt );
```

```c++
testXPath( LR"(div[@"id"="sitebox" @class="cf"])" );
```

##### 显示

```
===============
获取路径 : "div[@id=sitebox @class=cf]"
找到目录名称: "div"
获得路径名称:"div"
        获得属性名称:"id"
                获得属性值:"sitebox"
        获得属性名称:"class"
                获得属性值:"cf"

===============
```



#### void testHtmlNodeAttributeConverToXDirAttribute( const cylHtmlTools::HtmlString &parem )

```c++
/// <summary>
/// 测试：把一个 node 节点转换到 XDirAttribute 
/// </summary>
/// <param name="parem"></param>
void testHtmlNodeAttributeConverToXDirAttribute( const cylHtmlTools::HtmlString &parem );
```

```c++
	testHtmlNodeAttributeConverToXDirAttribute( LR"(id="sitebox" class="cf")" );
	testHtmlNodeAttributeConverToXDirAttribute( LR"(id="sitebox" class="cf de")" );
	testHtmlNodeAttributeConverToXDirAttribute( LR"(id="sitebox bs" class="cf de")" );
	testHtmlNodeAttributeConverToXDirAttribute( LR"(id="sitebox bs" sitebox1 bs2 class="cf de")" );
	testHtmlNodeAttributeConverToXDirAttribute( LR"(id="sitebox bs" sitebox1 bs2 class="cf de" cf3 de4)" );
```

##### 显示

```
=============== 测试节点属性转换到 Vector_XDirAttributeSPtr_Shared
        转换:"id="sitebox" class="cf""
                获得属性名称:"id"
                        获得属性值:"sitebox"
                获得属性名称:"class"
                        获得属性值:"cf"
===============
=============== 测试节点属性转换到 Vector_XDirAttributeSPtr_Shared
        转换:"id="sitebox" class="cf de""
                获得属性名称:"id"
                        获得属性值:"sitebox"
                获得属性名称:"class"
                        获得属性值:"cf"
                        获得属性值:"de"
===============
=============== 测试节点属性转换到 Vector_XDirAttributeSPtr_Shared
        转换:"id="sitebox bs" class="cf de""
                获得属性名称:"id"
                        获得属性值:"sitebox"
                        获得属性值:"bs"
                获得属性名称:"class"
                        获得属性值:"cf"
                        获得属性值:"de"
===============
=============== 测试节点属性转换到 Vector_XDirAttributeSPtr_Shared
        转换:"id="sitebox bs" sitebox1 bs2 class="cf de""
                获得属性名称:"id"
                        获得属性值:"sitebox"
                        获得属性值:"bs"
                        获得属性值:"sitebox1"
                        获得属性值:"bs2"
                获得属性名称:"class"
                        获得属性值:"cf"
                        获得属性值:"de"
===============
=============== 测试节点属性转换到 Vector_XDirAttributeSPtr_Shared
        转换:"id="sitebox bs" sitebox1 bs2 class="cf de" cf3 de4"
                获得属性名称:"id"
                        获得属性值:"sitebox"
                        获得属性值:"bs"
                        获得属性值:"sitebox1"
                        获得属性值:"bs2"
                获得属性名称:"class"
                        获得属性值:"cf"
                        获得属性值:"de"
                        获得属性值:"cf3"
                        获得属性值:"de4"
===============
```

