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
        Page& operator[](int address);
        // on page fault, checks the page at the given address with the VMA and sets the bits based on the VMA
        // Returns true if page is valid, false if invalid page
        bool setPageBits(int address);
        std::vector<Page>& pageTable();
        unsigned long& unmaps();
        unsigned long& maps();
        unsigned long& ins();
        unsigned long& outs();
        unsigned long& fins();
        unsigned long& fouts();
        unsigned long& zeros();
        unsigned long& segvs();
        unsigned long& segprots();

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
        // Statistics data
        unsigned long d_unmaps = 0;
        unsigned long d_maps = 0;
        unsigned long d_ins = 0;
        unsigned long d_outs = 0;
        unsigned long d_fins = 0;
        unsigned long d_fouts = 0;
        unsigned long d_zeros = 0;
        unsigned long d_segvs = 0;
        unsigned long d_segprots = 0;
    };
}
}
#endif