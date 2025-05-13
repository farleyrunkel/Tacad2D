

// Copyright [2024] SunCAD

#ifndef OCCT_OCCTHELPER_OcctHelper_H_
#define OCCT_OCCTHELPER_OcctHelper_H_

#include <string>

#include <TCollection_ExtendedString.hxx>

class OcctHelper
{
public:
    // ת�� TCollection_ExtendedString �� std::string
    static std::string ExtendedStringToStdString(const TCollection_ExtendedString& extStr)
    {
        // ʹ�� WideChar ת��Ϊ UTF-8 std::string
        std::wstring wideStr(extStr.ToWideString());
        return std::string(wideStr.begin(), wideStr.end());
    }
};

#endif  // OCCT_OCCTHELPER_OcctHelper_H_
