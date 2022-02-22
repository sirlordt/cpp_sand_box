#pragma once

#define __BEGIN_MAP_POSIX_SIGNAL_TO_CPP_EXCEPTION                            \
                                                                             \
    jmp_buf local_jump_buffer {};                                            \
                                                                             \
    const std::string thread_id = CommonUtilities::get_thread_id();          \
                                                                             \
    const int signal = setjmp( local_jump_buffer );                          \
                                                                             \
    if ( signal == 0 )                                                       \
    {                                                                        \
                                                                             \
      SystemUtilities::global_jump_buffer[ thread_id ]                       \
                      .push( local_jump_buffer[ 0 ] );                       \
                                                                             \
    }                                                                        \
    else {                                                                   \
                                                                             \
      const char *code = "";                                                 \
      const char *message = "";                                              \
                                                                             \
      if ( signal == SIGILL )                                                \
      {                                                                      \
                                                                             \
        code = "ILLEGAL_INSTRUCTION";                                        \
        message = "Illegal Instruction";                                     \
                                                                             \
      }                                                                      \
      else if ( signal == SIGFPE )                                           \
      {                                                                      \
                                                                             \
        code = "FLOATING_POINT_EXCEPTION";                                   \
        message = "Floating point exception";                                \
                                                                             \
      }                                                                      \
      else if ( signal == SIGSEGV )                                          \
      {                                                                      \
                                                                             \
        code = "INVALID_MEMORY_REFERENCE";                                   \
        message = "Invalid memory reference";                                \
                                                                             \
      }                                                                      \
      else if ( signal == SIGINT )                                           \
      {                                                                      \
                                                                             \
        code = "THREAD_INTERRUPTED";                                         \
        message = "Thread interrupted";                                      \
                                                                             \
      }                                                                      \
                                                                             \
      NSTD::Exception exceptionToThrow =                                     \
      {                                                                      \
        thread_id.c_str(),                                                   \
        SystemUtilities::global_thread_name[ thread_id ].c_str(),            \
        signal,                                                              \
        code,                                                                \
        message,                                                             \
        CommonUtilities::get_local_date_time( _DATETIME_FORMAT_02 ).c_str(), \
        &SystemUtilities::global_thread_call_stack[ thread_id ]              \
      };                                                                     \
                                                                             \
      throw exceptionToThrow;                                                \
                                                                             \
    }                                                                        \

#define __END_MAP_POSIX_SIGNAL_TO_CPP_EXCEPTION                              \
                                                                             \
  SystemUtilities::global_jump_buffer[ thread_id ].pop();                    \
                                                                             \

#define __REPORT_EXCEPTION__                                                                                                                     \
                                                                                                                                                 \
    std::osyncstream sync_out( std::cout );                                                                                                      \
                                                                                                                                                 \
    sync_out << "***** Exception Report Begin *****" << std::endl;                                                                               \
    sync_out << "Thread: " << ( exception.get_thread_name() ? exception.get_thread_name() : "null" ) <<                                          \
                      " (" << ( exception.get_thread_id() ? exception.get_thread_id(): "null" ) << ")" << std::endl;                             \
    sync_out << "Signal: " << exception.get_signal() << std::endl;                                                                               \
    sync_out << "Code: \"" << ( exception.get_code() ? exception.get_code(): "null" ) << "\"" << std::endl;                                      \
    sync_out << "Message: \"" << ( exception.get_message() ? exception.get_message(): "null" ) << "\"" << std::endl;                             \
    sync_out << "Function: \"" << __func__  << "\"" << std::endl;                                                                                \
    sync_out << "File: " << __FILE__ << ":" << __LINE__ << std::endl;                                                                            \
    sync_out << "At: " << ( exception.get_at() ? exception.get_at(): "null" ) << std::endl;                                                      \
    sync_out << "StackTrace: " << std::endl;                                                                                                     \
                                                                                                                                                 \
    if ( exception.get_call_stack() &&                                                                                                           \
         exception.get_call_stack()->size() ) {                                                                                                  \
                                                                                                                                                 \
      for ( const NSTD::CallStackFrame& call_stack_entry: *exception.get_call_stack() ) {                                                        \
                                                                                                                                                 \
        sync_out << call_stack_entry.get_function() << " => " << call_stack_entry.get_file() << ":" << call_stack_entry.get_line() << std::endl; \
                                                                                                                                                 \
      }                                                                                                                                          \
                                                                                                                                                 \
    }                                                                                                                                            \
                                                                                                                                                 \
    sync_out << "***** Exception Report End *****" << std::endl;                                                                                 \
                                                                                                                                                 \
