#ifndef EFILE_MODE
#define EFILE_MODE

enum EFileMode
{
	EFM_UNDEFINED = 1<<0,
	EFM_READ = 1<<1, 
	EFM_WRITE = 1<<2,
	EFM_APPEND = 1<<3,
};

#endif
