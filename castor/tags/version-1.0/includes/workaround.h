// Castor : Logic Programming Library for C++
// Copyright © 2007 Roshan Naik (naikrosh@gmail.com)

#ifndef CASTOR_WORKAROUND_H
#define CASTOR_WORKAROUND_H 1

// const ref arguments in templates not very well supported in BCB
#ifdef __BCPLUSPLUS__
#define CREF  const &
#else
#define CREF
#endif

#endif //#ifndef CASTOR_WORKAROUND_H
