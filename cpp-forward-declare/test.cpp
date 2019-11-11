#include "forward.h"

int main(int argc, char *argv[])
{
    Person* p = nullptr;
    initPerson(&p, "龚力", 26, "1993");
    printPerson(p);
}
