using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Ember
{
    public abstract class Component
    {
        public Entity Entity { get; internal set; }
    }

    public class TransformComponent : Component
    {
        public Vector3 Translation
        {
            get
            {
                InternalCalls.TransformComponent_GetPosition(Entity.ID, out Vector3 position);
                return position;
            }

            set
            {
                InternalCalls.TransformComponent_SetPosition(Entity.ID, ref value);
            }
        }

        public Vector3 Rotation
        {
            get
            {
                InternalCalls.TransformComponent_GetRotation(Entity.ID, out Vector3 rotation);
                return rotation;
            }

            set
            {
                InternalCalls.TransformComponent_SetRotation(Entity.ID, ref value);
            }
        }

        public Vector3 Scale
        {
            get
            {
                InternalCalls.TransformComponent_GetScale(Entity.ID, out Vector3 scale);
                return scale;
            }
            set
            {
                InternalCalls.TransformComponent_SetScale(Entity.ID, ref value);
            }
        }

        public Vector3 Forward
        {
            get
            {
                InternalCalls.TransformComponent_GetForward(Entity.ID, out Vector3 forward);
                return forward;
            }
        }

        public Vector3 Right
        {
            get
            {
                InternalCalls.TransformComponent_GetRight(Entity.ID, out Vector3 right);
                return right;
            }
        }
    }
}
