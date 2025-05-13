#ifndef SKETCHFACTORY_HXX
#define SKETCHFACTORY_HXX

#include <functional>
#include <map>
#include <string>

#include <TDF_Label.hxx>
#include <Standard_Handle.hxx>

class SketchFactory
{
public:
    static void Init();

    // ��������
    static Handle(Standard_Transient) Wrap(const TDF_Label& label);

    template<typename T>
    static Handle(T) Wrap(const TDF_Label& label)
    {
        return Handle(T)::DownCast(Wrap(label));
    }

    // ע������
    template<typename T>
    static void Register(const std::string& name)
    {
        _Wrappers[name] = [](const TDF_Label& label)->Handle(Standard_Transient)
        {
            return new T(label);
        };
    }

private:

    // ��������
    static Handle(Standard_Transient) Wrap(const std::string& name, const TDF_Label& label);

    using Creator = std::function<Handle(Standard_Transient)(const TDF_Label&)>;
    static std::map<std::string, Creator> _Wrappers;
};

#endif  // SKETCHFACTORY_HXX
