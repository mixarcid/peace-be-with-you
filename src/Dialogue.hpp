#include <Person.h>

#include "s7.h"
#include "Scripting.cpp"

static int Person_type_tag = 0;

static s7_pointer make_Person(s7_scheme* sc, s7_pointer args, Person* person);

// GET functions for Person struct IN C

static s7_pointer name(s7_scheme* sc, s7_pointer args);

static s7_pointer mom(s7_scheme* sc, s7_pointer args);

static s7_pointer dad(s7_scheme* sc, s7_pointer args);

static s7_pointer flags(s7_scheme* sc, s7_pointer args);

static s7_pointer relationship(s7_scheme* sc, s7_pointer args, Person* other);

static s7_pointer temparament(s7_scheme* sc, s7_pointer args);

// SET functions for Person struct IN C

static s7_pointer set_name(s7_scheme* sc, s7_pointer args);

static s7_pointer set_flags(s7_scheme* sc, s7_pointer args);
