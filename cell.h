#ifndef CELL_H
#define CELL_H

#include <QTableWidgetItem>

class Cell : public QTableWidgetItem
{
//    Q_OBJECT
public:
    explicit Cell();//the constructor

    QTableWidgetItem *clone() const;//it's the virtual function of the parents
    void setData(int role, const QVariant &value);//ditto
    QVariant data(int role) const;//ditto
    void setFormula(const QString &formula);
    QString formula() const;
    void setDirty();

private:
    QVariant value() const;
    QVariant evalExpression(const QString &str, int &pos) const;
    QVariant evalTerm(const QString &str, int &pos) const;
    QVariant evalFactor(const QString &str, int &pos) const;

    mutable QVariant cachedValue;
    mutable bool cacheIsDirty;
    
};

#endif // CELL_H
