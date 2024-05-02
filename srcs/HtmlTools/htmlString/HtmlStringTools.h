﻿#ifndef WSTRTOOLS_H_H_HEAD__FILE__
#define WSTRTOOLS_H_H_HEAD__FILE__
#pragma once

#include <locale>

#include "../nameSpace/HtmlTools.h"
namespace cylHtmlTools {
	class HTMLTOOLS_EXPORT HtmlStringTools {
	public:
		/// <summary>
		/// 是否为一个跳过的空字符 <br/>
		/// 其中包含控制字符等
		/// </summary>
		/// <param name="currentChar">检测的字符</param>
		/// <returns>true 表示空字符</returns>
		static bool isJumpSpace( HtmlChar currentChar );
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
		static bool jimpSace( const HtmlChar *foreachWCStr, size_t foreachMaxIndex, size_t *startIndex );
		/// <summary>
		/// 获取下个反斜杠的相对位置<br/>
		/// startIndex 存储最后的下标位置
		/// </summary>
		/// <param name="w_c_ptr">指向字符串检测的开始位置</param>
		/// <param name="maxIndex">查找最大的长度(startIndex >= maxIndex 返回)</param>
		/// <param name="find_w_c_char">查找的字符</param>
		/// <param name="startIndex">字符串开始检查的下标，存储最后的下标位置</param>
		/// <returns>成功返回 true</returns>
		static bool findNextWCharPotion( const HtmlChar *w_c_ptr, size_t maxIndex, const HtmlChar find_w_c_char, size_t *startIndex );
		/// <summary>
		/// 查找匹配的字符串
		/// </summary>
		/// <param name="w_c_ptr">源字符串</param>
		/// <param name="src_w_c_str_len">源字符串的长度</param>
		/// <param name="startIndex">开始下标</param>
		/// <param name="find_w_c_string">查找的目标字符串</param>
		/// <param name="find_w_c_str_len">查找的目标字符串的长度</param>
		/// <param name="maxIndex">查找最大的长度(startIndex >= maxIndex 返回)</param>
		/// <param name="result">相对位置</param>
		/// <returns>成功返回 true</returns>
		static bool findNextWStringPotion( const HtmlChar *w_c_ptr, size_t src_w_c_str_len, size_t startIndex, const HtmlChar *find_w_c_string, size_t find_w_c_str_len, size_t maxIndex, size_t *result );

		/// <summary>
		/// 比较两个字符串是否相等
		/// </summary>
		/// <param name="left">左操作符</param>
		/// <param name="right">有操作符</param>
		/// <returns>相等返回 true</returns>
		static bool equHtmlString( HtmlString &left, HtmlString &right );
		/// <summary>
		/// 删除左侧空白字符
		/// </summary>
		/// <param name="str">操作字符串</param>
		static void removeLeftSpace( HtmlString &str );
		/// <summary>
		/// 删除右侧空白字符
		/// </summary>
		/// <param name="str">操作字符串</param>
		static void removeRightSpace( HtmlString &str );
		/// <summary>
		/// 删除两侧空白字符
		/// </summary>
		/// <param name="str">操作字符串</param>
		static void removeBothSpace( HtmlString &str );

		enum RemoveSpaceStatus {
			none = 0x00
			, left = 0x01
			, right = 0x02
			, both = 0x03
		};

		/// <summary>
		/// 删除空白字符后比较两个字符串是否相等
		/// </summary>
		/// <param name="left">左操作符</param>
		/// <param name="rightStr">有操作符</param>
		/// <param name="removeSpaceStatus">删除空白字符的状态标识</param>
		/// <returns>相等返回 true</returns>
		static bool equRemoveSpaceOverHtmlString( HtmlString left, HtmlString rightStr, RemoveSpaceStatus removeSpaceStatus = both );
	};

}

#endif // WSTRTOOLS_H_H_HEAD__FILE__
