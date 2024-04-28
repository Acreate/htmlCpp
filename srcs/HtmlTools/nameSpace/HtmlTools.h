#ifndef XMLTOOLS_H_H_HEAD__FILE__
#define XMLTOOLS_H_H_HEAD__FILE__
#pragma once

#include <memory>
#include <unordered_map>
#include <vector>
#include <string>

#include "../export/HtmlTools_export.h" // 导出导入声明
namespace htmlTools {
	namespace charValue {
		static constexpr wchar_t singleQuotation = L'\''; // 单引号
		static constexpr wchar_t exclamation = L'!'; // 感叹号。用于识别 DOCTYPE 节点或注释节点
		static constexpr wchar_t doubleQuotation = L'\"'; // 双引号
		static constexpr wchar_t nodeStartChar = L'<'; // 节点开始
		static constexpr wchar_t nodeEndChar = L'>'; // 节点结束
		static constexpr wchar_t forwardSlash = L'/'; // 斜杠路径符。节点类型判定(单元素节点/双元素节点)
		static constexpr wchar_t backSlash = L'\\'; // 反斜杠路径符
		static constexpr wchar_t euq = L'='; // 等号
		static constexpr wchar_t dot = L'.'; // 点
		static constexpr wchar_t zero = L'\0'; // 字符串结束符
		static constexpr wchar_t at = L'@'; // 服务器位置符号
	}
	class HTMLTOOLS_EXPORT RefWStr;
	// html 文档
	class HTMLTOOLS_EXPORT HtmlDoc;
	// 宽字符工具
	class HTMLTOOLS_EXPORT WStrTools;
	// 节点
	class HTMLTOOLS_EXPORT HtmlNode;
	// 工具
	class HTMLTOOLS_EXPORT XPath;
	// 类型
	enum class HTMLTOOLS_EXPORT Html_Node_Type;
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
	/// 限定文本类型
	/// </summary>
	using HtmlString = std::wstring;
	/// <summary>
	/// 宽字符串共享指针
	/// </summary>
	using HtmlString_Shared = std::shared_ptr< HtmlString >;
	/// <summary>
	/// std::list< std::shared_ptr< HtmlString > >
	/// </summary>
	using List_HtmlStringSptr = std::list< HtmlString_Shared >;
	/// <summary>
	///  std::shared_ptr<std::list< std::shared_ptr< HtmlString > > >
	/// </summary>
	using List_HtmlStringSptr_Shared = std::shared_ptr< List_HtmlStringSptr >;
	/// <summary>
	/// 字符串配对的列表映射
	/// </summary>
	using WStringPairUnorderMap = std::unordered_map< HtmlString, HtmlString >;
	/// <summary>
	/// 字符串配对的列表映射 的共享指针
	/// </summary>
	using WStringPairUnorderMap_Shared = std::shared_ptr< WStringPairUnorderMap >;
}

#endif // XMLTOOLS_H_H_HEAD__FILE__
