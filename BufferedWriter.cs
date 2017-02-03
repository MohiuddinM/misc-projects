using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.IO;

namespace BufferedWriter
{
    class BufferedWriter
    {
        
        private static List<string> Buffer;
        private static Thread WriterThread;
        private static StreamWriter Writer;
        
	public BufferedWriter(string path)
        { 
            Buffer = new List<string>();
            WriterThread = new Thread(new ParameterizedThreadStart(BufferWriter));
            WriterThread.Priority = ThreadPriority.BelowNormal;
            WriterThread.Start(path);
        }
        
	public void Write(string content)
        {
            Buffer.Add(content);
        }
        
	public void WriteLine(string content)
        {
            Buffer.Add(content);
        }
        
	public void Dispose()
        {
            while (Buffer.Count != 0)
                Thread.Sleep(100);

            try
            {
                Writer.Flush();
                Writer.Dispose();
            }
            catch { }

            
            WriterThread.Abort();
            while (WriterThread.IsAlive);
        }
        
	private static void BufferWriter(object path)
        {
            Writer = File.AppendText((string) path);
            while (true)
            {
                try
                {
                    int count = Buffer.Count;
                    if (count != 0)
                    {
                        for (int i = 0; i < count; i++)
                        {
                            Writer.WriteLine(Buffer[0]);
                            Buffer.Remove(Buffer[0]);
                        }
                    }
                }
                catch { }
                Thread.Sleep(100);
            }
        }
    }
}
