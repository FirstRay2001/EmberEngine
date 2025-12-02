using System;

namespace Ember
{
    public class Entity
    {
        protected Entity() { ID = 0; }

        internal Entity(ulong id)
        {
            ID = id;
        }

        public readonly ulong ID;

        public Vector3 Position
        {
            get
            {
                InternalCalls.Transform_GetPosition(ID, out Vector3 position);
                return position;
            }
            set
            {
                InternalCalls.Transform_SetPosition(ID, ref value);
            }
        }
    }
}
