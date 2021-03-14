#include "method.h"
#include <sstream>

Method::Method():
    QListWidgetItem(),
    name(),
    templateTypes(),
    range(PUBLIC),
    returnType("void"),
    parameters(),
    modifier(NONE),
    constantMethod(false),
    finalMethod(false),
    specialMethod(BASIC)
{

}
QVariant Method::data(int role) const
{
    if (role == Qt::DisplayRole)
    {
        std::stringstream ss;

        switch (specialMethod) {
        case Special::BASIC:
            ss << name.toStdString();
            break;
        case Special::CONSTRUCTOR:
            ss << "<constructeur>";
            break;
        case Special::DESTRUCTOR:
            ss << "<destructeur>";
            break;
        }
        ss << '(';
        bool first = true;
        for (auto it = parameters.begin();it != parameters.end();++it)
        {
            if (first)
                first = false;
            else
                ss << ", ";
            ss << it->name.toStdString();
        }
        ss << ')';
        return QVariant(QString(ss.str().c_str()));
    }
    else
        return QListWidgetItem::data(role);
}
