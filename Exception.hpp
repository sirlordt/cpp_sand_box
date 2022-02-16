#pragma once

#include "CallStackFrame.hpp"

#include "String.hpp"

#include "Vector.hpp"

#include "CommonUtilities.hpp"

namespace NSTD {

  #define __DEBUG_EXCEPTION__       true
  #define __DEBUG_EXCEPTION_OUT__   std::cout
  //#define __DEBUG_CALLSTACKFRAME_LIMIT__ 30

  struct Exception
  {
    using VectorCallStackFrame = NSTD::Vector<NSTD::CallStackFrame>;

    Exception() {

      __INC_INDENT

      instance_id = instance_id_count++;

      #ifdef __DEBUG_EXCEPTION__

        __DEBUG_EXCEPTION_OUT__ << __INDENT << "[->][++constructor++][" << instance_id << "] Exception::Exception() //** Default **" << std::endl << std::endl;

      #endif

      this->clear();

      __DEC_INDENT

      #ifdef __DEBUG_CALLSTACKFRAME_LIMIT__

        if ( instance_id >= __DEBUG_CALLSTACKFRAME_LIMIT__ ) {

          __DEBUG_EXCEPTION_OUT__ << __INDENT_W_OFFSET( 2 ) << std::endl;
          __DEBUG_EXCEPTION_OUT__ << __INDENT_W_OFFSET( 2 ) << "*******************" << std::endl;
          __DEBUG_EXCEPTION_OUT__ << __INDENT_W_OFFSET( 2 ) << "[warning] Terminating program Exception::Exception():" + __LINE__;
          __DEBUG_EXCEPTION_OUT__ << __INDENT_W_OFFSET( 2 ) << "*******************" << std::endl;
          __DEBUG_EXCEPTION_OUT__ << __INDENT_W_OFFSET( 2 ) << std::endl;

          std::terminate();

        }

      #endif

    }

    Exception( const char *thread_id,
               const char *thread_name,
               int signal,
               const char *code,
               const char *message,
               const char *at )
               //const VectorCallStackFrame& call_stack )
    {

      __INC_INDENT

      instance_id = instance_id_count++;

      #ifdef __DEBUG_EXCEPTION__

        char buffer[ 150 ];

        memset( buffer, 0, 150 );

        sprintf( buffer, "%s%i%s", "[begin][++constructor++][", instance_id, "] Exception::Exception( " );

        int left_indent = strlen( buffer );

        //NSTD::String _INDENT_BUFFER = __INDENT_W_OFFSET_( left_indent ); //.c_str();

        __DEBUG_EXCEPTION_OUT__ << __INDENT << buffer << "const char *thread_id: '" << thread_id << "'," << std::endl;

        __INC_INDENT_W_OFFSET( left_indent )

        __DEBUG_EXCEPTION_OUT__ << __INDENT << "const char *thread_name: '" << thread_name << "'," << std::endl;
        __DEBUG_EXCEPTION_OUT__ << __INDENT << "int signal: " << signal << "," << std::endl;
        __DEBUG_EXCEPTION_OUT__ << __INDENT << "const char *code:'" << code << "'," << std::endl;
        __DEBUG_EXCEPTION_OUT__ << __INDENT << "const char *message='" << message << "'," <<  std::endl;
        __DEBUG_EXCEPTION_OUT__ << __INDENT << "const char* at='" << at << "'," << std::endl << std::endl;
        //__DEBUG_EXCEPTION_OUT__ << _INDENT << "VectorCallStackFrame& call_stack=" << &call_stack << " ) //** **" << std::endl << std::endl;

        __DEC_INDENT_W_OFFSET( left_indent )

      #endif

      this->copy( thread_id,
                  thread_name,
                  signal,
                  code,
                  message,
                  at,
                  nullptr );

      #ifdef __DEBUG_EXCEPTION__

        memset( buffer, 0, 150 );

        sprintf( buffer, "%s%i%s", "[end][++constructor++][", instance_id, "] Exception::Exception( " );

        left_indent = strlen( buffer );

        //_INDENT_BUFFER = __INDENT_W_OFFSET_( left_indent ); //.c_str();

        __DEBUG_EXCEPTION_OUT__ << __INDENT << buffer << "const char *thread_id: '" << thread_id << "'," << std::endl ;

        __INC_INDENT_W_OFFSET( left_indent )

        __DEBUG_EXCEPTION_OUT__ << __INDENT << "const char *thread_name: '" << thread_name << "'," << std::endl;
        __DEBUG_EXCEPTION_OUT__ << __INDENT << "int signal: " << signal << "," << std::endl;
        __DEBUG_EXCEPTION_OUT__ << __INDENT << "const char *code:'" << code << "'," << std::endl;
        __DEBUG_EXCEPTION_OUT__ << __INDENT << "const char *message='" << message << "'," <<  std::endl;
        __DEBUG_EXCEPTION_OUT__ << __INDENT << "const char* at='" << at << "'," << std::endl << std::endl;
        //__DEBUG_EXCEPTION_OUT__ << __INDENT_W_OFFSET( left_indent ) << "VectorCallStackFrame& call_stack=" << &call_stack << " ) //** **" << std::endl << std::endl;

        __DEC_INDENT_W_OFFSET( left_indent )


      #endif

      __DEC_INDENT

    }

    //Copy contructor pointer
    Exception( const Exception *copy ) { //:CallStackFrame( copy.function, copy.file, copy.line ) { // = default;

      __INC_INDENT

      instance_id = instance_id_count++;

      #ifdef __DEBUG_EXCEPTION__

        __DEBUG_EXCEPTION_OUT__ << __INDENT << "[begin][++constructor++][" << instance_id << "] Exception::Exception( const Exception *copy ) //** Copy Const Pointer **" << std::endl << std::endl;

      #endif

      if ( copy ) {

        this->copy( copy->thread_id,
                    copy->thread_name,
                    copy->signal,
                    copy->code,
                    copy->message,
                    copy->at,
                    &copy->call_stack );

      }

      #ifdef __DEBUG_EXCEPTION__

        __DEBUG_EXCEPTION_OUT__ << __INDENT << "[end][++constructor++][" << instance_id << "] Exception::Exception( const Exception *copy ) //** Copy Const Pointer **" << std::endl << std::endl;

      #endif

      __DEC_INDENT

    }

    //Copy contructor
    Exception( const Exception &copy ) { //:CallStackFrame( copy.function, copy.file, copy.line ) { // = default;

      __INC_INDENT

      instance_id = instance_id_count++;

      #ifdef __DEBUG_EXCEPTION__

        __DEBUG_EXCEPTION_OUT__ << __INDENT << "[begin][++constructor++][" << instance_id << "] Exception::Exception( const Exception &copy ) //** Copy Const Reference **" << std::endl << std::endl;

      #endif

      this->copy( copy.thread_id,
                  copy.thread_name,
                  copy.signal,
                  copy.code,
                  copy.message,
                  copy.at,
                  &copy.call_stack );

      #ifdef __DEBUG_EXCEPTION__

        __DEBUG_EXCEPTION_OUT__ << __INDENT << "[end][++constructor++][" << instance_id << "] Exception::Exception( const Exception &copy ) //** Copy Const Reference **" << std::endl << std::endl;

      #endif

      __DEC_INDENT

    }

    //Move contructor
    Exception( Exception &&move ) noexcept { // = default;

      __INC_INDENT

      instance_id = instance_id_count++;

      #ifdef __DEBUG_EXCEPTION__

        __DEBUG_EXCEPTION_OUT__ << __INDENT << "[begin][++constructor++][" << instance_id << "] Exception::Exception( Exception &&move ) noexcept //** Move **" << std::endl << std::endl;

      #endif

      this->move( move );

      #ifdef __DEBUG_EXCEPTION__

        __DEBUG_EXCEPTION_OUT__ << __INDENT << "[end][++constructor++][" << instance_id << "] Exception::Exception( Exception &&move ) noexcept //** Move **" << std::endl << std::endl;

      #endif

      __DEC_INDENT

    }

    ~Exception() {

      __INC_INDENT

      #ifdef __DEBUG_EXCEPTION__

        __DEBUG_EXCEPTION_OUT__ << __INDENT << "[begin][~~destructor~~][" << instance_id << "] Exception::~Exception() noexcept //** **" << std::endl;

        if ( this->thread_id || this->thread_name || this->code || this->message || this->call_stack.size() > 0 ) {

          __DEBUG_EXCEPTION_OUT__ << *this;

        }
        else {

          __DEBUG_EXCEPTION_OUT__ << std::endl;

        }

      #endif

      this->clear();

      #ifdef __DEBUG_EXCEPTION__

        __DEBUG_EXCEPTION_OUT__ << __INDENT << "[end][~~destructor~~][" << instance_id << "] Exception::~Exception() noexcept //** **" << std::endl << std::endl;

      #endif

      __DEC_INDENT

    }

    //Copy assign operator=
    Exception& operator=( const Exception &copy ) noexcept {  //  = default;

      __INC_INDENT

      #ifdef __DEBUG_EXCEPTION__

        __DEBUG_EXCEPTION_OUT__ << __INDENT << "[begin][**operator=**][" << instance_id << "] Exception::Exception& operator=( Exception& copy: " << &copy << " ) //** Copy **" << std::endl << std::endl;

      #endif

      this->copy( copy.thread_id,
                  copy.thread_name,
                  copy.signal,
                  copy.code,
                  copy.message,
                  copy.at,
                  &copy.call_stack );

      #ifdef __DEBUG_EXCEPTION__

        __DEBUG_EXCEPTION_OUT__ << __INDENT << "[end][**operator=**][" << instance_id << "] Exception::Exception& operator=( Exception& copy: " << &copy << " ) //** Copy **" << std::endl << std::endl;

      #endif

      __DEC_INDENT

      return *this;

    }

    //Move assign operator=
    Exception& operator=( Exception&& move ) noexcept {

      __INC_INDENT

      #ifdef __DEBUG_EXCEPTION__

        __DEBUG_EXCEPTION_OUT__ << __INDENT << "[begin][**operator=**][" << instance_id << "] Exception::Exception& operator=( Exception&& move: " << &move << " ) //** Move **" << std::endl << std::endl;

      #endif
      // function = call_stack_entry.function;
      // file = call_stack_entry.file;
      // line = call_stack_entry.line;
      this->move( move );

      #ifdef __DEBUG_EXCEPTION__

        __DEBUG_EXCEPTION_OUT__ << __INDENT << "[end][**operator=**][" << instance_id << "] Exception::Exception& operator=( Exception&& move: " << &move << " ) //** Move **" << std::endl << std::endl;

      #endif

      __DEC_INDENT

      return *this;

    }

    std::any operator[]( const char *name ) const {

      std::any result;

      if ( strcmp( name, "thread_id"  ) == 0 ) {

        result = this->thread_id;

      }
      else if ( strcmp( name, "thread_name" ) == 0 ) {

        result = this->thread_name;

      }
      else if ( strcmp( name, "signal" ) == 0 ) {

        result = this->signal;

      }

      return result;

    }

    // std::any& operator []( const char *name ) {

    //   //std::any& result;

    //   if ( strcmp( name, "thread_id" ) == 0 ) {

    //     return &this->thread_id;

    //   }
    //   else if ( strcmp( name, "thread_name" ) == 0 ) {

    //     return &this->thread_name;

    //   }
    //   else if ( strcmp( name, "signal" ) == 0 ) {

    //     result = this->signal;

    //   }

    //   return result;

    // }

    friend std::ostream& operator<<( std::ostream& os, const Exception& exception );

  private:

    inline static uint32_t instance_id_count = 0;

    u_int32_t instance_id = 0;

    char *thread_id = nullptr;
    char *thread_name = nullptr;
    int signal = -1;
    char *code = nullptr;
    char *message = nullptr;
    char *at = nullptr;
    VectorCallStackFrame call_stack;

    inline void clear() {

      if ( thread_id ) {

        delete []thread_id;

      }

      thread_id = nullptr;

      if ( thread_name ) {

        delete []thread_name;

      }

      thread_name = nullptr;

      signal = 0;

      if ( code ) {

        delete []code;

      }

      code = nullptr;

      if ( message ) {

        delete []message;

      }

      message = nullptr;

      if ( at ) {

        delete []at;

      }

      at = nullptr;


    }

    inline void copy( const char *thread_id,
                      const char *thread_name,
                      int signal,
                      const char *code,
                      const char *message,
                      const char *at,
                      const VectorCallStackFrame *call_stack ) {

      __INC_INDENT

      int size = 0;

      #ifdef __DEBUG_EXCEPTION__

        char buffer[ 150 ];

        memset( buffer, 0, 150 );

        sprintf( buffer, "%s%i%s", "[begin][==member==][", instance_id, "] inline void Exception::copy( " );

        int left_indent = strlen( buffer );

        __DEBUG_EXCEPTION_OUT__ << __INDENT << buffer << "const char *thread_id: '" << thread_id << "'," << std::endl ;

        __INC_INDENT_W_OFFSET( left_indent )

        __DEBUG_EXCEPTION_OUT__ << __INDENT << "const char *thread_name: '" << thread_name << "'," << std::endl;
        __DEBUG_EXCEPTION_OUT__ << __INDENT << "int signal: " << signal << "," << std::endl;
        __DEBUG_EXCEPTION_OUT__ << __INDENT << "const char *code:'" << code << "'," << std::endl;
        __DEBUG_EXCEPTION_OUT__ << __INDENT << "const char *message='" << message << "'," <<  std::endl;
        __DEBUG_EXCEPTION_OUT__ << __INDENT << "const char* at='" << at << "'," << std::endl;
        __DEBUG_EXCEPTION_OUT__ << __INDENT << "VectorCallStackFrame& call_stack=" << &call_stack << " ) //** **" << std::endl << std::endl;

        __DEC_INDENT_W_OFFSET( left_indent )

      #endif

      if ( thread_id ) {

        size = strlen( thread_id );

        if ( size ) {

          this->thread_id = new char[ size + 1 ];
          strcpy( this->thread_id, thread_id );

        }

        #ifdef __DEBUG_EXCEPTION__

          __INC_INDENT_W_OFFSET( 2 )

          __DEBUG_EXCEPTION_OUT__ << __INDENT << "*******************" << std::endl;
          __DEBUG_EXCEPTION_OUT__ << __INDENT << "Thread Id size: " << size << std::endl;

        #endif

      }

      if ( thread_name ) {

        size = strlen( thread_name );

        if ( size ) {

          this->thread_name = new char[ size + 1 ];
          strcpy( this->thread_name, thread_name );

        }

        #ifdef __DEBUG_EXCEPTION__

          __DEBUG_EXCEPTION_OUT__ << __INDENT << "Thread Name size: " << size << std::endl;

        #endif

      }

      this->signal = signal;

      if ( code ) {

        size = strlen( code );

        if ( size ) {

          this->code = new char[ size + 1 ];
          strcpy( this->code, code );

        }

        #ifdef __DEBUG_EXCEPTION__

          __DEBUG_EXCEPTION_OUT__ << __INDENT << "Code size: " << size << std::endl;

        #endif

      }

      if ( message ) {

        size = strlen( message );

        if ( size ) {

          this->message = new char[ size + 1 ];
          strcpy( this->message, message );

        }

        #ifdef __DEBUG_EXCEPTION__

          __DEBUG_EXCEPTION_OUT__ << __INDENT << "Message size: " << size << std::endl;

        #endif

      }

      if ( at ) {

        size = strlen( at );

        if ( size ) {

          this->at = new char[ size + 1 ];
          strcpy( this->at, at );

        }

        #ifdef __DEBUG_EXCEPTION__

          __DEBUG_EXCEPTION_OUT__ << __INDENT << "At size: " << size << std::endl;

        #endif

      }

      // if ( code ) {

      //   size = strlen( code );

      //   if ( size ) {

      //     this->code = new char[ size + 1 ];
      //     strcpy( this->code, code );

      //   }


      // }

      if ( call_stack &&
           call_stack->size() ) {

        this->call_stack = *call_stack;

      }

      #ifdef __DEBUG_EXCEPTION__

        __DEBUG_EXCEPTION_OUT__ << __INDENT << "Call Stack size: " << this->call_stack.size() << std::endl;
        __DEBUG_EXCEPTION_OUT__ << __INDENT << "*******************" << std::endl;

        __DEC_INDENT_W_OFFSET( 2 )

      #endif


      #ifdef __DEBUG_EXCEPTION__

        //__DEBUG_CALLSTACKFRAME_OUT___ << std::endl;

        __DEBUG_EXCEPTION_OUT__ << *this;

        memset( buffer, 0, 150 );

        sprintf( buffer, "%s%i%s", "[end][==member==][", instance_id, "] inline void Exception::copy( " );

        left_indent = strlen( buffer );

        __INC_INDENT_W_OFFSET( 2 )

        __DEBUG_EXCEPTION_OUT__ << __INDENT << buffer << "const char *thread_id: '" << thread_id << "'," << std::endl ;
        __DEBUG_EXCEPTION_OUT__ << __INDENT << "const char *thread_name: '" << thread_name << "'," << std::endl;
        __DEBUG_EXCEPTION_OUT__ << __INDENT << "int signal: " << signal << "," << std::endl;
        __DEBUG_EXCEPTION_OUT__ << __INDENT << "const char *code:'" << code << "'," << std::endl;
        __DEBUG_EXCEPTION_OUT__ << __INDENT << "const char *message='" << message << "'," <<  std::endl;
        __DEBUG_EXCEPTION_OUT__ << __INDENT << "const char* at='" << at << "'," << std::endl;
        __DEBUG_EXCEPTION_OUT__ << __INDENT << "VectorCallStackFrame& call_stack=" << &call_stack << " ) //** **" << std::endl << std::endl;

        //__DEBUG_EXCEPTION_OUT__ << __INDENT << "[end][==member==][" << instance_id << "] inline void Exception::copy( const char *thread_id: '" << thread_id << "', const char *thread_name: '" << thread_name << ", int signal: '" << signal << ", const char *code:'" << code << "', const char *message='" << message << "', const char* at='" << at << "', VectorCallStackFrame& call_stack=" << &call_stack << " ) //** **" << std::endl;

        __DEC_INDENT_W_OFFSET( 2 )

      #endif

      __DEC_INDENT

    }

    inline void move( Exception& move ) {

      std::swap( this->thread_id, move.thread_id );
      std::swap( this->thread_name, move.thread_name );
      std::swap( this->signal, move.signal );
      std::swap( this->code, move.code );
      std::swap( this->message, move.message );
      std::swap( this->at, move.at );

      this->call_stack = std::move( move.call_stack );

    }

  };

  std::ostream& operator<<( std::ostream& os, const Exception& exception )
  {

    __INC_INDENT_W_OFFSET( 2 )

    os << __INDENT << std::endl;
    os << __INDENT << "*******************" << std::endl;
    os << __INDENT << "Thread Id: " << ( exception.thread_id ? exception.thread_id: "null" ) << " " << static_cast<void*>(exception.thread_id) << std::endl;
    os << __INDENT << "File: " << ( exception.thread_name ? exception.thread_name: "null" ) << " " << static_cast<void*>(exception.thread_name) << std::endl;
    os << __INDENT << "Code: " << ( exception.code ? exception.code: "null" ) << " " << static_cast<void*>(exception.code) << std::endl;
    os << __INDENT << "Message: " << ( exception.message ? exception.message: "null" ) << " " << static_cast<void*>(exception.message) << std::endl;
    os << __INDENT << "At: " << ( exception.at ? exception.at: "null" ) << " " << static_cast<void*>(exception.at) << std::endl;
    os << __INDENT << "Call stack: " << std::endl;

    __INC_INDENT_W_OFFSET( 2 )

    for ( const CallStackFrame& callStackFrame: exception.call_stack ) {

      os << __INDENT << callStackFrame;

    }

    __DEC_INDENT_W_OFFSET( 2 )

    //os << exception.call_stack;

    os << __INDENT << "*******************" << std::endl;
    os << __INDENT << std::endl;

    __DEC_INDENT_W_OFFSET( 2 )

    return os;

  }


}
