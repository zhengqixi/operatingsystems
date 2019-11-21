#include "process.h"
#include <iostream>
int main()
{
    using namespace NYU::OperatingSystems;
    using namespace std;
    Process newProcess;
    newProcess.addVMA(0, 12, false, true);
    newProcess.addVMA(13, 18, true, false);
    auto& page = newProcess.getPage(4);
    if (newProcess.setPageBits(4)) {
        cout << "Page 4 faulted: " << page.vma() << '\n';
        cout << "Page write protection: " << page.writeProtected() << '\n';
    } else {
        cout << "No good!\n";
        return -1;
    }
    auto& page2 = newProcess.getPage(14);
    if (newProcess.setPageBits(14)) {
        cout << "Page 14 faulted: " << page2.vma() << '\n';
        cout << "Page write protection: " << page2.writeProtected() << '\n';
    } else {
        cout << "No good!\n";
        return -1;
    }
    if (newProcess.setPageBits(14)) {
        cout << "mad good!\n";
    } else {
        cout << "Should've already faulted!\n";
        return -1;
    }
    if (newProcess.setPageBits(30)) {
        cout << "No good! Not a valid page!\n";
        return -1;
    } else {
        cout << " mad good!\n";
    }
}