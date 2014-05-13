#ifndef SINGLETON_H
#define SINGLETON_H

#include <assert.h>
#include <stdio.h>

template <class T>
class Singleton
{
public:
    Singleton()
    {
        assert(s_instance != NULL);
        s_instance=static_cast<T*>(this);
    }

    virtual ~Singleton()
    {
        s_instance=NULL;
    }

    static T& getInstance()
    {
        return (*getInstancePtr());
    }

    static T* getInstancePtr()
    {
        return s_instance;
    }

    static const char* getClassTypeName()
    {
        return s_classTypeName;
    }

private:
    static T* s_instance;
protected:
    static const char* s_classTypeName;
};

#endif // SINGLETON_H
