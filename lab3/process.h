#ifndef PROCESS_H
#define PROCESS_H
#include "page.h"
#include <vector>
namespace NYU {
namespace OperatingSystems {
    class Process {
    public:
        Process();
        void addVMA(int start, int end, bool fileMapped, bool writeProtected);
        Page& getPage(int address);
        bool setPageBits(int address);

    private:
        struct VMA {
        public:
            VMA(int start, int end, bool fileMapped, bool writeProtected);
            int start = 0;
            int end = 0;
            bool fileMapped = false;
            bool writeProtected = false;
        };
        std::vector<VMA> d_VMA;
        std::vector<Page> d_pages;
    };
}
}
#endif