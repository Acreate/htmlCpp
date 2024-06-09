#ifndef XMLTOOLS_H_H_HEAD__FILE__
#define XMLTOOLS_H_H_HEAD__FILE__
#pragma once

#include <memory>
#include <unordered_map>
#include <vector>
#include <string>
#include <list>
#include "../auto_generate_files/export/HtmlTools_export.h" // 导出导入声明
namespace cylHtmlTools {
	/// <summary>
	/// 限定文本类型
	/// </summary>
	using HtmlString = std::wstring;
	/// <summary>
	/// 限定字符类型
	/// </summary>
	using HtmlChar = HtmlString::value_type;

	/// <summary>
	/// html 工具类
	/// </summary>
	class HtmlDocTools;
	// html 文档
	class HtmlDoc;
	// 宽字符工具
	class HtmlStringTools;
	// 节点
	class HtmlNode;
	// 工具
	class XPath;
	// 工具目录
	class XDir;

	// 工具目录属性
	class XDirAttribute;
	/// <summary>
	/// 路径工具
	/// </summary>
	class PathTools;

	// 工作线程
	class HtmlWorkThread;
	/// <summary>
	/// 线程池
	/// </summary>
	class HtmlWorkThreadPool;

	// std::shared_ptr< XDir > 封装
	using XDir_Shared = std::shared_ptr< XDir >;
	//  std::vector< std::shared_ptr< XDir >> 封装
	using Vector_XDirSPtr = std::vector< XDir_Shared >;
	//  std::shared_ptr< std::vector< std::shared_ptr< XDir >> > 封装
	using Vector_XDirSPtr_Shared = std::shared_ptr< Vector_XDirSPtr >;
	// 类型
	enum class Html_Node_Type;
	// std::shared_ptr< HtmlDoc > 封装
	using HtmlDoc_Shared = std::shared_ptr< HtmlDoc >;
	// std::shared_ptr< HtmlNode > 封装
	using HtmlNode_Shared = std::shared_ptr< HtmlNode >;
	//  std::vector< std::shared_ptr< HtmlNode_Shared > > 封装
	using Vector_HtmlNodeSPtr = std::vector< HtmlNode_Shared >;
	// std::shared_ptr< std::vector< std::shared_ptr< Vector_HtmlNodeSPtr > > > 封装
	using Vector_HtmlNodeSPtr_Shared = std::shared_ptr< Vector_HtmlNodeSPtr >;
	// std::shared_ptr< HtmlXPath > 封装指针


	/// <summary>
	/// 宽字符串共享指针
	/// </summary>
	using HtmlString_Shared = std::shared_ptr< HtmlString >;
	/// <summary>
	/// std::list< std::shared_ptr< HtmlString > >
	/// </summary>
	using List_HtmlStringSptr = std::list< HtmlString_Shared >;
	/// <summary>
	///  std::vector< HtmlString_Shared >
	/// </summary>
	using Vector_HtmlStringSPtr = std::vector< HtmlString_Shared >;
	// std::shared_ptr< Vector_HtmlStringSPtr >
	using Vector_HtmlStringSPtr_Shared = std::shared_ptr< Vector_HtmlStringSPtr >;
	/// <summary>
	///  std::shared_ptr<std::list< std::shared_ptr< HtmlString > > >
	/// </summary>
	using List_HtmlStringSptr_Shared = std::shared_ptr< List_HtmlStringSptr >;
	/// <summary>
	/// 字符串配对的列表映射
	/// </summary>
	using UMap_HtmlStringK_HtmlString = std::unordered_map< HtmlString, HtmlString >;
	/// <summary>
	/// 字符串配对的列表映射 的共享指针
	/// </summary>
	using UMap_HtmlStringK_HtmlStringV_Shared = std::shared_ptr< UMap_HtmlStringK_HtmlString >;

	// std::unordered_map< HtmlString, Vector_HtmlStringSPtr_Shared >
	using UMap_HtmlK_VectorSPtr_HtmlStringSPtrV = std::unordered_map< HtmlString, Vector_HtmlStringSPtr_Shared >;

	/// <summary>
	/// std::shared_ptr< XDirAttribute >
	/// </summary>
	using XDirAttribute_Shared = std::shared_ptr< XDirAttribute >;

	/// <summary>
	/// XDirAttribute_Shared 迭代器 @see XDirAttribute_Shared
	/// </summary>
	using Vector_XDirAttributeSPtr = std::vector< XDirAttribute_Shared >;
	/// <summary>
	/// std::shared_ptr< std::vector< std::shared_ptr< XDirAttribute > > >
	/// </summary>
	using Vector_XDirAttributeSPtr_Shared = std::shared_ptr< Vector_XDirAttributeSPtr >;
	/// <summary>
	///  std::unordered_map< HtmlString, std::shared_ptr< std::vector< std::shared_ptr< XDirAttribute > > > >
	/// </summary>
	using UMap_HtmlStringK_VectorSPtr_XDirAttributeSPtrV = std::unordered_map< HtmlString, Vector_XDirAttributeSPtr_Shared >;
	/// <summary>
	///  std::shared_ptr< std::unordered_map< HtmlString, std::shared_ptr< std::vector< std::shared_ptr< XDirAttribute > > > > >
	/// </summary>
	using UMap_HtmlStringK_VectorSPtr_XDirAttributeSPtrV_Shared = std::shared_ptr< UMap_HtmlStringK_VectorSPtr_XDirAttributeSPtrV >;

}
namespace cylHtmlTools {
	namespace charValue {
		static constexpr HtmlChar singleQuotation = L'\''; // 单引号
		static constexpr HtmlChar exclamation = L'!'; // 感叹号。用于识别 DOCTYPE 节点或注释节点
		static constexpr HtmlChar doubleQuotation = L'\"'; // 双引号
		static constexpr HtmlChar nodeStartChar = L'<'; // 节点开始
		static constexpr HtmlChar nodeEndChar = L'>'; // 节点结束
		static constexpr HtmlChar forwardSlash = L'/'; // 斜杠路径符。节点类型判定(单元素节点/双元素节点)
		static constexpr HtmlChar backSlash = L'\\'; // 反斜杠路径符
		static constexpr HtmlChar equ = L'='; // 等号
		static constexpr HtmlChar dot = L'.'; // 点
		static constexpr HtmlChar zero = L'\0'; // 字符串结束符
		static constexpr HtmlChar at = L'@'; // 服务器位置符号
		static constexpr HtmlChar space = L' '; // 空格符
		static constexpr HtmlChar tab = L'\t'; // 制表符
		static constexpr HtmlChar leftSquareBracket = L'['; // 左中括号
		static constexpr HtmlChar rightSquareBracket = L']'; // 右中括号
	}
}
#endif // XMLTOOLS_H_H_HEAD__FILE__
