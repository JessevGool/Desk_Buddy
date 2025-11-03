#include "displaypage.h"

namespace DeskBuddy
{
    DisplayPage::DisplayPage(const std::string &name) : pageName(name) {}
    const std::string &DisplayPage::GetName() const
    {
        return pageName;
    }
}