#ifndef WINAPISHELL_H
#define WINAPISHELL_H

#include "ShellNotifier.h"

#include <Shlobj.h>
#include <string>

class WindowsApiShellNotifier : public AbstractShellNotifier
{
public :
    void notify(const std::string& path) override
    {
        SHChangeNotify(SHCNE_UPDATEITEM, SHCNF_PATH, path.data(), NULL);
    }
};

#endif // WINAPISHELL_H
