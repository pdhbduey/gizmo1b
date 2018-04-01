using DeviceManager.ViewModel;
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
    /// Interaction logic for TecView.xaml
    /// </summary>
    public partial class TecView : UserControl
    {
        public TecView()
        {
            InitializeComponent();
        }

        private void Slider_DragCompleted(object sender, System.Windows.Controls.Primitives.DragCompletedEventArgs e)
        {
            if (sender is UserControl)
            {
                var userControl = (UserControl)sender;

                if (userControl.DataContext is TecViewModel)
                {
                    var tecViewModel = (TecViewModel)userControl.DataContext;
                    //tecViewModel.UpdateIref();
                }
                
            }
        }
    }
}
