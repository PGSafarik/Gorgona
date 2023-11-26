#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	//Date Version Types
	static const char DATE[]                 = "21";
	static const char MONTH[]                = "11";
	static const char YEAR[]                 = "2023";
	static const char UBUNTU_VERSION_STYLE[] = "11.23";
	
	//Software Status
	static const char STATUS[]     = "Devel"; // Alpha, Beta, Devel, Testing, Stable
	static const char STATUS_SHORT = 'D';     // A, B, D, T, S
	
	//Standard Version Type
	static const long MAJOR    = 1;
	static const long MINOR    = 6;
	static const long BUILD    = 5;
	static const long REVISION = 1;
	
	//Miscellaneous Version Types
	#define RC_FILEVERSION 1,6,5,1
	#define RC_FILEVERSION_STRING "1, 6, 5, 1\0"
	static const char FULLVERSION_STRING [] = "1.6.5.1";
	
	static const long BUILDS_COUNT  = 1;
	static const long BUILD_HISTORY = 0;
}
#endif //VERSION_H
