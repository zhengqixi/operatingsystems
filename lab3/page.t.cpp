#include "page.h"
#include <iostream>

int main()
{
    using namespace NYU::OperatingSystems;
    using namespace std;
    /*
        unsigned int d_present : 1;
        unsigned int d_writeProtected : 1;
        unsigned int d_modified : 1;
        unsigned int d_referenced : 1;
        unsigned int d_pagedOut : 1;
        unsigned int d_assignedFrame : 7;
        unsigned int d_padding : 20;
    */
    auto page = new Page();
    cout << "Size of page in bytes: " << sizeof(Page) << '\n';
    cout << "present : " << page->present() << '\n';
    page->present(true);
    cout << "present : " << page->present() << '\n';
    delete page;
}