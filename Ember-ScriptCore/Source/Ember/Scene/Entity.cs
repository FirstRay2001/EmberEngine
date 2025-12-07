using System;

namespace Ember
{
    /// <summary>
    /// 实体
    /// </summary>
    public class Entity
    {
        protected Entity() { ID = 0; }

        internal Entity(ulong id)
        {
            ID = id;
        }

        /// <summary>
        /// UUID
        /// </summary>
        public readonly ulong ID;

        /// <summary>
        /// 位置
        /// </summary>
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
