using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Collections.Specialized;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Common
{
    public class BulkObservableCollection<T> : ObservableCollection<T>
    {
        private bool suppressNotification = false;
        private int updateThreshold;
        private int counter;

        public BulkObservableCollection(int threshold)
        {
            updateThreshold = threshold;
            counter = 0;
        }

        public int UpdateThreshold { get; set; }

        protected override void OnCollectionChanged(NotifyCollectionChangedEventArgs e)
        {
            if (!suppressNotification)
                base.OnCollectionChanged(e);
        }

        public void SurpressedAdd(T item)
        {
            suppressNotification = true;
            Add(item);
            counter++;

            if (counter >= updateThreshold)
            {
                suppressNotification = false;
                OnCollectionChanged(new NotifyCollectionChangedEventArgs(NotifyCollectionChangedAction.Reset));
                counter = 0;
            }
        }

        public void SurpressedRemoveAt(int index)
        {
            suppressNotification = true;
            RemoveAt(index);
        }

        public void AddRangeAndUpdate(IEnumerable<T> list)
        {
            if (list == null)
                throw new ArgumentNullException("list");

            suppressNotification = true;

            foreach (T item in list)
            {
                Add(item);
            }
            suppressNotification = false;
            OnCollectionChanged(new NotifyCollectionChangedEventArgs(NotifyCollectionChangedAction.Reset));
            counter = 0;
        }
    }
}
