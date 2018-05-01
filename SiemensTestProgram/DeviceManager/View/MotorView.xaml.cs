using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace DeviceManager.View
{
    /// <summary>
    /// Interaction logic for MotorView.xaml
    /// </summary>
    public partial class MotorView : UserControl
    {
        public MotorView()
        {
            InitializeComponent();
        }

        private void TextBox_TextChanged(object sender, TextChangedEventArgs e)
        {
            //string item = registerTextbox.Text;
            //int n = 0;
            //if (!int.TryParse(item, System.Globalization.NumberStyles.HexNumber, System.Globalization.NumberFormatInfo.CurrentInfo, out n) &&
            //  item != String.Empty)
            //{
            //    registerTextbox.Text = item.Remove(item.Length - 1, 1);
            //    registerTextbox.SelectionStart = registerTextbox.Text.Length;
            //}
        }
    }
}
