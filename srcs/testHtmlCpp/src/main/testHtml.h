#ifndef TESTHTML_H_H_HEAD__FILE__
#define TESTHTML_H_H_HEAD__FILE__
#pragma once
#include <locale>
#include <ostream>

#include "nameSpace/cylHtmlTools.h"

int testHtml( std::locale locale = std::locale( ) );
/// <summary>
/// 输出一个 html doc 对象
/// </summary>
/// <param name="shared">输出对象</param>
void outHtmlDoc( const cylHtmlTools::HtmlDoc_Shared & shared );
/// <summary>
/// 测试 IncludeText 输出
/// </summary>
/// <param name="htmlDoc">html 文本</param>
void testNodeIncludeText( const cylHtmlTools::HtmlString &htmlDoc );
/// <summary>
/// 测试 IncludeText 输出
///	基于节点查找
/// </summary>
/// <param name="htmlDoc">html 文档节点对象</param>
/// <param name="lrDivIdSitebox">xptah 路径</param>
void testNodeIncludeText( cylHtmlTools::HtmlDoc_Shared htmlDoc, const cylHtmlTools::HtmlString &lrDivIdSitebox );
/// <summary>
/// 根查找
/// </summary>
/// <param name="stringstream"></param>
/// <param name="htmlDoc"></param>
/// <param name="xpath"></param>
/// <returns></returns>
bool getValue( std::wstringstream &stringstream, cylHtmlTools::HtmlDoc_Shared htmlDoc, cylHtmlTools::XPath &xpath );
/// <summary>
/// 节点查找
/// </summary>
/// <param name="stringstream"></param>
/// <param name="html_nodes"></param>
/// <param name="xpath"></param>
/// <returns></returns>
bool getValue( std::wstringstream &stringstream, cylHtmlTools::Vector_HtmlNodeSPtr_Shared &html_nodes, cylHtmlTools::XPath &xpath );

/// <summary>
/// HtmlDoc 案例
/// </summary>
/// <param name="locale">输出编码</param>
/// <returns>非 0 表示异常</returns>
int testHtmlDoc( std::locale locale = std::locale( ) );

/// <summary>
/// 测试 XDirAttribute
/// </summary>
/// <param name="test_paremt_name">XDirAttribute 名称</param>
/// <param name="test_paremt_value">XDirAttribute 值</param>
void testXAttribute( const cylHtmlTools::HtmlString &test_paremt_name, const cylHtmlTools::HtmlString &test_paremt_value );
void testXAttribute( const cylHtmlTools::HtmlString &test_paremt );
/// <summary>
/// 测试 XDir 对象
/// </summary>
/// <param name="test_paremt">生成 XDir 对象的参数</param>
void testXDir( const cylHtmlTools::HtmlString &test_paremt );
/// <summary>
/// 测试 XDir 对象的包含功能
/// </summary>
/// <param name="left_param">左生成对象字符串</param>
/// <param name="right_param">右生成对象字符串</param>
void testIncludeXDir( const cylHtmlTools::HtmlString &left_param, const cylHtmlTools::HtmlString &right_param );
/// <summary>
/// 测试 XPath 对象
/// </summary>
/// <param name="test_paremt"> xpath 对象的参数</param>
void testXPath( const cylHtmlTools::HtmlString &test_paremt );

/// <summary>
/// 测试属性对象是否包含指定属性值
/// </summary>
/// <param name="test_paremt_name">用于构建 xdir 对象的名称参数</param>
/// <param name="test_paremt_value">用于构建 xdir 对象的值参数</param>
/// <param name="value">用于匹配被包含的值列表</param>
void testXAttributeIsIncludeOther( const cylHtmlTools::HtmlString &test_paremt_name, const cylHtmlTools::HtmlString &test_paremt_value, const std::vector< cylHtmlTools::HtmlString > &value );
/// <summary>
/// 测试属性对象是否包含指定属性值
/// </summary>
/// <param name="test_paremt">用于构建 xdir 对象的名称参数</param>
/// <param name="value">用于匹配被包含的值列表</param>
void testXAttributeIsIncludeOther( const cylHtmlTools::HtmlString &test_paremt, const std::vector< cylHtmlTools::HtmlString > &value );
/// <summary>
/// 测试属性对象是否被包含在指定列表对象当中
/// </summary>
/// <param name="test_paremt_name">用于构建 xdir 对象的名称参数</param>
/// <param name="test_paremt_value">用于构建 xdir 对象的值参数</param>
/// <param name="value">用于匹配包含 xdir 对象的值列表</param>
void testXAttributeIsOtherInclude( const cylHtmlTools::HtmlString &test_paremt_name, const cylHtmlTools::HtmlString &test_paremt_value, const std::vector< cylHtmlTools::HtmlString > &value );
/// <summary>
/// 测试属性对象是否被包含在指定列表对象当中
/// </summary>
/// <param name="test_paremt">用于构建 xdir 对象的参数</param>
/// <param name="value">用于匹配包含 xdir 对象的值列表</param>
void testXAttributeIsOtherInclude( const cylHtmlTools::HtmlString &test_paremt, const std::vector< cylHtmlTools::HtmlString > &value );
void findElements( cylHtmlTools::HtmlDoc_Shared htmlDoc, const cylHtmlTools::HtmlString &lrDivIdSitebox );
/// <summary>
/// 转化一个html 节点
/// </summary>
/// <param name="html_doc_shared">html 文档</param>
void htmlDocNodeConverToXDir( const cylHtmlTools::HtmlDoc_Shared &html_doc_shared );
/// <summary>
/// 测试：把一个 node 节点转换到 XDirAttribute 
/// </summary>
/// <param name="parem"></param>
void testHtmlNodeAttributeConverToXDirAttribute( const cylHtmlTools::HtmlString &parem );
#endif // TESTHTML_H_H_HEAD__FILE__
