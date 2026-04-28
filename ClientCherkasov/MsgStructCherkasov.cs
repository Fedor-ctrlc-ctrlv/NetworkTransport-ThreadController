using System.Runtime.InteropServices;

namespace SharpCherkasov
{
    [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Unicode)]
    public struct MsgStructCherkasov
    {
        public int to;
        public int msgType;
        [MarshalAs(UnmanagedType.LPWStr)]
        public string data;
    }

    public static class MsgFactoryCherkasov
    {
        public static MsgStructCherkasov StartMsg = new MsgStructCherkasov
        {
            to = -2,
            msgType = 0,
            data = ""
        };

        public static MsgStructCherkasov StopMsg = new MsgStructCherkasov
        {
            to = -2,
            msgType = 1,
            data = ""
        };

        public static MsgStructCherkasov InfoMsg = new MsgStructCherkasov
        {
            to = -2,
            msgType = 3,
            data = ""
        };
    }
}