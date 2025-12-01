using System;

namespace Ember
{
    public class MonoBehaviour : Component
    {
        internal MonoBehaviour()
        {
            InternalCalls.NativeLog("MonoBehaviour created.");
        }

        void OnStart()
        {
            InternalCalls.NativeLog("MonoBehaviour started.");
        }
    }
}
