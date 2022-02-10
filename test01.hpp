#pragma once

#include <cstring>
#include <string>
#include <array>

namespace Test01
{

struct Address {

  char *name = nullptr;
  char *street = nullptr;
  char *zip_code = nullptr;
  char *county = nullptr;
  char *state = nullptr;
  char *country = nullptr;

  explicit Address( const char *name, const char *street, const char *zip_code, const char *county, const char *state, const char *country ) {

    int size = strlen( name );
    this->name = new char[ size + 1 ];
    strcpy( this->name, name );

    size = strlen( street );
    this->street = new char[ size + 1 ];
    strcpy( this->street, street );

    size = strlen( zip_code );
    this->zip_code = new char[ size + 1 ];
    strcpy( this->zip_code, zip_code );

    size = strlen( county );
    this->county = new char[ size + 1 ];
    strcpy( this->county, county );

    size = strlen( state );
    this->state = new char[ size + 1 ];
    strcpy( this->state, state );

    size = strlen( country );
    this->country = new char[ size + 1 ];
    strcpy( this->country, country );

  }

  explicit Address( const Address *address ) {

    //address->country = nullptr;

    int size = strlen( address->name );
    this->name = new char[ size + 1 ];
    strcpy( this->name, address->name );

    size = strlen( address->street );
    this->street = new char[ size + 1 ];
    strcpy( this->street, address->street );

    size = strlen( address->zip_code );
    this->zip_code = new char[ size + 1 ];
    strcpy( this->zip_code, address->zip_code );

    size = strlen( address->county );
    this->county = new char[ size + 1 ];
    strcpy( this->county, address->county );

    size = strlen( address->state );
    this->state = new char[ size + 1 ];
    strcpy( this->state, address->state );

    size = strlen( address->country );
    this->country = new char[ size + 1 ];
    strcpy( this->country, address->country );

  }

  explicit Address( const Address &address ) {

    //address->country = nullptr;
    int size = strlen( address.name );
    this->name = new char[ size + 1 ];
    strcpy( this->name, address.name );

    size = strlen( address.street );
    this->street = new char[ size + 1 ];
    strcpy( this->street, address.street );

    size = strlen( address.zip_code );
    this->zip_code = new char[ size + 1 ];
    strcpy( this->zip_code, address.zip_code );

    size = strlen( address.county );
    this->county = new char[ size + 1 ];
    strcpy( this->county, address.county );

    size = strlen( address.state );
    this->state = new char[ size + 1 ];
    strcpy( this->state, address.state );

    size = strlen( address.country );
    this->country = new char[ size + 1 ];
    strcpy( this->country, address.country );

  }

  ~Address() {

    std::cout << "Address::~Address Name: " << name << std::endl;

    if ( name ) {

      delete[] name;

    }

    if ( street ) {

      delete[] street;

    }

    if ( zip_code ) {

      delete[] zip_code;

    }

    if ( county ) {

      delete[] county;

    }

    if ( state ) {

      delete[] state;

    }

    if ( country ) {

      delete[] country;

    }

  }

  void set_name( const char *name ) {

    if ( this->name ) {

      delete[] this->name;

    }

    int size = strlen( name );
    this->name = new char[ size + 1 ];
    strcpy( this->name, name );

  }

  const char *get_name() {

    return this->name;

  }

};

struct Person
{

  char *id = nullptr;
  char *first_name = nullptr;
  char *last_name = nullptr;
  int age = 0;
  const Address *address = nullptr;

  explicit Person( const char *id, const char *first_name, const char *last_name, const int age, const Address *address ) {

    int size = strlen( id );
    this->id = new char[ size + 1 ];
    strcpy( this->id, id );

    size = strlen( first_name );
    this->first_name = new char[ size + 1 ];
    strcpy( this->first_name, first_name );

    size = strlen( last_name );
    this->last_name = new char[ size + 1 ];
    strcpy( this->last_name, last_name );

    this->age = age;

    //address->country = nullptr;

    if ( address ) {

      this->address = new Address( *address );

    }

    //this->address->country = nullptr;

  }

  ~Person() {

    std::cout << "Person::~Person first_name: " << first_name << std::endl;

    if ( id ) {

      delete[] id;

    }

    if ( first_name ) {

      delete[] first_name;

    }

    if ( last_name ) {

      delete[] last_name;

    }

    if ( address ) {

      delete address;

    }

    age = 0;

  }


//   void clearId() {

//     delete[] this->id;
//     this->id = nullptr;

//   }

  const char* getId() const {

    return this->id;

  }


  inline static std::array<std::string, 4> _FIELDS
  {
    "Id",
    "FirstName",
    "LastName",
    "Age"
  };

};

}
