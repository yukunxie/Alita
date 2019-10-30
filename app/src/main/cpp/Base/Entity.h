//
// Created by realxie on 2019-10-29.
//

#ifndef ALITA_ENTITY_H
#define ALITA_ENTITY_H

#include "ObjectBase.h"

#include <list>

NS_RX_BEGIN

class Component;

class Entity
{
public:
    void AddComponment(Component* componment);
    
    /*
     * Get component with RTTI
     * */
    template <typename Tp_>
    Tp_* GetComponent()
    {
        for (auto comp : components_)
        {
            if (dynamic_cast<Tp_*>(comp))
            {
                return dynamic_cast<Tp_*>(comp);
            }
        }
        return nullptr;
    }
    
protected:
    std::list<Component*> components_;
};

NS_RX_END


#endif //ALITA_ENTITY_H
