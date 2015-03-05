using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using TurboLib.Presentation;
using TurboLib.Presentation.Types;
//ing TurboBudget.Presentation;

namespace TurboApps.Presentation
{
    public class MainViewModel : TurboMainViewModel
    {
        public MainViewModel()
            : base()
        {
            Name = "Turbo Apps";

            AddPageViewModel(new TurboBudget.Presentation.HomeViewModel());
            AddPageViewModel(new TurboBudget.Presentation.MainViewModel());
        }
    }
}
