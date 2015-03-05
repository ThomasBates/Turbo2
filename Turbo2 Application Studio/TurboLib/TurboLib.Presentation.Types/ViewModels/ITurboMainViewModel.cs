using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Input;

namespace TurboLib.Presentation.Types
{
    public interface ITurboMainViewModel
    {
        #region ITurboMainViewModel Command Properties

        ICommand ChangePageCommand { get; }

        #endregion
        #region ITurboMainViewModel Properties

        List<ITurboPageViewModel> PageViewModels { get; }

        ITurboPageViewModel CurrentPageViewModel { get; set; }

        #endregion
        #region ITurboMainViewModel Methods

        void AddPageViewModel(ITurboPageViewModel pageViewModel);

        #endregion
    }
}
