using System;
using Ember;

namespace Client
{
    public class Player : Entity
    {
        internal Player()
        {
        }

        void OnCreate()
        {
            InternalCalls.NativeLog($"Player OnCreate. UUID: {ID}");
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

            Position += velocity * deltaTime;
        }
    }
}
