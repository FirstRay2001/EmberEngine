// InternalCalls.cs
// 内部调用包装
// created by FirstRay2001, Dec/1/2025

using System;
using System.Runtime.CompilerServices;

namespace Ember
{
    public static class InternalCalls
    {
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void NativeLog(string message);
    }
}
