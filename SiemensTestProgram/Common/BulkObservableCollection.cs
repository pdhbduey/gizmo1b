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
        private int counter;

        public BulkObservableCollection()
        {
            counter = 0;
        }

        public int UpdateThreshold { get; set; }

        protected override void OnCollectionChanged(NotifyCollectionChangedEventArgs e)
        {
            if (!suppressNotification)
                base.OnCollectionChanged(e);
        }

        public void SurpressedAdd(T item, int threshold)
        {
            suppressNotification = true;
            Add(item);
            counter++;

            if (Count >= threshold)
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
