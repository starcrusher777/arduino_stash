using System;
using System.IO.Ports;
using System.Runtime.InteropServices;

class Program
{
    private static SerialPort _serialPort;

    [DllImport("User32.dll")]
    private static extern short GetAsyncKeyState(int vKey);

    static void Main(string[] args)
    {
        try
        {
            _serialPort = new SerialPort("COM7", 9600);
            _serialPort.Open();
            Console.WriteLine("Running");

            while (true)
            {
                if (GetAsyncKeyState(0x41) < 0)
                {
                    SendCommand('a');
                }
                
                if (GetAsyncKeyState(0x44) < 0)
                {
                    SendCommand('d');
                }
                
                System.Threading.Thread.Sleep(100);
            }
        }
        catch (Exception ex)
        {
            Console.WriteLine($"Error: {ex.Message}");
        }
    }

    static void SendCommand(char command)
    {
        try
        {
            if (_serialPort.IsOpen)
            {
                _serialPort.Write(command.ToString());
                Console.WriteLine($"Sent '{command}' to esp.");
                System.Threading.Thread.Sleep(100);
            }
            else
            {
                Console.WriteLine("Port is not open.");
            }
        }
        catch (Exception ex)
        {
            Console.WriteLine($"Error: {ex.Message}");
        }
    }
}