#ifndef TEST_ASSERT_H
#define TEST_ASSERT_H

#include "token.h"

/* Colors */
#define ANSI_RED "\x1b[31m"
#define ANSI_RESET "\x1b[0m"
#define BOLD "\x1b[1m"

/* 
* Prints information about the test that failed a true assertion.
*/
void assert_true_failed(const char *file_name, int line, const char *func, const char *expr); 

/* Check if an expression is true */
#define ASSERT_TRUE(x) \
    do { \
        if (!(x)) { \
            assert_true_failed(__FILE__, __LINE__, __func__, #x);  \
            return false; \
        } \
    } while (0) 


/*
* Prints information about the test that failed an equals int assertion.
*/
void assert_eq_int_failed(int exp, int rec, const char *file_name, int line, const char *func);

/* Check if two integer variables are equal to each other */
#define ASSERT_EQ_INT(exp, rec) \
    do { \
        if ((exp) != (rec)) { \
            assert_eq_int_failed(exp, rec, __FILE__, __LINE__, __func__); \
            return false; \
        } \
    } while (0)

/*
* Prints information about the test that failed an equals value_t assertion.
*/
void assert_eq_value_t_failed(value_t exp, value_t rec, const char *file_name, int line, const char *func);


#define ASSERT_EQ_VALUE_T(exp, rec) \
    do { \
        value_t _exp = (value_t)(exp); \
        value_t _rec = (value_t)(rec); \
        if (_exp != _rec) { \
            assert_eq_value_t_failed(exp, rec, __FILE__, __LINE__, __func__); \
            return false; \
        } \
    } while (0) \

/* 
* Prints information about the test that failed.
*/
void assert_eq_number_t_failed(const number_t *exp, const number_t *rec, const char *file_name, int line, const char *func);

/* Check if two number_t objects have the same value and base. */
#define ASSERT_EQ_NUMBER_T(exp, rec) \
    do { \
        const number_t _exp = (exp); \
        const number_t _rec = (rec);  \
        if (_exp.val != _rec.val || _exp.base != _rec.base) { \
            assert_eq_value_t_failed(&exp, &rec, __FILE__, __LINE__, __func__); \
            return false; \
        } \
    } while (0)

#endif
