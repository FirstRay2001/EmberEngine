using System;
using System.Runtime.CompilerServices;

namespace Ember
{
    public static class InternalCalls
    {
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void NativeLog(string message);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void Window_CaptureMouse(bool capture);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static bool Input_IsKeyPressed(KeyCode key);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static bool Entity_HasComponent(ulong entityID, Type componentType);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void TransformComponent_GetPosition(ulong entityID, out Vector3 position);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void TransformComponent_SetPosition(ulong entityID, ref Vector3 position);
    }
}
