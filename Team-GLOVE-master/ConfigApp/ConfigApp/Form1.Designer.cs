namespace ConfigApp
{
    partial class Form1
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.CancelButton = new System.Windows.Forms.Button();
            this.OkButton = new System.Windows.Forms.Button();
            this.ApplyButton = new System.Windows.Forms.Button();
            this.ConnectButton = new System.Windows.Forms.Button();
            this.DisconnectButton = new System.Windows.Forms.Button();
            this.Tabs = new System.Windows.Forms.TabControl();
            this.LeftSettings = new System.Windows.Forms.TabPage();
            this.RightSettings = new System.Windows.Forms.TabPage();
            this.LeftGestureHeader = new System.Windows.Forms.LinkLabel();
            this.LeftMappingHeader = new System.Windows.Forms.Label();
            this.RightGestureHeading = new System.Windows.Forms.LinkLabel();
            this.RightMappingHeader = new System.Windows.Forms.Label();
            this.LeftWristMotionInput = new System.Windows.Forms.ComboBox();
            this.LeftPointerTouchInput = new System.Windows.Forms.ComboBox();
            this.LeftMiddleTouchInput = new System.Windows.Forms.ComboBox();
            this.LeftRingTouchInput = new System.Windows.Forms.ComboBox();
            this.LeftPinkyTouchInput = new System.Windows.Forms.ComboBox();
            this.LeftPointerBendInput = new System.Windows.Forms.ComboBox();
            this.LeftMiddleBendInput = new System.Windows.Forms.ComboBox();
            this.LeftRingBendInput = new System.Windows.Forms.ComboBox();
            this.LeftPinkyBendInput = new System.Windows.Forms.ComboBox();
            this.label2 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.label5 = new System.Windows.Forms.Label();
            this.label6 = new System.Windows.Forms.Label();
            this.label7 = new System.Windows.Forms.Label();
            this.label8 = new System.Windows.Forms.Label();
            this.label9 = new System.Windows.Forms.Label();
            this.label10 = new System.Windows.Forms.Label();
            this.RightWristMotionInput = new System.Windows.Forms.ComboBox();
            this.RightPointerTouchInput = new System.Windows.Forms.ComboBox();
            this.RightMiddleTouchInput = new System.Windows.Forms.ComboBox();
            this.RightRingTouchInput = new System.Windows.Forms.ComboBox();
            this.RightPinkyTouchInput = new System.Windows.Forms.ComboBox();
            this.RightPointerBendInput = new System.Windows.Forms.ComboBox();
            this.RightMiddleBendInput = new System.Windows.Forms.ComboBox();
            this.RightRingBendInput = new System.Windows.Forms.ComboBox();
            this.RightPinkyBendInput = new System.Windows.Forms.ComboBox();
            this.label1 = new System.Windows.Forms.Label();
            this.label11 = new System.Windows.Forms.Label();
            this.label12 = new System.Windows.Forms.Label();
            this.label13 = new System.Windows.Forms.Label();
            this.label14 = new System.Windows.Forms.Label();
            this.label15 = new System.Windows.Forms.Label();
            this.label16 = new System.Windows.Forms.Label();
            this.label17 = new System.Windows.Forms.Label();
            this.label18 = new System.Windows.Forms.Label();
            this.Tabs.SuspendLayout();
            this.LeftSettings.SuspendLayout();
            this.RightSettings.SuspendLayout();
            this.SuspendLayout();
            // 
            // CancelButton
            // 
            this.CancelButton.Location = new System.Drawing.Point(93, 343);
            this.CancelButton.Name = "CancelButton";
            this.CancelButton.Size = new System.Drawing.Size(75, 23);
            this.CancelButton.TabIndex = 0;
            this.CancelButton.Text = "Cancel";
            this.CancelButton.UseVisualStyleBackColor = true;
            this.CancelButton.Click += new System.EventHandler(this.button1_Click);
            // 
            // OkButton
            // 
            this.OkButton.Location = new System.Drawing.Point(12, 343);
            this.OkButton.Name = "OkButton";
            this.OkButton.Size = new System.Drawing.Size(75, 23);
            this.OkButton.TabIndex = 1;
            this.OkButton.Text = "Ok";
            this.OkButton.UseVisualStyleBackColor = true;
            // 
            // ApplyButton
            // 
            this.ApplyButton.Location = new System.Drawing.Point(174, 343);
            this.ApplyButton.Name = "ApplyButton";
            this.ApplyButton.Size = new System.Drawing.Size(75, 23);
            this.ApplyButton.TabIndex = 2;
            this.ApplyButton.Text = "Apply";
            this.ApplyButton.UseVisualStyleBackColor = true;
            // 
            // ConnectButton
            // 
            this.ConnectButton.Location = new System.Drawing.Point(12, 12);
            this.ConnectButton.Name = "ConnectButton";
            this.ConnectButton.Size = new System.Drawing.Size(75, 23);
            this.ConnectButton.TabIndex = 3;
            this.ConnectButton.Text = "Connect";
            this.ConnectButton.TextImageRelation = System.Windows.Forms.TextImageRelation.ImageAboveText;
            this.ConnectButton.UseVisualStyleBackColor = true;
            // 
            // DisconnectButton
            // 
            this.DisconnectButton.Location = new System.Drawing.Point(93, 12);
            this.DisconnectButton.Name = "DisconnectButton";
            this.DisconnectButton.Size = new System.Drawing.Size(75, 23);
            this.DisconnectButton.TabIndex = 5;
            this.DisconnectButton.Text = "Disconnect";
            this.DisconnectButton.UseVisualStyleBackColor = true;
            // 
            // Tabs
            // 
            this.Tabs.Controls.Add(this.LeftSettings);
            this.Tabs.Controls.Add(this.RightSettings);
            this.Tabs.Location = new System.Drawing.Point(12, 41);
            this.Tabs.Multiline = true;
            this.Tabs.Name = "Tabs";
            this.Tabs.SelectedIndex = 0;
            this.Tabs.Size = new System.Drawing.Size(281, 296);
            this.Tabs.TabIndex = 6;
            // 
            // LeftSettings
            // 
            this.LeftSettings.BackColor = System.Drawing.SystemColors.ControlLight;
            this.LeftSettings.Controls.Add(this.label10);
            this.LeftSettings.Controls.Add(this.label9);
            this.LeftSettings.Controls.Add(this.label8);
            this.LeftSettings.Controls.Add(this.label7);
            this.LeftSettings.Controls.Add(this.label6);
            this.LeftSettings.Controls.Add(this.label5);
            this.LeftSettings.Controls.Add(this.label4);
            this.LeftSettings.Controls.Add(this.label3);
            this.LeftSettings.Controls.Add(this.label2);
            this.LeftSettings.Controls.Add(this.LeftPinkyBendInput);
            this.LeftSettings.Controls.Add(this.LeftRingBendInput);
            this.LeftSettings.Controls.Add(this.LeftMiddleBendInput);
            this.LeftSettings.Controls.Add(this.LeftPointerBendInput);
            this.LeftSettings.Controls.Add(this.LeftPinkyTouchInput);
            this.LeftSettings.Controls.Add(this.LeftRingTouchInput);
            this.LeftSettings.Controls.Add(this.LeftMiddleTouchInput);
            this.LeftSettings.Controls.Add(this.LeftPointerTouchInput);
            this.LeftSettings.Controls.Add(this.LeftWristMotionInput);
            this.LeftSettings.Controls.Add(this.LeftMappingHeader);
            this.LeftSettings.Controls.Add(this.LeftGestureHeader);
            this.LeftSettings.Location = new System.Drawing.Point(4, 22);
            this.LeftSettings.Name = "LeftSettings";
            this.LeftSettings.Padding = new System.Windows.Forms.Padding(3);
            this.LeftSettings.Size = new System.Drawing.Size(273, 270);
            this.LeftSettings.TabIndex = 0;
            this.LeftSettings.Text = "Left Glove";
            this.LeftSettings.Click += new System.EventHandler(this.tabPage1_Click_1);
            // 
            // RightSettings
            // 
            this.RightSettings.BackColor = System.Drawing.SystemColors.ControlLight;
            this.RightSettings.Controls.Add(this.label18);
            this.RightSettings.Controls.Add(this.label17);
            this.RightSettings.Controls.Add(this.label16);
            this.RightSettings.Controls.Add(this.label15);
            this.RightSettings.Controls.Add(this.label14);
            this.RightSettings.Controls.Add(this.label13);
            this.RightSettings.Controls.Add(this.label12);
            this.RightSettings.Controls.Add(this.label11);
            this.RightSettings.Controls.Add(this.label1);
            this.RightSettings.Controls.Add(this.RightPinkyBendInput);
            this.RightSettings.Controls.Add(this.RightRingBendInput);
            this.RightSettings.Controls.Add(this.RightMiddleBendInput);
            this.RightSettings.Controls.Add(this.RightPointerBendInput);
            this.RightSettings.Controls.Add(this.RightPinkyTouchInput);
            this.RightSettings.Controls.Add(this.RightRingTouchInput);
            this.RightSettings.Controls.Add(this.RightMiddleTouchInput);
            this.RightSettings.Controls.Add(this.RightPointerTouchInput);
            this.RightSettings.Controls.Add(this.RightWristMotionInput);
            this.RightSettings.Controls.Add(this.RightMappingHeader);
            this.RightSettings.Controls.Add(this.RightGestureHeading);
            this.RightSettings.Location = new System.Drawing.Point(4, 22);
            this.RightSettings.Name = "RightSettings";
            this.RightSettings.Padding = new System.Windows.Forms.Padding(3);
            this.RightSettings.Size = new System.Drawing.Size(273, 270);
            this.RightSettings.TabIndex = 1;
            this.RightSettings.Text = "Right Glove";
            this.RightSettings.Click += new System.EventHandler(this.tabPage2_Click);
            // 
            // LeftGestureHeader
            // 
            this.LeftGestureHeader.AutoSize = true;
            this.LeftGestureHeader.Font = new System.Drawing.Font("Arial", 10F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.LeftGestureHeader.LinkColor = System.Drawing.Color.Black;
            this.LeftGestureHeader.Location = new System.Drawing.Point(6, 3);
            this.LeftGestureHeader.Name = "LeftGestureHeader";
            this.LeftGestureHeader.Size = new System.Drawing.Size(69, 16);
            this.LeftGestureHeader.TabIndex = 0;
            this.LeftGestureHeader.TabStop = true;
            this.LeftGestureHeader.Text = "Gesture:";
            // 
            // LeftMappingHeader
            // 
            this.LeftMappingHeader.AutoSize = true;
            this.LeftMappingHeader.Font = new System.Drawing.Font("Arial", 10F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.LeftMappingHeader.Location = new System.Drawing.Point(128, 3);
            this.LeftMappingHeader.Name = "LeftMappingHeader";
            this.LeftMappingHeader.Size = new System.Drawing.Size(144, 16);
            this.LeftMappingHeader.TabIndex = 1;
            this.LeftMappingHeader.Text = "Keyboard Mapping:";
            // 
            // RightGestureHeading
            // 
            this.RightGestureHeading.AutoSize = true;
            this.RightGestureHeading.Font = new System.Drawing.Font("Arial", 10F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.RightGestureHeading.LinkColor = System.Drawing.Color.Black;
            this.RightGestureHeading.Location = new System.Drawing.Point(6, 3);
            this.RightGestureHeading.Name = "RightGestureHeading";
            this.RightGestureHeading.Size = new System.Drawing.Size(69, 16);
            this.RightGestureHeading.TabIndex = 0;
            this.RightGestureHeading.TabStop = true;
            this.RightGestureHeading.Text = "Gesture:";
            // 
            // RightMappingHeader
            // 
            this.RightMappingHeader.AutoSize = true;
            this.RightMappingHeader.Font = new System.Drawing.Font("Arial", 10F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.RightMappingHeader.Location = new System.Drawing.Point(128, 3);
            this.RightMappingHeader.Name = "RightMappingHeader";
            this.RightMappingHeader.Size = new System.Drawing.Size(144, 16);
            this.RightMappingHeader.TabIndex = 1;
            this.RightMappingHeader.Text = "Keyboard Mapping:";
            // 
            // LeftWristMotionInput
            // 
            this.LeftWristMotionInput.FormattingEnabled = true;
            this.LeftWristMotionInput.Location = new System.Drawing.Point(131, 22);
            this.LeftWristMotionInput.Name = "LeftWristMotionInput";
            this.LeftWristMotionInput.Size = new System.Drawing.Size(121, 21);
            this.LeftWristMotionInput.TabIndex = 2;
            // 
            // LeftPointerTouchInput
            // 
            this.LeftPointerTouchInput.FormattingEnabled = true;
            this.LeftPointerTouchInput.Location = new System.Drawing.Point(131, 49);
            this.LeftPointerTouchInput.Name = "LeftPointerTouchInput";
            this.LeftPointerTouchInput.Size = new System.Drawing.Size(121, 21);
            this.LeftPointerTouchInput.TabIndex = 3;
            // 
            // LeftMiddleTouchInput
            // 
            this.LeftMiddleTouchInput.FormattingEnabled = true;
            this.LeftMiddleTouchInput.Location = new System.Drawing.Point(131, 76);
            this.LeftMiddleTouchInput.Name = "LeftMiddleTouchInput";
            this.LeftMiddleTouchInput.Size = new System.Drawing.Size(121, 21);
            this.LeftMiddleTouchInput.TabIndex = 4;
            // 
            // LeftRingTouchInput
            // 
            this.LeftRingTouchInput.FormattingEnabled = true;
            this.LeftRingTouchInput.Location = new System.Drawing.Point(131, 103);
            this.LeftRingTouchInput.Name = "LeftRingTouchInput";
            this.LeftRingTouchInput.Size = new System.Drawing.Size(121, 21);
            this.LeftRingTouchInput.TabIndex = 5;
            // 
            // LeftPinkyTouchInput
            // 
            this.LeftPinkyTouchInput.FormattingEnabled = true;
            this.LeftPinkyTouchInput.Location = new System.Drawing.Point(131, 130);
            this.LeftPinkyTouchInput.Name = "LeftPinkyTouchInput";
            this.LeftPinkyTouchInput.Size = new System.Drawing.Size(121, 21);
            this.LeftPinkyTouchInput.TabIndex = 6;
            // 
            // LeftPointerBendInput
            // 
            this.LeftPointerBendInput.FormattingEnabled = true;
            this.LeftPointerBendInput.Location = new System.Drawing.Point(131, 157);
            this.LeftPointerBendInput.Name = "LeftPointerBendInput";
            this.LeftPointerBendInput.Size = new System.Drawing.Size(121, 21);
            this.LeftPointerBendInput.TabIndex = 7;
            // 
            // LeftMiddleBendInput
            // 
            this.LeftMiddleBendInput.FormattingEnabled = true;
            this.LeftMiddleBendInput.Location = new System.Drawing.Point(131, 184);
            this.LeftMiddleBendInput.Name = "LeftMiddleBendInput";
            this.LeftMiddleBendInput.Size = new System.Drawing.Size(121, 21);
            this.LeftMiddleBendInput.TabIndex = 8;
            // 
            // LeftRingBendInput
            // 
            this.LeftRingBendInput.FormattingEnabled = true;
            this.LeftRingBendInput.Location = new System.Drawing.Point(131, 211);
            this.LeftRingBendInput.Name = "LeftRingBendInput";
            this.LeftRingBendInput.Size = new System.Drawing.Size(121, 21);
            this.LeftRingBendInput.TabIndex = 9;
            // 
            // LeftPinkyBendInput
            // 
            this.LeftPinkyBendInput.FormattingEnabled = true;
            this.LeftPinkyBendInput.Location = new System.Drawing.Point(131, 238);
            this.LeftPinkyBendInput.Name = "LeftPinkyBendInput";
            this.LeftPinkyBendInput.Size = new System.Drawing.Size(121, 21);
            this.LeftPinkyBendInput.TabIndex = 10;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(6, 25);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(69, 13);
            this.label2.TabIndex = 11;
            this.label2.Text = "Wrist Motion:";
            this.label2.Click += new System.EventHandler(this.label2_Click);
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(6, 52);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(77, 13);
            this.label3.TabIndex = 12;
            this.label3.Text = "Pointer Touch:";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(6, 79);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(75, 13);
            this.label4.TabIndex = 13;
            this.label4.Text = "Middle Touch:";
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(6, 106);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(66, 13);
            this.label5.TabIndex = 14;
            this.label5.Text = "Ring Touch:";
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(6, 133);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(70, 13);
            this.label6.TabIndex = 15;
            this.label6.Text = "Pinky Touch:";
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Location = new System.Drawing.Point(6, 160);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(71, 13);
            this.label7.TabIndex = 16;
            this.label7.Text = "Pointer Bend:";
            // 
            // label8
            // 
            this.label8.AutoSize = true;
            this.label8.Location = new System.Drawing.Point(6, 187);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(69, 13);
            this.label8.TabIndex = 17;
            this.label8.Text = "Middle Bend:";
            // 
            // label9
            // 
            this.label9.AutoSize = true;
            this.label9.Location = new System.Drawing.Point(6, 214);
            this.label9.Name = "label9";
            this.label9.Size = new System.Drawing.Size(60, 13);
            this.label9.TabIndex = 18;
            this.label9.Text = "Ring Bend:";
            // 
            // label10
            // 
            this.label10.AutoSize = true;
            this.label10.Location = new System.Drawing.Point(6, 241);
            this.label10.Name = "label10";
            this.label10.Size = new System.Drawing.Size(64, 13);
            this.label10.TabIndex = 19;
            this.label10.Text = "Pinky Bend:";
            // 
            // RightWristMotionInput
            // 
            this.RightWristMotionInput.FormattingEnabled = true;
            this.RightWristMotionInput.Location = new System.Drawing.Point(131, 22);
            this.RightWristMotionInput.Name = "RightWristMotionInput";
            this.RightWristMotionInput.Size = new System.Drawing.Size(121, 21);
            this.RightWristMotionInput.TabIndex = 2;
            // 
            // RightPointerTouchInput
            // 
            this.RightPointerTouchInput.FormattingEnabled = true;
            this.RightPointerTouchInput.Location = new System.Drawing.Point(131, 49);
            this.RightPointerTouchInput.Name = "RightPointerTouchInput";
            this.RightPointerTouchInput.Size = new System.Drawing.Size(121, 21);
            this.RightPointerTouchInput.TabIndex = 3;
            // 
            // RightMiddleTouchInput
            // 
            this.RightMiddleTouchInput.FormattingEnabled = true;
            this.RightMiddleTouchInput.Location = new System.Drawing.Point(131, 76);
            this.RightMiddleTouchInput.Name = "RightMiddleTouchInput";
            this.RightMiddleTouchInput.Size = new System.Drawing.Size(121, 21);
            this.RightMiddleTouchInput.TabIndex = 4;
            // 
            // RightRingTouchInput
            // 
            this.RightRingTouchInput.FormattingEnabled = true;
            this.RightRingTouchInput.Location = new System.Drawing.Point(131, 103);
            this.RightRingTouchInput.Name = "RightRingTouchInput";
            this.RightRingTouchInput.Size = new System.Drawing.Size(121, 21);
            this.RightRingTouchInput.TabIndex = 5;
            // 
            // RightPinkyTouchInput
            // 
            this.RightPinkyTouchInput.FormattingEnabled = true;
            this.RightPinkyTouchInput.Location = new System.Drawing.Point(131, 130);
            this.RightPinkyTouchInput.Name = "RightPinkyTouchInput";
            this.RightPinkyTouchInput.Size = new System.Drawing.Size(121, 21);
            this.RightPinkyTouchInput.TabIndex = 6;
            // 
            // RightPointerBendInput
            // 
            this.RightPointerBendInput.FormattingEnabled = true;
            this.RightPointerBendInput.Location = new System.Drawing.Point(131, 157);
            this.RightPointerBendInput.Name = "RightPointerBendInput";
            this.RightPointerBendInput.Size = new System.Drawing.Size(121, 21);
            this.RightPointerBendInput.TabIndex = 7;
            // 
            // RightMiddleBendInput
            // 
            this.RightMiddleBendInput.FormattingEnabled = true;
            this.RightMiddleBendInput.Location = new System.Drawing.Point(131, 184);
            this.RightMiddleBendInput.Name = "RightMiddleBendInput";
            this.RightMiddleBendInput.Size = new System.Drawing.Size(121, 21);
            this.RightMiddleBendInput.TabIndex = 8;
            // 
            // RightRingBendInput
            // 
            this.RightRingBendInput.FormattingEnabled = true;
            this.RightRingBendInput.Location = new System.Drawing.Point(131, 211);
            this.RightRingBendInput.Name = "RightRingBendInput";
            this.RightRingBendInput.Size = new System.Drawing.Size(121, 21);
            this.RightRingBendInput.TabIndex = 9;
            // 
            // RightPinkyBendInput
            // 
            this.RightPinkyBendInput.FormattingEnabled = true;
            this.RightPinkyBendInput.Location = new System.Drawing.Point(131, 238);
            this.RightPinkyBendInput.Name = "RightPinkyBendInput";
            this.RightPinkyBendInput.Size = new System.Drawing.Size(121, 21);
            this.RightPinkyBendInput.TabIndex = 10;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(6, 25);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(69, 13);
            this.label1.TabIndex = 11;
            this.label1.Text = "Wrist Motion:";
            // 
            // label11
            // 
            this.label11.AutoSize = true;
            this.label11.Location = new System.Drawing.Point(6, 52);
            this.label11.Name = "label11";
            this.label11.Size = new System.Drawing.Size(77, 13);
            this.label11.TabIndex = 12;
            this.label11.Text = "Pointer Touch:";
            // 
            // label12
            // 
            this.label12.AutoSize = true;
            this.label12.Location = new System.Drawing.Point(6, 79);
            this.label12.Name = "label12";
            this.label12.Size = new System.Drawing.Size(75, 13);
            this.label12.TabIndex = 13;
            this.label12.Text = "Middle Touch:";
            // 
            // label13
            // 
            this.label13.AutoSize = true;
            this.label13.Location = new System.Drawing.Point(6, 106);
            this.label13.Name = "label13";
            this.label13.Size = new System.Drawing.Size(66, 13);
            this.label13.TabIndex = 14;
            this.label13.Text = "Ring Touch:";
            // 
            // label14
            // 
            this.label14.AutoSize = true;
            this.label14.Location = new System.Drawing.Point(6, 133);
            this.label14.Name = "label14";
            this.label14.Size = new System.Drawing.Size(70, 13);
            this.label14.TabIndex = 15;
            this.label14.Text = "Pinky Touch:";
            // 
            // label15
            // 
            this.label15.AutoSize = true;
            this.label15.Location = new System.Drawing.Point(6, 160);
            this.label15.Name = "label15";
            this.label15.Size = new System.Drawing.Size(71, 13);
            this.label15.TabIndex = 16;
            this.label15.Text = "Pointer Bend:";
            // 
            // label16
            // 
            this.label16.AutoSize = true;
            this.label16.Location = new System.Drawing.Point(6, 187);
            this.label16.Name = "label16";
            this.label16.Size = new System.Drawing.Size(69, 13);
            this.label16.TabIndex = 17;
            this.label16.Text = "Middle Bend:";
            // 
            // label17
            // 
            this.label17.AutoSize = true;
            this.label17.Location = new System.Drawing.Point(6, 214);
            this.label17.Name = "label17";
            this.label17.Size = new System.Drawing.Size(60, 13);
            this.label17.TabIndex = 18;
            this.label17.Text = "Ring Bend:";
            // 
            // label18
            // 
            this.label18.AutoSize = true;
            this.label18.Location = new System.Drawing.Point(6, 241);
            this.label18.Name = "label18";
            this.label18.Size = new System.Drawing.Size(64, 13);
            this.label18.TabIndex = 19;
            this.label18.Text = "Pinky Bend:";
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.SystemColors.ActiveCaption;
            this.ClientSize = new System.Drawing.Size(305, 372);
            this.Controls.Add(this.Tabs);
            this.Controls.Add(this.DisconnectButton);
            this.Controls.Add(this.ConnectButton);
            this.Controls.Add(this.ApplyButton);
            this.Controls.Add(this.OkButton);
            this.Controls.Add(this.CancelButton);
            this.Name = "Form1";
            this.Text = "Glove Configuration";
            this.Load += new System.EventHandler(this.Form1_Load);
            this.Tabs.ResumeLayout(false);
            this.LeftSettings.ResumeLayout(false);
            this.LeftSettings.PerformLayout();
            this.RightSettings.ResumeLayout(false);
            this.RightSettings.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Button CancelButton;
        private System.Windows.Forms.Button OkButton;
        private System.Windows.Forms.Button ApplyButton;
        private System.Windows.Forms.Button ConnectButton;
        private System.Windows.Forms.Button DisconnectButton;
        private System.Windows.Forms.TabControl Tabs;
        private System.Windows.Forms.TabPage LeftSettings;
        private System.Windows.Forms.TabPage RightSettings;
        private System.Windows.Forms.Label LeftMappingHeader;
        private System.Windows.Forms.LinkLabel LeftGestureHeader;
        private System.Windows.Forms.Label RightMappingHeader;
        private System.Windows.Forms.LinkLabel RightGestureHeading;
        private System.Windows.Forms.ComboBox LeftPinkyBendInput;
        private System.Windows.Forms.ComboBox LeftRingBendInput;
        private System.Windows.Forms.ComboBox LeftMiddleBendInput;
        private System.Windows.Forms.ComboBox LeftPointerBendInput;
        private System.Windows.Forms.ComboBox LeftPinkyTouchInput;
        private System.Windows.Forms.ComboBox LeftRingTouchInput;
        private System.Windows.Forms.ComboBox LeftMiddleTouchInput;
        private System.Windows.Forms.ComboBox LeftPointerTouchInput;
        private System.Windows.Forms.ComboBox LeftWristMotionInput;
        private System.Windows.Forms.Label label10;
        private System.Windows.Forms.Label label9;
        private System.Windows.Forms.Label label8;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label18;
        private System.Windows.Forms.Label label17;
        private System.Windows.Forms.Label label16;
        private System.Windows.Forms.Label label15;
        private System.Windows.Forms.Label label14;
        private System.Windows.Forms.Label label13;
        private System.Windows.Forms.Label label12;
        private System.Windows.Forms.Label label11;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.ComboBox RightPinkyBendInput;
        private System.Windows.Forms.ComboBox RightRingBendInput;
        private System.Windows.Forms.ComboBox RightMiddleBendInput;
        private System.Windows.Forms.ComboBox RightPointerBendInput;
        private System.Windows.Forms.ComboBox RightPinkyTouchInput;
        private System.Windows.Forms.ComboBox RightRingTouchInput;
        private System.Windows.Forms.ComboBox RightMiddleTouchInput;
        private System.Windows.Forms.ComboBox RightPointerTouchInput;
        private System.Windows.Forms.ComboBox RightWristMotionInput;
    }
}

