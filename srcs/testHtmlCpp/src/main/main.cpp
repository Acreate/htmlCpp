#include <algorithm>
#include <iostream>
#include <locale>
#include "testThread.h"
#include "testHtml.h"
#include "www.yingsx.com.h"
int main( int argc, char *argv[ ] ) {
	std::locale locale( "zh_CN.UTF8" );
	std::locale::global( locale );
	std::wcout.imbue( locale );
	std::cout.imbue( locale );

	//testThread( locale );
	//testHtml( locale );
	test_www_yingsx_com( locale );
	return 0;

}
