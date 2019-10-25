//
// Created by realxie on 2019-10-25.
//

#ifndef ALITA_OBJECTBASE_H
#define ALITA_OBJECTBASE_H

#include "Macros.h"

#include <stdint.h>
#include <string>

NS_RX_BEGIN

class Noncopyable
{
public:
    Noncopyable() = default;

    Noncopyable(Noncopyable&&) = default;
    Noncopyable& operator =(Noncopyable&&) = default;

private:
    Noncopyable(const Noncopyable&) = delete;
    Noncopyable& operator=(const Noncopyable&) = delete;
};

class ObjectBase : public Noncopyable
{
protected:
    virtual ~ObjectBase() = default;

public:
    ObjectBase() = default;

    void Retain()
    {
        ++__referenceCount_;
    }

    void Release()
    {
        --__referenceCount_;
        if (__referenceCount_ <= 0)
        {
            OnDeleteEvent();
            delete this;
        }
    }

    std::int32_t GetReferenceCount() const
    {
        return __referenceCount_;
    }

    virtual void OnDeleteEvent()
    {
        // to override this function to process some logic before delete
    }

private:
    std::int32_t __referenceCount_ = 0;
};

NS_RX_END


#endif //ALITA_OBJECTBASE_H
