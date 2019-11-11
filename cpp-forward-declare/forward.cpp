#include "forward.h"
#include <string>
#include <iostream>

struct Person
{
    Person(const std::string& name, int age, const std::string& birthday)
    : name_(name), age_(age), birthday_(birthday)
    {}

    std::string name_;
    int         age_;
    std::string birthday_;
};

void initPerson(struct Person** p, const char* name, int age, const char* birthday)
{
    *p = new Person(name, age, birthday);
}

void printPerson(struct Person* p)
{
    std::cout << "** Person **" << std::endl;
    std::cout << "** name: " << p->name_ << std::endl;
    std::cout << "** age: " << p->age_ << std::endl;
    std::cout << "** Birthday: " << p->birthday_ << std::endl;
}