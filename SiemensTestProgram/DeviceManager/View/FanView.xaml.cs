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
    /// Interaction logic for FanView.xaml
    /// </summary>
    public partial class FanView : UserControl
    {
        public FanView()
        {
            InitializeComponent();
        }

        private void EnterUpdate(object sender, KeyEventArgs e)
        {
            if (e.Key == Key.Enter)
            {
                TextBox textBox = (TextBox)sender;
                DependencyProperty property = TextBox.TextProperty;

                BindingExpression binding = BindingOperations.GetBindingExpression(textBox, property);
                if (binding != null) { binding.UpdateSource(); }
            }
        }
    }
}
