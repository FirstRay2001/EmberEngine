using System;

namespace Ember
{
    public class Main
    {
        public float FloatVar{ get; set; }

        public Main()
        {
            Console.WriteLine("Main Constructor");
        }
        
        public void PrintMessage()
        {
            Console.WriteLine("Hello from C#!");
        }

        public void PrintInt(int value)
        {
            Console.WriteLine($"C# Integer value: {value}");
        }

        public void PrintCustomMessage(string msg)
        {
            Console.WriteLine($"C# Custom Message: {msg}");
        }
    }
}