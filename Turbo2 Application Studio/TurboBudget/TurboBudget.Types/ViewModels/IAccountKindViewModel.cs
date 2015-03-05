using System;
using System.Collections.Generic;
//ing System.Collections.ObjectModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace TurboBudget.Types
{
    public interface IAccountKindViewModel
    {
        ICollection<IPaymentMethodViewModel> PaymentMethods { get; }
    }
}
