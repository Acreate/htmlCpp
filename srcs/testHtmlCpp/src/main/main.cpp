#include <algorithm>
#include <iostream>
#include <locale>
#include "testHtml.h"
#include <htmls/htmlTools/HtmlWorkThread/HtmlWorkThreadPool.h>
int main( int argc, char *argv[ ] ) {
	std::locale locale( "zh_CN.UTF8" );
	std::locale::global( locale );
	std::wcout.imbue( locale );
	std::cout.imbue( locale );
	//testHtml( locale );

	cylHtmlTools::HtmlWorkThreadPool threadPool;
	for( int index = 0; index < 100; ++index )
		threadPool.appendWork( [index]( cylHtmlTools::HtmlWorkThread * ) {
			for( int subIndex = 0; subIndex < 1000; ++subIndex )
				std::cout << index << '\t';
			std::cout << index << '\t' << std::endl;
		} );
	threadPool.start( 8
		, []( cylHtmlTools::HtmlWorkThreadPool *, const size_t &, const size_t & ) {
			std::this_thread::sleep_for( std::chrono::seconds( 2 ) );
		} );
	threadPool.waiteOverJob( );
	return 0;

}
