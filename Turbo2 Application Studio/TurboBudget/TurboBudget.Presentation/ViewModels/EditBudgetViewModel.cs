using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using TurboLib.Presentation;
using TurboLib.Presentation.Types;

using TurboBudget.Types;

namespace TurboBudget.Presentation
{
    public class EditBudgetViewModel : TurboViewModelBase, ITurboPageViewModel, IEditBudgetViewModel
    {
        #region Private Members

        string _name = "Edit Budget";

        #endregion
        #region IPageViewModel Properties

        public string Name
        {
            get { return _name; }
            set
            {
                if (value != _name)
                {
                    _name = value;
                    NotifyPropertyChanged("Name");
                }
            }
        }

        #endregion
    }
}
