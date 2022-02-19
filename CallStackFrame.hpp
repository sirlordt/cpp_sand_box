#pragma once

#include <iostream>
#include <cstring>
#include <utility>
#include <any>

#include "CommonUtilities.hpp"

namespace NSTD {

  #define __DEBUG_CALLSTACKFRAME__       true
  #define __DEBUG_CALLSTACKFRAME_OUT__   std::cout
  //#define __DEBUG_CALLSTACKFRAME_LIMIT__ 30

  struct CallStackFrame {
  public:

    CallStackFrame() { // = default;

      __INC_INDENT

      this->instance_id = instance_id_count++;

      #ifdef __DEBUG_CALLSTACKFRAME__

        __DEBUG_CALLSTACKFRAME_OUT__ << __INDENT << "[->][++constructor++][" << instance_id << "] CallStackFrame::CallStackFrame() //** Simple **" << std::endl << std::endl;

      #endif

      this->_clear_values();

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

    }

    CallStackFrame( const char *function, const char *file, const char* line ): instance_id( 0 ) {

      __INC_INDENT

      instance_id = instance_id_count++;

      #ifdef __DEBUG_CALLSTACKFRAME__

        __DEBUG_CALLSTACKFRAME_OUT__ << __INDENT << "[begin][++constructor++][" << instance_id << "] CallStackFrame::CallStackFrame( const char *function: '" << function << "', const char *file: '" << file << "', const char* line: '" << line << "' ) //** **" << std::endl << std::endl;

      #endif

      this->_clear_values();

      this->_copy_values( function, file, line );

      #ifdef __DEBUG_CALLSTACKFRAME__

        __DEBUG_CALLSTACKFRAME_OUT__ << __INDENT << "[end][++constructor++][" << instance_id << "] CallStackFrame::CallStackFrame( const char *function: '" << function << "', const char *file: '" << file << "', const char* line: '" << line << "' ) //** **" << std::endl << std::endl;

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

      this->_clear_values();

      if ( copy ) {

        this->_copy_values( copy->function, copy->file, copy->line );

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

      this->_clear_values();

      this->_copy_values( copy.function, copy.file, copy.line );

      #ifdef __DEBUG_CALLSTACKFRAME__

        __DEBUG_CALLSTACKFRAME_OUT__ << __INDENT << "[end][++constructor++][" << instance_id << "] CallStackFrame( const CallStackFrame &copy ) //** Copy Const Reference **" << std::endl << std::endl;

      #endif

      __DEC_INDENT

    }

    //Move contructor
    CallStackFrame( CallStackFrame &&move ) { // = default;

      __INC_INDENT

      instance_id = instance_id_count++;

      #ifdef __DEBUG_CALLSTACKFRAME__

        __DEBUG_CALLSTACKFRAME_OUT__ << __INDENT << "[begin][++constructor++][" << instance_id << "] CallStackFrame::CallStackFrame( CallStackFrame &&move ) noexcept //** Move **" << std::endl << std::endl;

      #endif

      this->_clear_values();

      this->_move_values( move );

      #ifdef __DEBUG_CALLSTACKFRAME__

        __DEBUG_CALLSTACKFRAME_OUT__ << __INDENT << "[end][++constructor++][" << instance_id << "] CallStackFrame::CallStackFrame( CallStackFrame &&move ) noexcept //** Move **" << std::endl << std::endl;

      #endif

      __DEC_INDENT

    }

    ~CallStackFrame() { // = default;

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

      this->_clear_values();

      #ifdef __DEBUG_CALLSTACKFRAME__

        __DEBUG_CALLSTACKFRAME_OUT__ << __INDENT << "[end][~~destructor~~][" << this->instance_id << "] CallStackFrame::~CallStackFrame() noexcept //** **" << std::endl << std::endl;

      #endif

      __DEC_INDENT

    }

    //Copy assign operator=
    CallStackFrame& operator=( const CallStackFrame &copy ) noexcept {  //  = default;

      __INC_INDENT

      #ifdef __DEBUG_CALLSTACKFRAME__

        __DEBUG_CALLSTACKFRAME_OUT__ << __INDENT << "[begin][**operator=**][" << instance_id << "] CallStackFrame::CallStackFrame& operator=( CallStackFrame& copy: " << &copy << " ) //** Copy **" << std::endl << std::endl;

      #endif

      this->_copy_values( copy.function, copy.file, copy.line );

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
      this->_move_values( move );

      #ifdef __DEBUG_CALLSTACKFRAME__

        __DEBUG_CALLSTACKFRAME_OUT__ << __INDENT << "[end][**operator=**][" << instance_id << "] CallStackFrame::CallStackFrame& operator=( CallStackFrame&& move: " << &move << " ) //** Move **" << std::endl << std::endl;

      #endif

      __DEC_INDENT

      return *this;

    }

    // std::any operator[]( const char *name ) const {

    //   std::any result;

    //   if ( strcmp( name, "function"  ) ) {

    //     result = this->function;

    //   }
    //   else if ( strcmp( name, "file" ) ) {

    //     result = this->file;

    //   }
    //   else if ( strcmp( name, "line" ) ) {

    //     result = this->line;

    //   }

    //   return result;

    // }

    // std::any& operator []( const char *name ) {

    //   std::any result;

    //   if ( strcmp( name, "function"  ) ) {

    //     result = this->function;

    //   }
    //   else if ( strcmp( name, "file" ) ) {

    //     result = this->file;

    //   }
    //   else if ( strcmp( name, "line" ) ) {

    //     result = this->line;

    //   }

    //   return result;

    // }

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

    //using AllocatorChar = std::allocator<char>;
    //using AllocatorCharTraits = std::allocator_traits<AllocatorChar>; // The matching trait

    //AllocatorChar allocatorConstChar {};

    inline static uint32_t instance_id_count = 0;

    u_int32_t instance_id = 0;
    char *function = nullptr;
    char *file = nullptr;
    char *line = nullptr;

    char* buffer = nullptr;
    uint32_t buffer_total_size = 0;

    inline void _clear_values() {

      //*** The next code is not really needed the char* not had destructor
      //uint64_t buffer_offset = 0;

      // if ( function ) {

        // const char *temp = ( buffer + buffer_offset );

        // int size = strlen( temp );

        // //std::cout << temp << std::endl;

        // AllocatorCharTraits::destroy( allocatorConstChar, temp );

        // buffer_offset += size + 1;

        // //delete [] function;

      // }

      this->function = nullptr;

      // if ( file ) {

        // const char *temp = ( buffer + buffer_offset );

        // int size = strlen( temp );

        // //std::cout << temp << std::endl;

        // AllocatorCharTraits::destroy( allocatorConstChar, temp );

        // buffer_offset += size + 1;

        // //delete [] file;

      // }

      this->file = nullptr;

      // if ( line ) {

        // const char *temp = ( buffer + buffer_offset );

        // int size = strlen( temp );

        // //std::cout << temp << std::endl;

        // //int size = strlen( ( buffer + buffer_offset ) );

        // AllocatorCharTraits::destroy( allocatorConstChar, temp );

        // buffer_offset += size + 1;

        //delete [] line;

      // }

      this->line = nullptr;
      //*** The previous code is not really needed the char* not had destructor

      if ( this->buffer ) {

        //Destroy from the heap the linear memory buffer (Delete)
        //AllocatorCharTraits::deallocate( allocatorConstChar, buffer, buffer_total_size ); //reinterpret_cast<uint8_t *>(

        delete [] this->buffer;

      }

      this->buffer = nullptr;

      this->buffer_total_size = 0;

    }

    inline void _copy_values( const char *function, const char *file, const char* line ) {

      __INC_INDENT

      #ifdef __DEBUG_CALLSTACKFRAME__

        __DEBUG_CALLSTACKFRAME_OUT__ << __INDENT << "[begin][==member==][" << instance_id << "] inline void CallStackFrame::_copy_values( const char *function: '" << function << "', const char *file: '" << file << "', const char* line: '" << line << "' ) //** **" << std::endl;
        __DEBUG_CALLSTACKFRAME_OUT__ << __INDENT << std::endl;

      #endif

      this->_clear_values();

      uint32_t function_size = 0;
      uint32_t file_size = 0;
      uint32_t line_size = 0;

      uint32_t new_buffer_total_size = 0;
      uint64_t buffer_offset = 0;

      //Get the total of space needed in memory new_buffer_total_size. VERY IMPORTANT!!!
      if ( function ) {

        function_size = strlen( function );
        new_buffer_total_size = function_size + 1;

      }

      if ( file  ) {

        file_size = strlen( file );
        new_buffer_total_size += file_size + 1;

      }

      if ( line  ) {

        line_size = strlen( line );
        new_buffer_total_size += line_size + 1;

      }

      if ( new_buffer_total_size > 0 ) {


        //Create in the heap the linear memory buffer (NEW)
        //buffer = AllocatorCharTraits::allocate( allocatorConstChar, buffer_total_size ); //reinterpret_cast<char *>(

        this->buffer_total_size = new_buffer_total_size;
        this->buffer = new char[ buffer_total_size ];

        memset( this->buffer, 0, buffer_total_size ); //VERY IMPORTANT!!!. To avoid valgrind complaint avoid "Conditional jump or move depends on uninitialised value(s)"

        buffer_offset = 0;

        // for ( int i = 0; i < buffer_total_size; i++ ) {

        //   buffer[ i ] = 0;

        // }

      }

      if ( function_size > 0 ) {

        //memcpy( buffer, function, function_size );

        //char x = function[ 0 ];

        // for ( int i = 0; i < function_size; i++ ) {

        //   AllocatorCharTraits::construct( allocatorConstChar, buffer + i, function[ i ] );

        // }

        memcpy( buffer, function, function_size );

        //AllocatorConstCharTraits::construct( allocatorConstChar, buffer + strlen( function ), '\0' );

        this->function = (char*) buffer + buffer_offset;
        buffer_offset += ( function_size + 1 );

        //if ( size ) {

          //this->function = new char[ function_size + 1 ];
          //strcpy( this->function, function );

        //}

        // #ifdef __DEBUG_CALLSTACKFRAME__

        //   __INC_INDENT_W_OFFSET( 2 )

        //   __DEBUG_CALLSTACKFRAME_OUT__ << __INDENT << "*******************" << std::endl;
        //   __DEBUG_CALLSTACKFRAME_OUT__ << __INDENT << "Function size: " << size << std::endl;

        // #endif

      }

      if ( file_size ) {

        // for ( int i = 0; i < file_size; i++ ) {

        //   AllocatorCharTraits::construct( allocatorConstChar, buffer + buffer_offset + i, file[ i ] );

        // }

        memcpy( buffer + buffer_offset, file, file_size );

        this->file = (char*) buffer + buffer_offset;
        buffer_offset += ( file_size + 1 );

        // AllocatorConstCharTraits::construct( allocatorConstChar, &buffer + buffer_offset, (char*) file );

        //if ( size ) {

          //this->file = new char[ file_size + 1 ];
          //strcpy( this->file, file );

        //}

        // #ifdef __DEBUG_CALLSTACKFRAME__

        //   __DEBUG_CALLSTACKFRAME_OUT__ << __INDENT << "File size: " << size << std::endl;

        // #endif

      }

      if ( line_size > 0 ) {

        // for ( int i = 0; i < line_size; i++ ) {

        //   AllocatorCharTraits::construct( allocatorConstChar, buffer + buffer_offset + i, line[ i ] );

        // }

        memcpy( buffer  + buffer_offset, line, line_size );

        this->line = (char*) buffer + buffer_offset;
        buffer_offset += ( line_size + 1 );

        //AllocatorConstCharTraits::construct( allocatorConstChar, buffer + buffer_offset, line );
        // this->line = (char*) *(buffer + buffer_offset);
        // buffer_offset += ( line_size + 1 );

        //size = strlen( line );

        //if ( size ) {

          //this->line = new char[ line_size + 1 ];
          //strcpy( this->line, line );

        //}

        // #ifdef __DEBUG_CALLSTACKFRAME__

        //   __DEBUG_CALLSTACKFRAME_OUT__ << __INDENT << "Line size: " << size << std::endl;
        //   __DEBUG_CALLSTACKFRAME_OUT__ << __INDENT << "*******************" << std::endl;

        //   __DEC_INDENT_W_OFFSET( 2 )

        // #endif

      }

      // for ( int i = 0; i < buffer_total_size; i++ ) {

      //   const char temp = buffer[ i ];

      //   std::cout << temp <<  std::endl;

      // }

      #ifdef __DEBUG_CALLSTACKFRAME__

        __DEBUG_CALLSTACKFRAME_OUT__ << *this;

        __DEBUG_CALLSTACKFRAME_OUT__ << __INDENT << "[end][==member==][" << instance_id << "] inline void CallStackFrame::_copy_values( const char *function: '" << function << "', const char *file: '" << file << "', const char* line: '" << line << "' ) //** **" << std::endl << std::endl;

      #endif

      __DEC_INDENT

    }

    inline void _move_values( CallStackFrame& move ) {

      //std::swap( this->instance_id, move.instance_id );
      std::swap( this->function, move.function );
      std::swap( this->file, move.file );
      std::swap( this->line, move.line );
      std::swap( this->buffer, move.buffer );
      std::swap( this->buffer_total_size, move.buffer_total_size );

    }

  };

  std::ostream& operator<<( std::ostream& os, const CallStackFrame& call_stack_entry )
  {

    os << __INDENT << std::endl;

    __INC_INDENT_W_OFFSET( 2 )

    os << __INDENT << "*******************" << std::endl;
    os << __INDENT << "Instance: " << call_stack_entry.instance_id << std::endl;
    os << __INDENT << "Function: " << ( call_stack_entry.function ? call_stack_entry.function: "null" ) << " " << static_cast<void*>(call_stack_entry.function) << std::endl;
    os << __INDENT << "File: " << ( call_stack_entry.file ? call_stack_entry.file: "null" ) << " " << static_cast<void*>(call_stack_entry.file) << std::endl;
    os << __INDENT << "Line: " << ( call_stack_entry.line ? call_stack_entry.line: "null" ) << " " << static_cast<void*>(call_stack_entry.line) << std::endl;
    os << __INDENT << "*******************" << std::endl;
    os << __INDENT << std::endl;

    __DEC_INDENT_W_OFFSET( 2 )

    return os;

  }

}
