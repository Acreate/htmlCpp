#include "XDir.h"
using namespace htmlTools;
XDir::XDir( ) {

}

static bool insertOrAssign( HtmlStringMapToVectorHtmlStrSPtr &mapObj, HtmlString &key, HtmlString &value ) {
	auto iterator = mapObj.begin( );
	auto end = mapObj.end( );
	for( ; iterator != end; ++iterator )
		if( iterator->first == key ) {
			iterator->second->emplace_back( std::make_shared< HtmlString >( value ) );
			return true;
		}
	auto list( std::make_shared< Vector_HtmlStringSPtr >( ) );
	list->emplace_back( std::make_shared< HtmlString >( value ) );
	mapObj.insert( std::make_pair( key, list ) );
	return true;
}
static bool tryGet( HtmlStringMapToVectorHtmlStrSPtr &mapObj, HtmlString &key, Vector_HtmlStringSPtr &value ) {
	auto iterator = mapObj.begin( );
	auto end = mapObj.end( );
	for( ; iterator != end; ++iterator )
		if( iterator->first == key ) {
			auto sharedPtrs = iterator->second;
			for( auto &ptr : *sharedPtrs )
				value.emplace_back( std::make_shared< HtmlString >( *ptr ) );
			return true;
		}
	return false;
}

XDir::XDir( const htmlTools::HtmlString &param ) : XDir( ) {
	size_t length = param.length( );
	if( length == 0 )
		return;
	htmlTools::HtmlChar value;
	std::vector< htmlTools::HtmlChar > buff;
	for( size_t index = 0; index < length; ++index ) {
		value = param[ index ];
		if( value == L'[' ) { // 找到 ]
			HtmlString name( buff.data( ), buff.size( ) );
			namesList.emplace_back( name );
			buff.clear( );
			++index;
			for( ; index < length; ++index ) {
				value = param[ index ];
				if( value == L']' ) { // 找到 ]
					if( buff.size( ) > 0 ) {
						HtmlString mapValue( buff.data( ), buff.size( ) );
						insertOrAssign( attributesMapList, name, mapValue );
						buff.clear( );
					}
					break;
				}
				buff.emplace_back( value );
			}
			continue;
		}
		buff.emplace_back( value );
	}
	if( buff.size( ) > 0 ) {
		HtmlString name( buff.data( ), buff.size( ) );
		namesList.emplace_back( name );
	}
}
XDir::~XDir( ) {
}
bool XDir::hasName( const HtmlString &dir_name ) {
	for( auto &name : namesList )
		if( dir_name == name )
			return true;
	return false;
}
bool XDir::hasAttribute( const HtmlString &attribute ) {
	return false;
}
HtmlString XDir::getDirName( ) {
	HtmlString result;
	for( auto &name : namesList ) {
		result.append( name );
		Vector_HtmlStringSPtr outHtmlStringShared;
		if( tryGet( attributesMapList, name, outHtmlStringShared ) ) { // 存在
			result.append( L"[" );
			for( auto &attribute : outHtmlStringShared )
				result.append( *attribute );
			result.append( L"]" );
		}
	}
	return result;
}
