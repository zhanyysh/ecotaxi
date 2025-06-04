#ifndef NM_H
#define NM_H

#include <QtPlugin>

class nm {
public:
    virtual void changeWindow(int id) = 0;
    virtual bool openSettings(int id) = 0;
    virtual bool openEvents(int id, QDate = QDate()) = 0;
    virtual bool openReport(int index, int id = 0, QDate = QDate(), QDate = QDate()) = 0;
    virtual bool openFines(int index, int id = 0, QDate = QDate(), QDate = QDate()) = 0;
    virtual bool userOpenReport(int index, int id = 0, QDate = QDate(), QDate = QDate()) = 0;
    virtual bool openPaymentHistory() = 0;
};

#define nm_iid "alxnko.nm"

Q_DECLARE_INTERFACE(nm, nm_iid)

#endif // NM_H
