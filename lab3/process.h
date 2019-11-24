#ifndef PROCESS_H
#define PROCESS_H
#include "page.h"
#include <vector>
namespace NYU {
namespace OperatingSystems {
    class Process {
    public:
        Process(int numPages);
        void addVMA(int start, int end, bool fileMapped, bool writeProtected);
        Page& getPage(int address);
        // on page fault, checks the page at the given address with the VMA and sets the bits based on the VMA
        // Returns true if page is valid, false if invalid page
        bool setPageBits(int address);
        std::vector<Page>& pageTable();

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