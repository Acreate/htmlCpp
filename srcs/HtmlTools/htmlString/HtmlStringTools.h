#ifndef WSTRTOOLS_H_H_HEAD__FILE__
#define WSTRTOOLS_H_H_HEAD__FILE__
#pragma once

#include <locale>


#include "../../../../../libs/ShaderModel/shaders/NovelInfo/nameSpace/interfacePlugsType.h"
#include "../nameSpace/cylHtmlTools.h"
namespace cylHtmlTools {
	class HTMLTOOLS_EXPORT HtmlStringTools {
	public:
		/// <summary>
		/// 是否为一个跳过的空字符 <br/>
		/// 其中包含控制字符等
		/// </summary>
		/// <param name="currentChar">检测的字符</param>
		/// <returns>true 表示空字符</returns>
		static bool isSpace( const HtmlChar currentChar );
		/// <summary>
		/// 是否为一个跳过的空字符 <br/>
		/// 其中包含控制字符等
		/// </summary>
		/// <param name="currentChar">检测的字符</param>
		/// <returns>true 表示空字符</returns>
		static bool isSpace( const interfacePlugsType::HtmlDocString &currentChar );
		/// <summary>
		/// 是否为一个跳过的空字符 <br/>
		/// 其中包含控制字符等
		/// </summary>
		/// <param name="currentChar">检测的字符</param>
		/// <returns>true 表示空字符</returns>
		static bool isSpace( const std::string::value_type &currentChar );
		/// <summary>
		/// 获取匹配单引号的结束位置<br/>
		/// buff[start_index] 必须等于 charValue::singleQuotation
		/// </summary>
		/// <param name="buff">缓冲</param>
		/// <param name="buff_size">缓冲大小</param>
		/// <param name="start_index">开始位置</param>
		/// <param name="get_quoation_position_end">返回的单引号结束位置</param>
		/// <param name="get_quotation_position_s">返回的引号配对</param>
		/// <returns>成功返回 true</returns>
		static bool jumpSingleQuotation( const HtmlChar *buff, size_t buff_size, size_t start_index, size_t &get_quoation_position_end, std::vector< std::pair< size_t, size_t > > &get_quotation_position_s );
		/// <summary>
		/// 获取匹配双引号的结束位置<br/>
		/// buff[start_index] 必须等于 charValue::doubleQuotation
		/// </summary>
		/// <param name="buff">缓冲</param>
		/// <param name="buff_size">缓冲大小</param>
		/// <param name="start_index">开始位置</param>
		/// <param name="get_quoation_position_end">返回的双引号结束位置</param>
		/// <param name="get_quotation_position_s">返回的引号配对</param>
		/// <returns>成功返回 true</returns>
		static bool jumpDoubleQuotation( const HtmlChar *buff, size_t buff_size, size_t start_index, size_t &get_quoation_position_end, std::vector< std::pair< size_t, size_t > > &get_quotation_position_s );
		/// <summary>
		/// 是否是引号(单引号'或者双引号")
		/// </summary>
		/// <param name="char_value">判断字符</param>
		/// <returns>引号返回 true</returns>
		static bool isQuotation( const HtmlChar &char_value );
		/// <summary>
		/// 获取缓冲的的匹配引号位置<br/>
		/// buff[start_index] 必须等于 charValue::doubleQuotation 或者 charValue::singleQuotation，否则返回 false
		/// </summary>
		/// <param name="buff">缓冲</param>
		/// <param name="buff_size">缓冲大小</param>
		/// <param name="start_index">开始位置</param>
		/// <param name="get_quoation_position_end">最后的引号匹配位置</param>
		/// <param name="get_quotation_position_s">引号匹配列表（包含嵌套）</param>
		/// <returns>不存在返回 faluse</returns>
		static bool jumpQuotation( const HtmlChar *buff, const size_t buff_size, size_t start_index, size_t &get_quoation_position_end, std::vector< std::pair< size_t, size_t > > &get_quotation_position_s );


		/// <summary>
		/// 获取匹配单引号的结束位置<br/>
		/// buff[start_index] 必须等于 charValue::singleQuotation
		/// </summary>
		/// <param name="buff">缓冲</param>
		/// <param name="buff_size">缓冲大小</param>
		/// <param name="start_index">开始位置</param>
		/// <param name="get_quoation_position_end">返回的单引号结束位置</param>
		/// <returns>成功返回 true</returns>
		static bool jumpSingleQuotation( const HtmlChar *buff, size_t buff_size, size_t start_index, size_t &get_quoation_position_end );
		/// <summary>
		/// 获取匹配双引号的结束位置<br/>
		/// buff[start_index] 必须等于 charValue::doubleQuotation
		/// </summary>
		/// <param name="buff">缓冲</param>
		/// <param name="buff_size">缓冲大小</param>
		/// <param name="start_index">开始位置</param>
		/// <param name="get_quoation_position_end">返回的双引号结束位置</param>
		/// <returns>成功返回 true</returns>
		static bool jumpDoubleQuotation( const HtmlChar *buff, size_t buff_size, size_t start_index, size_t &get_quoation_position_end );
		/// <summary>
		/// 获取缓冲的的匹配引号位置<br/>
		/// buff[start_index] 必须等于 charValue::doubleQuotation 或者 charValue::singleQuotation，否则返回 false
		/// </summary>
		/// <param name="buff">缓冲</param>
		/// <param name="buff_size">缓冲大小</param>
		/// <param name="start_index">开始位置</param>
		/// <param name="get_quoation_position_end">最后的引号匹配位置</param>
		/// <returns>不存在返回 faluse</returns>
		static bool jumpQuotation( const HtmlChar *buff, const size_t buff_size, size_t start_index, size_t &get_quoation_position_end );

		/// <summary>
		/// 是否路径符(包含 '/' 与 '\')
		/// </summary>
		/// <param name="currentChar">检测符号</param>
		/// <returns>true 表示路径符</returns>
		static bool isRouteChar( HtmlChar currentChar );
		/// <summary>
		/// 找到第一个非空字符<br/>
		/// 并且把值放置在 startIndex 当中
		/// </summary>
		/// <param name="foreachWCStr">遍历的字符串</param>
		/// <param name="foreachMaxIndex">结束位置</param>
		/// <param name="startIndex">开始位置，如果成功时，该值返回非空坐标</param>
		/// <returns>成功返回 true</returns>
		static bool jumpSace( const HtmlChar *foreachWCStr, size_t foreachMaxIndex, size_t *startIndex );

		/// <summary>
		/// 获取指定字符的位置<br/>
		/// startIndex 存储最后的下标位置
		/// </summary>
		/// <param name="w_c_ptr">指向字符串检测的开始位置</param>
		/// <param name="maxIndex">查找最大的长度(startIndex >= maxIndex 返回)</param>
		/// <param name="find_w_c_char">查找的字符</param>
		/// <param name="startIndex">字符串开始检查的下标，存储最后的下标位置</param>
		/// <returns>成功返回 true</returns>
		static bool findNextHtmlCharPotion( const HtmlChar *w_c_ptr, size_t maxIndex, const HtmlChar find_w_c_char, size_t &startIndex );
		/// <summary>
		/// 返回长度，忽略 \0
		/// </summary>
		/// <param name="find_org_html_string">检测长度的字符串</param>
		/// <returns>长度</returns>
		static size_t HtmlStringLen( const HtmlChar *find_org_html_string ) {
			size_t strLen = 0;
			if( find_org_html_string )
				while( *( find_org_html_string + strLen ) != cylHtmlTools::charValue::zero )
					++strLen;
			return strLen;
		}
		/// <summary>
		/// 返回长度，忽略 \0
		/// </summary>
		/// <param name="find_org_html_string">检测长度的字符串</param>
		/// <returns>长度</returns>
		static size_t HtmlStringLen( const HtmlString *find_org_html_string ) {
			size_t strLen = 0;
			if( find_org_html_string )
				for( auto cStr = find_org_html_string->c_str( ); cStr[ strLen ] != cylHtmlTools::charValue::zero; ++strLen );
			return strLen;
		}
		/// <summary>
		/// 返回长度，忽略 \0
		/// </summary>
		/// <param name="find_org_html_string">检测长度的字符串</param>
		/// <returns>长度</returns>
		static size_t HtmlStringLen( const HtmlString &find_org_html_string ) {
			return HtmlStringLen( &find_org_html_string );
		}

		/// <summary>
		/// 查找匹配的字符串
		/// 如果 maxIndex 等于 0， 则使用 find_org_html_string 的长度 的值
		/// </summary>
		/// <param name="find_org_html_string">源字符串</param>
		/// <param name="find_target_html_string">查找的目标字符串</param>
		/// <param name="result">相对位置，如果为 nullptr，则不会辅助</param>
		/// <returns>成功返回 true</returns>
		static bool findNextHtmlStringPotion( const HtmlString &find_org_html_string, const HtmlString &find_target_html_string, size_t *result = nullptr ) {
			return findNextHtmlStringPotion( &find_org_html_string, &find_target_html_string, result );
		}
		/// <summary>
		/// 查找匹配的字符串
		/// 如果 maxIndex 等于 0， 则使用 find_org_html_string 的长度 的值
		/// </summary>
		/// <param name="find_org_html_string">源字符串</param>
		/// <param name="find_target_html_string">查找的目标字符串</param>
		/// <param name="result">相对位置，如果为 nullptr，则不会辅助</param>
		/// <returns>成功返回 true</returns>
		static bool findNextHtmlStringPotion( const HtmlString *find_org_html_string, const HtmlString *find_target_html_string, size_t *result = nullptr ) {
			size_t leftLen = HtmlStringLen( find_org_html_string );
			size_t rightLen = HtmlStringLen( find_target_html_string );
			return findNextHtmlStringPotion( find_org_html_string->c_str( ), leftLen, 0, find_target_html_string->c_str( ), rightLen, 0, result );
			//if( result ) {
			//	*result = find_org_html_string->find( *find_target_html_string );
			//	return *result != HtmlString::npos;
			//} else
			//	return find_org_html_string->find( *find_target_html_string ) != HtmlString::npos;
		}
		/// <summary>
		/// 查找匹配的字符串
		/// 如果 maxIndex 等于 0， 则使用 find_org_html_string 的长度 的值
		/// </summary>
		/// <param name="find_org_html_string">源字符串</param>
		/// <param name="find_target_html_string">查找的目标字符串</param>
		/// <param name="maxIndex">查找最大的长度(startIndex >= maxIndex 返回)</param>
		/// <param name="result">相对位置，如果为 nullptr，则不会辅助</param>
		/// <returns>成功返回 true</returns>
		static bool findNextHtmlStringPotion( const HtmlString *find_org_html_string, const HtmlString *find_target_html_string, size_t maxIndex, size_t *result = nullptr ) {
			if( result )
				*result = find_org_html_string->find( find_target_html_string->c_str( ) + maxIndex );
			else
				return find_org_html_string->find( find_target_html_string->c_str( ) + maxIndex ) != HtmlString::npos;
			return *result != HtmlString::npos;
		}

		/// <summary>
		/// 查找匹配的字符串
		/// 如果 maxIndex 等于 0， 则使用 find_org_html_string 的长度 的值
		/// </summary>
		/// <param name="find_org_html_string">源字符串</param>
		/// <param name="startIndex">开始下标</param>
		/// <param name="find_target_html_string">查找的目标字符串</param>
		/// <param name="maxIndex">查找最大的长度(startIndex >= maxIndex 返回)</param>
		/// <param name="result">相对位置，如果为 nullptr，则不会辅助</param>
		/// <returns>成功返回 true</returns>
		static bool findNextHtmlStringPotion( const HtmlString *find_org_html_string, size_t startIndex, const HtmlString *find_target_html_string, size_t maxIndex = 0, size_t *result = nullptr ) {
			return findNextHtmlStringPotion( find_org_html_string->c_str( ), find_org_html_string->length( ), startIndex, find_target_html_string->c_str( ), find_target_html_string->length( ), maxIndex, result );
		}

		/// <summary>
		/// 查找匹配的字符串
		/// 如果 maxIndex 等于 0， 则使用 find_org_html_string 的长度 的值
		/// </summary>
		/// <param name="find_org_html_string">源字符串</param>
		/// <param name="startIndex">开始下标</param>
		/// <param name="find_w_c_string">查找的目标字符串</param>
		/// <param name="find_w_c_str_len">查找的目标字符串的长度</param>
		/// <param name="maxIndex">查找最大的长度(startIndex >= maxIndex 返回)</param>
		/// <param name="result">相对位置，如果为 nullptr，则不会辅助</param>
		/// <returns>成功返回 true</returns>
		static bool findNextHtmlStringPotion( const HtmlString *find_org_html_string, size_t startIndex, const HtmlChar *find_w_c_string, size_t find_w_c_str_len, size_t maxIndex = 0, size_t *result = nullptr ) {
			return findNextHtmlStringPotion( find_org_html_string->c_str( ), find_org_html_string->length( ), startIndex, find_w_c_string, find_w_c_str_len, maxIndex, result );
		}
		/// <summary>
		/// 查找匹配的字符串
		/// 如果 maxIndex 等于 0， 则使用 src_w_c_str_len 的值
		/// </summary>
		/// <param name="w_c_ptr">源字符串</param>
		/// <param name="src_w_c_str_len">源字符串的长度</param>
		/// <param name="startIndex">开始下标</param>
		/// <param name="find_w_c_string">查找的目标字符串</param>
		/// <param name="find_w_c_str_len">查找的目标字符串的长度</param>
		/// <param name="maxIndex">查找最大的长度(startIndex >= maxIndex 返回)</param>
		/// <param name="result">相对位置，如果为 nullptr，则不会辅助</param>
		/// <returns>成功返回 true</returns>
		static bool findNextHtmlStringPotion( const HtmlChar *w_c_ptr, size_t src_w_c_str_len, size_t startIndex, const HtmlChar *find_w_c_string, size_t find_w_c_str_len, size_t maxIndex = 0, size_t *result = nullptr );

		/// <summary>
		/// 比较两个字符串是否相等
		/// </summary>
		/// <param name="left">左操作符</param>
		/// <param name="right">有操作符</param>
		/// <returns>相等返回 true</returns>
		inline static bool equHtmlString( const HtmlString_Shared &left, const HtmlString_Shared &right ) {
			return equHtmlString( *left, *right );
		}
		/// <summary>
		/// 比较两个字符串是否相等
		/// </summary>
		/// <param name="left">左操作符</param>
		/// <param name="right">有操作符</param>
		/// <returns>相等返回 true</returns>
		static bool equHtmlString( const HtmlString &left, const HtmlString &right );
		/// <summary>
		/// 删除左侧空白字符
		/// </summary>
		/// <param name="str">操作字符串</param>
		static HtmlString & removeLeftSpace( HtmlString &str );
		/// <summary>
		/// 删除全部空白字符
		/// </summary>
		/// <param name="str">操作字符串</param>
		static HtmlString & removeAllSpace( HtmlString &str );
		/// <summary>
		/// 删除全部空白字符
		/// </summary>
		/// <param name="str">操作字符串</param>
		static std::string & removeAllSpace( std::string &str );
		/// <summary>
		/// 删除右侧空白字符
		/// </summary>
		/// <param name="str">操作字符串</param>
		static HtmlString & removeRightSpace( HtmlString &str );
		/// <summary>
		/// 删除两侧空白字符
		/// </summary>
		/// <param name="str">操作字符串</param>
		static HtmlString & removeBothSpace( HtmlString &str );

		enum RemoveSpaceStatus {
			none = 0x00, left = 0x01, right = 0x02, both = 0x03
		};

		/// <summary>
		/// 删除空白字符后比较两个字符串是否相等
		/// </summary>
		/// <param name="leftStr">左操作符</param>
		/// <param name="rightStr">有操作符</param>
		/// <param name="removeSpaceStatus">删除空白字符的状态标识</param>
		/// <returns>相等返回 true</returns>
		inline static bool equRemoveSpaceOverHtmlString( HtmlString *leftStr, HtmlString *rightStr, RemoveSpaceStatus removeSpaceStatus = both ) {
			if( leftStr == nullptr && rightStr != nullptr )
				return false;
			if( leftStr != nullptr && rightStr == nullptr )
				return false;
			return equRemoveSpaceOverHtmlString( *leftStr, *rightStr, removeSpaceStatus );
		}
		/// <summary>
		/// 删除空白字符后比较两个字符串是否相等
		/// </summary>
		/// <param name="leftStr">左操作符</param>
		/// <param name="rightStr">有操作符</param>
		/// <param name="removeSpaceStatus">删除空白字符的状态标识</param>
		/// <returns>相等返回 true</returns>
		inline static bool equRemoveSpaceOverHtmlString( HtmlString_Shared &leftStr, HtmlString_Shared &rightStr, RemoveSpaceStatus removeSpaceStatus = both ) {
			return equRemoveSpaceOverHtmlString( *leftStr, *rightStr, removeSpaceStatus );
		}
		/// <summary>
		/// 删除空白字符后比较两个字符串是否相等
		/// </summary>
		/// <param name="leftStr">左操作符</param>
		/// <param name="rightStr">有操作符</param>
		/// <param name="removeSpaceStatus">删除空白字符的状态标识</param>
		/// <returns>相等返回 true</returns>
		static bool equRemoveSpaceOverHtmlString( HtmlString leftStr, HtmlString rightStr, RemoveSpaceStatus removeSpaceStatus = both );
		/// <summary>
		/// 根据字符切分字符串
		/// </summary>
		/// <param name="src_html_string">被切分的字符串</param>
		/// <param name="split_char">切分字符</param>
		/// <returns>切分完成后的字符串</returns>
		static std::vector< HtmlString > split( const HtmlString &src_html_string, const HtmlChar split_char );
		/// <summary>
		/// 调整关键字，存在子字符串则不需要源字符串(a -> aa / 其中只保留 a，不保留 aa)
		/// </summary>
		/// <param name="str_vector">匹配的字符串</param>
		/// <returns>完成匹配的字符串</returns>
		static std::vector< HtmlString > vectorStrAdjustSubStr( std::vector< HtmlString > &str_vector );
		/// <summary>
		/// 从末端开始查找
		/// </summary>
		/// <param name="find_string">查找的源对象</param>
		/// <param name="find_target_char">查找的目标</param>
		/// <param name="index">存储下标</param>
		/// <returns>成功返回 true</returns>
		static bool lastFindChar( const HtmlString &find_string, const HtmlString::value_type &find_target_char, size_t *index );
		/// <summary>
		/// 截取字符串
		/// </summary>
		/// <param name="mid_string">被截取的字符串</param>
		/// <param name="start_index">开始截取下标</param>
		/// <param name="n_len">截取长度</param>
		/// <returns>截取完成的字符串</returns>
		static HtmlString mid( const HtmlString &mid_string, const size_t start_index, const size_t n_len );
		/// <summary>
		/// 截取字符串
		/// </summary>
		/// <param name="mid_string">被截取的字符串</param>
		/// <param name="start_index">开始截取下标</param>
		/// <returns>截取完成的字符串</returns>
		static HtmlString mid( const HtmlString &mid_string, const size_t start_index );
	};

}

#endif // WSTRTOOLS_H_H_HEAD__FILE__
