using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Net;
using System.Data;

namespace DBUI
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
            AsynchronousClient.form = this;
        }

        private void toolStripButton1_Click(object sender, EventArgs e)
        {
            string []ips = IPTextbox.Text.Split('.');
            if (ips.Count() != 4) return;
            byte[] ipb = new byte[4];
            for (int i = 0; i < 4; i++) ipb[i] = Byte.Parse(ips[i]);
            AsynchronousClient.ipAddress = new IPAddress(ipb);
            AsynchronousClient.port = Int32.Parse(PortTextbox.Text);
            AsynchronousClient.StartClient();
            toolStripButton1.Enabled = false;
        }

        private void button1_Click(object sender, EventArgs e)
        {
            AsynchronousClient.Send(textBox1.Text);
            AsynchronousClient.Receive();
            //AsynchronousClient.receiveDone.WaitOne();
        }

        public void setData(string str)
        {
            string[] lines = str.Split('\n');
            if (lines.Count() > 2)
            {
                string []cols =  lines[0].Split(' ');
                string []types = lines[1].Split(' ');
                dataGridView1.Columns.Clear();
                DataTable table = new DataTable();
                if (cols[cols.Length - 1] == "")
                {
                    List<string> tmp = new List<string>(cols);
                    tmp.RemoveAt(tmp.Count - 1);
                    cols = tmp.ToArray();
                }

                for (int i = 0; i < cols.Count(); i++) {
                    
                    switch (int.Parse(types[i * 2]))
                    {
                        case 0:
                            table.Columns.Add(cols[i], typeof(int)); //int
                            break;
                        case 1:
                            table.Columns.Add(cols[i], typeof(string)); //varchar
                            break;
                        default:
                            table.Columns.Add(cols[i]); //varchar
                            break;
                    }
                }
                for (int i = 2; i < lines.Count(); i++) {
                    DataRow row = table.NewRow();
                    string []rowdata = lines[i].Split(' ');
                    if (rowdata[0] == "") break;
                    object[] rowArray = new object[cols.Count()];
                    for (int j = 0; j < System.Math.Min(cols.Count(), rowdata.Count()) ; j++)
                    {
                        switch (int.Parse(types[j * 2]))
                        {
                            case 0:
                                rowArray[j] = int.Parse(rowdata[j]); //int
                                break;
                            case 1:
                                rowArray[j] = rowdata[j]; //varchar
                                break;
                        };
                    }
                    row.ItemArray = rowArray;
                    table.Rows.Add(row);

                }
                dataGridView1.DataSource = table;
            }
        }

    } //class Form1

} //namespace DBUI
