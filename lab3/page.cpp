#include "page.h"
namespace NYU {
namespace OperatingSystems {
    /*
        unsigned int d_present : 1;
        unsigned int d_writeProtected : 1;
        unsigned int d_modified : 1;
        unsigned int d_referenced : 1;
        unsigned int d_pagedOut : 1;
        unsigned int d_assignedFrame : 7;
        unsigned int d_padding : 20;
    */
    Page::Page()
        : d_present{ 0 }
        , d_writeProtected{ 0 }
        , d_modified{ 0 }
        , d_referenced{ 0 }
        , d_pagedOut{ 0 }
        , d_assignedFrame{ 0 }
        , d_vma{ 0 }
        , d_padding{ 0 }

    {
    }
    bool Page::present() const
    {
        return d_present == 1;
    }
    void Page::present(bool set)
    {
        if (set) {
            d_present = 1;
        } else {
            d_present = 0;
        }
    }
    bool Page::writeProtected() const
    {
        return d_writeProtected == 1;
    }
    void Page::writeProtected(bool set)
    {
        if (set) {
            d_writeProtected = 1;
        } else {
            d_writeProtected = 0;
        }
    }
    bool Page::modified() const
    {
        return d_modified == 1;
    }
    void Page::modified(bool set)
    {
        if (set) {
            d_modified = 1;
        } else {
            d_modified = 0;
        }
    }
    bool Page::referenced() const
    {
        return d_referenced == 1;
    }
    void Page::referenced(bool set)
    {
        if (set) {
            d_referenced = 1;
        } else {
            d_referenced = 0;
        }
    }
    bool Page::pagedOut() const
    {
        return d_pagedOut == 1;
    }
    void Page::pagedOut(bool set)
    {
        if (set) {
            d_pagedOut = 1;
        } else {
            d_pagedOut = 0;
        }
    }
    unsigned int Page::assignedFrame() const
    {
        return d_assignedFrame;
    }
    void Page::assignedFrame(unsigned int newFrame)
    {
        d_assignedFrame = newFrame;
    }
    bool Page::vma() const
    {
        return d_vma == 1;
    }
    void Page::vma(bool set)
    {
        if (set) {
            d_vma = 1;
        } else {
            d_vma = 0;
        }
    }
}
} // namespace NYU
