#ifndef __INCLUDE_FORWARD_H__
#define __INCLUDE_FORWARD_H__

struct Person;
void initPerson(struct Person** p, const char* name, int age, const char* birthday);
void printPerson(struct Person* p);

#endif