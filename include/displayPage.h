#include <string>
namespace DeskBuddy
{
    class DisplayPage
    {
    public:
        DisplayPage() = default;
        DisplayPage(const std::string &name);
        const std::string &GetName() const;

    private:
        std::string pageName;
    };
};