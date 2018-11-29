/* 2017 The MathWorks, Inc. */

#ifndef MATLAB_FUNCTION_TYPE_HPP_
#define MATLAB_FUNCTION_TYPE_HPP_

namespace matlab {
    namespace data {
        namespace detail {
            enum class FunctionType : int {
                CREATE_ENUM_ARRAY = 0,
                CREATE_ARRAY_WITH_DIMS,
                CREATE_ARRAY_WITH_DIMS_AND_DATA,
                CREATE_SCALAR_ARRAY,
                CREATE_SCALAR_STRING,
                CREATE_SCALAR_MISSING_STRING,
                CREATE_CHAR_ARRAY_FROM_CHAR16,
                CREATE_CHAR_ARRAY_FROM_STRING,
                ADD_NAME,
                CREATE_ENUM_ARRAY_WITH_ENUMS,
                CREATE_STRUCT_ARRAY,
                CREATE_ARRAY_FROM_BUFFER,
                CREATE_SPARSE_ARRAY_FROM_BUFFER,
                CREATE_BUFFER,
                CREATE_ARRAY_FACTORY,
                DESTROY_ARRAY_FACTORY,
                REFERENCE_ADD_INDEX,
                CREATE_NAMES,
                DESTROY_NAMES,
                REFERENCE_ADD_STRING_INDEX,
                REFERENCE_MATLAB_STRING_EQUAL_STRING,
                REFERENCE_MATLAB_STRING_EQUAL_REFERENCE,
                REFERENCE_MATLAB_STRING_EQUAL_STRING16,
                REFERENCE_DESTROY,
                ARRAY_REFERENCE_GET_TYPE,
                ARRAY_REFERENCE_GET_DIMENSIONS,
                ARRAY_REFERENCE_GET_NUM_ELEMENTS,
                ARRAY_REFERENCE_IS_EMPTY,
                ARRAY_REFERENCE_GET_REFERENCE,
                ARRAY_REFERENCE_BEGIN,
                ARRAY_REFERENCE_END,
                CHAR_ARRAY_GET_STRING,
                CHAR_ARRAY_GET_ASCII,
                CHAR_ARRAY_REF_GET_STRING,
                CHAR_ARRAY_REF_GET_ASCII,
                ENUM_ARRAY_GET_VALUE,
                ENUM_ARRAY_SET_VALUE,
                ENUM_ARRAY_GET_CLASS,
                ENUM_REF_GET_CLASS,
                FORWARD_ITERATOR_CLONE,
                FORWARD_ITERATOR_DESTROY,
                FORWARD_ITERATOR_PLUS_PLUS,
                FORWARD_ITERATOR_EQUAL,
                FORWARD_ITERATOR_GET_REF,
                FORWARD_ITERATOR_INCREMENT,
                FIELD_ID_CREATE,
                FIELD_ID_DESTROY,
                FIELD_ID_IS_EQUAL,
                FIELD_ID_GET_STRING,
                ARRAY_CREATE_EMPTY,
                ARRAY_DESTROY,
                ARRAY_GET_DIMENSIONS,
                ARRAY_GET_NUM_ELEMENTS,
                ARRAY_GET_TYPE,
                ARRAY_IS_EMPTY,
                ARRAY_UNSHARE,
                ARRAY_CREATE_REFERENCE,
                TYPED_ARRAY_BEGIN,
                TYPED_ARRAY_END,
                TYPED_ARRAY_IS_VALID_CONVERSION,
                ARRAY_RELEASE_BUFFER,
                SPARSE_ARRAY_GET_INDEX,
                SPARSE_ARRAY_GET_NUM_NONZERO_ELEMENTS,
                TYPED_ITERATOR_CLONE,
                TYPED_ITERATOR_DESTROY,
                TYPED_ITERATOR_PLUS_PLUS,
                TYPED_ITERATOR_MINUS_MINUS,
                TYPED_ITERATOR_INCREMENT,
                TYPED_ITERATOR_DECREMENT,
                TYPED_ITERATOR_EQUAL,
                TYPED_ITERATOR_DISTANCE_TO,
                TYPED_ITERATOR_GET_POD_VALUE,
                TYPED_ITERATOR_GET_PROXY,
                STRING_GET_VALUE,
                STRUCT_GET_FIELD_COPY,
                STRUCT_REFERENCE_GET_INDEX,
                STRUCT_ARRAY_BEGIN_ID,
                STRUCT_ARRAY_END_ID,
                STRUCT_ARRAY_REF_BEGIN_ID,
                STRUCT_ARRAY_REF_END_ID,
                STRUCT_ARRAY_GET_NUM_FIELDS,
                STRUCT_ARRAY_REF_GET_NUM_FIELDS,
                SYMBOL_INFO_DESTROY,
                SYMBOL_INFO_GET_NAME,
                SYMBOL_INFO_SET_STATUS,
                SYMBOL_INFO_SET_VALUE,
                VARIABLE_INFO_DESTROY,
                VARIABLE_INFO_GET_NAME,
                VARIABLE_INFO_GET_VALUE,
                SPARSE_ARRAY_REF_GET_NUM_NONZERO_ELEMENTS,
                TYPED_ARRAY_REFERENCE_VALIDATE_TYPE,
                REFERENCE_VALIDATE_TYPE,
                REFERENCE_VALIDATE_INDEX,
                TYPED_REFERENCE_GET_POD_VALUE,
                TYPED_REFERENCE_GET_COMPLEX_VALUE,
                TYPED_REFERENCE_SET_POD_VALUE,
                TYPED_REFERENCE_SET_COMPLEX_VALUE,
                ARRAY_REFERENCE_SHARED_COPY,
                REFERENCE_SET_CHAR16_STRING,
                REFERENCE_SET_STRING,
                REFERENCE_SET_MISSING_CHAR16_STRING,
                REFERENCE_GET_CONTAINER_TYPE,
                REFERENCE_SET_REFERENCE_VALUE,
                REFERENCE_GET_REFERENCE_VALUE,
                ARRAY_GET_IMPL_FOR_WORKSPACE,
                ARRAY_RELEASE_IMPL_FROM_WORKSPACE,
                PLACEHOLDER
            };
        }
    }
}

#endif
