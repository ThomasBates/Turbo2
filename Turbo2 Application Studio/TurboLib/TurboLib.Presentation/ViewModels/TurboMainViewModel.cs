using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Input;

using TurboLib.Presentation.Types;

namespace TurboLib.Presentation
{
    public class TurboMainViewModel : TurboViewModelBase,
                                      ITurboMainViewModel,
                                      ITurboPageViewModel
    {
        #region Private Members

        string _name;
        ICommand _changePageCommand;
        List<ITurboPageViewModel> _pageViewModels;
        ITurboPageViewModel _currentPageViewModel;

        #endregion
        #region Constructors

        public TurboMainViewModel()
        {
        }

        #endregion
        #region ITurboMainViewModel Command Properties

        public ICommand ChangePageCommand
        {
            get
            {
                if (_changePageCommand == null)
                    _changePageCommand = new RelayCommand(
                        p => ChangeViewModel((ITurboPageViewModel)p),
                        p => p is ITurboPageViewModel);

                return _changePageCommand;
            }
        }

        #endregion
        #region ITurboMainViewModel Properties

        public List<ITurboPageViewModel> PageViewModels
        {
            get
            {
                if (_pageViewModels == null)
                    _pageViewModels = new List<ITurboPageViewModel>();

                return _pageViewModels;
            }
        }

        public ITurboPageViewModel CurrentPageViewModel
        {
            get
            {
                return _currentPageViewModel;
            }
            set
            {
                if (_currentPageViewModel != value)
                {
                    _currentPageViewModel = value;
                    NotifyPropertyChanged("CurrentPageViewModel");
                }
            }
        }

        #endregion
        #region ITurboMainViewModel Methods

        public void AddPageViewModel(ITurboPageViewModel pageViewModel)
        {
            if (!PageViewModels.Contains(pageViewModel))
                PageViewModels.Add(pageViewModel);

            if (PageViewModels.Count == 1)
                CurrentPageViewModel = PageViewModels[0];
        }

        #endregion
        #region ITurboPageViewModel Properties

        public string Name
        {
            get { return _name; }
            set
            {
                if (_name != value)
                {
                    _name = value;
                    NotifyPropertyChanged("Name");
                }
            }
        }

        #endregion
        #region Private Methods

        void ChangeViewModel(ITurboPageViewModel viewModel)
        {
            if (!PageViewModels.Contains(viewModel))
                PageViewModels.Add(viewModel);

            CurrentPageViewModel = PageViewModels
                .FirstOrDefault(vm => vm == viewModel);
        }

        #endregion

    }
}
