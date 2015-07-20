#include "RebirthItem.h"

RebirthItem::RebirthItem(const QString& name,
                         const QString& type,
                         const QString& charge,
                         const QString& pools,
                         const QString& pickup,
                         const QString& effect)
{
    m_Name = name;
    m_Type = (type == "Active" ? ACTIVE : PASSIVE);
    m_Charge = charge.toInt();
    m_Pools = pools;
    m_PickupText = pickup;
    m_Effect = effect;
}
