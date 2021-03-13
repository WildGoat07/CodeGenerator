#ifndef TEMPLATENAME_H
#define TEMPLATENAME_H
#include <QListWidgetItem>
#include <QVariant>
#include <QString>


struct TemplateName : public QListWidgetItem
{
public:
    TemplateName();
    TemplateName(const QString& name);
    QString name;
    virtual QVariant data(int role) const override;
    TemplateName& operator=(const QString& name);
};

#endif // TEMPLATENAME_H
