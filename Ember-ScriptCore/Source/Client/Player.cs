using System;
using Ember;

namespace Client
{
    public class Player : Entity
    {
        private TransformComponent m_Transform { get; set; }

        internal Player()
        {
        }

        void OnCreate()
        {
            InternalCalls.NativeLog($"Player OnCreate. UUID: {ID}");
            m_Transform = GetComponent<TransformComponent>();
        }

        void OnStart()
        {
            InternalCalls.NativeLog("Player OnStart");
            InternalCalls.Window_CaptureMouse(true);
        }

        void OnUpdate(float deltaTime)
        {
            float speed = 1.0f;
            Vector3 velocity = Vector3.Zero;

            if (Input.IsKeyPressed(KeyCode.W))
                velocity.Z = -1.0f;
            else if (Input.IsKeyPressed(KeyCode.S))
                velocity.Z = +1.0f;

            if (Input.IsKeyPressed(KeyCode.A))
                velocity.X = -1.0f;
            else if (Input.IsKeyPressed(KeyCode.D))
                velocity.X = 1.0f;

            velocity *= speed;

            m_Transform.Translation += velocity * deltaTime;

            // 鼠标捕获
            if (Input.IsKeyPressed(KeyCode.M))
            {
                InternalCalls.Window_CaptureMouse(true);
            }
            else if(Input.IsKeyPressed(KeyCode.Escape))
            {
                InternalCalls.Window_CaptureMouse(false);
            }
        }
    }
}
