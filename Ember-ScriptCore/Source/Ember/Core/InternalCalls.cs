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
        public extern static bool Input_IsMouseButtonPressed(MouseButton button);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void Input_GetMousePosition(out Vector2 position);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static bool Entity_HasComponent(ulong entityID, Type componentType);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void TransformComponent_GetPosition(ulong entityID, out Vector3 position);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void TransformComponent_SetPosition(ulong entityID, ref Vector3 position);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void TransformComponent_GetRotation(ulong entityID, out Vector3 rotation);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void TransformComponent_SetRotation(ulong entityID, ref Vector3 rotation);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void TransformComponent_GetScale(ulong entityID, out Vector3 scale);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void TransformComponent_SetScale(ulong entityID, ref Vector3 scale);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void TransformComponent_GetForward(ulong entityID, out Vector3 forward);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void TransformComponent_GetRight(ulong entityID, out Vector3 right);
    }
}
