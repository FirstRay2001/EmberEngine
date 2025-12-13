using System;

namespace Ember
{
    public class Entity
    {
        protected Entity() 
        { 
            ID = 0;
        }

        internal Entity(ulong id)
        {
            ID = id;
        }

        public readonly ulong ID;

        public bool HasComponent<T>() where T : Component, new()
        {
            Type componentType = typeof(T);
            return InternalCalls.Entity_HasComponent(ID, componentType);
        }

        public T GetComponent<T>() where T : Component, new()
        {
            if (!HasComponent<T>())
            {
                throw new Exception($"Entity does not have component of type {typeof(T)}");
            }
            T component = new T();
            component.Entity = this;
            return component;
        }
    }
}
