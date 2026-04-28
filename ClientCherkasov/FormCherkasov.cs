using System;
using System.Runtime.InteropServices;
using System.Threading;
using System.Windows.Forms;

namespace SharpCherkasov
{
    public partial class FormCherkasov : Form
    {
        private int threadCounter = 0;
        private Thread receiveThread;
        private bool receiveRunning = true;
        private bool isFirstLoad = true;

        [DllImport("NetworkLibCherkasov.dll", CallingConvention = CallingConvention.StdCall)]
        public static extern bool ConnectToServer(string host, int port);

        [DllImport("NetworkLibCherkasov.dll", CallingConvention = CallingConvention.StdCall)]
        public static extern void DisconnectFromServer();

        [DllImport("NetworkLibCherkasov.dll", CallingConvention = CallingConvention.StdCall)]
        public static extern void SendMsgFromSharp(MsgStructCherkasov msg);

        [DllImport("NetworkLibCherkasov.dll", CallingConvention = CallingConvention.StdCall)]
        public static extern bool IsConnected();

        [DllImport("NetworkLibCherkasov.dll", CallingConvention = CallingConvention.StdCall)]
        public static extern bool ReceiveMessageFromServer(out MsgStructCherkasov msg);

        public FormCherkasov()
        {
            InitializeComponent();

            if (!ConnectToServer("127.0.0.1", 5555))
            {
                toolStripStatusLabel.Text = "Не подключено";
                lblStatus.Text = "Статус: ошибка";
                MessageBox.Show("Сервер не запущен!");
                return;
            }

            toolStripStatusLabel.Text = "Подключено";
            lblStatus.Text = "Статус: подключение...";

            StartReceiveThread();

            //UpdateThreadCount();
        }

        private void StartReceiveThread()
        {
            receiveThread = new Thread(() =>
            {
                while (receiveRunning && IsConnected())
                {
                    try
                    {
                        MsgStructCherkasov msg;
                        if (ReceiveMessageFromServer(out msg))
                        {
                            if (msg.msgType == 3) // MT_INFO
                            {
                                if (int.TryParse(msg.data, out int count))
                                {
                                    UpdateUIThreadCount(count);
                                }
                            }
                        }
                        Thread.Sleep(10);
                    }
                    catch (Exception ex)
                    {
                        Console.WriteLine($"Receive error: {ex.Message}");
                        Thread.Sleep(100);
                    }
                }
            });
            receiveThread.IsBackground = true;
            receiveThread.Start();
        }

        private void UpdateUIThreadCount(int count)
        {
            if (this.InvokeRequired)
            {
                this.Invoke(new Action<int>(UpdateUIThreadCount), count);
                return;
            }

            threadCounter = count;
            comboBoxThreads.Items.Clear();
            comboBoxThreads.Items.Add("Главный поток");
            comboBoxThreads.Items.Add("Все потоки");

            for (int i = 1; i <= threadCounter; i++)
                comboBoxThreads.Items.Add($"Поток {i}");

            if (comboBoxThreads.Items.Count > 0 && comboBoxThreads.SelectedIndex == -1)
                comboBoxThreads.SelectedIndex = 0;

            toolStripStatusLabel.Text = $"Потоков: {threadCounter}";
            lblStatus.Text = $"Статус: активных потоков - {threadCounter}";

            Console.WriteLine($"UI обновлён: {threadCounter} потоков");
        }

        private void UpdateThreadCount()
        {
            Console.WriteLine("Запрашиваем количество потоков...");
            SendMsgFromSharp(MsgFactoryCherkasov.InfoMsg);
        }

        private void btnStart_Click(object sender, EventArgs e)
        {
            int count = (int)numericThreadCount.Value;
            Console.WriteLine($"Start {count} потоков...");

            for (int i = 0; i < count; i++)
            {
                SendMsgFromSharp(MsgFactoryCherkasov.StartMsg);
                Thread.Sleep(100);
            }
        }

        private void btnStop_Click(object sender, EventArgs e)
        {
            if (threadCounter > 0)
            {
                Console.WriteLine("Stop один поток...");
                SendMsgFromSharp(MsgFactoryCherkasov.StopMsg);
            }
        }

        private void btnSend_Click(object sender, EventArgs e)
        {
            if (string.IsNullOrWhiteSpace(textInput.Text)) return;

            int selectedIndex = comboBoxThreads.SelectedIndex;
            int to;

            if (selectedIndex == 0)
                to = -2;
            else if (selectedIndex == 1)
                to = -1;
            else
                to = selectedIndex - 2;

            Console.WriteLine($"Send to={to}, text={textInput.Text}");

            MsgStructCherkasov dataMsg = new MsgStructCherkasov
            {
                to = to,
                msgType = 2, // MT_DATA
                data = textInput.Text
            };
            SendMsgFromSharp(dataMsg);
        }

        private void comboBoxThreads_DropDown(object sender, EventArgs e)
        {
            UpdateThreadCount();
        }

        private void FormCherkasov_FormClosing(object sender, FormClosingEventArgs e)
        {
            receiveRunning = false;
            if (receiveThread != null && receiveThread.IsAlive)
                receiveThread.Join(500);
            DisconnectFromServer();
        }

        private void FormCherkasov_Load(object sender, EventArgs e)
        {
            numericThreadCount.Minimum = 1;
            numericThreadCount.Maximum = 10;
            numericThreadCount.Value = 1;
        }
    }
}