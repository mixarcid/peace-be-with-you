#include <Dialogue.hpp>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static int Person_type_tag = 0;

/* This might not be necessary, or it might not be enough. Don't leave
   these values unallocated: each needs to be filled with a real person. */

static s7_pointer make_Person(s7_scheme* sc, s7_pointer args, Person* person) {
  Person* pip; // probs don't need this w/ above Person ponter...
  pip = (Person*)malloc(sizeof(Person));
  // CREATE ALL MEMBERS!
  // pip->name = s7_real
  if (s7_cdr(args) != s7_nil(sc)) {
    pip->temparament = s7_cadr(args);
  }
  else pip->temparament = s7_nil(sc);
  return(s7_make_object(sc, Person_type_tag, (void*)pip));
}

// GET functions for Person struct IN C

static s7_pointer name(s7_scheme* sc, s7_pointer args) {
  Person* pip;
  pip = (Person*)s7_object_value(s7_car(args));
  return(pip->name); // does this work? Does pip just type itself correctly?
  // or is it return(s7_string(s7_name_to_value(sc, "name")));  ?
}

static s7_pointer mom(s7_scheme* sc, s7_pointer args) {
  Person* pip;
  pip = (Person*)s7_object_value(s7_car(args));
  return(pip->mom); // I really question whether this will type correctly…
}

static s7_pointer dad(s7_scheme* sc, s7_pointer args) {
  Person* pip;
  pip = (Person*)s7_object_value(s7_car(args));
  return(pip->dad);
}

static s7_pointer flags(s7_scheme* sc, s7_pointer args) {
  Person* pip;
  pip = (Person*)s7_object_value(s7_car(args));
  return(pip->flags); // this should be an int: "001010011" etc.
}

static s7_pointer relationship(s7_scheme* sc, s7_pointer args, Person* other) {
  Person* pip;
  pip = (Person*)s7_object_value(s7_car(args));
  return(pip->Relationships[*other]); // no current way of accessing a person from the vector
}

static s7_pointer temparament(s7_scheme* sc, s7_pointer args) {
  Person* pip;
  pip = (Person*)s7_object_value(s7_car(args));
  // do I want (if (< courage 10)) or (if (temparsment[1] < 10))? (Obv not in that syntax).
  // s7_pointer tip;
  
  return(/* make a list with s7_make_real(sc, pip->Temparament) */);
}

// SET functions for Person struct IN C

static s7_pointer set_name(s7_scheme* sc, s7_pointer args) {
  s7_define_variable(s7, "zavut", s7_make_string(s7, nameGen()));
  Person* pip;
  pip = (Person*)s7_object_value(s7_car(args));
  pip->name = // what?
}

static s7_pointer set_flags(s7_scheme* sc, s7_pointer args) {
  Person* pip;
  pip = (Person*)s7_object_value(s7_car(args));
  pip->flags = s7_real(s7_cadr(args));
  return(s7_cadr(args));
}
