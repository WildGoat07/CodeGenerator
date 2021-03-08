#include "generator.h"
#include <tuple>
#include <iostream>
#include "method.h"
#include "attribute.h"
#include "variableMode.h"

using namespace std;
using namespace cmpt_info;

Generator::Generator():
    classes()
{

}

string getRange(Range r)
{
    switch (r) {
    case PUBLIC:
        return "public";
    case PROTECTED:
        return "protected";
    case PRIVATE:
        return "private";
    default:
        return "";
    }
}

void indent(int count, ostream &stream)
{
    stream << string(count << 2, ' ');
}

void writeCppType(const Type& type, ostream& stream, bool addSpace)
{
    if (type.constant)
        stream << "const ";
    stream << type.name;
    if (!type.templateValues.empty())
    {
        stream << "<";
        bool first = true;
        for (auto templ = type.templateValues.begin();templ != type.templateValues.end();++templ)
        {
            if (first)
                first = false;
            else
                stream << ", ";
            writeCppType(*templ, stream, false);
        }
        stream << '>';
    }
    switch (type.mode) {
    case PTR:
        stream << (addSpace ? " *" : "*");
        break;
    case REF:
        stream << (addSpace ? " &" : "&");
        break;
    case COPY:
        stream << (addSpace ? " " : "");
        break;
    default:
        break;
    }
}

void writeCppAttribute(const Attribute& attribute, ostream &stream)
{
    indent(1, stream);
    writeCppType(attribute.variable.varType, stream, true);
    stream << ' ' << attribute.variable.name << ';' << endl;
}

void writeCppMethod(const Method& method, const string& className, ostream& stream)
{
    if (method.specialMethod == Method::BASIC)
    {
        if (!method.templateTypes.empty())
        {
            indent(1, stream);
            stream << "template<";
            bool first = true;
            for (auto templ = method.templateTypes.begin();templ != method.templateTypes.end();++templ)
            {
                if (first)
                    first = false;
                else
                    stream << ", ";
                stream << "typename " << *templ;
            }
            stream << '>' << endl;
        }
        indent(1, stream);
        switch (method.modifier)
        {
        case Method::VIRTUAL:
        case Method::OVERRIDE:
        case Method::ABSTRACT:
            stream << "virtual ";
            break;
        case Method::STATIC:
            stream << "static ";
            break;
        default:
            break;
        }
        writeCppType(method.returnType, stream, true);
        stream << method.name << '(';
    }
    else
    {
        indent(1, stream);
        if (method.specialMethod == Method::DESTRUCTOR)
            stream << '~';
        stream << className << '(';
    }
    bool first = true;
    for (auto paramIt = method.parameters.begin();paramIt != method.parameters.end();++paramIt)
    {
        if (first)
            first = false;
        else
            stream << ", ";
        writeCppType(paramIt->varType, stream, true);
        stream << paramIt->name;
    }
    stream << ')';
    if (method.constantMethod)
        stream << " const";
    if (method.modifier == Method::OVERRIDE)
        stream << " override";
    if (method.modifier == Method::ABSTRACT)
        stream << " = 0";
    stream << ';' << endl;
}

void writeCppClass(const Class& c, ostream &stream)
{
    if (!c.templateTypes.empty())
    {
        stream << "template<";
        bool first = true;
        for (auto templ = c.templateTypes.begin();templ != c.templateTypes.end();++templ)
        {
            if (first)
                first = false;
            else
                stream << ", ";
            stream << "typename " << *templ;
        }
        stream << '>' << endl;
    }
    stream << "class " << c.name;
    if (!c.parents.empty())
    {
        stream << " : ";
        bool first = true;
        for (auto parentIt = c.parents.begin();parentIt != c.parents.end();++parentIt)
        {
            if (first)
                first = false;
            else
                stream << ", ";
            Class* parent;
            Range range;
            tie(range, parent) = *parentIt;
            stream << getRange(range) << ' ' << parent->name;
        }
    }
    stream << endl << '{' << endl;
    List<Method*> publicMethods;
    List<Method*> protectedMethods;
    List<Method*> privateMethods;
    List<Attribute*> publicAttributes;
    List<Attribute*> protectedAttributes;
    List<Attribute*> privateAttributes;

    for (auto methodIt = c.methods.begin();methodIt != c.methods.end();++methodIt)
        switch (methodIt->range) {
        case PUBLIC:
            publicMethods.push_back(&*methodIt);
            break;
        case PROTECTED:
            protectedMethods.push_back(&*methodIt);
            break;
        case PRIVATE:
            privateMethods.push_back(&*methodIt);
            break;
        }
    for (auto attributeIt = c.attributes.begin();attributeIt != c.attributes.end();++attributeIt)
        switch (attributeIt->range) {
        case PUBLIC:
            publicAttributes.push_back(&*attributeIt);
            break;
        case PROTECTED:
            protectedAttributes.push_back(&*attributeIt);
            break;
        case PRIVATE:
            privateAttributes.push_back(&*attributeIt);
            break;
        }
    if (!publicAttributes.empty())
    {
        stream << endl;
        stream << "public: // public attributes" << endl;
        for (auto attributeIt = publicAttributes.begin();attributeIt != publicAttributes.end();++attributeIt)
            writeCppAttribute(**attributeIt, stream);
    }
    if (!protectedAttributes.empty())
    {
        stream << endl;
        stream << "protected: // protected attributes" << endl;
        for (auto attributeIt = protectedAttributes.begin();attributeIt != protectedAttributes.end();++attributeIt)
            writeCppAttribute(**attributeIt, stream);
    }
    if (!privateAttributes.empty())
    {
        stream << endl;
        stream << "private: // private attributes" << endl;
        for (auto attributeIt = privateAttributes.begin();attributeIt != privateAttributes.end();++attributeIt)
            writeCppAttribute(**attributeIt, stream);
    }
    if (!publicMethods.empty())
    {
        stream << endl;
        stream << "public: // public methods" << endl;
        for (auto methodIt = publicMethods.begin();methodIt != publicMethods.end();++methodIt)
            writeCppMethod(**methodIt, c.name, stream);
    }
    if (!protectedMethods.empty())
    {
        stream << endl;
        stream << "protected: // protected methods" << endl;
        for (auto methodIt = protectedMethods.begin();methodIt != protectedMethods.end();++methodIt)
            writeCppMethod(**methodIt, c.name, stream);
    }
    if (!privateMethods.empty())
    {
        stream << endl;
        stream << "private: // private methods" << endl;
        for (auto methodIt = privateMethods.begin();methodIt != privateMethods.end();++methodIt)
            writeCppMethod(**methodIt, c.name, stream);
    }

    stream << "};" << endl;
}

void Generator::GenerateCpp(ostream &stream)
{
    for (auto it = classes.begin();it != classes.end();++it)
        stream << "class " << it->name << ';' << endl;
    for (auto it = classes.begin();it != classes.end();++it)
    {
        stream << endl;
        writeCppClass(*it, stream);
    }
}

