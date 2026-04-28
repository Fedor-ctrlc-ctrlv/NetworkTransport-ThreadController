namespace SharpCherkasov
{
    partial class FormCherkasov
    {
        private System.ComponentModel.IContainer components = null;

        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
                components.Dispose();
            base.Dispose(disposing);
        }

        private void InitializeComponent()
        {
            this.btnStart = new System.Windows.Forms.Button();
            this.btnStop = new System.Windows.Forms.Button();
            this.btnSend = new System.Windows.Forms.Button();
            this.numericThreadCount = new System.Windows.Forms.NumericUpDown();
            this.textInput = new System.Windows.Forms.TextBox();
            this.comboBoxThreads = new System.Windows.Forms.ComboBox();
            this.lblThreadCount = new System.Windows.Forms.Label();
            this.lblInput = new System.Windows.Forms.Label();
            this.lblStatus = new System.Windows.Forms.Label();
            this.statusStrip = new System.Windows.Forms.StatusStrip();
            this.toolStripStatusLabel = new System.Windows.Forms.ToolStripStatusLabel();
            ((System.ComponentModel.ISupportInitialize)(this.numericThreadCount)).BeginInit();
            this.statusStrip.SuspendLayout();
            this.SuspendLayout();

            this.btnStart.Location = new System.Drawing.Point(30, 30);
            this.btnStart.Name = "btnStart";
            this.btnStart.Size = new System.Drawing.Size(90, 35);
            this.btnStart.TabIndex = 0;
            this.btnStart.Text = "Start";
            this.btnStart.UseVisualStyleBackColor = true;
            this.btnStart.Click += new System.EventHandler(this.btnStart_Click);

            this.btnStop.Location = new System.Drawing.Point(130, 30);
            this.btnStop.Name = "btnStop";
            this.btnStop.Size = new System.Drawing.Size(90, 35);
            this.btnStop.TabIndex = 1;
            this.btnStop.Text = "Stop";
            this.btnStop.UseVisualStyleBackColor = true;
            this.btnStop.Click += new System.EventHandler(this.btnStop_Click);

            this.btnSend.Location = new System.Drawing.Point(230, 30);
            this.btnSend.Name = "btnSend";
            this.btnSend.Size = new System.Drawing.Size(90, 35);
            this.btnSend.TabIndex = 2;
            this.btnSend.Text = "Send";
            this.btnSend.UseVisualStyleBackColor = true;
            this.btnSend.Click += new System.EventHandler(this.btnSend_Click);

            this.numericThreadCount.Location = new System.Drawing.Point(130, 85);
            this.numericThreadCount.Minimum = new decimal(new int[] { 1, 0, 0, 0 });
            this.numericThreadCount.Maximum = new decimal(new int[] { 10, 0, 0, 0 });
            this.numericThreadCount.Value = new decimal(new int[] { 1, 0, 0, 0 });
            this.numericThreadCount.Name = "numericThreadCount";
            this.numericThreadCount.Size = new System.Drawing.Size(60, 23);
            this.numericThreadCount.TabIndex = 3;

            this.lblThreadCount.AutoSize = true;
            this.lblThreadCount.Location = new System.Drawing.Point(30, 87);
            this.lblThreadCount.Name = "lblThreadCount";
            this.lblThreadCount.Size = new System.Drawing.Size(94, 15);
            this.lblThreadCount.TabIndex = 4;
            this.lblThreadCount.Text = "Количество:";

            this.textInput.Location = new System.Drawing.Point(130, 120);
            this.textInput.Name = "textInput";
            this.textInput.Size = new System.Drawing.Size(220, 23);
            this.textInput.TabIndex = 5;

            this.lblInput.AutoSize = true;
            this.lblInput.Location = new System.Drawing.Point(30, 123);
            this.lblInput.Name = "lblInput";
            this.lblInput.Size = new System.Drawing.Size(81, 15);
            this.lblInput.TabIndex = 6;
            this.lblInput.Text = "Сообщение:";

            this.comboBoxThreads.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboBoxThreads.FormattingEnabled = true;
            this.comboBoxThreads.Location = new System.Drawing.Point(130, 155);
            this.comboBoxThreads.Name = "comboBoxThreads";
            this.comboBoxThreads.Size = new System.Drawing.Size(220, 23);
            this.comboBoxThreads.TabIndex = 7;
            this.comboBoxThreads.DropDown += new System.EventHandler(this.comboBoxThreads_DropDown);

            this.lblStatus.AutoSize = true;
            this.lblStatus.Font = new System.Drawing.Font("Segoe UI", 9F, System.Drawing.FontStyle.Italic);
            this.lblStatus.ForeColor = System.Drawing.Color.Gray;
            this.lblStatus.Location = new System.Drawing.Point(30, 195);
            this.lblStatus.Name = "lblStatus";
            this.lblStatus.Size = new System.Drawing.Size(96, 15);
            this.lblStatus.TabIndex = 8;
            this.lblStatus.Text = "Статус: готов";

            this.statusStrip.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.toolStripStatusLabel});
            this.statusStrip.Location = new System.Drawing.Point(0, 230);
            this.statusStrip.Name = "statusStrip";
            this.statusStrip.Size = new System.Drawing.Size(384, 22);
            this.statusStrip.TabIndex = 9;

            this.toolStripStatusLabel.Name = "toolStripStatusLabel";
            this.toolStripStatusLabel.Size = new System.Drawing.Size(98, 17);
            this.toolStripStatusLabel.Text = "Не подключено";

            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 15F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(384, 252);
            this.Controls.Add(this.statusStrip);
            this.Controls.Add(this.lblStatus);
            this.Controls.Add(this.comboBoxThreads);
            this.Controls.Add(this.lblInput);
            this.Controls.Add(this.textInput);
            this.Controls.Add(this.lblThreadCount);
            this.Controls.Add(this.numericThreadCount);
            this.Controls.Add(this.btnSend);
            this.Controls.Add(this.btnStop);
            this.Controls.Add(this.btnStart);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
            this.MaximizeBox = false;
            this.Name = "FormCherkasov";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "Cherkasov Lab3 - Клиент";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.FormCherkasov_FormClosing);
            this.Load += new System.EventHandler(this.FormCherkasov_Load);
            ((System.ComponentModel.ISupportInitialize)(this.numericThreadCount)).EndInit();
            this.statusStrip.ResumeLayout(false);
            this.statusStrip.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();
        }

        private System.Windows.Forms.Button btnStart;
        private System.Windows.Forms.Button btnStop;
        private System.Windows.Forms.Button btnSend;
        private System.Windows.Forms.NumericUpDown numericThreadCount;
        private System.Windows.Forms.TextBox textInput;
        private System.Windows.Forms.ComboBox comboBoxThreads;
        private System.Windows.Forms.Label lblThreadCount;
        private System.Windows.Forms.Label lblInput;
        private System.Windows.Forms.Label lblStatus;
        private System.Windows.Forms.StatusStrip statusStrip;
        private System.Windows.Forms.ToolStripStatusLabel toolStripStatusLabel;
    }
}