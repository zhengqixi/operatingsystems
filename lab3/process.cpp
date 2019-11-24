#include "process.h"
#include "page.h"
#include <algorithm>
#include <vector>
namespace NYU {
namespace OperatingSystems {
    Process::Process(int numPages)
        : d_pages{ static_cast<unsigned long int>(numPages) }
    {
    }
    Process::VMA::VMA(int start, int end, bool fileMapped, bool writeProtected)
        : start{ start }
        , end{ end }
        , fileMapped{ fileMapped }
        , writeProtected{ writeProtected }
    {
    }
    void Process::addVMA(int start, int end, bool fileMapped, bool writeProtected)
    {
        d_VMA.push_back(VMA(start, end, fileMapped, writeProtected));
    }
    bool Process::setPageBits(int address)
    {
        Page& page = d_pages[address];
        if (page.vma()) {
            return true;
        }
        auto validVMA = std::find_if(d_VMA.begin(), d_VMA.end(), [address](const VMA& vma) {
            return vma.start <= address && vma.end >= address;
        });
        if (validVMA == d_VMA.end()) {
            return false;
        }
        page.vma(true);
        page.writeProtected(validVMA->writeProtected);
        page.fileMapped(validVMA->fileMapped);
        return true;
    }
    std::vector<Page>& Process::pageTable()
    {
        return d_pages;
    }
    Page& Process::operator[](int address)
    {
        return d_pages[address];
    }
    unsigned long& Process::unmaps()
    {
        return d_unmaps;
    }
    unsigned long& Process::maps()
    {
        return d_maps;
    }
    unsigned long& Process::ins()
    {
        return d_ins;
    }
    unsigned long& Process::outs()
    {
        return d_outs;
    }
    unsigned long& Process::fins()
    {
        return d_fins;
    }
    unsigned long& Process::fouts()
    {
        return d_fouts;
    }
    unsigned long& Process::zeros()
    {
        return d_zeros;
    }
    unsigned long& Process::segvs()
    {
        return d_segvs;
    }
    unsigned long& Process::segprots()
    {
        return d_segprots;
    }
}
}