#include <cstdarg>
#include "String.hpp"
#include "Containers.hpp"

NAMESPACE {

  /*
  template <typename Char,
	    typename Alloc,
	    u8 BuffSize>
  struct BasicString {

    typedef u32 SizeType;

    SizeType length;
    union {
      Char buffer[BuffSize];
      Char* contents;
    };

    BasicString()
      : length(0),
	contents(NULL) {}

    BasicString(const Char* str)
      : length(strlen(str)) {
      debugAssert(str,
		  "Why are you initializing a BasicString"
		  " with the NULL pointer?");
      if (length < BuffSize) {
	strcpy(buffer, str);
      } else {
	contents = (Char*) Alloc::malloc((length+1)*sizeof(Char));
	strcpy(contents, str);
      }
    }
    
    BasicString(const BasicString& str)
      : length(str.length) {
      if (length < BuffSize) {
	strcpy(buffer, str.buffer);
      } else {
	contents = (Char*) Alloc::malloc((length+1)*sizeof(Char));
	strcpy(contents, str.contents);
      }
    }
    
    BasicString(BasicString&& str)
      : length(str.length) {
      if (length < BuffSize) {
	strcpy(buffer, str.buffer);
      } else {
	contents = str.contents;
        str.length = 0;
      }
    }

    ~BasicString() {
      if (length >= BuffSize) {
	Alloc::free((void*)contents);
      }
    }

    const Char* c_str() const {
      if (length < BuffSize) {
	return buffer;
      } else {
	return contents;
      }
    }

    BasicString& operator=(const BasicString& b) {
      BasicString tmp(b);
      *this = std::move(tmp);
      return *this;
    }

    BasicString& operator=(BasicString&& b) {
      length = b.length;
      if (length < BuffSize) {
	strcpy(buffer, b.buffer);
      } else {
	free(contents);
	contents = b.contents;
	b.length = 0;
      }
    }

    BasicString& operator+=(const BasicString& b) {
      SizeType new_len = length + b.length;
      if (new_len < BuffSize) {
	strcpy(buffer+length, b.buffer);
      } else {
	if (length < BuffSize) {
	  contents = (Char*) Alloc::malloc((new_len+1)*sizeof(Char));
	  strcpy(contents, buffer);
	} else {
	  contents = (Char*) Alloc::realloc(contents, (new_len+1)*sizeof(Char));
	}
	strcpy(contents+length, b.c_str());
      }
      length = new_len;
      return *this;
    }
    
  };

template <typename Char,
	    typename Alloc,
	    u8 BuffSize>
BasicString<Char,Alloc,BuffSize>
operator+(const BasicString<Char,Alloc,BuffSize>& a,
	  const BasicString<Char,Alloc,BuffSize>& b) {
  BasicString<Char,Alloc,BuffSize> ret;
  ret.length = a.length + b.length;
  if (ret.length < BuffSize) {
    strcpy(ret.buffer,a.buffer);
    strcpy(ret.buffer+a.length,b.buffer);
  } else {
    ret.contents = (Char*) Alloc::malloc((ret.length+1)*sizeof(Char));
    strcpy(ret.contents,a.c_str());
    strcpy(ret.contents+a.length,b.c_str());
  }
  return ret;
}

template <typename Char,
	    typename Alloc,
	    u8 BuffSize>
bool operator==(const BasicString<Char,Alloc,BuffSize>& a,
		const BasicString<Char,Alloc,BuffSize>& b) {
  if (a.length != b.length) {
    return false;
  } else {
    return memcmp(a.c_str(),b.c_str(),a.length) == 0;
  }
}

template <typename Char,
	    typename Alloc,
	    u8 BuffSize>
bool operator!=(const BasicString<Char,Alloc,BuffSize>& a,
		const BasicString<Char,Alloc,BuffSize>& b) {
  return !(a==b);
}

typedef BasicString<char,GameAllocator,8> MyString;
  */

  namespace str {
    
    String vformat (const char *fmt, va_list ap)
    {
      // Allocate a buffer on the stack that's big enough for us almost
      // all the time.  Be prepared to allocate dynamically if it doesn't fit.
      size_t size = 1024;
      char stackbuf[1024];
      Array<char> dynamicbuf;
      char *buf = &stackbuf[0];

      while (1) {
	// Try to vsnprintf into our buffer.
	int needed = vsnprintf (buf, size, fmt, ap);
	// NB. C99 (which modern Linux and OS X follow) says vsnprintf
	// failure returns the length it would have needed.  But older
	// glibc and current Windows return -1 for failure, i.e., not
	// telling us how much was needed.

	if (needed <= (int)size && needed >= 0) {
	  // It fit fine so we're done.
	  return String (buf, (size_t) needed);
	}

	// vsnprintf reported that it wanted to write more characters
	// than we allotted.  So try again using a dynamic buffer.  This
	// doesn't happen very often if we chose our initial size well.
	size = (needed > 0) ? (needed+1) : (size*2);
	dynamicbuf.reserve (size);
	buf = &dynamicbuf[0];
      }
    }

    String format (const char *fmt, ...)
    {
      va_list ap;
      va_start (ap, fmt);
      String buf = vformat (fmt, ap);
      va_end (ap);
      return buf;
    }
    
  }

}
