using System;
using Ember;

namespace Client
{
    public class Player : Entity
    {
        private TransformComponent m_Transform { get; set; }

        private bool m_FirstMouse = true;
        private Vector2 m_PreMousePos;
        private float m_MouseSensity = 0.1f;


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
            // 鼠标控制视角
            if(m_FirstMouse)
            {
                InternalCalls.Input_GetMousePosition(out m_PreMousePos);
                m_FirstMouse = false;
            }
            else
            {
                Vector2 mousePos;
                InternalCalls.Input_GetMousePosition(out mousePos);
                float xOffset = m_PreMousePos.X - mousePos.X;
                float yOffset = m_PreMousePos.Y - mousePos.Y;
                m_PreMousePos = mousePos;
                xOffset *= m_MouseSensity;
                yOffset *= m_MouseSensity;

                Vector3 rotation = m_Transform.Rotation;
                rotation.Y += xOffset;
                rotation.X += yOffset;
                m_Transform.Rotation = rotation;
            }

            float speed = 1.0f;
            Vector3 velocity = Vector3.Zero;

            if (Input.IsKeyPressed(KeyCode.W))
                velocity += m_Transform.Forward;
            else if (Input.IsKeyPressed(KeyCode.S))
                velocity -= m_Transform.Forward;

            if (Input.IsKeyPressed(KeyCode.A))
                velocity -= m_Transform.Right;
            else if (Input.IsKeyPressed(KeyCode.D))
                velocity += m_Transform.Right;

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
