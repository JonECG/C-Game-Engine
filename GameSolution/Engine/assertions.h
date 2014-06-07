#ifdef _DEBUG
#define ASSSERT( x, ... ) do { if (!(x)) { Logger::log( Severity::SEVERE, __FILE__, __LINE__, #__VA_ARGS__ ## " [ASSERTION FAIL: " ## #x ## "]" ); Logger::outputLog( "CRASHlog.html" ); exit(1); } } while(0,0)
#else
#define ASSSERT( x, ... ) do { (void)sizeof(x); } while(0,0) 
#endif