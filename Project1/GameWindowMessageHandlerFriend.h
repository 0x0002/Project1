// forward declaration
struct HWND__;
typedef HWND__*      Hwnd;

#ifdef _X86
typedef unsigned int WParam;
typedef long         LParam;
typedef long         LResult;
#endif

#ifdef _X64
typedef uint64_t     WParam;
typedef int64_t      LParam;
typedef int64_t      LResult;
#endif

// use this macro to define message handler friend functions
#define FRIEND_HANDLER( message ) \
    friend LResult message##_HANDLER( Hwnd, WParam, LParam )