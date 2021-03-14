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

void indent(int count, ostream &stream)
{
    stream << string(count << 2, ' ');
}

void writeCppType(const Type& type, ostream& stream, bool addSpace)
{
    if (type.constant)
        stream << "const ";
    if (type.array)
        stream << "vector<";
    if (type.name == "boolean")
        stream << "bool";
    else if (type.name == "String" || type.name == "string")
        stream << "std::string";
    else
        stream << type.name.toStdString();
    if (type.array)
        stream << '>';
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
    if (attribute.staticAttribute)
        stream << "static ";
    writeCppType(attribute.variable.varType, stream, true);
    stream << ' ' << attribute.variable.name.toStdString() << ';' << endl;
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
                stream << "typename " << templ->name.toStdString();
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
        stream << method.name.toStdString() << '(';
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
        stream << paramIt->name.toStdString();
    }
    stream << ')';
    if (method.constantMethod)
        stream << " const";
    if (method.finalMethod)
        stream << " final";
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
            stream << "typename " << templ->name.toStdString();
        }
        stream << '>' << endl;
    }
    stream << "class " << c.name.toStdString();
    if (c.finalClass)
        stream << " final";
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
            stream << getRange(parentIt->range) << " virtual " << parentIt->name.toStdString();
            if (!parentIt->templates.empty())
            {
                stream << "<";
                bool first = true;
                for (auto templ = parentIt->templates.begin();templ != parentIt->templates.end();++templ)
                {
                    if (first)
                        first = false;
                    else
                        stream << ", ";
                    writeCppType(*templ, stream, false);
                }
                stream << '>';
            }
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
            writeCppMethod(**methodIt, c.name.toStdString(), stream);
    }
    if (!protectedMethods.empty())
    {
        stream << endl;
        stream << "protected: // protected methods" << endl;
        for (auto methodIt = protectedMethods.begin();methodIt != protectedMethods.end();++methodIt)
            writeCppMethod(**methodIt, c.name.toStdString(), stream);
    }
    if (!privateMethods.empty())
    {
        stream << endl;
        stream << "private: // private methods" << endl;
        for (auto methodIt = privateMethods.begin();methodIt != privateMethods.end();++methodIt)
            writeCppMethod(**methodIt, c.name.toStdString(), stream);
    }

    stream << "};" << endl;
}

void Generator::GenerateCpp(ostream &stream)
{
    for (auto it = classes.begin();it != classes.end();++it)
        stream << "class " << it->name.toStdString() << ';' << endl;
    for (auto it = classes.begin();it != classes.end();++it)
    {
        stream << endl;
        writeCppClass(*it, stream);
    }
}

void writeJavaType(const Type& type, ostream &stream)
{
    if (type.array)
        stream << "List<";
    if (type.name == "bool")
        stream << "boolean";
    else if (type.name == "string" || type.name == "std::string")
        stream << "String";
    else
        stream << type.name.toStdString();
    if (type.array)
        stream << '>';
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
            writeJavaType(*templ, stream);
        }
        stream << '>';
    }
}

void writeJavaAttribute(const Attribute& attribute, ostream &stream)
{
    indent(1, stream);
    stream << getRange(attribute.range) << ' ';
    if (attribute.staticAttribute)
        stream << "static ";
    if (attribute.variable.varType.constant)
        stream << "final ";
    if (attribute.transientAttribute)
        stream << "transient ";
    writeJavaType(attribute.variable.varType, stream);
    stream << ' ' << attribute.variable.name.toStdString() << ';' << endl;
}

void writeJavaMethod(const Method& method, bool interface, const string& className, ostream &stream)
{
    indent(1, stream);
    if (method.specialMethod == Method::BASIC)
    {
        if (!interface)
        {
            if (method.modifier == Method::OVERRIDE)
            {
                stream << "@Override" << endl;
                indent(1, stream);
            }
            stream << getRange(method.range) << ' ';
            if (method.modifier == Method::STATIC)
                stream << "static ";
            if (method.modifier == Method::ABSTRACT)
                stream << "abstract ";
            if (method.finalMethod)
                stream << "final ";
        }
        if (!method.templateTypes.empty())
        {
            stream << "<";
            bool first = true;
            for (auto templ = method.templateTypes.begin();templ != method.templateTypes.end();++templ)
            {
                if (first)
                    first = false;
                else
                    stream << ", ";
                stream << templ->name.toStdString();
            }
            stream << "> ";
        }
        writeJavaType(method.returnType, stream);
        stream << ' ' << method.name.toStdString() << '(';
    }
    else if (method.specialMethod == Method::CONSTRUCTOR && !interface)
    {
        stream << getRange(method.range) << ' ';
        stream << className << '(';
    }
    if (method.specialMethod == Method::BASIC || !interface)
    {
        bool first = true;
        for (auto paramIt = method.parameters.begin();paramIt != method.parameters.end();++paramIt)
        {
            if (first)
                first = false;
            else
                stream << ", ";
            if (paramIt->varType.constant)
                stream << "final ";
            writeJavaType(paramIt->varType, stream);
            stream << ' ' << paramIt->name.toStdString();
        }
        stream << ')';
        if (method.modifier == Method::ABSTRACT || interface)
            stream << ';' << endl;
        else
        {
            stream << " {" << endl;
            indent(2, stream);
            stream << "return";
            if (method.specialMethod != Method::CONSTRUCTOR && method.returnType.name != "void")
            {
                if (method.returnType.name == "int" ||
                        method.returnType.name == "byte" ||
                        method.returnType.name == "char" ||
                        method.returnType.name == "short" ||
                        method.returnType.name == "long")
                    stream << " 0";
                else if (method.returnType.name == "float")
                    stream << " .0f";
                else if (method.returnType.name == "double")
                    stream << " .0";
                else if (method.returnType.name == "boolean")
                    stream << " false";
                else
                    stream << " null";
            }
            stream << ';' << endl;
            indent(1, stream);
            stream << '}' << endl;
        }
    }
}

void writeJavaClass(const Class& c, ostream &stream)
{
    if (c.finalClass)
        stream << "final ";
    stream << (c.interface ? "interface " : "class ") << c.name.toStdString();
    if (!c.templateTypes.empty())
    {
        stream << "<";
        bool first = true;
        for (auto templ = c.templateTypes.begin();templ != c.templateTypes.end();++templ)
        {
            if (first)
                first = false;
            else
                stream << ", ";
            stream << templ->name.toStdString();
        }
        stream << '>';
    }
    if (!c.interface)
        for (auto parentIt = c.parents.begin();parentIt != c.parents.end();++parentIt)
        {
            if (!parentIt->interface)
            {
                stream << " extends " << parentIt->name.toStdString();
                if (!parentIt->templates.empty())
                {
                    stream << "<";
                    bool first = true;
                    for (auto templ = parentIt->templates.begin();templ != parentIt->templates.end();++templ)
                    {
                        if (first)
                            first = false;
                        else
                            stream << ", ";
                        writeJavaType(*templ, stream);
                    }
                    stream << '>';
                }
                break;
            }
        }
    List<tuple<string, List<Type> > > interfaces;
    for (auto parentIt = c.parents.begin();parentIt != c.parents.end();++parentIt)
        if (parentIt->interface)
            interfaces.push_back(make_tuple(parentIt->name.toStdString(), parentIt->templates));
    if (!interfaces.empty())
    {
        stream << (c.interface ? " extends " : " implements ");
        bool first = true;
        for (auto parentIt = interfaces.begin();parentIt != interfaces.end();++parentIt)
        {
            if (first)
                first = false;
            else
                stream << ", ";
            stream << get<0>(*parentIt);
            auto templateTypes = get<1>(*parentIt);
            if (!templateTypes.empty())
            {
                stream << "<";
                bool first = true;
                for (auto templ = templateTypes.begin();templ != templateTypes.end();++templ)
                {
                    if (first)
                        first = false;
                    else
                        stream << ", ";
                    writeJavaType(*templ, stream);
                }
                stream << '>';
            }
        }
    }
    stream << " {" << endl;

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
    if (!c.interface)
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
        indent(1, stream);
        stream << "// public attributes" << endl;
        for (auto attributeIt = publicAttributes.begin();attributeIt != publicAttributes.end();++attributeIt)
            writeJavaAttribute(**attributeIt, stream);
    }
    if (!protectedAttributes.empty())
    {
        stream << endl;
        indent(1, stream);
        stream << "// protected attributes" << endl;
        for (auto attributeIt = protectedAttributes.begin();attributeIt != protectedAttributes.end();++attributeIt)
            writeJavaAttribute(**attributeIt, stream);
    }
    if (!privateAttributes.empty())
    {
        stream << endl;
        indent(1, stream);
        stream << "// private attributes" << endl;
        for (auto attributeIt = privateAttributes.begin();attributeIt != privateAttributes.end();++attributeIt)
            writeJavaAttribute(**attributeIt, stream);
    }
    if (!publicMethods.empty())
    {
        stream << endl;
        indent(1, stream);
        stream << "// public methods" << endl;
        for (auto methodIt = publicMethods.begin();methodIt != publicMethods.end();++methodIt)
            writeJavaMethod(**methodIt, c.interface, c.name.toStdString(), stream);
    }
    if (!protectedMethods.empty())
    {
        stream << endl;
        indent(1, stream);
        stream << "// protected methods" << endl;
        for (auto methodIt = protectedMethods.begin();methodIt != protectedMethods.end();++methodIt)
            writeJavaMethod(**methodIt, c.interface, c.name.toStdString(), stream);
    }
    if (!privateMethods.empty())
    {
        stream << endl;
        indent(1, stream);
        stream << "// private methods" << endl;
        for (auto methodIt = privateMethods.begin();methodIt != privateMethods.end();++methodIt)
            writeJavaMethod(**methodIt, c.interface, c.name.toStdString(), stream);
    }

    stream << '}' << endl;
}

void Generator::GenerateJava(ostream &stream)
{
    for (auto it = classes.begin();it != classes.end();++it)
    {
        writeJavaClass(*it, stream);
        stream << endl;
    }
}
