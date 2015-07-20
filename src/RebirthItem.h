#ifndef REBIRTHITEM_H
#define REBIRTHITEM_H

#include <QString>

#include <map>



class RebirthItem
{

private:
    enum ItemType { ACTIVE=0, PASSIVE=1 };

    QString m_Name;
    ItemType m_Type;
    QString m_Charge;
    QString m_Pools;
    QString m_PickupText;
    QString m_Effect;
public:
    RebirthItem() {m_Name = "unknown";}
    RebirthItem(const QString& name,
                 const QString& type,
                 const QString& charge,
                 const QString& pools,
                 const QString& pickup,
                 const QString& effect);

    QString GetName() { return m_Name;}
    QString GetTypeStr() { return m_Type ? "Passive" : "Active";  }
    QString GetPools() { return m_Pools; }
    QString GetPickup() { return m_PickupText; }
    QString GetEffect() { return m_Effect;}
    QString GetChargeTime() { return m_Charge; }
};

#endif // REBIRTHITEM_H
