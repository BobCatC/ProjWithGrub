#ifndef _STD_DEF_H
#define _STD_DEF_H

typedef DWORD X_RET_TYPE;

#define FATAL(iError)           ((X_RET_TYPE)iError | (1 << (sizeof(X_RET_TYPE) - 1)))
#define NOTFATAL(iError)         ((X_RET_TYPE)iError & (1 << (sizeof(X_RET_TYPE) - 1)))

#define X_NO_ERROR              NOTFATAL(0x0)



#define X_PROGRAM_NO_ERROR      X_NO_ERROR

#define IS_ERROR_FATAL(iError)  ((X_RET_TYPE)iError >> ((sizeof(X_RET_TYPE) << 3) - 1))


#endif
