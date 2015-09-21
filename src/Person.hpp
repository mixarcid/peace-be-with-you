//#include <michael's stuff>
#include <String.h>

enum PersonFlags {
  ALIVE = 0x01,
  MALE = 0x02,
  OTHER_FLAG = 0x04
  // etc.
};

enum RelationshipFlags {
  MARRIED = 0x01,
  DATING = 0x02,
  CRUSH = 0x04
};

struct Person;

struct Temparament {
  i8 courage;
  i8 loudness;
  i8 intelligence;
  i8 maturity;
};

struct Relationship {
  Person* person;
  Temparament mood;
  RelationshipFlags flags;
  Relationship(Person* _person,
               Temparament _mood,
               RelationshipFlags _flags)
                : person(_person),
                  mood(_mood),
                  flags(_flags) {}
};

struct Person {
  String name;
  Person* mom, dad;
  PersonFlags flags;
  Array<Person> children;
  Array<Relationship> relationships;
  Temparament temparament;
  Person(String _name,
        Person* _mom, _dad,
        PersonFlags _flags,
        Array<Relationship> _relationships,
        Temparament _temparament)
          : name(_name),
            mom(_mom),
            dad(_dad),
            children(_children),
            relationships(_relationships),
            temparament(_temparament) {}
};
