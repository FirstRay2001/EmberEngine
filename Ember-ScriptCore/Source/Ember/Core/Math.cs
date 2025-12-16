namespace Ember
{
    public static class MathHelper
    {
        public static float DegreesToRadians(float degrees)
        {
            return degrees * ((float)System.Math.PI / 180.0f);
        }

        public static float RadiansToDegrees(float radians)
        {
            return radians * (180.0f / (float)System.Math.PI);
        }
    }

    /// <summary>
    /// 三维向量
    /// </summary>
    public struct Vector3
    {
        public float X, Y, Z;

        public static Vector3 Zero => new Vector3(0);

        public Vector3(float x, float y, float z)
        {
            X = x;
            Y = y;
            Z = z;
        }

        public Vector3(float scalar)
        {
            X = scalar;
            Y = scalar;
            Z = scalar;
        }

        public static Vector3 Normalize(Vector3 v)
        {
            float length = (float)System.Math.Sqrt(v.X * v.X + v.Y * v.Y + v.Z * v.Z);
            if (length > 0)
            {
                return new Vector3(v.X / length, v.Y / length, v.Z / length);
            }
            return Vector3.Zero;
        }

        public static float Dot(Vector3 a, Vector3 b)
        {
            return a.X * b.X + a.Y * b.Y + a.Z * b.Z;
        }

        public static Vector3 Cross(Vector3 a, Vector3 b)
        {
            return new Vector3(
                a.Y * b.Z - a.Z * b.Y,
                a.Z * b.X - a.X * b.Z,
                a.X * b.Y - a.Y * b.X
            );
        }

        public static Vector3 operator +(Vector3 a, Vector3 b)
        {
            return new Vector3(a.X + b.X, a.Y + b.Y, a.Z + b.Z);
        }

        public static Vector3 operator -(Vector3 a, Vector3 b)
        {
            return new Vector3(a.X - b.X, a.Y - b.Y, a.Z - b.Z);
        }

        public static Vector3 operator *(Vector3 v, float scalar)
        {
            return new Vector3(v.X * scalar, v.Y * scalar, v.Z * scalar);
        }

        public static Vector3 operator /(Vector3 v, float scalar)
        {
            return new Vector3(v.X / scalar, v.Y / scalar, v.Z / scalar);
        }

        public override string ToString()
        {
            return $"({X}, {Y}, {Z})";
        }
    }

    /// <summary>
    /// 二维向量
    /// </summary>
    public struct Vector2
    {
        public float X, Y;
        public static Vector2 Zero => new Vector2(0, 0);
        public static Vector2 One => new Vector2(1, 1);

        public Vector2(float x, float y)
        {
            X = x;
            Y = y;
        }

        public override string ToString()
        {
            return $"({X}, {Y})";
        }
    }
}
