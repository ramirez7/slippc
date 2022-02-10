#ifndef TESTS_H_
#define TESTS_H_

#include <string>

#include "util.h"
#include "parser.h"
#include "analyzer.h"
#include "compressor.h"

#ifdef _WIN32
#include <Windows.h> //sleep()
#else
#include <unistd.h> //sleep()
#endif

#include <algorithm>

namespace slip {

// ANSI color codes don't work on Windows (I think?)
#ifdef _WIN32
    #define BLK ""
    #define RED ""
    #define GRN ""
    #define YLW ""
    #define BLU ""
    #define MGN ""
    #define CYN ""
    #define WHT ""
    #define CRT ""
    #define BLN ""
#else
    #define BLK "\u001b[30m"
    #define RED "\u001b[31m"
    #define GRN "\u001b[32m"
    #define YLW "\u001b[33m"
    #define BLU "\u001b[34m"
    #define MGN "\u001b[35m"
    #define CYN "\u001b[36m"
    #define WHT "\u001b[37m"
    #define CRT "\u001b[41m"
    #define BLN "\u001b[0m"
#endif

typedef struct _test_suite {
  const char* name;
  unsigned p = 0;  //pass
  unsigned w = 0;  //warn
  unsigned f = 0;  //fail
  unsigned e = 0;  //error
  unsigned t = 0;  //total
  _test_suite* next;
} test_suite;

static unsigned __tests_total  = 0;
static unsigned __tests_passed = 0;
static unsigned __tests_warned = 0;
static unsigned __tests_failed = 0;
static unsigned __tests_errors = 0;
static test_suite* suites = NULL, *tail = NULL;
static test_suite* s;

#define TSUITE(tag) \
  std::cout << "----------\nTesting suite " << CYN << (tag) << BLN << std::endl; \
  s = (test_suite*)calloc(1,sizeof(test_suite)); \
  s->name = (tag); \
  if(!tail) {suites     = s;} \
  else      {tail->next = s;} \
  tail = s;

#define TPASS() \
  std::cout << "\r " << GRN << " pass\n" << BLN; \
  ++__tests_total; \
  ++__tests_passed; \
  ++(tail->t); \
  ++(tail->p);
#define TWARN() \
  std::cout << "\r " << YLW << " warn\n" << BLN; \
  ++__tests_total; \
  ++__tests_warned; \
  ++(tail->t); \
  ++(tail->w);
#define TFAIL() \
  std::cout << "\r " << RED << " fail\n" << BLN; \
  ++__tests_total; \
  ++__tests_failed; \
  ++(tail->t); \
  ++(tail->f);
#define TERR(e) \
  std::cout << "\r " << CRT << "error\n" << BLN; \
  ++__tests_total; \
  ++__tests_errors; \
  ++(tail->t); \
  ++(tail->e);

#define ASSERT(name,expr,onfail) \
  std::cout << "       " << (name) << BLN; \
  try { \
    if ((expr)) { TPASS(); } else { TFAIL(); std::cout << "    " << RED << onfail << BLN << " (Asserted: " << RED << #expr << BLN << ")" << std::endl;} \
  } catch (const std::exception& e) { TERR(e); }

#define SUGGEST(name,expr,onfail) \
  std::cout << "       " << (name) << BLN; \
  try { \
    if ((expr)) { TPASS(); } else { TWARN(); std::cout << "    " << YLW << onfail << BLN << " (Suggested: " << YLW << #expr << BLN << ")" << std::endl;} \
  } catch (const std::exception& e) { TERR(e); }

#define TESTRESULTS() \
  std::cout << "----------TEST SUMMARY----------" << std::endl; \
  std::cout << GRN << "  " << __tests_passed << "/" << __tests_total << " tests passed" << " (" << ((100.0*__tests_passed)/(__tests_passed+__tests_failed+__tests_warned+__tests_errors))<< "%)" << BLN; \
  if (__tests_warned) { \
    std::cout << " with " << YLW << __tests_warned << " warning" << (__tests_warned==1 ? "" : "s") << BLN; \
    if (__tests_failed) { \
      std::cout << " and " << RED << __tests_failed << " failure" << (__tests_failed==1 ? "" : "s") << BLN; \
    } \
    if (__tests_errors) { \
      std::cout << " and " << CRT << __tests_errors << " error" << (__tests_errors==1 ? "" : "s") << BLN; \
    } \
  } else if (__tests_failed) { \
    std::cout << " with " << RED << __tests_failed << " failure" << (__tests_failed==1 ? "" : "s") << BLN; \
    if (__tests_errors) { \
      std::cout << " and " << CRT << __tests_errors << " error" << (__tests_errors==1 ? "" : "s") << BLN; \
    } \
  } else if (__tests_errors) { \
    std::cout << " with " << CRT << __tests_errors << " error" << (__tests_errors==1 ? "" : "s") << BLN; \
  } \
  std::cout << std::endl; \
  for(test_suite* s = suites; s; ) { \
    printf("    %s%30s%s: %3u test%s passed (%3.0f%%)",s->e ? CRT : s->f ? RED : s->w ? YLW : GRN,s->name,BLN,s->p,s->p==1 ? " " : "s",(100.0f*s->p/s->t)); \
    if (s->w) { \
      printf(" with %s%u warning%s%s",YLW,s->w,s->w==1 ? "" : "s",BLN);\
      if (s->f) { \
        printf(" and %s%u failure%s%s",RED,s->f,s->f==1 ? "" : "s",BLN);\
      }\
      if (s->e) { \
        printf(" and %s%u error%s%s",CRT,s->e,s->e==1 ? "" : "s",BLN);\
      }\
    } else if (s->f) { \
      printf(" with %s%u failure%s%s",RED,s->f,s->f==1 ? "" : "s",BLN);\
      if (s->e) { \
        printf(" and %s%u error%s%s",CRT,s->e,s->e==1 ? "" : "s",BLN);\
      }\
    } else if (s->e) { \
      printf(" with %s%u error%s%s",CRT,s->e,s->e==1 ? "" : "s",BLN);\
    }\
    printf("\n"); \
    struct _test_suite* next = s->next; \
    free(s); \
    s = next; \
  } \
  std::cout << "--------END TEST SUMMARY--------" << std::endl; \

}

#endif /* TESTS_H_ */

