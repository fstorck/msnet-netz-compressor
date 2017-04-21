using System;
using System.Runtime.InteropServices;

namespace netz.subsys
{
	public class Win32PESubSystem
	{

		[DllImport("subsys")]
		public static extern int GetSubSystem(string file);

		public static readonly int IMAGE_SUBSYSTEM_ERROR = -1;
		public static readonly int IMAGE_SUBSYSTEM_NATIVE = 1;
		public static readonly int IMAGE_SUBSYSTEM_WINDOWS_GUI = 2;
		public static readonly int IMAGE_SUBSYSTEM_WINDOWS_CUI = 3;
		public static readonly int IMAGE_SUBSYSTEM_OS2_CUI = 5;
		public static readonly int IMAGE_SUBSYSTEM_POSIX_CUI = 7;
		public static readonly int IMAGE_SUBSYSTEM_NATIVE_WINDOWS = 8;
		public static readonly int IMAGE_SUBSYSTEM_WINDOWS_CE_GUI = 9;
	    public static readonly int IMAGE_SUBSYSTEM_EFI_APPLICATION =     10;  //
		public static readonly int IMAGE_SUBSYSTEM_EFI_BOOT_SERVICE_DRIVER = 11;   //
		public static readonly int IMAGE_SUBSYSTEM_EFI_RUNTIME_DRIVER =  12;  //
		public static readonly int IMAGE_SUBSYSTEM_EFI_ROM =             13;
		public static readonly int IMAGE_SUBSYSTEM_XBOX =                14;
		public static readonly int IMAGE_SUBSYSTEM_WINDOWS_BOOT_APPLICATION = 16;
		public static readonly int IMAGE_SUBSYSTEM_XBOX_CODE_CATALOG =   17;
	}//EOC
}