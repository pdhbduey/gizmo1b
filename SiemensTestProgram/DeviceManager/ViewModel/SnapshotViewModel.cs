using Common.Bindings;
using DeviceManager.Model;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DeviceManager.ViewModel
{
    public class SnapshotViewModel : BindableBase
    {
        private ISnapshotModel snapshotModel;
        private int selectedResolution;
        private int numberOfSamples;

        public SnapshotViewModel(ISnapshotModel snapshotModel)
        {
            this.snapshotModel = snapshotModel;

            Resolutions = SnapshotDefaults.Resolutions;
            SelectedResolution = Resolutions[0];
        }

        public RelayCommand StartCommand { get; set; }

        public RelayCommand StopCommand { get; set; }

        public RelayCommand SetNumberOfSamplesCommand { get; set; }

        public List<int> Resolutions { get; set; }

        public int SelectedResolution
        {
            get
            {
                return selectedResolution;
            }

            set
            {
                selectedResolution = value;
                OnPropertyChanged(nameof(SelectedResolution));
            }
        }

        public int NumberOfSamples
        {
            get
            {
                return numberOfSamples;
            }

            set
            {
                numberOfSamples = value;
                OnPropertyChanged(nameof(NumberOfSamples));
            }
        }


    }
}
