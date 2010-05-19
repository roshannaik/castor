// Castor : Logic Programming Library
// Copyright © 2008 Roshan Naik (roshan@mpprogramming.com).
// This software is goverened by the MIT license (http://www.opensource.org/licenses/mit-license.php).

#if !defined CASTOR_COROUTINE_H
#define CASTOR_COROUTINE_H 1

namespace castor {

class Coroutine {
protected:
  int co_entryPt;
public:
  Coroutine() : co_entryPt(0)
  { }
};


// macros for implementing relations imperatively (as coroutines)
#define co_begin()    switch (co_entryPt) { case 0:
#define co_yield(x)   { co_entryPt = (x)? __LINE__ : -1; return (co_entryPt==__LINE__); case __LINE__: ; }
#define co_return(x)  { co_entryPt = -1; return (x); }
#define co_end()      break; } co_entryPt = -1; return false;



} // namespace castor
#endif
