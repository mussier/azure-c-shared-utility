#umockcall requirements
 
#Overview

umockcall is a module that encapsulates a umock call.

#Exposed API

```c
    typedef struct UMOCKCALL_TAG* UMOCKCALL_HANDLE;
    typedef void(*UMOCKCALL_DATA_FREE_FUNC)(void* umockcall_data);
    typedef char*(*UMOCKCALL_DATA_STRINGIFY_FUNC)(void* umockcall_data);
    typedef int(*UMOCKCALL_DATA_ARE_EQUAL_FUNC)(void* left, void* right);

    extern UMOCKCALL_HANDLE umockcall_create(const char* function_name, void* umockcall_data, UMOCKCALL_DATA_FREE_FUNC umockcall_data_free, UMOCKCALL_DATA_STRINGIFY_FUNC umockcall_data_stringify, UMOCKCALL_DATA_ARE_EQUAL_FUNC umockcall_data_are_equal);
    extern void umockcall_destroy(UMOCKCALL_HANDLE umockcall);
    extern int umockcall_are_equal(UMOCKCALL_HANDLE left, UMOCKCALL_HANDLE right);
    extern char* umockcall_stringify(UMOCKCALL_HANDLE umockcall);
    extern void* umockcall_get_call_data(UMOCKCALL_HANDLE umockcall);
```

##umockcall_create

```c
extern UMOCKCALL_HANDLE umockcall_create(const char* function_name, void* umockcall_data, UMOCKCALL_DATA_FREE_FUNC umockcall_data_free, UMOCKCALL_DATA_STRINGIFY_FUNC umockcall_data_stringify, UMOCKCALL_DATA_ARE_EQUAL_FUNC umockcall_data_are_equal);
```

**SRS_UMOCKCALL_01_001: [** umockcall_create shall create a new instance of a umock call and on success it shall return a non-NULL handle to it. **]**
**SRS_UMOCKCALL_01_002: [** If allocating memory for the umock call instance fails, umockcall_create shall return NULL. **]**
**SRS_UMOCKCALL_01_003: [** If any of the arguments are NULL, umockcall_create shall fail and return NULL. **]**   

##umockcall_destroy

```c
extern void umockcall_destroy(UMOCKCALL_HANDLE umockcall);
```

**SRS_UMOCKCALL_01_004: [** umockcall_destroy shall free a previously allocated umock call instance. **]**
**SRS_UMOCKCALL_01_005: [** If the umockcall argument is NULL then umockcall_destroy shall do nothing. **]**   

##umockcall_are_equal

```c
extern int umockcall_are_equal(UMOCKCALL_HANDLE left, UMOCKCALL_HANDLE right);
```

**SRS_UMOCKCALL_01_006: [** umockcall_are_equal shall compare the two mock calls and return whether they are equal or not. **]**
**SRS_UMOCKCALL_01_024: [** If both left and right pointers are equal, umockcall_are_equal shall return 1. **]**
**SRS_UMOCKCALL_01_015: [** If only one of the left or right arguments are NULL, umockcall_are_equal shall return 0. **]** 
**SRS_UMOCKCALL_01_025: [** If the function name does not match for the 2 calls, umockcall_are_equal shall return 0. **]**
**SRS_UMOCKCALL_01_026: [** The call data shall be evaluated by calling the umockcall_data_are_equal function passed in umockcall_create while passing as arguments the umockcall_data associated with each call handle. **]**
**SRS_UMOCKCALL_01_027: [** If the underlying umockcall_data_are_equal returns 1, then umockcall_are_equal shall return 1. **]**
**SRS_UMOCKCALL_01_028: [** If the underlying umockcall_data_are_equal returns 0, then umockcall_are_equal shall return 0. **]**
**SRS_UMOCKCALL_01_029: [** If the underlying umockcall_data_are_equal fails (returns anything else than 0 or 1), then umockcall_are_equal shall fail and return -1. **]**
**SRS_UMOCKCALL_01_014: [** If the two calls have different are_equal functions that have been passed to umockcall_create then the calls shall be considered different and 0 shall be returned. **]**

##umockcall_to_string

```c
extern char* umockcall_stringify(UMOCKCALL_HANDLE umockcall);
```

**SRS_UMOCKCALL_01_016: [** umockcall_stringify shall return a string representation of the mock call in the form \[function_name(arguments)\]. **]**
**SRS_UMOCKCALL_01_018: [** The returned string shall be a newly allocated string and it is to be freed by the caller. **]**
**SRS_UMOCKCALL_01_017: [** If the umockcall argument is NULL, umockcall_stringify shall return NULL. **]**
**SRS_UMOCKCALL_01_019: [** To obtain the arguments string, umockcall_stringify shall call the umockcall_data_stringify function passed to umockcall_create and pass to it the umockcall_data pointer (also given in umockcall_create). **]** 
**SRS_UMOCKCALL_01_030: [** umockcall_stringify shall free the string obtained from umockcall_data_stringify. **]**
**SRS_UMOCKCALL_01_020: [** If the underlying umockcall_data_stringify call fails, umockcall_stringify shall fail and return NULL. **]**
**SRS_UMOCKCALL_01_021: [** If not enough memory can be allocated for the string to be returned, umockcall_stringify shall fail and return NULL. **]** 

##umockcall_get_call_data

```c
extern void* umockcall_get_call_data(UMOCKCALL_HANDLE umockcall);
```

XX**SRS_UMOCKCALL_01_022: [** umockcall_get_call_data shall return the associated umock call data that was passed to umockcall_create. **]**
XX**SRS_UMOCKCALL_01_023: [** If umockcall is NULL, umockcall_get_call_data shall return NULL. **]**
