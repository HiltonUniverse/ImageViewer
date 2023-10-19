#pragma once

#include <QDebug>

#include <vector>
namespace pattern
{
    template<typename Type, typename Event>
    class Observer
    {
    public:
        virtual void changed(Type* type, const Event& event) = 0;
    };

    template<typename Type, typename Event>
    class Observable
    {
    public:
        //-----------------------------------
        void attach(Observer<Type,Event>* observer)
        {
            m_observers.push_back(observer);
        }

        //-----------------------------------
        void detach(Observer<Type,Event>* observer)
        {
            if(observer)
            {
                m_observers.erase(std::remove(m_observers.begin(), m_observers.end(), observer), m_observers.end());
            }
        }

        //-----------------------------------
        void notify(const Event& event)
        {
            for(auto& obs : m_observers)
            {
                obs->changed(static_cast<Type*>(this), event);
            }
        }

    private:
        std::vector<Observer<Type,Event>*> m_observers;
    };
}
