using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using TurboLib.Presentation;
using TurboLib.Presentation.Types;

namespace TurboBudget.Presentation
{
    public class MainViewModel : TurboMainViewModel
    {
        public MainViewModel() 
            : base()
        {
            Name = "Bates Budget";

            AddPageViewModel(new HomeViewModel());
            AddPageViewModel(new BudgetViewModel());
            AddPageViewModel(new EditBudgetViewModel());
        }
    }
}
