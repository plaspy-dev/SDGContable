using System;
using System.ComponentModel;
using System.Runtime.InteropServices;

static class CheckApplicationModule
{
    [StructLayout(LayoutKind.Sequential)]
    private struct Bitmap
    {
        public int Type;
        public int Width;
        public int Height;
        public int WidthBytes;
        public ushort Planes;
        public ushort BitsPerPixel;
        public IntPtr Pixels;
    }

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    private delegate IntPtr ReadLogo();

    [DllImport("kernel32.dll", CharSet = CharSet.Unicode, SetLastError = true)]
    private static extern IntPtr LoadLibrary(string name);

    [DllImport("kernel32.dll", CharSet = CharSet.Ansi, ExactSpelling = true, SetLastError = true)]
    private static extern IntPtr GetProcAddress(IntPtr library, string name);

    [DllImport("kernel32.dll")]
    private static extern bool FreeLibrary(IntPtr library);

    [DllImport("gdi32.dll", EntryPoint = "GetObjectW", SetLastError = true)]
    private static extern int ReadBitmap(IntPtr handle, int size, out Bitmap image);

    [DllImport("gdi32.dll")]
    private static extern bool DeleteObject(IntPtr handle);

    public static int Main(string[] arguments)
    {
        if (arguments.Length != 1)
        {
            Console.Error.WriteLine("Usage / Uso: ProbarModulo.exe <appt.dll>");
            return 2;
        }

        IntPtr module = LoadLibrary(arguments[0]);
        if (module == IntPtr.Zero)
            throw new Win32Exception(Marshal.GetLastWin32Error());

        try
        {
            foreach (string name in new[] { "GetExeName", "GetAppName", "GetLongName",
                                            "GetCompany", "GetLogo", "GetAppKey", "GetIDApp", "GetURL" })
            {
                if (GetProcAddress(module, name) == IntPtr.Zero)
                    throw new Win32Exception(Marshal.GetLastWin32Error(), name);
            }

            IntPtr address = GetProcAddress(module, "GetLogo");
            var readLogo = (ReadLogo)Marshal.GetDelegateForFunctionPointer(address, typeof(ReadLogo));
            IntPtr handle = readLogo();
            if (handle == IntPtr.Zero)
                throw new InvalidOperationException("Missing bitmap resource / Falta el mapa de bits");

            try
            {
                Bitmap image;
                if (ReadBitmap(handle, Marshal.SizeOf(typeof(Bitmap)), out image) == 0)
                    throw new Win32Exception(Marshal.GetLastWin32Error());
                Console.WriteLine("appt.dll: 8 exports / 8 exportaciones");
                Console.WriteLine("GetLogo: {0} x {1}", image.Width, image.Height);
                return image.Width > 0 && image.Height > 0 ? 0 : 1;
            }
            finally
            {
                DeleteObject(handle);
            }
        }
        finally
        {
            FreeLibrary(module);
        }
    }
}
