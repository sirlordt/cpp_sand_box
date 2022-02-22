#pragma once

#define __INDENT CommonUtilities::call_left_indent_buffer
#define __INC_INDENT __INC_INDENT_W_OFFSET( 2 )
#define __DEC_INDENT __DEC_INDENT_W_OFFSET( 2 )
#define __INC_INDENT_W_OFFSET( offset ) \
    memset( CommonUtilities::call_left_indent_buffer + CommonUtilities::call_left_indent, \
            ' ', \
            offset ); \
    CommonUtilities::call_left_indent += offset;
#define __DEC_INDENT_W_OFFSET( offset ) \
    memset( CommonUtilities::call_left_indent_buffer + CommonUtilities::call_left_indent - offset, \
            0, \
            CommonUtilities::call_left_indent ); \
    CommonUtilities::call_left_indent -= offset;
