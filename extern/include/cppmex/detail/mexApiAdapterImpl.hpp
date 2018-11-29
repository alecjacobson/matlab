/* Copyright 2017-2018 The MathWorks, Inc. */

#ifndef __MEX_API_ADAPTER_HPP__
#define __MEX_API_ADAPTER_HPP__

#include "MatlabDataArray.hpp"
#include "mex.hpp"
#include "mexEngineUtilImpl.hpp"
#include "mexExceptionType.hpp"
#include <vector>
#include <iostream>
#include "assert.h"

LIBMWMEX_API_EXTERN_C{

    void mexApiFeval(void*, const int, void**, const int, void**, const char*, int*, void**, void*, void*, void(*)(void*, const char16_t*, size_t), void(*)(void*));

    void* mexApiGetVariable(void*, const char*, const char16_t*, int*, void**);

    void* mexApiSetVariable(void*, void*, const char*, const char16_t*, int*, void**);

    void* mexApiGetProperty(void*, void*, size_t, const char16_t*, int*, void**);

    void* mexApiSetProperty(void*, void*, size_t, const char16_t*, void*, int*, void**);
}

namespace matlab {
    namespace engine {
        namespace detail {

            inline matlab::engine::MATLABExecutionException createMATLABExecutionException(const matlab::data::StructArray& mException);

            inline std::vector<matlab::engine::MATLABExecutionException> createCause(const matlab::data::CellArray& cause) {
                size_t nCauses = cause.getNumberOfElements();
                std::vector<matlab::engine::MATLABExecutionException> causes(nCauses);
                for (size_t i = 0; i < nCauses; i++) {
                    matlab::data::Array exRef = cause[i];
                    matlab::data::StructArray ex(exRef);
                    causes[i] = createMATLABExecutionException(ex);
                }
                return causes;
            }

            inline std::vector<matlab::engine::StackFrame> createStackTrace(const matlab::data::StructArray& stack) {
                size_t nFrames = stack.getNumberOfElements();
                std::vector<matlab::engine::StackFrame> stackFrames(nFrames);

                for (size_t i = 0; i < nFrames; i++) {
                    matlab::data::Array fileRef = stack[i]["File"];
                    matlab::data::CharArray fileStr(fileRef);

                    matlab::data::Array nameRef = stack[i]["Name"];
                    matlab::data::CharArray nameStr(nameRef);

                    matlab::data::Array lineRef = stack[i]["Line"];
                    double line = lineRef[0];
                    stackFrames[i] = matlab::engine::StackFrame(fileStr.toUTF16(), nameStr.toUTF16(), uint32_t(line));
                }

                return stackFrames;
            }

            inline matlab::engine::MATLABExecutionException createMATLABExecutionException(const matlab::data::StructArray& mException) {
                matlab::data::Array idRef = mException[0][std::string("identifier")];
                matlab::data::CharArray id(idRef);
                matlab::data::Array messageRef = mException[0][std::string("message")];
                matlab::data::CharArray message(messageRef);

                matlab::data::Array stackRef = mException[0][std::string("stack")];
                matlab::data::StructArray stack(stackRef);

                matlab::data::Array causeRef = mException[0][std::string("cause")];
                matlab::data::CellArray cause(causeRef);

                std::vector<matlab::engine::MATLABExecutionException> meCause = createCause(cause);
                std::vector<matlab::engine::StackFrame> meStack = createStackTrace(stack);

                return matlab::engine::MATLABExecutionException(id.toAscii(), message.toUTF16(), meStack, meCause);
            }

            inline matlab::engine::MATLABSyntaxException createMATLABSyntaxException(const matlab::data::StructArray& mException) {
                matlab::data::Array idRef = mException[0][std::string("identifier")];
                matlab::data::CharArray id(idRef);
                matlab::data::Array messageRef = mException[0][std::string("message")];
                matlab::data::CharArray message(messageRef);

                return matlab::engine::MATLABSyntaxException(id.toAscii(), message.toUTF16());
            }

            inline matlab::engine::MATLABException createMATLABException(const matlab::data::StructArray& mException) {
                matlab::data::Array idRef = mException[0][std::string("identifier")];
                matlab::data::CharArray id(idRef);
                matlab::data::Array messageRef = mException[0][std::string("message")];
                matlab::data::CharArray message(messageRef);

                return matlab::engine::MATLABException(id.toAscii(), message.toUTF16());
            }

            template<typename T>
            inline void validateTIsSupported() {
                using U = typename std::remove_cv<typename std::remove_reference<T>::type>::type;
                static_assert(
                    std::is_same<U, bool>::value
                    || std::is_same<U, int>::value
                    || std::is_same<U, int8_t>::value
                    || std::is_same<U, int16_t>::value
                    || std::is_same<U, int32_t>::value
                    || std::is_same<U, int64_t>::value
                    || std::is_same<U, uint8_t>::value
                    || std::is_same<U, uint16_t>::value
                    || std::is_same<U, uint32_t>::value
                    || std::is_same<U, uint64_t>::value
                    || std::is_same<U, float>::value
                    || std::is_same<U, double>::value, "Attempted to use unsupported types.");
            }

            template<class T>
            matlab::data::Array createRhs(matlab::data::ArrayFactory& factory, T&& value) {
                validateTIsSupported<T>();
                using U = typename std::remove_cv<typename std::remove_reference<T>::type>::type;
                return factory.createArray<U>({ 1, 1 }, { value });
            }

            template<typename T, typename A>
            matlab::data::Array createRhs(matlab::data::ArrayFactory& factory, std::vector <T, A>&& value) {
                validateTIsSupported<T>();
                return factory.createArray({ 1, value.size() }, value.begin(), value.end());
            }

            template <std::size_t ...Ints>
            struct index_sequence {
                using value_type = std::size_t;
                static std::size_t size() { return sizeof...(Ints); }
            };

            template<std::size_t N, std::size_t... Values>
            struct make_index_sequence_impl {
                using type = typename make_index_sequence_impl<N - 1, Values..., sizeof...(Values)>::type;
            };

            template<std::size_t... Values>
            struct make_index_sequence_impl < 0, Values... > {
                using type = index_sequence < Values... >;
            };

            template<std::size_t N>
            using make_index_sequence = typename make_index_sequence_impl<N>::type;

            template<typename T>
            struct createLhs {
                static const size_t nlhs = 1;

                T operator()(std::vector<matlab::data::Array>&& lhs) const {
                    if (lhs.empty()) {
                        throw matlab::engine::TypeConversionException("The result is empty.");
                    }

                    T value;
                    try {
                        value = (*this)(matlab::data::TypedArray<T>(std::move(lhs.front())));
                    }
                    catch (const std::exception& e) {
                        throw matlab::engine::TypeConversionException(e.what());
                    }
                    return value;
                }

                T operator()(matlab::data::TypedArray<T> lhs) const {
                    validateTIsSupported<T>();
                    auto const begin = lhs.begin();
                    auto const end = lhs.end();
                    if (begin == end) {
                        throw matlab::engine::TypeConversionException("The result is empty.");
                    }
                    return *begin;
                }
            };

            template<>
            struct createLhs < void > {
                static const size_t nlhs = 0;
                void operator()(std::vector<matlab::data::Array>&& lhs) const {}
            };

            template<typename... TupleTypes>
            struct createLhs < std::tuple<TupleTypes...> > {
                static const size_t nlhs = sizeof...(TupleTypes);
                using T = std::tuple < TupleTypes... >;

                T operator()(std::vector<matlab::data::Array>&& lhs) const {
                    //we are not validating the LHS here as it can be any combinations of types for std::tuple.
                    if (lhs.size() < sizeof...(TupleTypes)) { throw std::runtime_error(""); }
                    return (*this)(std::move(lhs), detail::make_index_sequence<sizeof...(TupleTypes)>());
                }

            private:
                template<size_t Index>
                using TupleElement = typename std::remove_cv<typename std::remove_reference<typename std::tuple_element<Index, std::tuple<TupleTypes...> >::type>::type>::type;

                template<size_t... IndexList>
                std::tuple <TupleTypes...> operator()(std::vector<matlab::data::Array>&& lhs, detail::index_sequence<IndexList...>) const {
                    return std::tuple <TupleTypes...>(createLhs<TupleElement<IndexList>>()(std::move(lhs[IndexList]))...);
                }
            };
        }
    }
}

void implDeleter(matlab::data::impl::ArrayImpl** impl) {
    if (impl != nullptr)
        free(impl);
}

inline void writeToStreamBuffer(void* buffer, const char16_t* stream, size_t n) {
    std::shared_ptr<matlab::engine::StreamBuffer>* output = reinterpret_cast<std::shared_ptr<matlab::engine::StreamBuffer>*>(buffer);
    output->get()->sputn(stream, n);
}

inline void deleteStreamBufferImpl(void* impl) {
    delete static_cast<std::shared_ptr<matlab::engine::StreamBuffer>*>(impl);
}

inline void throwIfError(int errID, void* mexcept) {
    matlab::mex::detail::ErrorType errorID(static_cast<matlab::mex::detail::ErrorType>(errID));
    switch (errorID) {
    case matlab::mex::detail::ErrorType::NoException:
        break;
    case matlab::mex::detail::ErrorType::RuntimeError: {
        matlab::data::impl::ArrayImpl* impl = reinterpret_cast<matlab::data::impl::ArrayImpl*>(mexcept);
        matlab::data::Array exArr(matlab::data::detail::Access::createObj<matlab::data::Array>(impl));
        matlab::data::StructArray sArr(exArr);
        matlab::data::TypedArray<int> errStatus = sArr[0][std::string("status")];
        int rErrID_ = errStatus[0];
        matlab::mex::detail::ErrorType rErrorID(static_cast<matlab::mex::detail::ErrorType>(rErrID_));
        switch (rErrorID) {
        case matlab::mex::detail::ErrorType::SyntaxError: {
            matlab::engine::MATLABSyntaxException exp = matlab::engine::detail::createMATLABSyntaxException(sArr);
            throw exp;
        }
        case matlab::mex::detail::ErrorType::ExecutionError: {
            matlab::engine::MATLABExecutionException exp = matlab::engine::detail::createMATLABExecutionException(sArr);
            throw exp;
        }
        case matlab::mex::detail::ErrorType::EngineError: {
            matlab::engine::MATLABException exp = matlab::engine::detail::createMATLABException(sArr);
            throw exp;
        }
        default:
            throw matlab::engine::MATLABException("MATLAB:mex:CppMexException", matlab::engine::convertUTF8StringToUTF16String("Runtime Error"));
        }
    }
    case matlab::mex::detail::ErrorType::ThreadError: {
        const std::string msg = "MATLAB Engine functions must be called from the thread as the MexFunction class.";
        throw matlab::engine::MATLABException("MATLAB:mex:CppMexThreadMismatch", matlab::engine::convertUTF8StringToUTF16String(msg));
    }
    case matlab::mex::detail::ErrorType::OutOfMemory: {
        std::string outOfMemoryError = "Not enough memory available to support the request.";
        throw matlab::OutOfMemoryException(outOfMemoryError);
    }
    case matlab::mex::detail::ErrorType::SystemError: {
        std::string msg = "Unexpected exception caught in feval.";
        throw matlab::engine::MATLABException("MATLAB:mex:CppMexException", matlab::engine::convertUTF8StringToUTF16String(msg));
    }
    default:
        throw matlab::engine::MATLABException("MATLAB:mex:CppMexException", matlab::engine::convertUTF8StringToUTF16String("Error"));
    }
}

/*** matlab::engine::MATLABEngine ***/
std::vector<matlab::data::Array> matlab::engine::MATLABEngine::feval(const std::u16string &function,
                                                                     const int nlhs,
                                                                     const std::vector<matlab::data::Array> &args,
                                                                     const std::shared_ptr<matlab::engine::StreamBuffer> &output,
                                                                     const std::shared_ptr<matlab::engine::StreamBuffer> &error) {
    int nrhs = static_cast<int>(args.size());

    matlab::data::impl::ArrayImpl** plhs_arr_impl = (nlhs == 0) ? nullptr : (matlab::data::impl::ArrayImpl**) malloc(sizeof(matlab::data::impl::ArrayImpl*) * nlhs);
    std::unique_ptr<matlab::data::impl::ArrayImpl*, void(*)(matlab::data::impl::ArrayImpl**)> plhs_impl_guard(plhs_arr_impl, &implDeleter);

    matlab::data::impl::ArrayImpl** args_arr_impl = (nrhs == 0) ? nullptr : (matlab::data::impl::ArrayImpl**) malloc(sizeof(matlab::data::impl::ArrayImpl*) * nrhs);
    std::unique_ptr<matlab::data::impl::ArrayImpl*, void(*)(matlab::data::impl::ArrayImpl**)> args_impl_guard(args_arr_impl, &implDeleter);

    void** plhs_impl = reinterpret_cast<void**>(plhs_arr_impl);
    void** args_impl = reinterpret_cast<void**>(args_arr_impl);

    arrayToImpl(nrhs, args_impl, args);

    void* mexcept = nullptr;

    std::string function_utf8 = matlab::engine::convertUTF16StringToUTF8String(function);

    void* output_ = output ? new std::shared_ptr<StreamBuffer>(std::move(output)) : nullptr;
    void* error_ = error ? new std::shared_ptr<StreamBuffer>(std::move(error)) : nullptr;

    int errID = 0;
    mexApiFeval(pImpl, nlhs, plhs_impl, nrhs, args_impl, function_utf8.c_str(), &errID, &mexcept, output_, error_, &writeToStreamBuffer, &deleteStreamBufferImpl);

    throwIfError(errID, mexcept);

    std::vector<matlab::data::Array> plhs;
    plhs.reserve(nlhs);
    implToArray(nlhs, plhs_impl, plhs);

    return plhs;
}

std::vector<matlab::data::Array> matlab::engine::MATLABEngine::feval(const std::string &function,
                                                                     const int nlhs,
                                                                     const std::vector<matlab::data::Array> &args,
                                                                     const std::shared_ptr<matlab::engine::StreamBuffer> &output,
                                                                     const std::shared_ptr<matlab::engine::StreamBuffer> &error) {
    return feval(std::u16string(function.begin(), function.end()), nlhs, args, output, error);
}

matlab::data::Array matlab::engine::MATLABEngine::feval(const std::u16string &function,
                                                        const std::vector<matlab::data::Array> &args,
                                                        const std::shared_ptr<matlab::engine::StreamBuffer> &output,
                                                        const std::shared_ptr<matlab::engine::StreamBuffer> &error) {

    std::vector<matlab::data::Array> out_vec = feval(function, 1, args, output, error);
    return out_vec.at(0);
}

matlab::data::Array matlab::engine::MATLABEngine::feval(const std::string &function,
                                                        const std::vector<matlab::data::Array> &args,
                                                        const std::shared_ptr<matlab::engine::StreamBuffer> &output,
                                                        const std::shared_ptr<matlab::engine::StreamBuffer> &error) {
    return feval(std::u16string(function.begin(), function.end()), args, output, error);
}

matlab::data::Array matlab::engine::MATLABEngine::feval(const std::u16string &function,
                                                        const matlab::data::Array &arg,
                                                        const std::shared_ptr<matlab::engine::StreamBuffer> &output,
                                                        const std::shared_ptr<matlab::engine::StreamBuffer> &error) {
    return feval(function, std::vector<matlab::data::Array>({ arg }), output, error);
}

matlab::data::Array matlab::engine::MATLABEngine::feval(const std::string &function,
                                                        const matlab::data::Array &arg,
                                                        const std::shared_ptr<matlab::engine::StreamBuffer> &output,
                                                        const std::shared_ptr<matlab::engine::StreamBuffer> &error) {
    return feval(std::u16string(function.begin(), function.end()), std::vector<matlab::data::Array>({arg}), output, error);
}

template<class ReturnType, typename...RhsArgs>
ReturnType matlab::engine::MATLABEngine::feval(const std::u16string &function,
                                               const std::shared_ptr<StreamBuffer> &output,
                                               const std::shared_ptr<StreamBuffer> &error,
                                               RhsArgs&&... rhsArgs) {
    matlab::data::ArrayFactory factory;
    std::vector<matlab::data::Array> rhsList({
        detail::createRhs(factory, std::forward<RhsArgs>(rhsArgs))...
    });

    auto const nlhs = detail::createLhs<ReturnType>::nlhs;
    std::vector<matlab::data::Array> f = feval(function, nlhs, rhsList, output, error);
    detail::createLhs<ReturnType> lhsFactory;
    return lhsFactory(std::move(f));
}

template<class ReturnType, typename...RhsArgs>
ReturnType matlab::engine::MATLABEngine::feval(const std::string &function,
                                               const std::shared_ptr<StreamBuffer> &output,
                                               const std::shared_ptr<StreamBuffer> &error,
                                               RhsArgs&&... rhsArgs) {
    return feval(std::u16string(function.begin(), function.end()), output, error, std::forward<RhsArgs>(rhsArgs)...);
}

template<class ReturnType, typename...RhsArgs>
ReturnType matlab::engine::MATLABEngine::feval(const std::u16string &function,
                                               RhsArgs&&... rhsArgs) {
    const std::shared_ptr<StreamBuffer> defaultStream;
    return feval<ReturnType>(function, defaultStream, defaultStream, std::forward<RhsArgs>(rhsArgs)...);
}

template<class ReturnType, typename...RhsArgs>
ReturnType matlab::engine::MATLABEngine::feval(const std::string &function,
                                               RhsArgs&&... rhsArgs) {
    return feval<ReturnType>(std::u16string(function.begin(), function.end()), std::forward<RhsArgs>(rhsArgs)...);
}

void matlab::engine::MATLABEngine::eval(const std::u16string &str,
                                        const std::shared_ptr<matlab::engine::StreamBuffer> &output,
                                        const std::shared_ptr<matlab::engine::StreamBuffer> &error) {
    matlab::data::ArrayFactory factory;
    auto mStr = factory.createScalar(str);

    feval(matlab::engine::convertUTF8StringToUTF16String("eval"), 0, std::vector<matlab::data::Array>({ mStr }), output, error);
}

matlab::data::Array matlab::engine::MATLABEngine::getVariable(const std::u16string &varName,
                                                              matlab::engine::WorkspaceType workspaceType) {
    const char* workspace = (workspaceType == matlab::engine::WorkspaceType::BASE) ? "base" : "global";
    void* mexcept = nullptr;

    int errID = 0;
    void* res = mexApiGetVariable(pImpl, workspace, varName.c_str(), &errID, &mexcept);

    throwIfError(errID, mexcept);

    matlab::data::Array ret = getArray(res);
    return ret;
}

matlab::data::Array matlab::engine::MATLABEngine::getVariable(const std::string &varName,
                                                              matlab::engine::WorkspaceType workspaceType) {
    return getVariable(std::u16string(varName.begin(), varName.end()), workspaceType);
}

void matlab::engine::MATLABEngine::setVariable(const std::u16string &varName,
                                               const matlab::data::Array &var,
                                               matlab::engine::WorkspaceType workspaceType) {
    const char* workspace = (workspaceType == matlab::engine::WorkspaceType::BASE) ? "base" : "global";
    void* mexcept = nullptr;

    void* varImpl = matlab::data::detail::Access::getImpl<matlab::data::impl::ArrayImpl>(var);

    int errID = 0;
    mexApiSetVariable(pImpl, varImpl, workspace, varName.c_str(), &errID, &mexcept);

    throwIfError(errID, mexcept);
}

void matlab::engine::MATLABEngine::setVariable(const std::string &varName,
                                               const matlab::data::Array &var,
                                               matlab::engine::WorkspaceType workspaceType) {
    setVariable(std::u16string(varName.begin(), varName.end()), var, workspaceType);
}

matlab::data::Array matlab::engine::MATLABEngine::getProperty(const matlab::data::Array &object,
                                                              size_t index,
                                                              const std::u16string &propertyName) {
    void* mexcept = nullptr;
    void* objImpl = matlab::data::detail::Access::getImpl<matlab::data::impl::ArrayImpl>(object);

    int errID = 0;
    void* impl = mexApiGetProperty(pImpl, objImpl, index, propertyName.c_str(), &errID, &mexcept);

    throwIfError(errID, mexcept);

    return matlab::data::detail::Access::createObj<matlab::data::Array>(reinterpret_cast<matlab::data::impl::ArrayImpl*>(impl));
}

matlab::data::Array matlab::engine::MATLABEngine::getProperty(const matlab::data::Array &object,
                                                              size_t index,
                                                              const std::string &propertyName) {
    return getProperty(object, index, std::u16string(propertyName.begin(), propertyName.end()));
}

matlab::data::Array matlab::engine::MATLABEngine::getProperty(const matlab::data::Array &object,
                                                              const std::u16string &propertyName) {
    return getProperty(object, 0, propertyName);
}

matlab::data::Array matlab::engine::MATLABEngine::getProperty(const matlab::data::Array &object,
                                                              const std::string &propertyName) {
    return getProperty(object, 0, std::u16string(propertyName.begin(), propertyName.end()));
}

void matlab::engine::MATLABEngine::setProperty(matlab::data::Array &object,
                                               size_t index,
                                               const std::u16string &propertyName,
                                               const matlab::data::Array &value) {
    void* mexcept = nullptr;
    void* objImpl = matlab::data::detail::Access::getImpl<matlab::data::impl::ArrayImpl>(object);
    void* propertyImpl = matlab::data::detail::Access::getImpl<matlab::data::impl::ArrayImpl>(value);

    int errID = 0;
    void* impl = mexApiSetProperty(pImpl, objImpl, index, propertyName.c_str(), propertyImpl, &errID, &mexcept);

    throwIfError(errID, mexcept);

    object = matlab::data::detail::Access::createObj<matlab::data::Array>(reinterpret_cast<matlab::data::impl::ArrayImpl*>(impl));
}

void matlab::engine::MATLABEngine::setProperty(matlab::data::Array &object,
                                               size_t index,
                                               const std::string &propertyName,
                                               const matlab::data::Array &value) {
    setProperty(object, index, std::u16string(propertyName.begin(), propertyName.end()), value);
}

void matlab::engine::MATLABEngine::setProperty(matlab::data::Array &object,
                                               const std::u16string &propertyName,
                                               const matlab::data::Array &value) {
    setProperty(object, 0, propertyName, value);
}

void matlab::engine::MATLABEngine::setProperty(matlab::data::Array &object,
                                               const std::string &propertyName,
                                               const matlab::data::Array &value) {
    setProperty(object, 0, std::u16string(propertyName.begin(), propertyName.end()), value);
}

#endif
