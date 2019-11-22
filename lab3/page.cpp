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
        d_present = static_cast<unsigned int>(set);
    }
    bool Page::writeProtected() const
    {
        return d_writeProtected == 1;
    }
    void Page::writeProtected(bool set)
    {
        d_writeProtected = static_cast<unsigned int>(set);
    }
    bool Page::modified() const
    {
        return d_modified == 1;
    }
    void Page::modified(bool set)
    {
        d_modified = static_cast<unsigned int>(set);
    }
    bool Page::referenced() const
    {
        return d_referenced == 1;
    }
    void Page::referenced(bool set)
    {
        d_referenced = static_cast<unsigned int>(set);
    }
    bool Page::pagedOut() const
    {
        return d_pagedOut == 1;
    }
    void Page::pagedOut(bool set)
    {
        d_pagedOut = static_cast<unsigned int>(set);
    }
    unsigned int Page::assignedFrame() const
    {
        return d_assignedFrame;
    }
    void Page::assignedFrame(unsigned int newFrame)
    {
        present(true);
        d_assignedFrame = newFrame;
    }
    bool Page::vma() const
    {
        return d_vma == 1;
    }
    void Page::vma(bool set)
    {
        d_vma = static_cast<unsigned int>(set);
    }
}
} // namespace NYU
