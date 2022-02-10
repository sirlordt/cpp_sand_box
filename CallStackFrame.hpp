#pragma once

#include <iostream>
#include <cstring>
#include <utility>

namespace KISS {

#define __DEBUG_CALLSTACKFRAME__       true
#define __DEBUG_CALLSTACKFRAME_OUT__   std::cout
//#define __DEBUG_CALLSTACKFRAME_LIMIT__ 0

struct CallStackFrame {
public:

  CallStackFrame() noexcept { // = default;

    __INC_INDENT

    instance_id = instance_id_count++;

    #ifdef __DEBUG_CALLSTACKFRAME__

      __DEBUG_CALLSTACKFRAME_OUT__ << __INDENT << "[->][++constructor++][" << instance_id << "] CallStackFrame::CallStackFrame() //** Simple **" << std::endl << std::endl;

    #endif

    __DEC_INDENT

    #ifdef __DEBUG_CALLSTACKFRAME_LIMIT__

      if ( instance_id >= __DEBUG_CALLSTACKFRAME_LIMIT__ ) {

        __DEBUG_CALLSTACKFRAME_OUT__ << __INDENT_W_OFFSET( 2 ) << std::endl;
        __DEBUG_CALLSTACKFRAME_OUT__ << __INDENT_W_OFFSET( 2 ) << "*******************" << std::endl;
        __DEBUG_CALLSTACKFRAME_OUT__ << __INDENT_W_OFFSET( 2 ) << "[warning] Terminating program CallStackFrame::CallStackFrame():" + __LINE__;
        __DEBUG_CALLSTACKFRAME_OUT__ << __INDENT_W_OFFSET( 2 ) << "*******************" << std::endl;
        __DEBUG_CALLSTACKFRAME_OUT__ << __INDENT_W_OFFSET( 2 ) << std::endl;

        std::terminate();

      }

    #endif

    //this->clear();

  }

  CallStackFrame( const char *function, const char *file, const char* line ) {

    __INC_INDENT

    instance_id = instance_id_count++;

    #ifdef __DEBUG_CALLSTACKFRAME__

      __DEBUG_CALLSTACKFRAME_OUT__ << __INDENT << "[begin][++constructor++][" << instance_id << "] CallStackFrame::CallStackFrame( const char *function: '" << function << "', const char *file: '" << file << "', const char* line: '" << line << "' ) //** **" << std::endl << std::endl;

    #endif

    this->copy( function, file, line );

    #ifdef __DEBUG_CALLSTACKFRAME__

      __DEBUG_CALLSTACKFRAME_OUT__ << __INDENT << "[end][~~destructor~~][" << instance_id << "] CallStackFrame::CallStackFrame( const char *function: '" << function << "', const char *file: '" << file << "', const char* line: '" << line << "' ) //** **" << std::endl << std::endl;

    #endif

    __DEC_INDENT

  }

  //Copy contructor pointer
  CallStackFrame( const CallStackFrame *copy ) { //:CallStackFrame( copy.function, copy.file, copy.line ) { // = default;

    __INC_INDENT

    instance_id = instance_id_count++;

    #ifdef __DEBUG_CALLSTACKFRAME__

      __DEBUG_CALLSTACKFRAME_OUT__ << __INDENT << "[begin][++constructor++][" << instance_id << "] CallStackFrame::CallStackFrame( const CallStackFrame *copy ) //** Copy Const Pointer **" << std::endl << std::endl;

    #endif

    if ( copy ) {

      this->copy( copy->function, copy->file, copy->line );

    }

    #ifdef __DEBUG_CALLSTACKFRAME__

      __DEBUG_CALLSTACKFRAME_OUT__ << __INDENT << "[end][++constructor++][" << instance_id << "] CallStackFrame::CallStackFrame( const CallStackFrame *copy ) //** Copy Const Pointer **" << std::endl << std::endl;

    #endif

    __DEC_INDENT

  }

  //Copy contructor
  CallStackFrame( const CallStackFrame &copy ) { //:CallStackFrame( copy.function, copy.file, copy.line ) { // = default;

    __INC_INDENT

    instance_id = instance_id_count++;

    #ifdef __DEBUG_CALLSTACKFRAME__

      __DEBUG_CALLSTACKFRAME_OUT__ << __INDENT << "[begin][++constructor++][" << instance_id << "] CallStackFrame( const CallStackFrame &copy ) //** Copy Const Reference **" << std::endl << std::endl;

    #endif

    this->copy( copy.function, copy.file, copy.line );

    #ifdef __DEBUG_CALLSTACKFRAME__

      __DEBUG_CALLSTACKFRAME_OUT__ << __INDENT << "[end][++constructor++][" << instance_id << "] CallStackFrame( const CallStackFrame &copy ) //** Copy Const Reference **" << std::endl << std::endl;

    #endif

    __DEC_INDENT

  }

  //Move contructor
  CallStackFrame( CallStackFrame &&move ) noexcept { // = default;

    __INC_INDENT

    instance_id = instance_id_count++;

    #ifdef __DEBUG_CALLSTACKFRAME__

      __DEBUG_CALLSTACKFRAME_OUT__ << __INDENT << "[begin][++constructor++][" << instance_id << "] CallStackFrame::CallStackFrame( CallStackFrame &&move ) noexcept //** Move **" << std::endl << std::endl;

    #endif

    this->move( move );

    #ifdef __DEBUG_CALLSTACKFRAME__

      __DEBUG_CALLSTACKFRAME_OUT__ << __INDENT << "[end][++constructor++][" << instance_id << "] CallStackFrame::CallStackFrame( CallStackFrame &&move ) noexcept //** Move **" << std::endl << std::endl;

    #endif

    __DEC_INDENT

  }

  ~CallStackFrame() noexcept { // = default;

    __INC_INDENT

    #ifdef __DEBUG_CALLSTACKFRAME__

      __DEBUG_CALLSTACKFRAME_OUT__ << __INDENT << "[begin][~~destructor~~][" << instance_id << "] CallStackFrame::~CallStackFrame() noexcept //** **" << std::endl;

      if ( this->function || this->file || this->line ) {

        __DEBUG_CALLSTACKFRAME_OUT__ << *this;

      }
      else {

        __DEBUG_CALLSTACKFRAME_OUT__ << std::endl;

      }

    #endif

    this->clear();

    #ifdef __DEBUG_CALLSTACKFRAME__

      __DEBUG_CALLSTACKFRAME_OUT__ << __INDENT << "[end][~~destructor~~][" << instance_id << "] CallStackFrame::~CallStackFrame() noexcept //** **" << std::endl << std::endl;

    #endif

    __DEC_INDENT

  }

  //Copy assign operator=
  CallStackFrame& operator=( const CallStackFrame &copy ) noexcept {  //  = default;

    __INC_INDENT

    #ifdef __DEBUG_CALLSTACKFRAME__

      __DEBUG_CALLSTACKFRAME_OUT__ << __INDENT << "[begin][**operator=**][" << instance_id << "] CallStackFrame::CallStackFrame& operator=( CallStackFrame& copy: " << &copy << " ) //** Copy **" << std::endl << std::endl;

    #endif

    this->copy( copy.function, copy.file, copy.line );

    #ifdef __DEBUG_CALLSTACKFRAME__

      __DEBUG_CALLSTACKFRAME_OUT__ << __INDENT << "[end][**operator=**][" << instance_id << "] CallStackFrame::CallStackFrame& operator=( CallStackFrame& copy: " << &copy << " ) //** Copy **" << std::endl << std::endl;

    #endif

    __DEC_INDENT

    return *this;

  }

  //Move assign operator=
  CallStackFrame& operator=( CallStackFrame&& move ) noexcept {

    __INC_INDENT

    #ifdef __DEBUG_CALLSTACKFRAME__

      __DEBUG_CALLSTACKFRAME_OUT__ << __INDENT << "[begin][**operator=**][" << instance_id << "] CallStackFrame::CallStackFrame& operator=( CallStackFrame&& move: " << &move << " ) //** Move **" << std::endl << std::endl;

    #endif
    // function = call_stack_entry.function;
    // file = call_stack_entry.file;
    // line = call_stack_entry.line;
    this->move( move );

    #ifdef __DEBUG_CALLSTACKFRAME__

      __DEBUG_CALLSTACKFRAME_OUT__ << __INDENT << "[end][**operator=**][" << instance_id << "] CallStackFrame::CallStackFrame& operator=( CallStackFrame&& move: " << &move << " ) //** Move **" << std::endl << std::endl;

    #endif

    __DEC_INDENT

    return *this;

  }

  inline const char *get_function() const {

    return function;

  }

  inline const char *get_file() const {

    return file;

  }

  inline const char *get_line() const {

    return line;

  }

  inline virtual uint32_t get_instance_id() const {

    return instance_id;

  }

  inline virtual uint32_t get_instance_id_count() const {

    return instance_id_count;

  }

  inline virtual const char* get_class_name() const {

    return "CallStackFrame";

  }

  friend std::ostream& operator<<( std::ostream& os, const CallStackFrame& call_stack_entry );

private:

  inline static uint32_t instance_id_count = 0;

  u_int32_t instance_id = 0;
  char *function = nullptr;
  char *file = nullptr;
  char *line = nullptr;

  inline void clear() {

    if ( function ) {

      delete []function;

    }

    function = nullptr;

    if ( file ) {

      delete []file;

    }

    file = nullptr;

    if ( line ) {

      delete []line;

    }

    line = nullptr;

  }

  inline void copy( const char *function, const char *file, const char* line ) {

    __INC_INDENT

    int size = 0;

    if ( function ) {

      size = strlen( function );

      if ( size ) {

        this->function = new char[ size + 1 ];
        strcpy( this->function, function );

      }

      #ifdef __DEBUG_CALLSTACKFRAME__

        __DEBUG_CALLSTACKFRAME_OUT__ << __INDENT << "[begin][==member==][" << instance_id << "] inline void CallStackFrame::copy( const char *function: '" << function << "', const char *file: '" << file << "', const char* line: '" << line << "' ) //** **" << std::endl;

        __DEBUG_CALLSTACKFRAME_OUT__ << __INDENT_W_OFFSET( 2 ) << std::endl;
        __DEBUG_CALLSTACKFRAME_OUT__ << __INDENT_W_OFFSET( 2 ) << "*******************" << std::endl;
        __DEBUG_CALLSTACKFRAME_OUT__ << __INDENT_W_OFFSET( 2 ) << "Function size: " << size << std::endl;

      #endif

    }

    if ( file ) {

      size = strlen( file );

      if ( size ) {

        this->file = new char[ size + 1 ];
        strcpy( this->file, file );

      }

      #ifdef __DEBUG_CALLSTACKFRAME__

        __DEBUG_CALLSTACKFRAME_OUT__ << __INDENT_W_OFFSET( 2 ) << "File size: " << size << std::endl;

      #endif

    }

    if ( line ) {

      size = strlen( line );

      if ( size ) {

        this->line = new char[ size + 1 ];
        strcpy( this->line, line );

      }

      #ifdef __DEBUG_CALLSTACKFRAME__

        __DEBUG_CALLSTACKFRAME_OUT__ << __INDENT_W_OFFSET( 2 ) << "Line size: " << size << std::endl;
        __DEBUG_CALLSTACKFRAME_OUT__ << __INDENT_W_OFFSET( 2 ) << "*******************" << std::endl;
        //__DEBUG_CALLSTACKFRAME_OUT___ << std::endl;

        __DEBUG_CALLSTACKFRAME_OUT__ << *this;

        __DEBUG_CALLSTACKFRAME_OUT__ << __INDENT << "[end][==member==][" << instance_id << "] inline void CallStackFrame::copy( const char *function: '" << function << "', const char *file: '" << file << "', const char* line: '" << line << "' ) //** **" << std::endl << std::endl;

      #endif

    }

    __DEC_INDENT

  }

  inline void move( CallStackFrame& move ) {

    std::swap( this->function, move.function );
    std::swap( this->file, move.file );
    std::swap( this->line, move.line );

  }

};

std::ostream& operator<<( std::ostream& os, const CallStackFrame& call_stack_entry )
{

  os << __INDENT << std::endl;
  os << __INDENT_W_OFFSET( 2 ) << "*******************" << std::endl;
  os << __INDENT_W_OFFSET( 2 ) << "Function: " << ( call_stack_entry.function ? call_stack_entry.function: "null" ) << " " << static_cast<void*>(call_stack_entry.function) << std::endl;
  os << __INDENT_W_OFFSET( 2 ) << "File: " << ( call_stack_entry.file ? call_stack_entry.file: "null" ) << " " << static_cast<void*>(call_stack_entry.file) << std::endl;
  os << __INDENT_W_OFFSET( 2 ) << "Line: " << ( call_stack_entry.line ? call_stack_entry.line: "null" ) << " " << static_cast<void*>(call_stack_entry.line) << std::endl;
  os << __INDENT_W_OFFSET( 2 ) << "*******************" << std::endl;
  os << __INDENT << std::endl;

  return os;

}

}
