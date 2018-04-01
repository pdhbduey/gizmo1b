using System.Windows;

namespace SiemensTestProgram
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        public MainWindow()
        {
            InitializeComponent();
            DataContext = Factory.Instance.GetMainViewModel();
        }
    }
}
