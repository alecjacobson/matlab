/* Copyright 2017 The MathWorks, Inc. */

#ifndef CPP_ENGINE_API_HPP
#define CPP_ENGINE_API_HPP

#include "cpp_engine_api_util.hpp"
#include <MatlabDataArray/TypedArray.hpp>

/**
* Exported API to be loaded by Engine clients located outside of bin/<arch> folder.
*/
CPP_ENGINE_C_API void engine_create_session();
CPP_ENGINE_C_API uint64_t engine_create_out_of_process_matlab_async(char16_t** options, size_t size);
CPP_ENGINE_C_API uint64_t engine_create_out_of_process_matlab(char16_t** options, size_t size, bool* errFlag);

CPP_ENGINE_C_API uint64_t engine_get_out_of_process_matlab(const uint64_t matlabHandle, bool* errFlag);

CPP_ENGINE_C_API uint64_t engine_attach_shared_matlab_async(const char16_t* name);
CPP_ENGINE_C_API uint64_t engine_attach_shared_matlab(const char16_t* name, bool* errFlag);

CPP_ENGINE_C_API bool engine_is_done_matlab(const uint64_t matlabHandle);

CPP_ENGINE_C_API void engine_cancel_matlab_async(const uint64_t matlabHandle);

CPP_ENGINE_C_API bool engine_wait_matlab(const uint64_t matlab, const double seconds);

CPP_ENGINE_C_API void engine_wait_matlab_forever(const uint64_t matlab);

CPP_ENGINE_C_API void engine_destroy_matlab_handle(const uint64_t matlabHandle);

/**
* The vector passed in will be filled with names of shared MATLAB sessions.
*/
CPP_ENGINE_C_API size_t engine_find_shared_matlab(char16_t*** names);

CPP_ENGINE_C_API void engine_destroy_names(char16_t** names, size_t size);
CPP_ENGINE_C_API void engine_destroy_handles(uintptr_t* handles);


CPP_ENGINE_C_API void engine_write_stream_buffer(const uintptr_t outputHandle, void* buffer, void(*write)(void*, const char16_t*, size_t));
CPP_ENGINE_C_API uintptr_t engine_feval_with_completion(const uint64_t matlabHandle, const char* function, size_t nlhs, bool scalar, matlab::data::impl::ArrayImpl** args, size_t nrhs, void(*success)(void*, size_t, bool, matlab::data::impl::ArrayImpl**), void(*exception)(void*, size_t, bool, size_t, const void*), void* p, void* output, void* error, void(*write)(void*, const char16_t*, size_t), void(*deleter)(void*));
CPP_ENGINE_C_API void engine_eval_with_completion(const uint64_t matlabHandle, const char16_t* statement, void(*success)(void*), void(*exception)(void*, size_t, const void*), void* p, void* output, void* error, void(*write)(void*, const char16_t*, size_t), void(*deleter)(void*), uintptr_t** handles);

CPP_ENGINE_C_API bool engine_cancel_feval_with_completion(uintptr_t taskHandle, bool allowInteruption);
CPP_ENGINE_C_API void engine_destroy_task_handle(uintptr_t taskHandle);
CPP_ENGINE_C_API size_t engine_get_stacktrace_number(const uintptr_t frameHandle);
CPP_ENGINE_C_API const char* engine_get_stacktrace_message(const uintptr_t frameHandle);
CPP_ENGINE_C_API const char16_t* engine_get_stackframe_file(const uintptr_t frameHandle, size_t frameNumber);
CPP_ENGINE_C_API const char* engine_get_stackframe_func(const uintptr_t frameHandle, size_t frameNumber);
CPP_ENGINE_C_API uint64_t engine_get_stackframe_line(const uintptr_t frameHandle, size_t frameNumber);

CPP_ENGINE_C_API void engine_evaluate_function(const uint64_t matlabHandle, const char* function, const size_t nlhs, void* args, size_t argsSize, const bool outoutFlag, const bool errorFlag, uint64_t* handles, size_t handlesSize);

CPP_ENGINE_C_API void engine_get_feval_result(uint64_t* handles, size_t handlesSize, void* output, void* error, const double timeout, void* result, size_t resultSize);

CPP_ENGINE_C_API void engine_destroy_feval_result(uint64_t* handles, size_t handlesSize);

CPP_ENGINE_C_API bool engine_is_cancelled_feval(const uint64_t futureHandle);

CPP_ENGINE_C_API bool engine_cancel_feval(const uint64_t futureHandle);

CPP_ENGINE_C_API bool engine_is_done_feval(const uint64_t futureHandle);

CPP_ENGINE_C_API bool engine_wait_feval(const uint64_t futureHandle, const double seconds);

CPP_ENGINE_C_API void engine_terminate_out_of_process_matlab(const uint64_t matlabHandle);

CPP_ENGINE_C_API void engine_terminate_session();

#endif /* CPP_ENGINE_API_HPP */