// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include <stdlib.h>
#ifdef _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif
#include <stdio.h>
#include <string.h>

#include "umockcallrecorder.h"
#include "umockcall.h"
#include "umocktypes.h"
#include "umock_log.h"

typedef struct UMOCK_EXPECTED_CALL_TAG
{
    UMOCKCALL_HANDLE umockcall;
    unsigned int is_matched : 1;
} UMOCK_EXPECTED_CALL;

typedef struct UMOCKCALLRECORDER_TAG
{
    size_t expected_call_count;
    UMOCK_EXPECTED_CALL* expected_calls;
    size_t actual_call_count;
    UMOCKCALL_HANDLE* actual_calls;
    char* expected_calls_string;
    char* actual_calls_string;
} UMOCKCALLRECORDER;

UMOCKCALLRECORDER_HANDLE umockcallrecorder_create(void)
{
    UMOCKCALLRECORDER_HANDLE result;

    /* Codes_SRS_UMOCKCALLRECORDER_01_001: [ umockcallrecorder_create shall create a new instance of a call recorder and return a non-NULL handle to it on success. ]*/
    result = (UMOCKCALLRECORDER_HANDLE)malloc(sizeof(UMOCKCALLRECORDER));
    /* Codes_SRS_UMOCKCALLRECORDER_01_002: [ If allocating memory for the call recorder fails, umockcallrecorder_create shall return NULL. ]*/
    if (result != NULL)
    {
        result->expected_call_count = 0;
        result->expected_calls = NULL;
        result->expected_calls_string = NULL;
        result->actual_call_count = 0;
        result->actual_calls = NULL;
        result->actual_calls_string = NULL;
    }

    return result;
}

void umockcallrecorder_destroy(UMOCKCALLRECORDER_HANDLE umock_call_recorder)
{
    /* Codes_SRS_UMOCKCALLRECORDER_01_004: [ If umock_call_recorder is NULL, umockcallrecorder_destroy shall do nothing. ]*/
    if (umock_call_recorder != NULL)
    {
        /* Codes_SRS_UMOCKCALLRECORDER_01_003: [ umockcallrecorder_destroy shall free the resources associated with a the call recorder identified by the umock_call_recorder argument. ]*/
        (void)umockcallrecorder_reset_all_calls(umock_call_recorder);
        free(umock_call_recorder->actual_calls_string);
        free(umock_call_recorder->expected_calls_string);
        free(umock_call_recorder);
    }
}

int umockcallrecorder_reset_all_calls(UMOCKCALLRECORDER_HANDLE umock_call_recorder)
{
    int result;

    if (umock_call_recorder == NULL)
    {
        /* Codes_SRS_UMOCKCALLRECORDER_01_007: [ If umock_call_recorder is NULL, umockcallrecorder_reset_all_calls shall fail and return a non-zero value. ]*/
        UMOCK_LOG("umockcallrecorder: Reset all calls failed: NULL umock_call_recorder.");
        result = __LINE__;
    }
    else
    {
        /* Codes_SRS_UMOCKCALLRECORDER_01_005: [ umockcallrecorder_reset_all_calls shall free all the expected and actual calls for the call recorder identified by umock_call_recorder. ]*/
        if (umock_call_recorder->expected_calls != NULL)
        {
            size_t i;
            for (i = 0; i < umock_call_recorder->expected_call_count; i++)
            {
                umockcall_destroy(umock_call_recorder->expected_calls[i].umockcall);
            }

            free(umock_call_recorder->expected_calls);
            umock_call_recorder->expected_calls = NULL;
        }
        umock_call_recorder->expected_call_count = 0;

        if (umock_call_recorder->actual_calls != NULL)
        {
            size_t i;
            for (i = 0; i < umock_call_recorder->actual_call_count; i++)
            {
                umockcall_destroy(umock_call_recorder->actual_calls[i]);
            }

            free(umock_call_recorder->actual_calls);
            umock_call_recorder->actual_calls = NULL;
        }
        umock_call_recorder->actual_call_count = 0;

        /* Codes_SRS_UMOCKCALLRECORDER_01_006: [ On success umockcallrecorder_reset_all_calls shall return 0. ]*/
        result = 0;
    }

    return result;
}

int umockcallrecorder_add_expected_call(UMOCKCALLRECORDER_HANDLE umock_call_recorder, UMOCKCALL_HANDLE mock_call)
{
    int result;

    if ((umock_call_recorder == NULL) ||
        (mock_call == NULL))
    {
        /* Codes_SRS_UMOCKCALLRECORDER_01_012: [ If any of the arguments is NULL, umockcallrecorder_add_expected_call shall fail and return a non-zero value. ]*/
        UMOCK_LOG("umockcallrecorder: Bad arguments in add expected call: umock_call_recorder = %p, mock_call = %p.",
            umock_call_recorder, mock_call);
        result = __LINE__;
    }
    else
    {
        UMOCK_EXPECTED_CALL* new_expected_calls = (UMOCK_EXPECTED_CALL*)realloc(umock_call_recorder->expected_calls, sizeof(UMOCK_EXPECTED_CALL) * (umock_call_recorder->expected_call_count + 1));
        if (new_expected_calls == NULL)
        {
            /* Codes_SRS_UMOCKCALLRECORDER_01_013: [ If allocating memory for the expected calls fails, umockcallrecorder_add_expected_call shall fail and return a non-zero value. ] */
            UMOCK_LOG("umockcallrecorder: Cannot allocate memory in add expected call.");
            result = __LINE__;
        }
        else
        {
            /* Codes_SRS_UMOCKCALLRECORDER_01_008: [ umockcallrecorder_add_expected_call shall add the mock_call call to the expected call list maintained by the call recorder identified by umock_call_recorder. ]*/
            umock_call_recorder->expected_calls = new_expected_calls;
            umock_call_recorder->expected_calls[umock_call_recorder->expected_call_count].umockcall = mock_call;
            umock_call_recorder->expected_calls[umock_call_recorder->expected_call_count++].is_matched = 0;

            /* Codes_SRS_UMOCKCALLRECORDER_01_009: [ On success umockcallrecorder_add_expected_call shall return 0. ]*/
            result = 0;
        }
    }

    return result;
}

int umockcallrecorder_add_actual_call(UMOCKCALLRECORDER_HANDLE umock_call_recorder, UMOCKCALL_HANDLE mock_call, UMOCKCALL_HANDLE* matched_call)
{
    int result;

    if ((umock_call_recorder == NULL) ||
        (mock_call == NULL) ||
        (matched_call == NULL))
    {
        /* Codes_SRS_UMOCKCALLRECORDER_01_019: [ If any of the arguments is NULL, umockcallrecorder_add_actual_call shall fail and return a non-zero value. ]*/
        UMOCK_LOG("umockcallrecorder: Bad arguments in add actual call: umock_call_recorder = %p, mock_call = %p, matched_call = %p.",
            umock_call_recorder, mock_call, matched_call);
        result = __LINE__;
    }
    else
    {
        size_t i;
        unsigned int is_error = 0;

        *matched_call = NULL;

        /* Codes_SRS_UMOCK_C_LIB_01_115: [ umock_c shall compare calls in order. ]*/
        for (i = 0; i < umock_call_recorder->expected_call_count; i++)
        {
            if (umock_call_recorder->expected_calls[i].is_matched == 0)
            {
                /* Codes_SRS_UMOCKCALLRECORDER_01_017: [ Comparing the calls shall be done by calling umockcall_are_equal. ]*/
                int are_equal_result = umockcall_are_equal(umock_call_recorder->expected_calls[i].umockcall, mock_call);
                if (are_equal_result == 1)
                {
                    /* Codes_SRS_UMOCKCALLRECORDER_01_016: [ If the call matches one of the expected calls, a handle to the matched call shall be filled into the matched_call argument. ]*/
                    *matched_call = umock_call_recorder->expected_calls[i].umockcall;
                    umock_call_recorder->expected_calls[i].is_matched = 1;
                    break;
                }
                /* Codes_SRS_UMOCKCALLRECORDER_01_021: [ If umockcall_are_equal fails, umockcallrecorder_add_actual_call shall fail and return a non-zero value. ]*/
                else if (are_equal_result != 0)
                {
                    is_error = 1;
                    break;
                }
                else
                {
                    i = umock_call_recorder->expected_call_count;
                    break;
                }
            }
        }

        if (is_error)
        {
            UMOCK_LOG("umockcallrecorder: Error in finding a matched call.");
            result = __LINE__;
        }
        else
        {
            if (i == umock_call_recorder->expected_call_count)
            {
                /* an unexpected call */
                /* Codes_SRS_UMOCKCALLRECORDER_01_014: [ umockcallrecorder_add_actual_call shall check whether the call mock_call matches any of the expected calls maintained by umock_call_recorder. ]*/
                UMOCKCALL_HANDLE* new_actual_calls = (UMOCKCALL_HANDLE*)realloc(umock_call_recorder->actual_calls, sizeof(UMOCKCALL_HANDLE) * (umock_call_recorder->actual_call_count + 1));
                if (new_actual_calls == NULL)
                {
                    UMOCK_LOG("umockcallrecorder: Cannot allocate memory for actual calls.");
                    result = __LINE__;
                }
                else
                {
                    umock_call_recorder->actual_calls = new_actual_calls;
                    umock_call_recorder->actual_calls[umock_call_recorder->actual_call_count++] = mock_call;

                    /* Codes_SRS_UMOCKCALLRECORDER_01_018: [ When no error is encountered, umockcallrecorder_add_actual_call shall return 0. ]*/
                    result = 0;
                }
            }
            else
            {
                umockcall_destroy(mock_call);

                /* Codes_SRS_UMOCKCALLRECORDER_01_018: [ When no error is encountered, umockcallrecorder_add_actual_call shall return 0. ]*/
                result = 0;
            }
        }
    }

    return result;
}

const char* umockcallrecorder_get_expected_calls(UMOCKCALLRECORDER_HANDLE umock_call_recorder)
{
    const char* result;

    if (umock_call_recorder == NULL)
    {
        /* Codes_SRS_UMOCKCALLRECORDER_01_029: [ If the umock_call_recorder is NULL, umockcallrecorder_get_expected_calls shall fail and return NULL. ]*/
        UMOCK_LOG("umockcallrecorder: NULL umock_call_recorder in get expected calls.");
        result = NULL;
    }
    else
    {
        size_t i;
        char* new_expected_calls_string;
        size_t current_length = 0;

        for (i = 0; i < umock_call_recorder->expected_call_count; i++)
        {
            if (umock_call_recorder->expected_calls[i].is_matched == 0)
            {
                /* Codes_SRS_UMOCKCALLRECORDER_01_028: [ The string for each call shall be obtained by calling umockcall_stringify. ]*/
                char* stringified_call = umockcall_stringify(umock_call_recorder->expected_calls[i].umockcall);
                if (stringified_call == NULL)
                {
                    /* Codes_SRS_UMOCKCALLRECORDER_01_030: [ If umockcall_stringify fails, umockcallrecorder_get_expected_calls shall fail and return NULL. ]*/
                    break;
                }
                else
                {
                    size_t stringified_call_length = strlen(stringified_call);
                    new_expected_calls_string = (char*)realloc(umock_call_recorder->expected_calls_string, current_length + stringified_call_length + 1);
                    if (new_expected_calls_string == NULL)
                    {
                        free(stringified_call);

                        /* Codes_SRS_UMOCKCALLRECORDER_01_031: [ If allocating memory for the resulting string fails, umockcallrecorder_get_expected_calls shall fail and return NULL. ]*/
                        break;
                    }
                    else
                    {
                        umock_call_recorder->expected_calls_string = new_expected_calls_string;
                        (void)memcpy(umock_call_recorder->expected_calls_string + current_length, stringified_call, stringified_call_length + 1);
                        current_length += stringified_call_length;
                    }

                    free(stringified_call);
                }
            }
        }

        if (i < umock_call_recorder->expected_call_count)
        {
            result = NULL;
        }
        else
        {
            if (current_length == 0)
            {
                new_expected_calls_string = (char*)realloc(umock_call_recorder->expected_calls_string, 1);
                if (new_expected_calls_string == NULL)
                {
                    /* Codes_SRS_UMOCKCALLRECORDER_01_031: [ If allocating memory for the resulting string fails, umockcallrecorder_get_expected_calls shall fail and return NULL. ]*/
                    UMOCK_LOG("umockcallrecorder: Cannot allocate memory for expected calls.");
                    result = NULL;
                }
                else
                {
                    umock_call_recorder->expected_calls_string = new_expected_calls_string;
                    umock_call_recorder->expected_calls_string[0] = '\0';

                    /* Codes_SRS_UMOCKCALLRECORDER_01_027: [ umockcallrecorder_get_expected_calls shall return a pointer to the string representation of all the expected calls. ]*/
                    result = umock_call_recorder->expected_calls_string;
                }
            }
            else
            {
                /* Codes_SRS_UMOCKCALLRECORDER_01_027: [ umockcallrecorder_get_expected_calls shall return a pointer to the string representation of all the expected calls. ]*/
                result = umock_call_recorder->expected_calls_string;
            }
        }
    }

    return result;
}

const char* umockcallrecorder_get_actual_calls(UMOCKCALLRECORDER_HANDLE umock_call_recorder)
{
    const char* result;

    if (umock_call_recorder == NULL)
    {
        /* Codes_SRS_UMOCKCALLRECORDER_01_024: [ If the umock_call_recorder is NULL, umockcallrecorder_get_actual_calls shall fail and return NULL. ]*/
        UMOCK_LOG("umockcallrecorder: NULL umock_call_recorder in get actual calls.");
        result = NULL;
    }
    else
    {
        size_t i;
        char* new_actual_calls_string;

        if (umock_call_recorder->actual_call_count == 0)
        {
            new_actual_calls_string = (char*)realloc(umock_call_recorder->actual_calls_string, 1);
            if (new_actual_calls_string == NULL)
            {
                /* Codes_SRS_UMOCKCALLRECORDER_01_026: [ If allocating memory for the resulting string fails, umockcallrecorder_get_actual_calls shall fail and return NULL. ]*/
                UMOCK_LOG("umockcallrecorder: Cannot allocate memory for actual calls.");
                result = NULL;
            }
            else
            {
                umock_call_recorder->actual_calls_string = new_actual_calls_string;
                umock_call_recorder->actual_calls_string[0] = '\0';

                /* Codes_SRS_UMOCKCALLRECORDER_01_022: [ umockcallrecorder_get_actual_calls shall return a pointer to the string representation of all the actual calls. ]*/
                result = umock_call_recorder->actual_calls_string;
            }
        }
        else
        {
            size_t current_length = 0;

            for (i = 0; i < umock_call_recorder->actual_call_count; i++)
            {
                /* Codes_SRS_UMOCKCALLRECORDER_01_023: [ The string for each call shall be obtained by calling umockcall_stringify. ]*/
                char* stringified_call = umockcall_stringify(umock_call_recorder->actual_calls[i]);
                if (stringified_call == NULL)
                {
                    /* Codes_SRS_UMOCKCALLRECORDER_01_025: [ If umockcall_stringify fails, umockcallrecorder_get_actual_calls shall fail and return NULL. ]*/
                    break;
                }
                else
                {
                    size_t stringified_call_length = strlen(stringified_call);
                    new_actual_calls_string = (char*)realloc(umock_call_recorder->actual_calls_string, current_length + stringified_call_length + 1);
                    if (new_actual_calls_string == NULL)
                    {
                        free(stringified_call);

                        /* Codes_SRS_UMOCKCALLRECORDER_01_026: [ If allocating memory for the resulting string fails, umockcallrecorder_get_actual_calls shall fail and return NULL. ]*/
                        break;
                    }
                    else
                    {
                        umock_call_recorder->actual_calls_string = new_actual_calls_string;
                        (void)memcpy(umock_call_recorder->actual_calls_string + current_length, stringified_call, stringified_call_length + 1);
                        current_length += stringified_call_length;
                    }

                    free(stringified_call);
                }
            }

            if (i < umock_call_recorder->actual_call_count)
            {
                result = NULL;
            }
            else
            {
                /* Codes_SRS_UMOCKCALLRECORDER_01_022: [ umockcallrecorder_get_actual_calls shall return a pointer to the string representation of all the actual calls. ]*/
                result = umock_call_recorder->actual_calls_string;
            }
        }
    }

    return result;
}

UMOCKCALL_HANDLE umockcallrecorder_get_last_expected_call(UMOCKCALLRECORDER_HANDLE umock_call_recorder)
{
    UMOCKCALL_HANDLE result;

    if (umock_call_recorder == NULL)
    {
        /* Codes_SRS_UMOCKCALLRECORDER_01_033: [ If umock_call_recorder is NULL, umockcallrecorder_get_last_expected_call shall fail and return NULL. ]*/
        UMOCK_LOG("umockcallrecorder: NULL umock_call_recorder in get last expected calls.");
        result = NULL;
    }
    else
    {
        if (umock_call_recorder->expected_call_count == 0)
        {
            /* Codes_SRS_UMOCKCALLRECORDER_01_034: [ If no expected call has been recorded for umock_call_recorder then umockcallrecorder_get_last_expected_call shall fail and return NULL. ]*/
            UMOCK_LOG("umockcallrecorder: No expected calls recorded.");
            result = NULL;
        }
        else
        {
            /* Codes_SRS_UMOCKCALLRECORDER_01_032: [ umockcallrecorder_get_last_expected_call shall return the last expected call for the umock_call_recorder call recorder. ]*/
            result = umock_call_recorder->expected_calls[umock_call_recorder->expected_call_count - 1].umockcall;
        }
    }

    return result;
}
