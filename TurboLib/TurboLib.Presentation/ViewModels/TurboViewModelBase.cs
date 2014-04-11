using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using TurboLib.Presentation.Types;

namespace TurboLib.Presentation
{
    public class TurboViewModelBase : INotifyPropertyChanged
    {
        #region Private Members

        event PropertyChangedEventHandler _propertyChanged;

        #endregion
        #region INotifyPropertyChanged Events

        public event PropertyChangedEventHandler PropertyChanged
        {
            add { _propertyChanged += value; }
            remove { _propertyChanged -= value; }
        }

        #endregion
        #region Local Support Methods

        protected void NotifyPropertyChanged(string propertyName)
        {
            VerifyPropertyName(propertyName);

            PropertyChangedEventHandler eventHandler = _propertyChanged;
            if (eventHandler != null)
                eventHandler(this, new PropertyChangedEventArgs(propertyName));
        }

        [Conditional("DEBUG")]
        [DebuggerStepThrough]
        public virtual void VerifyPropertyName(string propertyName)
        {
            // Verify that the property name matches a real,
            // public, instance property on this object.
            if (TypeDescriptor.GetProperties(this)[propertyName] == null)
            {
                string msg = "Invalid property name: " + propertyName;

                throw new Exception(msg);
                //Debug.Fail(msg);
            }
        }
        #endregion
    }
}
