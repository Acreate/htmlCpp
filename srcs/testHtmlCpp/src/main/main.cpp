#include <algorithm>
#include <iostream>
#include <locale>
#include "testThread.h"
#include "testHtml.h"
#include "pathTest.h"
int main( int argc, char *argv[ ] ) {
	std::locale locale( "zh_CN.UTF8" );
	std::locale::global( locale );
	std::wcout.imbue( locale );
	std::cout.imbue( locale );
	testThread( locale );
	testHtml( locale );
	return 0;

}
