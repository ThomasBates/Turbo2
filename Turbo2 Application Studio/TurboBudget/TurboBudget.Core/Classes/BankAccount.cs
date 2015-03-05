using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using TurboBudget.Types;

namespace TurboBudget.Core
{
    public class BankAccount : INamed, IAccount, IBankAccount
    {
        #region INamed Properties

        string INamed.Name { get; set; }
        string INamed.Description { get; set; }

        #endregion
        #region IBankAccount Properties

        int IBankAccount.NextTransactionIndex { get; set; }
        string IBankAccount.AccountNumber { get; set; }

        #endregion
    }
}
