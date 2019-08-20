#ifndef __JSON_CHECKER_H__
#define __JSON_CHECKER_H__

/* JSON_checker.h */

/* 2016-11-11 */

/*
    The JSON_checker_struct is used to hold the state of the JSON_checker
    so that the code can be reentrant.
*/

typedef struct JSON_checker_struct {
    int Valid;
    int State;
    int Depth;
    int Top;
    int* Stack;
} * JSON_checker;


extern JSON_checker new_JSON_checker(unsigned int depth);

/*
    Make a new JSON_checker. You indicate the maximum depth that is allowed.
    It will return an object that you will pass to the other functions.
    They will destroy the object for you.
*/

extern int JSON_checker_char(JSON_checker jc, int next_char);

/*
    You should call JSON_checker_char for each character of the JSON text.
    It will return false if the text is not right.
*/

extern int JSON_checker_done(JSON_checker jc);

/*
    When there are no more JSON text characters, call JSON_checker_done.
    It will return false if the text was not right.
*/

#endif
