#include <iostream>
#include <fstream>
#include <unistd.h>

/*
 * Show memory usesage on Linux platform
 */

#if defined(__linux__)
#define PLATFORM_NAME "linux"
#else
#define PLATFORM_NAME "other"
#endif

void process_mem_usage(double& vm_usage, double& resident_set)
{
    vm_usage     = 0.0;
    resident_set = 0.0;

    // the two fields we want
    unsigned long vsize;
    long rss;
    {
        std::string ignore;
        std::ifstream ifs("/proc/self/stat", std::ios_base::in);
        ifs >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore
                >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore
                >> ignore >> ignore >> vsize >> rss;
    }

    long page_size_kb = sysconf(_SC_PAGE_SIZE) / 1024; // in case x86-64 is configured to use 2MB pages
    vm_usage = vsize / 1024.0;
    resident_set = rss * page_size_kb;
}

int main()
{
    using std::cout;
    using std::endl;

    if (PLATFORM_NAME == "linux")
    {
        double vm, rss;
        process_mem_usage(vm, rss);
        cout << "VM: " << vm / 1024.0 << "MB" << "; RSS: " << rss / 1024.0 << "MB" << endl;
    }
    else
    {
        cout << "Platform not support!" << endl;
    }
}
