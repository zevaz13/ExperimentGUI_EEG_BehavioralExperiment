
using System;
using System.IO;
using System.Linq;
using System.Windows.Forms;
using RJCP.IO.Ports;

namespace GUImetamers
{
    public partial class screen3LinBeh : Form
    {
        private Screen2 previousScreen; // Store a reference to the previous screen
        private string expType = "";
        private string subID = "";
        private string fileName = "";
        private int expFlag = 0;
        private string command = "";
        private string folderName;
        private string newFileName;

        private string currentFileName;
        private StreamWriter? writer; // Class-level StreamWriter for the data file
        private SerialPortStream serialPort;
        
        public screen3LinBeh(Screen2 screen2, int expMode, string subjectID, string Folder, SerialPortStream port)
        {
            subID = subjectID;
            expFlag = expMode;
            folderName = Folder;
            InitializeComponent();
            serialPort = port;
            previousScreen = screen2; // Initialize the previous screen reference

            switch (expMode)
            {
                case 1:
                    expType = "RGgridEEG";
                    this.labelHeader.Text = "grid Red Green EEG experiment";
                    break;
                case 2:
                    expType = "BGgridEEG";
                    this.labelHeader.Text = "grid Blue Green EEG experiment";
                    break;
                default:
                    break;
            }
        }

        private void ActionButton1_Click(object sender, EventArgs e)
        {
            //CloseFile();
            this.Hide();
            previousScreen.Show();
        }

        private void ActionButton2_Click(object sender, EventArgs e)
        {
            ConfigureAction("", expFlag == 1 ? "RGgrid" : "BGgrid");
            //Thread.Sleep(500);
            Form1.serialPort.WriteLine("@1!");
        }

        private void ActionButton3_Click(object sender, EventArgs e)
        {
            StopDataCollection();
        }

        private void ActionButton4_Click(object sender, EventArgs e)
        {
            StopDataCollection();
        }

        private void ConfigureAction(string actionSuffix, string commandCode)
        {
            ToggleButtons(false);
            fileName = $"{subID}_{expType}_{actionSuffix}";
            command = commandCode;
            Form1.serialPort.WriteLine(command);
            //CreateTextFileWithHeader();
            //this.infoLabel.Text = Form1.serialPort.ReadLine();
            Form1.serialPort.DataReceived -= DataReceivedHandler;
            Form1.serialPort.DataReceived += DataReceivedHandler;
        }
        private void DataReceivedHandler(object sender, SerialDataReceivedEventArgs e)
        {
            try
            {
                string response = Form1.serialPort.ReadLine();
                if (!string.IsNullOrEmpty(response))
                {
                    this.Invoke((MethodInvoker)delegate
                    {
                        infoLabel.Text = response; // or += if you want to append
                    });
                }
            }
            catch (Exception ex)
            {
                this.Invoke((MethodInvoker)delegate
                {
                    infoLabel.Text = "Error: " + ex.Message;
                });
            }
        }


        private void ToggleButtons(bool enable)
        {
            this.actionButton1.Enabled = enable;
            this.actionButton3.Enabled = !enable;
            this.actionButton2.Enabled = enable;
            //this.actionButton4.Enabled = !enable;
        }

        private void StopDataCollection()
        {
            ToggleButtons(true);
            this.infoLabel.Text = "Select an action above to proceed.";
            Form1.serialPort.WriteLine("stop");
            Form1.serialPort.DataReceived -= DataReceivedHandler;

            //serialPort.DataReceived -= DataReceivedHandler; // Unsubscribe from event to prevent further data capture
            //CloseFile();
        }


        private void DisplayError(string message)
        {
            this.Invoke((MethodInvoker)delegate
            {
                infoLabel.Text += "\n" + message;
            });
        }
    }
}
